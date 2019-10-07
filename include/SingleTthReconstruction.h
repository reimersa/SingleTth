#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesis.h"
#include "TMinuit.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/common/include/TopJetIds.h"

typedef std::function< std::vector<LorentzVector>  (const LorentzVector & lepton, const LorentzVector & met)> NeutrinoReconstructionMethod;


class HighMassSingleTthReconstruction: public uhh2::AnalysisModule {
public:

  explicit HighMassSingleTthReconstruction(uhh2::Context & ctx, const NeutrinoReconstructionMethod & neutrinofunction, JetId btag);
  virtual bool process(uhh2::Event & event) override;
  virtual ~HighMassSingleTthReconstruction();

private:
  NeutrinoReconstructionMethod m_neutrinofunction;
  uhh2::Event::Handle<std::vector<SingleTthReconstructionHypothesis>> h_recohyps;
  uhh2::Event::Handle<bool> h_is_tprime_reco;
  JetId btag;
};

std::vector<LorentzVector> SingleTthNeutrinoReconstruction(const LorentzVector & lepton, const LorentzVector & met);
