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



class MuonTriggerWeights: public uhh2::AnalysisModule{

 public:
  explicit MuonTriggerWeights(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path;
  Year year;
  std::unique_ptr<TGraphAsymmErrors> g_sf_30to50, g_sf_50to100, g_sf_100to200, g_sf_200toinf;
  uhh2::Event::Handle<float> h_muon_weight, h_muon_weight_up, h_muon_weight_down;

};



class ElectronTriggerWeights: public uhh2::AnalysisModule{

 public:
  explicit ElectronTriggerWeights(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path;
  Year year;
  double pt_split;
  std::unique_ptr<TGraphAsymmErrors> g_sf_low, g_sf_high;
  uhh2::Event::Handle<float> h_elec_weight, h_elec_weight_up, h_elec_weight_down;

};


class MuonTriggerWeightsOffical: public uhh2::AnalysisModule{

 public:
  explicit MuonTriggerWeightsOffical(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path;
  Year year;
  std::unique_ptr<TGraphAsymmErrors> g_sf_eta0p9to1p2, g_sf_eta0to0p9, g_sf_eta1p2to2p1, g_sf_eta2p1to2p4;
  uhh2::Event::Handle<float> h_muon_weight, h_muon_weight_up, h_muon_weight_down;

};

class ElectronTriggerWeightsOfficial: public uhh2::AnalysisModule{

 public:
  explicit ElectronTriggerWeightsOfficial(uhh2::Context & ctx, TString path_, Year year_);
  virtual bool process(uhh2::Event & event) override;

 private:
  TString path;
  Year year;
  double pt_split;
  std::unique_ptr<TGraphAsymmErrors> g_sf_eta0p9to1p2, g_sf_eta0to0p9, g_sf_eta1p2to2p1, g_sf_eta2p1to2p4;
  uhh2::Event::Handle<float> h_elec_weight, h_elec_weight_up, h_elec_weight_down;

};
