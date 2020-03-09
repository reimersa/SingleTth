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

  take_ntupleweights = !(m_oname.Contains("QCD") || m_oname.Contains("ST_tW"));

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

  if(event.genInfo->systweights().size() < 100 && take_ntupleweights) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is too small but ntupleweights shall be taken. Is this correct? In this case add exception to take_ntupleweights.");
  if(event.genInfo->systweights().size() >110 && (!take_ntupleweights)) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is NOT empty but take_ntupleweights is set to 'false'. Is this correct? In this case Thomas says the genInfo weight should be used. Add this sample to take_ntupleweights");

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


MuonTriggerWeights::MuonTriggerWeights(Context & ctx, TString path_, Year year_): path(path_), year(year_){

  h_muon_weight      = ctx.declare_event_output<float>("weight_sfmu_trigger");
  h_muon_weight_up   = ctx.declare_event_output<float>("weight_sfmu_trigger_up");
  h_muon_weight_down = ctx.declare_event_output<float>("weight_sfmu_trigger_down");

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: MuonTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  }

  TString yeartag = "2016";
  if(year == Year::is2017v1 || year == Year::is2017v2) yeartag = "2017";
  else if(year == Year::is2018) yeartag = "2018";

  unique_ptr<TFile> file_30to50, file_50to100, file_100to200, file_200toinf;
  file_30to50.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt30to50.root","READ"));
  file_50to100.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt50to100.root","READ"));
  file_100to200.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt100to200.root","READ"));
  file_200toinf.reset(new TFile(path+"/" + yeartag + "/MuonTriggerScaleFactors_eta_mu_binned_pt200toInf.root","READ"));

  g_sf_30to50.reset((TGraphAsymmErrors*)file_30to50->Get("ScaleFactors"));
  g_sf_50to100.reset((TGraphAsymmErrors*)file_50to100->Get("ScaleFactors"));
  g_sf_100to200.reset((TGraphAsymmErrors*)file_100to200->Get("ScaleFactors"));
  g_sf_200toinf.reset((TGraphAsymmErrors*)file_200toinf->Get("ScaleFactors"));

}

bool MuonTriggerWeights::process(Event & event){

  if(event.isRealData){
    event.set(h_muon_weight, 1.);
    event.set(h_muon_weight_up, 1.);
    event.set(h_muon_weight_down, 1.);
    return true;
  }

  const Muon muon = event.muons->at(0);
  double eta = muon.eta();
  double pt = muon.pt();
  if(fabs(eta) > 2.4) throw runtime_error("In LQToTopMuModules.cxx, MuonTriggerWeights.process(): Muon-|eta| > 2.4 is not supported at the moment.");
  if(pt < 30) throw runtime_error("In LQToTopMuModules.cxx, MuonTriggerWeights.process(): Muon-pt < 30 is not supported at the moment.");

  // find number of correct eta bin
  int idx = 0;
  if(pt < 50){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_30to50->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_30to50->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  if(pt < 100){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_50to100->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_50to100->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else if (pt < 200){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_100to200->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_100to200->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else{
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_200toinf->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_200toinf->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }


  //access scale factors, add 2% t&p systematic uncertainty
  double sf, sf_up, sf_down, dummy_x;
  double stat_up = -1., stat_down = -1., tp = 0.02, total_up = -1., total_down = -1.;
  if(pt < 50){
    g_sf_30to50->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_30to50->GetErrorYhigh(idx);
    stat_down = g_sf_30to50->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  if(pt < 100){
    g_sf_50to100->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_50to100->GetErrorYhigh(idx);
    stat_down = g_sf_50to100->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else if(pt < 200){
    g_sf_100to200->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_100to200->GetErrorYhigh(idx);
    stat_down = g_sf_100to200->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else{
    g_sf_200toinf->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_200toinf->GetErrorYhigh(idx);
    stat_down = g_sf_200toinf->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }

  event.weight *= sf;
  event.set(h_muon_weight, sf);
  event.set(h_muon_weight_up, sf_up);
  event.set(h_muon_weight_down, sf_down);
  return true;
}


ElectronTriggerWeights::ElectronTriggerWeights(Context & ctx, TString path_, Year year_): path(path_), year(year_){

  h_elec_weight      = ctx.declare_event_output<float>("weight_sfelec_trigger");
  h_elec_weight_up   = ctx.declare_event_output<float>("weight_sfelec_trigger_up");
  h_elec_weight_down = ctx.declare_event_output<float>("weight_sfelec_trigger_down");

  auto dataset_type = ctx.get("dataset_type");
  bool is_mc = dataset_type == "MC";
  if(!is_mc){
    cout << "Warning: ElectronTriggerWeights will not have an effect on this non-MC sample (dataset_type = '" + dataset_type + "')" << endl;
    return;
  }

  TString yeartag = "2016";
  if(year == Year::is2017v1 || year == Year::is2017v2) yeartag = "2017";
  else if(year == Year::is2018) yeartag = "2018";

  unique_ptr<TFile> file_low, file_high;
  pt_split = 175;
  file_low.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt30to175.root","READ"));
  file_high.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt175toInf.root","READ"));
  if(year == Year::is2017v1 || year == Year::is2017v2){
    file_low.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt30to200.root","READ"));
    file_high.reset(new TFile(path+"/" + yeartag + "/ElectronTriggerScaleFactors_eta_ele_binned_pt200toInf.root","READ"));
    pt_split = 200;
  }

  g_sf_low.reset((TGraphAsymmErrors*)file_low->Get("ScaleFactors"));
  g_sf_high.reset((TGraphAsymmErrors*)file_high->Get("ScaleFactors"));

}

bool ElectronTriggerWeights::process(Event & event){

  if(event.isRealData){
    event.set(h_elec_weight, 1.);
    event.set(h_elec_weight_up, 1.);
    event.set(h_elec_weight_down, 1.);
    return true;
  }

  const Electron elec = event.electrons->at(0);
  double eta = elec.eta();
  double pt = elec.pt();
  if(fabs(eta) > 2.4) throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): Elec-|eta| > 2.4 is not supported at the moment.");
  if(pt < 30) throw runtime_error("In LQToTopMuModules.cxx, ElectronTriggerWeights.process(): Elec-pt < 30 is not supported at the moment.");

  // find number of correct eta bin
  int idx = 0;
  if(pt < pt_split){
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_low->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_low->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }
  else{
    bool keep_going = true;
    while(keep_going){
      double x,y;
      g_sf_high->GetPoint(idx,x,y);
      keep_going = eta > x + g_sf_high->GetErrorXhigh(idx);
      if(keep_going) idx++;
    }
  }


  //access scale factors, add 2% t&p systematic uncertainty
  double sf, sf_up, sf_down, dummy_x;
  double stat_up = -1., stat_down = -1., tp = 0.02, total_up = -1., total_down = -1.;
  if(pt < pt_split){
    g_sf_low->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_low->GetErrorYhigh(idx);
    stat_down = g_sf_low->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }
  else{
    g_sf_high->GetPoint(idx,dummy_x,sf);

    stat_up = g_sf_high->GetErrorYhigh(idx);
    stat_down = g_sf_high->GetErrorYlow(idx);
    total_up = sqrt(pow(stat_up,2) + pow(tp,2));
    total_down = sqrt(pow(stat_down,2) + pow(tp,2));

    sf_up = sf + total_up;
    sf_down = sf + total_down;
  }

  event.weight *= sf;
  event.set(h_elec_weight, sf);
  event.set(h_elec_weight_up, sf_up);
  event.set(h_elec_weight_down, sf_down);
  return true;
}

