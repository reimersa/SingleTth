#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetIds.h"
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include "LHAPDF/LHAPDF.h"
#include "TSystem.h"
#include "UHH2/common/include/PDFWeights.h"



class PDFWeightHandleProducer: public uhh2::AnalysisModule{

public:
  explicit PDFWeightHandleProducer(uhh2::Context & ctx);
  virtual bool process(uhh2::Event & event) override;

private:
  std::vector<uhh2::Event::Handle<float>> h_pdfweights;
  bool is_mc, take_ntupleweights;
  TString m_oname;
  std::unique_ptr<PDFWeights> m_pdfweights;
};


class ElectronTriggerWeights: public uhh2::AnalysisModule{

public:
  explicit ElectronTriggerWeights(uhh2::Context & ctx, TString path_, TString SysDirection_);
  virtual bool process(uhh2::Event & event) override;

private:
  TString path, SysDirection;
  std::unique_ptr<TGraphAsymmErrors> Eff_lowpt_MC, Eff_lowpt_DATA, Eff_highpt_MC, Eff_highpt_DATA;
  uhh2::Event::Handle<float> h_elec_weight, h_elec_weight_up, h_elec_weight_down;
};
