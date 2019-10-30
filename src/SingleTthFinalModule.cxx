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

  class SingleTthFinalModule: public ModuleBASE {
  public:

    explicit SingleTthFinalModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void book_pdf_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string, bool, bool);

  private:
    unique_ptr<HighMassSingleTthReconstruction> tprime_reco;
    unique_ptr<SingleTthChi2Discriminator> tprime_chi2;

    bool is_mc, is_much;
    TString dataset_version;

    vector<TString> handlenames, systnames, systshift, systshift_scale;
    vector<string> histogramtags, histogramtags_scale, histogramtags_pdf;

    uhh2::Event::Handle<float> eventweight_nominal;
    vector<uhh2::Event::Handle<float>> systweight_handles, systweight_scale_handles, scalefactor_handles;

    uhh2::Event::Handle<bool> h_is_tprime_reco;
    uhh2::Event::Handle<std::vector<SingleTthReconstructionHypothesis>> h_hyps;
  };

  void SingleTthFinalModule::book_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = "chi2h_2_ech_sr_" + tag;
      book_HFolder(mytag, new SingleTthHists(ctx,mytag));
      mytag = "chi2h_2_much_sr_" + tag;
      book_HFolder(mytag, new SingleTthHists(ctx,mytag));
      mytag = "chi2h_2_ech_cr_" + tag;
      book_HFolder(mytag, new SingleTthHists(ctx,mytag));
      mytag = "chi2h_2_much_cr_" + tag;
      book_HFolder(mytag, new SingleTthHists(ctx,mytag));
    }
  }

  void SingleTthFinalModule::book_pdf_histograms(uhh2::Context& ctx, vector<string> tags){
    for(const auto & tag : tags){
      cout << "booking histograms with tag " << tag << endl;
      string mytag = "chi2h_2_ech_sr_" + tag;
      book_HFolder(mytag, new SingleTthPDFHists(ctx,mytag));
      mytag = "chi2h_2_much_sr_" + tag;
      book_HFolder(mytag, new SingleTthPDFHists(ctx,mytag));
      mytag = "chi2h_2_ech_cr_" + tag;
      book_HFolder(mytag, new SingleTthPDFHists(ctx,mytag));
      mytag = "chi2h_2_much_cr_" + tag;
      book_HFolder(mytag, new SingleTthPDFHists(ctx,mytag));
    }
  }

  void SingleTthFinalModule::fill_histograms(uhh2::Event& event, string tag, bool is_sr, bool is_much){
    string mytag = "chi2h_2_";
    if(is_much) mytag += "much_";
    else        mytag += "ech_";

    if(is_sr)   mytag += "sr_";
    else        mytag += "cr_";
    mytag += tag;
    HFolder(mytag)->fill(event);
  }


  SingleTthFinalModule::SingleTthFinalModule(Context & ctx){

    cout << "Hello World from SingleTthFinalModule!" << endl;
    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    is_mc = ctx.get("dataset_type") == "MC";
    dataset_version = ctx.get("dataset_version");

    JetId DeepjetTight = BTag(BTag::DEEPJET, BTag::WP_TIGHT);
    tprime_reco.reset(new HighMassSingleTthReconstruction(ctx, SingleTthNeutrinoReconstruction, DeepjetTight));
    tprime_chi2.reset(new SingleTthChi2Discriminator(ctx));
    h_is_tprime_reco = ctx.get_handle<bool>("is_tprime_reco");
    h_hyps = ctx.get_handle<vector<SingleTthReconstructionHypothesis>>("TprimeHypotheses");

    eventweight_nominal = ctx.get_handle<float>("eventweight_final");

    // Easy systematics
    systnames = {"muid", "pu", "eleid", "elereco", "eletrigger", "muiso", "mutrigger", "btag_bc", "btag_udsg"};
    handlenames = {"weight_sfmu_id", "weight_pu", "weight_sfelec_id", "weight_sfelec_reco", "weight_sfelec_trigger", "weight_sfmu_iso", "weight_sfmu_trigger", "weight_btag" , "weight_btag"};
    systshift = {"up", "down"};
    if(systnames.size() != handlenames.size()) throw runtime_error("In SingleTthFinalModule.cxx: Length of systnames and handlenames is not equal.");

    histogramtags = {};
    for(unsigned int i=0; i<systnames.size(); i++){
      for(unsigned int j=0; j<systshift.size(); j++){


        TString handlename = handlenames[i] + "_" + systshift[j];
        TString sf_name = handlenames[i];

        // B-tagging uncertainties require a special treatment
        if(systnames[i] == "btag_bc"){
          handlename = handlenames[i] + "_bc_" + systshift[j];
        }
        else if(systnames[i] == "btag_udsg"){
          handlename = handlenames[i] + "_udsg_" + systshift[j];
        }

        uhh2::Event::Handle<float> handle1 = ctx.declare_event_output<float>((string)handlename);
        uhh2::Event::Handle<float> handle2 = ctx.declare_event_output<float>((string)sf_name);
        systweight_handles.emplace_back(handle1);
        scalefactor_handles.emplace_back(handle2);

        TString histname = systnames[i] + "_" + systshift[j];
        histogramtags.emplace_back(histname);
      }
    }

    histogramtags.emplace_back("nominal");

    //book all the histogram folders
    book_histograms(ctx, histogramtags);





    // Scale variation
    histogramtags_scale = {};
    systshift_scale = {"upup", "upnone", "noneup", "nonedown", "downnone", "downdown"};
    for(unsigned int i=0; i<systshift_scale.size(); i++){
      TString handlename = "weight_murmuf_" + systshift_scale[i];

      uhh2::Event::Handle<float> handle = ctx.declare_event_output<float>((string)handlename);
      systweight_scale_handles.emplace_back(handle);

      TString histname = "scale_" + systshift_scale[i];
      histogramtags_scale.emplace_back(histname);
    }

    book_histograms(ctx, histogramtags_scale);


    // Separately book one set of PDF hists (each contains the 100 variations for M_Tprime)
    book_pdf_histograms(ctx, {"pdf"});



  }


  bool SingleTthFinalModule::process(Event & event) {
    // cout << "++++++++++ NEW EVENT ++++++++++" << endl;
    event.set(h_is_tprime_reco, false);

    // MUCH or ECH?
    is_much = (event.muons->size() >= 1);

    // T' reconstruction
    tprime_reco->process(event);
    tprime_chi2->process(event);
    bool is_tprime_reco = event.get(h_is_tprime_reco);
    if(!is_tprime_reco) throw runtime_error("After reconstruction, the T still isn't reconstructed. How?");
    std::vector<SingleTthReconstructionHypothesis> hyps = event.get(h_hyps);
    const SingleTthReconstructionHypothesis* hyp = get_best_hypothesis( hyps, "Chi2" );
    float chi2 = hyp->discriminator("Chi2");
    float chi2h = hyp->discriminator("Chi2_higgs");

    // SR or CR?
    bool is_sr = chi2 < 10. && chi2h < 2.;

    // Read out nominal eventweight
    float weight_nominal = event.get(eventweight_nominal);

    // Fill histograms once with nominal weights
    event.weight = weight_nominal;
    // cout << "weight nominal: " << weight_nominal << endl;
    fill_histograms(event, "nominal", is_sr, is_much);

    // Loop over easy systematics
    for(unsigned int i=0; i<systnames.size(); i++){
      for(unsigned int j=0; j<systshift.size(); j++){

        int idx = 2*i + j;
        float systweight = event.get(systweight_handles[idx]);
        float sfweight = event.get(scalefactor_handles[idx]);
        event.weight = weight_nominal * systweight / sfweight;
        // cout << "idx: " << idx << ", systweight/sfweight: " << systweight/sfweight << endl;

        TString tag = systnames[i] + "_" + systshift[j];
        fill_histograms(event, (string)tag, is_sr, is_much);
      }
    }

    // Loop over scale systematics
    for(unsigned int j=0; j<systshift_scale.size(); j++){

      float systweight = event.get(systweight_scale_handles[j]);
      event.weight = weight_nominal * systweight;

      TString tag = "scale_" + systshift_scale[j];
      fill_histograms(event, (string)tag, is_sr, is_much);
    }

    // Fill PDF histograms
    event.weight = weight_nominal;
    fill_histograms(event, "pdf", is_sr, is_much);

    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(SingleTthFinalModule)
}
