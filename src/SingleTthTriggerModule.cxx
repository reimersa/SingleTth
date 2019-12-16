#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/TopJetIds.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/TopPtReweight.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/TauHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/SingleTth/include/SingleTthSelections.h"
#include "UHH2/SingleTth/include/SingleTthHists.h"
// #include "UHH2/SingleTth/include/SingleTthPDFHists.h"
#include "UHH2/SingleTth/include/SingleTthModules.h"
#include "UHH2/common/include/PrintingModules.h"
// #include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
// #include "UHH2/SingleTth/include/SingleTthReconstruction.h"
// #include "UHH2/common/include/TTbarGen.h"
// #include "UHH2/SingleTth/include/SingleTthGen.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "LHAPDF/LHAPDF.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class SingleTthTriggerModule: public AnalysisModule {
  public:

    explicit SingleTthTriggerModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    
    Year year;
    unique_ptr<CommonModules> common;
    unique_ptr<AnalysisModule> SF_muonID, SF_muonIso, SF_eleReco, SF_eleID, SF_btag, scale_variation_module;
    unique_ptr<MCMuonScaleFactor> SF_muonTrigger;
    unique_ptr<ElectronTriggerWeights> SF_eleTrigger;
    // unique_ptr<PDFWeightHandleProducer> pdf_weight_producer;

    // declare the Selections to use.
    unique_ptr<Selection>  btag_loose_sel, btag_2medium_sel, btag_3medium_sel, btag_2tight_sel, btag_3tight_sel, trigger1_much_sel_2016, trigger2_much_sel_2016,trigger1_much_sel_2017, trigger1_ech_sel_2016, trigger2_ech_sel_2016,trigger1_ech_sel_2017, muon_sel_much, ele_sel_much, muon_sel_ech, ele_sel_ech,ref_trigger_much_sel;


    // store the Hists collection as member variables.
    unique_ptr<Hists> h_much_nocuts, h_much_jets_nocuts, h_much_ele_nocuts, h_much_mu_nocuts, h_much_event_nocuts, h_much_lumi_nocuts;
    unique_ptr<Hists> h_much_cleaner, h_much_jets_cleaner, h_much_ele_cleaner, h_much_mu_cleaner, h_much_event_cleaner, h_much_lumi_cleaner;
    unique_ptr<Hists> h_much_trigger, h_much_jets_trigger, h_much_ele_trigger, h_much_mu_trigger, h_much_event_trigger, h_much_lumi_trigger, h_much_btageff_trigger;
    unique_ptr<Hists> h_much_before_trigger, h_much_jets_before_trigger, h_much_ele_before_trigger, h_much_mu_before_trigger, h_much_event_before_trigger, h_much_lumi_before_trigger, h_much_btageff_before_trigger;
    unique_ptr<Hists> h_ech_nocuts, h_ech_jets_nocuts, h_ech_ele_nocuts, h_ech_mu_nocuts, h_ech_event_nocuts, h_ech_lumi_nocuts;
    unique_ptr<Hists> h_ech_cleaner, h_ech_jets_cleaner, h_ech_ele_cleaner, h_ech_mu_cleaner, h_ech_event_cleaner, h_ech_lumi_cleaner;
    unique_ptr<Hists> h_ech_trigger, h_ech_jets_trigger, h_ech_ele_trigger, h_ech_mu_trigger, h_ech_event_trigger, h_ech_lumi_trigger, h_ech_btageff_trigger;
    unique_ptr<Hists> h_ech_before_trigger, h_ech_jets_before_trigger, h_ech_ele_before_trigger, h_ech_mu_before_trigger, h_ech_event_before_trigger, h_ech_lumi_before_trigger, h_ech_btageff_before_trigger;


    bool is_mc, is_much;
    TString dataset_version, region;
    uhh2::Event::Handle<bool> h_is_tprime_reco;
  };


  SingleTthTriggerModule::SingleTthTriggerModule(Context & ctx){

    cout << "Hello World from SingleTthTriggerModule!" << endl;

    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    is_mc = ctx.get("dataset_type") == "MC";
    dataset_version = ctx.get("dataset_version");
    year = extract_year(ctx);
    // region = ctx.get("Region");
    // if(region != "SR" && region != "HighChi2") throw runtime_error("Invalid value of 'Region' in xml file.");


    h_is_tprime_reco = ctx.get_handle<bool>("is_tprime_reco");

    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->init(ctx, "nominal");


    SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, "nominal"));
    if(year == Year::is2016v2 || year == Year::is2016v3){
      SF_muonTrigger.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root", "IsoMu24_OR_IsoTkMu24_PtEtaBins", 2.5, "trigger", false, "nominal"));
    }
    SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, "nominal"));

    SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", "nominal"));
    SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", "nominal"));
    // SF_eleTrigger.reset(new ElectronTriggerWeights(ctx, "/nfs/dust/cms/user/reimersa/LQToTopMu/Run2_80X_v3/TagProbe/Optimization/35867fb_Iso27_NonIso115/ElectronEfficiencies.root", "nominal"));


    // 2. set up selections
    //Selection

    muon_sel_much.reset(new NMuonSelection(1, 1));
    ele_sel_much.reset(new NElectronSelection(0, 0));
    muon_sel_ech.reset(new NMuonSelection(0, 0));
    ele_sel_ech.reset(new NElectronSelection(1, 1));

    trigger1_much_sel_2016.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    trigger2_much_sel_2016.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));
    trigger1_much_sel_2017.reset(new TriggerSelection("HLT_IsoMu27_v*"));
    ref_trigger_much_sel.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    trigger1_ech_sel_2016.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
    trigger2_ech_sel_2016.reset(new TriggerSelection("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*"));
    trigger1_ech_sel_2017.reset(new TriggerSelection("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v*"));

    // 3. Set up Hists classes:
    h_much_nocuts.reset(new SingleTthHists(ctx, "nocuts_much"));
    h_much_jets_nocuts.reset(new JetHists(ctx, "Jets_nocuts_much"));
    h_much_ele_nocuts.reset(new ElectronHists(ctx, "Ele_nocuts_much"));
    h_much_mu_nocuts.reset(new MuonHists(ctx, "Mu_nocuts_much"));
    h_much_event_nocuts.reset(new EventHists(ctx, "Event_nocuts_much"));
    h_much_lumi_nocuts.reset(new LuminosityHists(ctx, "Lumi_nocuts_much"));
      
    h_much_cleaner.reset(new SingleTthHists(ctx, "cleaner_much"));
    h_much_jets_cleaner.reset(new JetHists(ctx, "Jets_cleaner_much"));
    h_much_ele_cleaner.reset(new ElectronHists(ctx, "Ele_cleaner_much"));
    h_much_mu_cleaner.reset(new MuonHists(ctx, "Mu_cleaner_much"));
    h_much_event_cleaner.reset(new EventHists(ctx, "Event_cleaner_much"));
    h_much_lumi_cleaner.reset(new LuminosityHists(ctx, "Lumi_cleaner_much"));
      
    h_much_trigger.reset(new SingleTthHists(ctx, "trigger_much"));
    h_much_jets_trigger.reset(new JetHists(ctx, "Jets_trigger_much"));
    h_much_ele_trigger.reset(new ElectronHists(ctx, "Ele_trigger_much"));
    h_much_mu_trigger.reset(new MuonHists(ctx, "Mu_trigger_much"));
    h_much_event_trigger.reset(new EventHists(ctx, "Event_trigger_much"));
    h_much_lumi_trigger.reset(new LuminosityHists(ctx, "Lumi_trigger_much"));

    h_much_before_trigger.reset(new SingleTthHists(ctx, "before_trigger_much"));
    h_much_jets_before_trigger.reset(new JetHists(ctx, "Jets_before_trigger_much"));
    h_much_ele_before_trigger.reset(new ElectronHists(ctx, "Ele_before_trigger_much"));
    h_much_mu_before_trigger.reset(new MuonHists(ctx, "Mu_before_trigger_much"));
    h_much_event_before_trigger.reset(new EventHists(ctx, "Event_before_trigger_much"));
    h_much_lumi_before_trigger.reset(new LuminosityHists(ctx, "Lumi_before_trigger_much"));


    h_ech_nocuts.reset(new SingleTthHists(ctx, "nocuts_ech"));
    h_ech_jets_nocuts.reset(new JetHists(ctx, "Jets_nocuts_ech"));
    h_ech_ele_nocuts.reset(new ElectronHists(ctx, "Ele_nocuts_ech"));
    h_ech_mu_nocuts.reset(new MuonHists(ctx, "Mu_nocuts_ech"));
    h_ech_event_nocuts.reset(new EventHists(ctx, "Event_nocuts_ech"));
    h_ech_lumi_nocuts.reset(new LuminosityHists(ctx, "Lumi_nocuts_ech"));
      
    h_ech_cleaner.reset(new SingleTthHists(ctx, "cleaner_ech"));
    h_ech_jets_cleaner.reset(new JetHists(ctx, "Jets_cleaner_ech"));
    h_ech_ele_cleaner.reset(new ElectronHists(ctx, "Ele_cleaner_ech"));
    h_ech_mu_cleaner.reset(new MuonHists(ctx, "Mu_cleaner_ech"));
    h_ech_event_cleaner.reset(new EventHists(ctx, "Event_cleaner_ech"));
    h_ech_lumi_cleaner.reset(new LuminosityHists(ctx, "Lumi_cleaner_ech"));
      
    h_ech_trigger.reset(new SingleTthHists(ctx, "trigger_ech"));
    h_ech_jets_trigger.reset(new JetHists(ctx, "Jets_trigger_ech"));
    h_ech_ele_trigger.reset(new ElectronHists(ctx, "Ele_trigger_ech"));
    h_ech_mu_trigger.reset(new MuonHists(ctx, "Mu_trigger_ech"));
    h_ech_event_trigger.reset(new EventHists(ctx, "Event_trigger_ech"));
    h_ech_lumi_trigger.reset(new LuminosityHists(ctx, "Lumi_trigger_ech"));

    h_ech_before_trigger.reset(new SingleTthHists(ctx, "before_trigger_ech"));
    h_ech_jets_before_trigger.reset(new JetHists(ctx, "Jets_before_trigger_ech"));
    h_ech_ele_before_trigger.reset(new ElectronHists(ctx, "Ele_before_trigger_ech"));
    h_ech_mu_before_trigger.reset(new MuonHists(ctx, "Mu_before_trigger_ech"));
    h_ech_event_before_trigger.reset(new EventHists(ctx, "Event_before_trigger_ech"));
    h_ech_lumi_before_trigger.reset(new LuminosityHists(ctx, "Lumi_before_trigger_ech"));


  }


  bool SingleTthTriggerModule::process(Event & event) {
    cout << "++++++++++ NEW EVENT ++++++++++" << endl;
    event.set(h_is_tprime_reco, false);

    is_much = (muon_sel_much->passes(event) && ele_sel_much->passes(event));
    if(!is_much){
      // if this is not the muon channel, it should be the electron channel satisfying the condition below. If not, abort!
      if(!(muon_sel_ech->passes(event) && ele_sel_ech->passes(event))) throw runtime_error("This appears to be neither the muon nor the electron channel. Abort.");
    }

    //now also check if it's not both, electron AND muon channel at the same time
    if(is_much && (muon_sel_ech->passes(event) && ele_sel_ech->passes(event))) throw runtime_error("This appears to be noth the muon AND the electron channel. Abort.");


    if(is_much){
      h_much_nocuts->fill(event);
      h_much_jets_nocuts->fill(event);
      h_much_ele_nocuts->fill(event);
      h_much_mu_nocuts->fill(event);
      h_much_event_nocuts->fill(event);
      h_much_lumi_nocuts->fill(event);
    }else{
      h_ech_nocuts->fill(event);
      h_ech_jets_nocuts->fill(event);
      h_ech_ele_nocuts->fill(event);
      h_ech_mu_nocuts->fill(event);
      h_ech_event_nocuts->fill(event);
      h_ech_lumi_nocuts->fill(event);
    }

    bool pass_common = common->process(event);
    if(!pass_common) return false;

    event.weight = event.weight *  event.prefiringWeight;

    SF_muonID->process(event);
    SF_muonIso->process(event);
    SF_eleReco->process(event);
    SF_eleID->process(event);

    if(is_much){
      h_much_cleaner->fill(event);
      h_much_jets_cleaner->fill(event);
      h_much_ele_cleaner->fill(event);
      h_much_mu_cleaner->fill(event);
      h_much_event_cleaner->fill(event);
      h_much_lumi_cleaner->fill(event);
    }else{
      h_ech_cleaner->fill(event);
      h_ech_jets_cleaner->fill(event);
      h_ech_ele_cleaner->fill(event);
      h_ech_mu_cleaner->fill(event);
      h_ech_event_cleaner->fill(event);
      h_ech_lumi_cleaner->fill(event);
    }

    // Trigger
    if(is_much){
      if((year == Year::is2016v2) || (year == Year::is2016v3)){ 
	if(!(trigger1_much_sel_2016->passes(event) || trigger2_much_sel_2016->passes(event)) ) return false;
	SF_muonTrigger->process_onemuon(event,0);
      }else if(year == Year::is2017v2){
	if(!(ref_trigger_much_sel->passes(event)))return false;
	h_much_before_trigger->fill(event);
	h_much_jets_before_trigger->fill(event);
	h_much_ele_before_trigger->fill(event);
	h_much_mu_before_trigger->fill(event);
	h_much_event_before_trigger->fill(event);
	h_much_lumi_before_trigger->fill(event);

	if(!(trigger1_much_sel_2017->passes(event)) ) return false;
	h_much_trigger->fill(event);
	h_much_jets_trigger->fill(event);
	h_much_ele_trigger->fill(event);
	h_much_mu_trigger->fill(event);
	h_much_event_trigger->fill(event);
	h_much_lumi_trigger->fill(event);
	
      }
    }
    else{
      if((year == Year::is2016v2) || (year == Year::is2016v3)){ 
      if(!(trigger1_ech_sel_2016->passes(event) || trigger2_ech_sel_2016->passes(event)) ) return false;
      // SF_eleTrigger->process(event);
      }else  if(year == Year::is2017v2){
	if(!(trigger1_ech_sel_2017->passes(event) || trigger2_ech_sel_2016->passes(event)) ) return false;
	h_much_trigger->fill(event);
	h_much_jets_trigger->fill(event);
	h_much_ele_trigger->fill(event);
	h_much_mu_trigger->fill(event);
	h_much_event_trigger->fill(event);
	h_much_lumi_trigger->fill(event);

      }
    }

    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(SingleTthTriggerModule)
}
