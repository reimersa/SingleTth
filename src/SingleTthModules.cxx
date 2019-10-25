#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/SingleTth/include/SingleTthModules.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/JetCorrections.h"
#include <TH1D.h>


using namespace uhh2;
using namespace std;


PDFWeightHandleProducer::PDFWeightHandleProducer(Context & ctx){
  is_mc = ctx.get("dataset_type") == "MC";
  m_oname = ctx.get("dataset_version");
  TString m_pdfname = "NNPDF30_lo_as_0130";

  take_ntupleweights = !(m_oname.Contains("VLQ") || m_oname.Contains("Diboson") || m_oname.Contains("WJets") || m_oname.Contains("QCD"));

  if(is_mc && !take_ntupleweights) m_pdfweights.reset(new PDFWeights(m_pdfname));

  for(int i=0; i<100; i++){
    TString pdfvarname = "weight_pdf_";
    pdfvarname += i+1;
    string s_pdfvarname = (string)pdfvarname;
    uhh2::Event::Handle<float> handle = ctx.declare_event_output<float>(s_pdfvarname);
    h_pdfweights.emplace_back(handle);
  }

}

bool PDFWeightHandleProducer::process(Event & event){
  if(!is_mc){
    for(int i=0; i<100; i++){
      event.set(h_pdfweights[i], 1.);
    }
    return false;
  }

  if(event.genInfo->systweights().size() == 0 && take_ntupleweights) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is empty but ntupleweights shall be taken. Is this correct? In this case add exception to take_ntupleweights.");
  if(event.genInfo->systweights().size() != 0 && (!take_ntupleweights)) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is NOT empty but take_ntupleweights is set to 'false'. Is this correct? In this case Thomas says the genInfo weight should be used. Add this sample to take_ntupleweights");

  if(take_ntupleweights){
    for(int i=0; i<100; i++){

      double pdf_weight = event.genInfo->systweights().at(i+9) / event.genInfo->originalXWGTUP();
      event.set(h_pdfweights[i], pdf_weight);
    }
    // event.set(h_pdfweights, 1.);
  }
  else{
    std::vector<double> weights = m_pdfweights->GetWeightList(event);
    for(int i=0; i<100; i++){
      event.set(h_pdfweights[i], weights[i]);
    }
    // event.set(h_pdfweights, 1.);

  }
  return true;
}


