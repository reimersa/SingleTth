#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesis.h"
#include "UHH2/SingleTth/include/SingleTthGen.h"
#include "UHH2/common/include/PDFWeights.h"


namespace uhh2examples {

  /**  \brief Example class for booking and filling histograms
  *
  * NOTE: This class uses the 'hist' method to retrieve histograms.
  * This requires a string lookup and is therefore slow if you have
  * many histograms. Therefore, it is recommended to use histogram
  * pointers as member data instead, like in 'common/include/ElectronHists.h'.
  */
  class SingleTthPDFHists: public uhh2::Hists {
  public:
    // use the same constructor arguments as Hists for forwarding:
    SingleTthPDFHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    uhh2::Event::Handle<std::vector<SingleTthReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<bool> h_is_tprime_reco;
    uhh2::Event::Handle<TString> h_best_cat;
    std::vector<TString> histo_names1, histo_names2;
    // bool is_LO;
    bool is_mc, take_ntupleweights;
    TString m_oname;
    std::unique_ptr<PDFWeights> m_pdfweights;
    Year year;
    // std::vector<uhh2::Event::Handle<float>> h_pdfweights;
    // uhh2::Event::Handle<float> h_pdfweights;


    virtual ~SingleTthPDFHists();
  };

}
