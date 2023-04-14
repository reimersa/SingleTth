#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesis.h"


namespace uhh2examples {

  class SingleTthHists: public uhh2::Hists {
  public:
    // use the same constructor arguments as Hists for forwarding:
    SingleTthHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    uhh2::Event::Handle<std::vector<SingleTthReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<bool> h_is_tprime_reco;
    uhh2::Event::Handle<TString> h_best_cat;
    bool is_mc;
    bool is_signal_sample;
    TH2F* eta_phi_map;
    TH2F* MTPrime_vs_MA;
    virtual ~SingleTthHists();
  };

}