ElectronTriggerWeights::ElectronTriggerWeights(Context & ctx, TString path_, TString SysDirection_): path(path_), SysDirection(SysDirection_){

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  h_elec_weight      = ctx.declare_event_output<float>("weight_sfelec_trigger");
  h_elec_weight_up   = ctx.declare_event_output<float>("weight_sfelec_trigger_up");
  h_elec_weight_down = ctx.declare_event_output<float>("weight_sfelec_trigger_down");
  if(!is_mc){
    cout << "Warning: ElectronTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  }


  unique_ptr<TFile> file;
  file.reset(new TFile(path,"READ"));

  Eff_lowpt_MC.reset((TGraphAsymmErrors*)file->Get("gr_lowpt_eta_TTbar_eff"));
  Eff_highpt_MC.reset((TGraphAsymmErrors*)file->Get("gr_highpt_eta_TTbar_eff"));
  Eff_lowpt_DATA.reset((TGraphAsymmErrors*)file->Get("gr_lowpt_eta_DATA_eff"));
  Eff_highpt_DATA.reset((TGraphAsymmErrors*)file->Get("gr_highpt_eta_DATA_eff"));

  if(SysDirection != "nominal" && SysDirection != "up" && SysDirection != "down") throw runtime_error("In SingleTthModules.cxx, ElectronTriggerWeights.process(): Invalid SysDirection specified.");

}

bool ElectronTriggerWeights::process(Event & event){

  if(event.isRealData){
    event.set(h_elec_weight, 1.);
    event.set(h_elec_weight_up, 1.);
    event.set(h_elec_weight_down, 1.);
    return true;
  }

  const auto ele = event.electrons->at(0);
  double eta = ele.eta();
  if(fabs(eta) > 2.4) throw runtime_error("In SingleTthModules.cxx, ElectronTriggerWeights.process(): Ele-|eta| > 2.4 is not supported at the moment.");


  //find right bin in eta
  int idx = 0;
  bool lowpt = false;
  if(30 <= ele.pt() && ele.pt() < 120){
    lowpt = true;
    //lowpt trigger
    bool keep_going = true;
    while(keep_going){
      double x,y;
      Eff_lowpt_MC->GetPoint(idx,x,y);
      keep_going = eta > x + Eff_lowpt_MC->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else if(ele.pt() >= 120){
    //highpt trigger
    bool keep_going = true;
    while(keep_going){
      double x,y;
      Eff_highpt_MC->GetPoint(idx,x,y);
      keep_going = eta > x + Eff_highpt_MC->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else throw runtime_error("In SingleTthModules.cxx, ElectronTriggerWeights.process(): Electron has pt<30. Clean electron collection before applying weights.");

  //access efficiencies for MC and DATA, possibly accout for systematics = statistical + add. 2% up/down + 2% because SFs are from 80X
  double eff_data = -1, eff_mc = -1, eff_data_up = -1, eff_mc_up = -1, eff_data_down = -1, eff_mc_down = -1, dummy_x;
  double stat_data_up = -1, stat_mc_up = -1, stat_data_down = -1, stat_mc_down = -1, tp = 0.04, total_syst_data_up = -1, total_syst_mc_up = -1, total_syst_data_down = -1, total_syst_mc_down = -1;
  if(lowpt){
    Eff_lowpt_MC->GetPoint(idx,dummy_x,eff_mc);
    Eff_lowpt_DATA->GetPoint(idx,dummy_x,eff_data);
    Eff_lowpt_MC->GetPoint(idx,dummy_x,eff_mc_up);
    Eff_lowpt_DATA->GetPoint(idx,dummy_x,eff_data_up);
    Eff_lowpt_MC->GetPoint(idx,dummy_x,eff_mc_down);
    Eff_lowpt_DATA->GetPoint(idx,dummy_x,eff_data_down);

    // if(SysDirection == "up"){
    stat_mc_up = Eff_lowpt_MC->GetErrorYlow(idx);
    stat_data_up = Eff_lowpt_DATA->GetErrorYhigh(idx);
    total_syst_mc_up = sqrt(pow(stat_mc_up,2) + pow(tp,2));
    total_syst_data_up = sqrt(pow(stat_data_up,2) + pow(tp,2));
    eff_mc_up -= total_syst_mc_up;
    eff_data_up += total_syst_data_up;
    // }
    // else if(SysDirection == "down"){
    stat_mc_down = Eff_lowpt_MC->GetErrorYhigh(idx);
    stat_data_down = Eff_lowpt_DATA->GetErrorYlow(idx);
    total_syst_mc_down = sqrt(pow(stat_mc_down,2) + pow(tp,2));
    total_syst_data_down = sqrt(pow(stat_data_down,2) + pow(tp,2));
    eff_mc_down += total_syst_mc_down;
    eff_data_down -= total_syst_data_down;
    // }

  }
  else{
    Eff_highpt_MC->GetPoint(idx,dummy_x,eff_mc);
    Eff_highpt_DATA->GetPoint(idx,dummy_x,eff_data);
    Eff_highpt_MC->GetPoint(idx,dummy_x,eff_mc_up);
    Eff_highpt_DATA->GetPoint(idx,dummy_x,eff_data_up);
    Eff_highpt_MC->GetPoint(idx,dummy_x,eff_mc_down);
    Eff_highpt_DATA->GetPoint(idx,dummy_x,eff_data_down);

    // if(SysDirection == "up"){
    stat_mc_up = Eff_highpt_MC->GetErrorYlow(idx);
    stat_data_up = Eff_highpt_DATA->GetErrorYhigh(idx);
    total_syst_mc_up = sqrt(pow(stat_mc_up,2) + pow(tp,2));
    total_syst_data_up = sqrt(pow(stat_data_up,2) + pow(tp,2));
    eff_mc_up -= total_syst_mc_up;
    eff_data_up += total_syst_data_up;
    // }
    // else if(SysDirection == "down"){
    stat_mc_down = Eff_highpt_MC->GetErrorYhigh(idx);
    stat_data_down = Eff_highpt_DATA->GetErrorYlow(idx);
    total_syst_mc_down = sqrt(pow(stat_mc_down,2) + pow(tp,2));
    total_syst_data_down = sqrt(pow(stat_data_down,2) + pow(tp,2));
    eff_mc_down += total_syst_mc_down;
    eff_data_down -= total_syst_data_down;
    // }
  }

  event.set(h_elec_weight, eff_data/eff_mc);
  event.set(h_elec_weight_up, eff_data_up/eff_mc_up);
  event.set(h_elec_weight_down, eff_data_down/eff_mc_down);

  //Scale weight by (eff_data) / (eff_mc)
  if(SysDirection == "up")        event.weight *= eff_data_up / eff_mc_up;
  else if(SysDirection == "down") event.weight *= eff_data_down / eff_mc_down;
  else                            event.weight *= eff_data / eff_mc;


  return true;
}
