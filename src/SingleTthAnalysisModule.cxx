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
#include "UHH2/SingleTth/include/SingleTthPDFHists.h"
#include "UHH2/SingleTth/include/SingleTthModules.h"
#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
#include "UHH2/SingleTth/include/SingleTthReconstruction.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/SingleTth/include/SingleTthGen.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "LHAPDF/LHAPDF.h"
#include <UHH2/SingleTth/include/ModuleBASE.h>

using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class SingleTthAnalysisModule: public ModuleBASE {
  public:

    explicit SingleTthAnalysisModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:

    Year year;
    unique_ptr<CommonModules> common;
    unique_ptr<AnalysisModule> SF_muonID, SF_muonIso, SF_eleReco, SF_eleID, SF_btag, scale_variation_module, leptonremoval;
    unique_ptr<MuonTriggerWeightsOffical> SF_muonTrigger;
    unique_ptr<ElectronTriggerWeights> SF_eleTrigger;
    // unique_ptr<PDFWeightHandleProducer> pdf_weight_producer;

    unique_ptr<ElectronCleaner> ele_cleaner_2016, ele_cleaner_2017, ele_cleaner_2018;

    // declare the Selections to use.
    unique_ptr<Selection>  btag_loose_sel, btag_2medium_sel, btag_3medium_sel, btag_2tight_sel, btag_3tight_sel, trigger1_much_sel_2016, trigger2_much_sel_2016,trigger1_much_sel_2017,trigger1_much_sel_2018, trigger1_ech_sel_2016, trigger2_ech_sel_2016,trigger1_ech_sel_2017,trigger2_ech_sel_2017,trigger1_ech_sel_2018, muon_sel_much, ele_sel_much, muon_sel_ech, ele_sel_ech, HEMIssue_sel_2018;

    unique_ptr<HighMassSingleTthReconstruction> tprime_reco;
    unique_ptr<SingleTthChi2Discriminator> tprime_chi2;

    // store a few trigger hist to check correctness of trigger sf
    unique_ptr<Hists> h_btageff_trigger, h_btageff_trigger_ele, h_aftertrigger_ele,h_triggerSF_ele,h_triggeroffline_ele, h_aftertrigger,h_triggerSF,h_triggeroffline ;

    // unique_ptr<Hists> h_PDF_variations_much_sr, h_PDF_variations_much_cr, h_PDF_variations_ech_sr, h_PDF_variations_ech_cr;

    BTag::algo btag_algo;
    BTag::wp wp_loose, wp_medium, wp_tight;

    bool is_mc, is_much;
    TString dataset_version, region;
    string sys_muonid, sys_muoniso, sys_muontrigger, sys_elereco, sys_eleid, sys_eletrigger, sys_btag, sys_pu;

    uhh2::Event::Handle<float> h_L1prefiring;
    uhh2::Event::Handle<float> h_L1prefiring_up;
    uhh2::Event::Handle<float> h_L1prefiring_down;

    uhh2::Event::Handle<float> h_muon_triggerweight;
    uhh2::Event::Handle<float> h_muon_triggerweight_up;
    uhh2::Event::Handle<float> h_muon_triggerweight_down;
    uhh2::Event::Handle<float> h_electron_triggerweight;
    uhh2::Event::Handle<float> h_electron_triggerweight_up;
    uhh2::Event::Handle<float> h_electron_triggerweight_down;

    uhh2::Event::Handle<float> h_eventweight_lumi, h_eventweight_final;

    uhh2::Event::Handle<bool> h_is_tprime_reco;
    uhh2::Event::Handle<std::vector<SingleTthReconstructionHypothesis>> h_hyps;    
    uhh2::Event::Handle<TString> h_best_cat;    

  };

  void SingleTthAnalysisModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      //  cout << "booking histograms with tag " << tag << endl;
      book_HFolder("h_"+tag, new SingleTthHists(ctx,tag));
      book_HFolder("h_jets_"+tag, new JetHists(ctx,"Jets_"+tag));
      book_HFolder("h_ele_"+tag, new ElectronHists(ctx,"Ele_"+tag));
      book_HFolder("h_mu_"+tag, new MuonHists(ctx,"Mu_"+tag));
      book_HFolder("h_event_"+tag, new EventHists(ctx,"Event_"+tag));
      book_HFolder("h_lumi_"+tag, new LuminosityHists(ctx,"Lumi_"+tag));
    }
  }

  void SingleTthAnalysisModule::fill_histograms(uhh2::Event& event, string tag){
    HFolder("h_"+tag)->fill(event); 
    HFolder("h_jets_"+tag)->fill(event); 
    HFolder("h_ele_"+tag)->fill(event); 
    HFolder("h_mu_"+tag)->fill(event); 
    HFolder("h_event_"+tag)->fill(event); 
    HFolder("h_lumi_"+tag)->fill(event); 
  }


  SingleTthAnalysisModule::SingleTthAnalysisModule(Context & ctx){

    cout << "Hello World from SingleTthAnalysisModule!" << endl;

    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    is_mc = ctx.get("dataset_type") == "MC";
    dataset_version = ctx.get("dataset_version");
    year = extract_year(ctx);

    sys_muonid = ctx.get("sys_muonid");
    sys_muoniso = ctx.get("sys_muoniso");
    sys_muontrigger = ctx.get("sys_muontrigger");
    sys_elereco = ctx.get("sys_elereco");
    sys_eleid = ctx.get("sys_eleid");
    sys_eletrigger = ctx.get("sys_eletrigger");
    sys_btag = ctx.get("sys_btag");
    sys_pu = ctx.get("sys_pu");


    btag_algo = BTag::DEEPJET;
    //    btag_algo = BTag::DEEPCSV;
    wp_loose = BTag::WP_LOOSE;
    wp_medium = BTag::WP_MEDIUM;
    wp_tight = BTag::WP_TIGHT;

    JetId DeepjetLoose = BTag(btag_algo, wp_loose);
    JetId DeepjetMedium = BTag(btag_algo, wp_medium);
    JetId DeepjetTight = BTag(btag_algo, wp_tight);

    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    JetId Jet_ID = AndId<Jet>(jet_pfid, PtEtaCut(30.0, 2.4));
    MuonId MuId;
    ElectronId EleId;
    EleId = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(30.0, 2.4));
    if (year == Year::is2016v2) MuId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
    else                        MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonID(Muon::PFIsoTight));


    common.reset(new CommonModules());
    //    common->switch_jetlepcleaner(true);
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->set_electron_id(EleId);
    common->set_muon_id(MuId);
    common->set_jet_id(Jet_ID);
    common->init(ctx, sys_pu);

    leptonremoval.reset(new JetLeptonOverlapRemoval(0.4));
    ele_cleaner_2016.reset(new ElectronCleaner(AndId<Electron>(ElectronID_Fall17_tight,PtEtaCut(30.0, 2.4))));
    ele_cleaner_2017.reset(new ElectronCleaner(AndId<Electron>(ElectronID_Fall17_tight,PtEtaCut(40.0, 2.4))));
    ele_cleaner_2018.reset(new ElectronCleaner(AndId<Electron>(ElectronID_Fall17_tight,PtEtaCut(35.0, 2.4))));

    SF_muonTrigger.reset(new MuonTriggerWeightsOffical(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/LQTopLep/data/", year));
    SF_eleTrigger.reset(new ElectronTriggerWeights(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/LQTopLep/data/", year));


    if(year == Year::is2016v2 || year == Year::is2016v3){
      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightID_DEN_genTracks_eta_pt", 0., "id", false, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt", 0., "iso", false, "nominal"));
      
      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/2016LegacyReReco_ElectronTight_Fall17V2.root", 1, "id", "nominal"));
    }
    else if(year == Year::is2017v1 || year == Year::is2017v2){

      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonID_94X_RunBCDEF_SF_ID.root", "NUM_TightID_DEN_genTracks_pt_abseta", 0., "id", true, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/MuonIso_94X_RunBCDEF_SF_ISO.root", "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta", 0., "iso", true, "nominal"));
      
      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2017/2017_ElectronTight.root", 1, "id", "nominal"));
    }
    else if(year == Year::is2018){
      
      SF_muonID.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_ID_SF_RunABCD.root", "NUM_TightID_DEN_TrackerMuons_pt_abseta", 0., "id", true, "nominal"));
      SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/Muon_Iso_SF_RunABCD.root", "NUM_TightRelIso_DEN_TightIDandIPCut_pt_abseta", 0., "iso", true, "nominal"));
      
      SF_eleReco.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/egammaEffi.txt_EGM2D_updatedAll.root", 1, "reco", "nominal"));
      SF_eleID.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2018/2018_ElectronTight.root", 1, "id", "nominal"));
    }
    
    //    if((year == Year::is2016v2) || (year == Year::is2016v3) || (year == Year::is2017v2)){
    //    if((year == Year::is2016v2) || (year == Year::is2016v3)){
    //SF_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, wp_medium, "jets", sys_btag, "comb"));
    SF_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, wp_medium, "jets", sys_btag, "mujets"));
    if(year == Year::is2018)            SF_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, wp_medium, "jets", sys_btag, "comb"));
    //    }
    
    if(year != Year::is2018){
      h_L1prefiring = ctx.declare_event_output<float>("weight_sfL1prefiring");
      h_L1prefiring_up = ctx.declare_event_output<float>("weight_sfL1prefiring_up");
      h_L1prefiring_down = ctx.declare_event_output<float>("weight_sfL1prefiring_down");
    }
    
    // pdf_weight_producer.reset(new PDFWeightHandleProducer(ctx));
    h_muon_triggerweight = ctx.declare_event_output<float>("weight_sfmu_trigger");
    h_muon_triggerweight_up = ctx.declare_event_output<float>("weight_sfmu_trigger_up");
    h_muon_triggerweight_down = ctx.declare_event_output<float>("weight_sfmu_trigger_down");


    h_electron_triggerweight = ctx.declare_event_output<float>("weight_sfelec_trigger");
    h_electron_triggerweight_up = ctx.declare_event_output<float>("weight_sfelec_trigger_up");
    h_electron_triggerweight_down = ctx.declare_event_output<float>("weight_sfelec_trigger_down");

    h_eventweight_lumi = ctx.declare_event_output<float>("eventweight_lumi");
    h_eventweight_final = ctx.declare_event_output<float>("eventweight_final");

    h_is_tprime_reco = ctx.get_handle<bool>("is_tprime_reco");
    h_hyps = ctx.get_handle<vector<SingleTthReconstructionHypothesis>>("TprimeHypotheses");
    h_best_cat = ctx.declare_event_output<TString>("Best_cat");

    // 2. set up selections
    //Selection

    muon_sel_much.reset(new NMuonSelection(1, 1, MuId));
    ele_sel_much.reset(new NElectronSelection(0, 0, EleId));
    muon_sel_ech.reset(new NMuonSelection(0, 0, MuId));
    ele_sel_ech.reset(new NElectronSelection(1, 1, EleId));

    btag_loose_sel.reset(new NJetSelection(3, -1, DeepjetLoose));
    btag_2medium_sel.reset(new NJetSelection(2,-1,DeepjetMedium));
    btag_3medium_sel.reset(new NJetSelection(3,-1,DeepjetMedium));
    btag_2tight_sel.reset(new NJetSelection(2,-1,DeepjetTight));
    btag_3tight_sel.reset(new NJetSelection(3,-1,DeepjetTight));
    trigger1_much_sel_2016.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    trigger2_much_sel_2016.reset(new TriggerSelection("HLT_IsoTkMu24_v*"));
    trigger1_much_sel_2017.reset(new TriggerSelection("HLT_IsoMu27_v*"));
    trigger1_much_sel_2018.reset(new TriggerSelection("HLT_IsoMu24_v*"));
    trigger1_ech_sel_2016.reset(new TriggerSelection("HLT_Ele27_WPTight_Gsf_v*"));
    trigger2_ech_sel_2016.reset(new TriggerSelection("HLT_Photon175_v*"));
    trigger1_ech_sel_2017.reset(new TriggerSelection("HLT_Ele35_WPTight_Gsf_v*"));
    trigger2_ech_sel_2017.reset(new TriggerSelection("HLT_Photon200_v*"));
    trigger1_ech_sel_2018.reset(new TriggerSelection("HLT_Ele32_WPTight_Gsf_v*"));

    //    tprime_reco.reset(new HighMassSingleTthReconstruction(ctx, SingleTthNeutrinoReconstruction, DeepjetTight));
    tprime_reco.reset(new HighMassSingleTthReconstruction(ctx, SingleTthNeutrinoReconstruction, DeepjetMedium));
    tprime_chi2.reset(new SingleTthChi2Discriminator(ctx,year));

    scale_variation_module.reset(new MCScaleVariation(ctx));

    HEMIssue_sel_2018.reset(new HEMIssueSelection(DeepjetMedium));

    // 3. Set up Hists classes:
    book_histograms(ctx,{"nocuts","cleaner","trigger","btagsf","btag","btag_2m","btag_3m","btag_2t"});

    book_histograms(ctx,{"cleaner_ele","trigger_ele","btagsf_ele","btag_ele","btag_2m_ele","btag_3m_ele"});

    book_histograms(ctx,{"reco_much_sr","reco_much_cr","reco_chi2h_2_much_sr","reco_catma60_much_sr","reco_catma90_much_sr","reco_catma175_much_sr","reco_catma300_much_sr"});
    book_histograms(ctx,{"chi2h_2_much_sr","catma60_much_sr","catma90_much_sr","catma175_much_sr","catma300_much_sr","chi2h_2_much_cr"});
    book_histograms(ctx,{"chi2_3_chi2h_2_much_sr","chi2_3_catma60_much_sr","chi2_3_catma90_much_sr","chi2_3_catma175_much_sr","chi2_3_catma300_much_sr","chi2_3_chi2h_2_much_cr"});

    book_histograms(ctx,{"reco_ech_sr","reco_ech_cr","reco_chi2h_2_ech_sr","reco_catma60_ech_sr","reco_catma90_ech_sr","reco_catma175_ech_sr","reco_catma300_ech_sr"});
    book_histograms(ctx,{"chi2h_2_ech_sr","catma60_ech_sr","catma90_ech_sr","catma175_ech_sr","catma300_ech_sr","chi2h_2_ech_cr"});
    book_histograms(ctx,{"chi2_3_chi2h_2_ech_sr","chi2_3_catma60_ech_sr","chi2_3_catma90_ech_sr","chi2_3_catma175_ech_sr","chi2_3_catma300_ech_sr","chi2_3_chi2h_2_ech_cr"});

    book_histograms(ctx,{"best_chi2_10_chi2h_2_much_sr","best_chi2_10_catma60_much_sr","best_chi2_10_catma90_much_sr","best_chi2_10_catma175_much_sr","best_chi2_10_catma300_much_sr"});
    book_histograms(ctx,{"best_chi2_10_chi2h_2_ech_sr","best_chi2_10_catma60_ech_sr","best_chi2_10_catma90_ech_sr","best_chi2_10_catma175_ech_sr","best_chi2_10_catma300_ech_sr"});

    book_histograms(ctx,{"best_chi2_25_chi2h_2_much_sr","best_chi2_25_catma60_much_sr","best_chi2_25_catma90_much_sr","best_chi2_25_catma175_much_sr","best_chi2_25_catma300_much_sr"});
    book_histograms(ctx,{"best_chi2_25_chi2h_2_ech_sr","best_chi2_25_catma60_ech_sr","best_chi2_25_catma90_ech_sr","best_chi2_25_catma175_ech_sr","best_chi2_25_catma300_ech_sr"});

    book_histograms(ctx,{"best_chi2_3_chi2h_2_much_sr","best_chi2_3_catma60_much_sr","best_chi2_3_catma90_much_sr","best_chi2_3_catma175_much_sr","best_chi2_3_catma300_much_sr"});
    book_histograms(ctx,{"best_chi2_3_chi2h_2_ech_sr","best_chi2_3_catma60_ech_sr","best_chi2_3_catma90_ech_sr","best_chi2_3_catma175_ech_sr","best_chi2_3_catma300_ech_sr"});

    book_histograms(ctx,{"best_chi2h_2_much_sr","best_catma60_much_sr","best_catma90_much_sr","best_catma175_much_sr","best_catma300_much_sr"});
    book_histograms(ctx,{"best_chi2h_2_ech_sr","best_catma60_ech_sr","best_catma90_ech_sr","best_catma175_ech_sr","best_catma300_ech_sr"});



    //    //    h_btageff_trigger.reset(new BTagMCEfficiencyHists(ctx, "btageff_trigger", DeepjetTight));
    h_btageff_trigger.reset(new BTagMCEfficiencyHists(ctx, "btageff_trigger", DeepjetMedium));
    h_btageff_trigger_ele.reset(new BTagMCEfficiencyHists(ctx, "btageff_trigger_ele", DeepjetMedium));



    h_aftertrigger_ele.reset(new SingleTthHists(ctx, "aftertrigger_ele"));
    h_triggerSF_ele.reset(new SingleTthHists(ctx, "triggerSF_ele"));
    h_triggeroffline_ele.reset(new SingleTthHists(ctx, "triggeroffline_ele"));
    h_aftertrigger.reset(new SingleTthHists(ctx, "aftertrigger"));
    h_triggerSF.reset(new SingleTthHists(ctx, "triggerSF"));
    h_triggeroffline.reset(new SingleTthHists(ctx, "triggeroffline"));



    // h_PDF_variations_much_sr.reset(new SingleTthPDFHists(ctx, "PDF_variations_much_sr"));
    // h_PDF_variations_much_cr.reset(new SingleTthPDFHists(ctx, "PDF_variations_much_cr"));
    // h_PDF_variations_ech_sr.reset(new SingleTthPDFHists(ctx, "PDF_variations_ech_sr"));
    // h_PDF_variations_ech_cr.reset(new SingleTthPDFHists(ctx, "PDF_variations_ech_cr"));

  }


  bool SingleTthAnalysisModule::process(Event & event) {
    // cout << "++++++++++ NEW EVENT ++++++++++" << endl;
    event.set(h_is_tprime_reco, false);


    is_much = (muon_sel_much->passes(event) && ele_sel_much->passes(event));
    if(!is_much){
      // if this is not the muon channel, it should be the electron channel satisfying the condition below. If not, abort!
      if(!(muon_sel_ech->passes(event) && ele_sel_ech->passes(event))) throw runtime_error("This appears to be neither the muon nor the electron channel. Abort.");
    }

    //now also check if it's not both, electron AND muon channel at the same time
    if(is_much && (muon_sel_ech->passes(event) && ele_sel_ech->passes(event))) throw runtime_error("This appears to be noth the muon AND the electron channel. Abort.");


    // Now writing handles in any case
    scale_variation_module->process(event);

    if(is_much){
      fill_histograms(event,"nocuts");
    }

    bool pass_common = common->process(event);
    if(!pass_common) return false;

    leptonremoval->process(event);

    double eventweight_lumi = event.weight;
    event.set(h_eventweight_lumi, eventweight_lumi);

    if(year != Year::is2018){
      event.weight = event.weight *  event.prefiringWeight;
      event.set(h_L1prefiring,event.prefiringWeight);
      event.set(h_L1prefiring_up, event.prefiringWeightUp);
      event.set(h_L1prefiring_down, event.prefiringWeightDown);
    }

    SF_muonID->process(event);
    SF_muonIso->process(event);
    SF_eleReco->process(event);
    SF_eleID->process(event);

    if(is_much){
      fill_histograms(event,"cleaner");
    }else{
      fill_histograms(event,"cleaner_ele");
    }

    // Trigger
    if(is_much){
      if((year == Year::is2016v2) || (year == Year::is2016v3)){
	h_triggeroffline->fill(event);
        if(!(trigger1_much_sel_2016->passes(event) || trigger2_much_sel_2016->passes(event)) ) return false;
	h_aftertrigger->fill(event);

	SF_muonTrigger->process(event);
	h_triggerSF->fill(event);
      }
      else if(year == Year::is2017v2){
	h_triggeroffline->fill(event);
        if(!(trigger1_much_sel_2017->passes(event)) ) return false;
	h_aftertrigger->fill(event);

	SF_muonTrigger->process(event);
	h_triggerSF->fill(event);
      }
      else if(year == Year::is2018){
	h_triggeroffline->fill(event);
        if(!(trigger1_much_sel_2018->passes(event)) ) return false;
	h_aftertrigger->fill(event);


	SF_muonTrigger->process(event);
	h_triggerSF->fill(event);
      // event.set(h_muon_triggerweight, 1.);
      // event.set(h_muon_triggerweight_up, 1.);
      // event.set(h_muon_triggerweight_down, 1.);

      }
      event.set(h_electron_triggerweight, 1.);
      event.set(h_electron_triggerweight_up, 1.);
      event.set(h_electron_triggerweight_down, 1.);
    }
    else{
      if((year == Year::is2016v2) || (year == Year::is2016v3)){
	ele_cleaner_2016->process(event);
	if(!event.electrons->size()) return false;
	h_triggeroffline_ele->fill(event);


	if(dataset_version.Contains("Photon")){
	  if(trigger1_ech_sel_2016->passes(event)) return false;
	  if(!trigger2_ech_sel_2016->passes(event)) return false;
	}else if(!is_mc){
	  if(!trigger1_ech_sel_2016->passes(event)) return false;
	}else{
	  if(!(trigger1_ech_sel_2016->passes(event) || trigger2_ech_sel_2016->passes(event)) ) return false; 
	}
	h_aftertrigger_ele->fill(event);
	SF_eleTrigger->process(event);  
	h_triggerSF_ele->fill(event);      
      }
      else  if(year == Year::is2017v2){
	ele_cleaner_2017->process(event);
	if(!event.electrons->size()) return false;
	h_triggeroffline_ele->fill(event);

	if(dataset_version.Contains("Photon")){
	  if(trigger1_ech_sel_2017->passes(event)) return false;
	  if(!trigger2_ech_sel_2017->passes(event)) return false;
	}else if(!is_mc){
	  if(!trigger1_ech_sel_2017->passes(event)) return false;
	}else{
	  if(!(trigger1_ech_sel_2017->passes(event) || trigger2_ech_sel_2017->passes(event)) ) return false; 
	}
	h_aftertrigger_ele->fill(event);
	SF_eleTrigger->process(event);
	h_triggerSF_ele->fill(event);

      }
      else  if(year == Year::is2018){
	ele_cleaner_2018->process(event);
	if(!event.electrons->size()) return false;
	h_triggeroffline_ele->fill(event);

        if(!(trigger1_ech_sel_2018->passes(event)||trigger2_ech_sel_2017->passes(event)) ) return false;
	
	h_aftertrigger_ele->fill(event);

	SF_eleTrigger->process(event);
	h_triggerSF_ele->fill(event);
      // event.set(h_electron_triggerweight, 1.);
      // event.set(h_electron_triggerweight_up, 1.);
      // event.set(h_electron_triggerweight_down, 1.);

      }
      event.set(h_muon_triggerweight, 1.);
      event.set(h_muon_triggerweight_up, 1.);
      event.set(h_muon_triggerweight_down, 1.);
      if(event.electrons->size()<1) return false;
    }

    if(is_much){
      fill_histograms(event,"trigger");
      h_btageff_trigger->fill(event);
    }else fill_histograms(event,"trigger_ele");


    SF_btag->process(event);
    
    if(is_much)  fill_histograms(event, "btagsf");
    else fill_histograms(event, "btagsf_ele");

    // 3 b-tags
    if(!btag_loose_sel->passes(event)) return false;
    
    if(is_much)  fill_histograms(event,"btag");
    else fill_histograms(event,"btag_ele");
    

    if(!(btag_2medium_sel->passes(event))) return false;
    if(is_much)  fill_histograms(event,"btag_2m");
    else  fill_histograms(event,"btag_2m_ele");
    

    if(!(btag_3medium_sel->passes(event))) return false;
    if(is_much) fill_histograms(event,"btag_3m");  
    else fill_histograms(event,"btag_3m_ele");  
    

    if(year == Year::is2018){
      bool pass_HEMIssue = HEMIssue_sel_2018 -> passes(event);
      if (!pass_HEMIssue) return false;
    }

    // if(!(btag_2tight_sel->passes(event))) return false;
    if(is_much) fill_histograms(event,"btag_2t");
    

    tprime_reco->process(event);
    tprime_chi2->process(event);

    bool is_tprime_reco = event.get(h_is_tprime_reco);
    if(!is_tprime_reco) throw runtime_error("After reconstruction, the T still isn't reconstructed. How?");

    if(is_much){
      fill_histograms(event, "reco_much_sr");
      fill_histograms(event, "reco_much_cr");
    }
    else{
      fill_histograms(event, "reco_ech_sr");
      fill_histograms(event, "reco_ech_cr");
    }

    std::vector<SingleTthReconstructionHypothesis> hyps = event.get(h_hyps);
    const SingleTthReconstructionHypothesis* hyp = get_best_hypothesis( hyps, "Chi2" );

    const SingleTthReconstructionHypothesis* hyp_catma60 = get_best_hypothesis( hyps, "Chi2_top+ma60" );

    const SingleTthReconstructionHypothesis* hyp_catma90 = get_best_hypothesis( hyps, "Chi2_top+ma90" );

    const SingleTthReconstructionHypothesis* hyp_catma175 = get_best_hypothesis( hyps, "Chi2_top+ma175" );

    const SingleTthReconstructionHypothesis* hyp_catma300 = get_best_hypothesis( hyps, "Chi2_top+ma300" );

    std::map<TString,const SingleTthReconstructionHypothesis*> hyp_catmap;
    hyp_catmap["Chi2_top+ma60"] = hyp_catma60;
    hyp_catmap["Chi2_top+ma90"] = hyp_catma90;
    hyp_catmap["Chi2"] = hyp;
    hyp_catmap["Chi2_top+ma175"] = hyp_catma175;
    hyp_catmap["Chi2_top+ma300"] = hyp_catma300;

    /// find smallest hypothesis
    float smallest_chi2 = 99999;
    TString smallest_cat = "";
    // float smallest_dR = 999999;

    for(auto index:hyp_catmap){
      float current_chi2 = index.second->discriminator((std::string)index.first);
      if(current_chi2 < smallest_chi2){
	smallest_chi2 = current_chi2;
	smallest_cat = index.first;
	// smallest_dR = deltaR(index.second->higgs_jets().at(0),index.second->higgs_jets().at(1));
      }
    }
    

    
    float chi2 = hyp->discriminator("Chi2");
    float chi2_catma60 = hyp_catma60->discriminator("Chi2_top+ma60");
    float chi2_catma90 = hyp_catma90->discriminator("Chi2_top+ma90");
    float chi2_catma175 = hyp_catma175->discriminator("Chi2_top+ma175");
    float chi2_catma300 = hyp_catma300->discriminator("Chi2_top+ma300");
    float chi2h = hyp->discriminator("Chi2_higgs");
    float dR_bH_bH = deltaR(hyp->higgs_jets().at(0),hyp->higgs_jets().at(1));
    LorentzVector W = (hyp->lepton_v4() + hyp->neutrino_v4());
    float dR_bt_Wt = deltaR(W, hyp->toplep_jets().at(0));
    float dR_t_H = deltaR(hyp->toplep_v4(), hyp->higgs_v4());

    /// prio cat ma=125
    //    if(chi2<10&& chi2h<2 && dR_bH_bH < 1.7) {
    // if(chi2<10&& chi2h<2) {
    //   smallest_cat = "Chi2";
    //   smallest_chi2 = chi2;
    //   smallest_dR = dR_bH_bH;
    // }

    event.set(h_best_cat,smallest_cat);

    //////// first without cuts ...
    if(smallest_cat=="Chi2_top+ma60"){
      if(is_much)  fill_histograms(event,"reco_catma60_much_sr");
      else  fill_histograms(event,"reco_catma60_ech_sr");
    }
    else if(smallest_cat=="Chi2_top+ma90"){
      if(is_much)  fill_histograms(event,"reco_catma90_much_sr");
      else  fill_histograms(event,"reco_catma90_ech_sr");
    }
    else if(smallest_cat=="Chi2_top+ma175"){
      if(is_much)  fill_histograms(event,"reco_catma175_much_sr");
      else  fill_histograms(event,"reco_catma175_ech_sr");
    }
    else if(smallest_cat=="Chi2_top+ma300"){
      if(is_much)  fill_histograms(event,"reco_catma300_much_sr");
      else  fill_histograms(event,"reco_catma300_ech_sr");
    }
    else if(smallest_cat=="Chi2"){
      if(is_much)  fill_histograms(event,"reco_chi2h_2_much_sr");
      else  fill_histograms(event,"reco_chi2h_2_ech_sr");
    }

    //////////// ..... than with cuts
    //    if(smallest_cat=="Chi2_top+ma60" && smallest_chi2 < 10 && smallest_dR < 1.7){
    if(smallest_cat=="Chi2_top+ma60" && smallest_chi2 < 10 ){
      if(is_much)  fill_histograms(event,"catma60_much_sr");
      else  fill_histograms(event,"catma60_ech_sr");
      
      if(smallest_chi2 < 3){
	if(is_much)  fill_histograms(event,"chi2_3_catma60_much_sr");
	else  fill_histograms(event,"chi2_3_catma60_ech_sr");
      }	
      //    }else if(smallest_cat=="Chi2_top+ma90" && smallest_chi2 < 10 && smallest_dR < 1.7){
    }else if(smallest_cat=="Chi2_top+ma90" && smallest_chi2 < 10){
      if(is_much) fill_histograms(event,"catma90_much_sr");
      else  fill_histograms(event,"catma90_ech_sr");

      if(smallest_chi2 < 3){
	if(is_much)  fill_histograms(event,"chi2_3_catma90_much_sr");
	else  fill_histograms(event,"chi2_3_catma90_ech_sr");
      }	
	
      //    }else if(smallest_cat=="Chi2_top+ma175" && smallest_chi2 < 10 && smallest_dR < 1.7){
    }else if(smallest_cat=="Chi2_top+ma175" && smallest_chi2 < 10){
      if(is_much)  fill_histograms(event,"catma175_much_sr");
      else  fill_histograms(event,"catma175_ech_sr");
	
      if(smallest_chi2 < 3){
	if(is_much)  fill_histograms(event,"chi2_3_catma175_much_sr");
	else  fill_histograms(event,"chi2_3_catma175_ech_sr");
      }	

    }else if(smallest_cat=="Chi2_top+ma300" && smallest_chi2 < 10){
      if(is_much)  fill_histograms(event,"catma300_much_sr");
      else  fill_histograms(event,"catma300_ech_sr");

      if(smallest_chi2 < 3){
	if(is_much)  fill_histograms(event,"chi2_3_catma300_much_sr");
	else  fill_histograms(event,"chi2_3_catma300_ech_sr");
      }	

      //    }else if(smallest_cat=="Chi2"  && smallest_chi2 < 10 && chi2h<2 && smallest_dR < 1.7){
    }else if(smallest_cat=="Chi2"  && smallest_chi2 < 10 && chi2h<2){

      bool chi2_10_sr = (chi2 < 10.);
      bool chi2h_05_sr = (chi2h < 5.);
      bool chi2h_02_sr = (chi2h < 2.);
      
      if(is_much){
	fill_histograms(event,"chi2h_2_much_sr");
	//	  // h_PDF_variations_much_sr->fill(event);
      }
      else{
	fill_histograms(event,"chi2h_2_ech_sr");
	//	  // h_PDF_variations_ech_sr->fill(event);
      }

      if(smallest_chi2 < 3){
	if(is_much)  fill_histograms(event,"chi2_3_chi2h_2_much_sr");
	else  fill_histograms(event,"chi2_3_chi2h_2_ech_sr");
      }	

    }else{
      if(is_much){
	fill_histograms(event,"chi2h_2_much_cr");
	// h_PDF_variations_much_cr->fill(event);
      }
      else{
	fill_histograms(event,"chi2h_2_much_cr");
	// h_PDF_variations_ech_cr->fill(event);
      }
    }

    event.set(h_best_cat,"Chi2_top+ma60");
    if(is_much)  fill_histograms(event,"best_catma60_much_sr");
    else  fill_histograms(event,"best_catma60_ech_sr");
    if(chi2_catma60 < 25 ){
      if(is_much)  fill_histograms(event,"best_chi2_25_catma60_much_sr");
      else  fill_histograms(event,"best_chi2_25_catma60_ech_sr");
      if(chi2_catma60 < 10 ){
	if(is_much)  fill_histograms(event,"best_chi2_10_catma60_much_sr");
	else  fill_histograms(event,"best_chi2_10_catma60_ech_sr");
	if(chi2_catma60 < 3 ){
	  if(is_much)  fill_histograms(event,"best_chi2_3_catma60_much_sr");
	  else  fill_histograms(event,"best_chi2_3_catma60_ech_sr");
	}
      }
    }


    event.set(h_best_cat,"Chi2_top+ma90");
    if(is_much)  fill_histograms(event,"best_catma90_much_sr");
    else  fill_histograms(event,"best_catma90_ech_sr");
    if(chi2_catma90 < 25 ){
      if(is_much)  fill_histograms(event,"best_chi2_25_catma90_much_sr");
      else  fill_histograms(event,"best_chi2_25_catma90_ech_sr");
      if(chi2_catma90 < 10 ){
	if(is_much)  fill_histograms(event,"best_chi2_10_catma90_much_sr");
	else  fill_histograms(event,"best_chi2_10_catma90_ech_sr");
	if(chi2_catma90 < 3 ){
	  if(is_much)  fill_histograms(event,"best_chi2_3_catma90_much_sr");
	  else  fill_histograms(event,"best_chi2_3_catma90_ech_sr");
	}
      }
    }


    event.set(h_best_cat,"Chi2_top+ma175");
    if(is_much)  fill_histograms(event,"best_catma175_much_sr");
    else  fill_histograms(event,"best_catma175_ech_sr");
    if(chi2_catma175 < 25 ){
      if(is_much)  fill_histograms(event,"best_chi2_25_catma175_much_sr");
      else  fill_histograms(event,"best_chi2_25_catma175_ech_sr");
      if(chi2_catma175 < 10 ){
	if(is_much)  fill_histograms(event,"best_chi2_10_catma175_much_sr");
	else  fill_histograms(event,"best_chi2_10_catma175_ech_sr");
	if(chi2_catma175 < 3 ){
	  if(is_much)  fill_histograms(event,"best_chi2_3_catma175_much_sr");
	  else  fill_histograms(event,"best_chi2_3_catma175_ech_sr");
	}
      }
    }

    event.set(h_best_cat,"Chi2_top+ma300");
    if(is_much)  fill_histograms(event,"best_catma300_much_sr");
    else  fill_histograms(event,"best_catma300_ech_sr");
    if(chi2_catma300 < 25 ){
      if(is_much)  fill_histograms(event,"best_chi2_25_catma300_much_sr");
      else  fill_histograms(event,"best_chi2_25_catma300_ech_sr");
      if(chi2_catma300 < 10 ){
	if(is_much)  fill_histograms(event,"best_chi2_10_catma300_much_sr");
	else  fill_histograms(event,"best_chi2_10_catma300_ech_sr");
	if(chi2_catma300 < 3 ){
	  if(is_much)  fill_histograms(event,"best_chi2_3_catma300_much_sr");
	  else  fill_histograms(event,"best_chi2_3_catma300_ech_sr");
	}
      }
    }

    event.set(h_best_cat,"Chi2");
    if(is_much)  fill_histograms(event,"best_chi2h_2_much_sr");
    else  fill_histograms(event,"best_chi2h_2_ech_sr");
    if(chi2 < 25 ){
      if(is_much)  fill_histograms(event,"best_chi2_25_chi2h_2_much_sr");
      else  fill_histograms(event,"best_chi2_25_chi2h_2_ech_sr");
      if(chi2 < 10 ){
	if(is_much)  fill_histograms(event,"best_chi2_10_chi2h_2_much_sr");
	else  fill_histograms(event,"best_chi2_10_chi2h_2_ech_sr");
	if(chi2 < 3 ){
	  if(is_much)  fill_histograms(event,"best_chi2_3_chi2h_2_much_sr");
	  else  fill_histograms(event,"best_chi2_3_chi2h_2_ech_sr");
	}
      }
    }


    event.set(h_best_cat,smallest_cat);
    
    
    // pdf_weight_producer->process(event);
    event.set(h_eventweight_final, event.weight);
    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(SingleTthAnalysisModule)
}
