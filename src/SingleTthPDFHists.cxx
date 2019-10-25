#include "UHH2/SingleTth/include/SingleTthPDFHists.h"
#include "UHH2/SingleTth/include/SingleTthReconstruction.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesis.h"
#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/JetIds.h"
#include <math.h>
#include <sstream>

#include "TH1F.h"
#include "TH2D.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;


SingleTthPDFHists::SingleTthPDFHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

  is_mc = ctx.get("dataset_type") == "MC";

  h_hyps           = ctx.get_handle<std::vector<SingleTthReconstructionHypothesis>>("TprimeHypotheses");
  h_is_tprime_reco = ctx.get_handle<bool>("is_tprime_reco");
  m_oname = ctx.get("dataset_version");
  TString m_pdfname = "NNPDF30_lo_as_0130";
  // TString weightpath = ctx.get("PDFWeightPath");
  // cout << "File: " << weightpath+m_oname << endl;


  //take ntupleweights if
  //1) use_ntupleweights = true and the sample has ntupleweights stored

  //take weights from txt files if
  //1) the sample is LO (and doesn't have ntupleweights for that reason) (this assumption is protected by a runtime_error later)
  //2) the sample is NLO and yet doesn't have ntupleweights

  take_ntupleweights = !(m_oname.Contains("VLQ") || m_oname.Contains("Diboson") || m_oname.Contains("WJets") || m_oname.Contains("QCD"));
  // take_ntupleweights =  use_ntupleweights && (!is_LO || m_oname.Contains("DYJets")) && (m_oname != "SingleTop_T_tWch" && m_oname != "SingleTop_Tbar_tWch");

  cout << "Are ntupleweights taken for this sample?: " << take_ntupleweights << endl;

  if(is_mc && !take_ntupleweights){
    // Only take shape effect into account for signals
    // if(m_oname.Contains("VLQ")) m_pdfweights.reset(new PDFWeights(m_pdfname,weightpath+m_oname));
    // else m_pdfweights.reset(new PDFWeights(m_pdfname));

    // Also take normalization into account for signals, i.e. treat them like LO backgrounds
    m_pdfweights.reset(new PDFWeights(m_pdfname));
  }



  for(int i=0; i<100; i++){
    TString name1 = "M_Tprime_PDF_";
    name1 += i+1;
    TString name2 = "M_Tprime_rebinlimit_PDF_";
    name2 += i+1;

    TString title1 = "M_{T} [GeV] (fine binning) for PDF No. ";
    title1 += i+1;
    TString title2 = "M_{T} [GeV] (limit binning) for PDF No. ";;
    title2 += i+1;

    histo_names1.emplace_back(name1);
    histo_names2.emplace_back(name2);



    vector<double> bins_limits = {0, 300, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1200, 3000};
    book<TH1F>(name1, title1, 300, 0, 3000);
    book<TH1F>(name2, title2, bins_limits.size()-1, &bins_limits[0]);

    TString pdfvarname = "weight_pdf_";
    pdfvarname += i+1;
    string s_pdfvarname = (string)pdfvarname;
    // uhh2::Event::Handle<float> handle = ctx.declare_event_output<float>(s_pdfvarname);
    // h_pdfweights.emplace_back(handle);
  }
  // h_pdfweights = ctx.declare_event_output<float>("test");

}

void SingleTthPDFHists::fill(const Event & event){
  double weight = event.weight;

  if(!is_mc) return;

  if(event.genInfo->systweights().size() == 0 && take_ntupleweights) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is empty but ntupleweights shall be taken. Is this correct? In this case add exception to take_ntupleweights.");
  if(event.genInfo->systweights().size() != 0 && (!take_ntupleweights)) throw runtime_error("In SingleTthPDFHists.cxx: Systweights in event.genInfo() is NOT empty but take_ntupleweights is set to 'false'. Is this correct? In this case Thomas says the genInfo weight should be used. Add this sample to take_ntupleweights");


  bool is_tprime_reco = event.get(h_is_tprime_reco);
  if(!is_tprime_reco) throw runtime_error("In SingleTthPDFHists.cxx: Tprime mass has not been reconstructed.");

  vector<SingleTthReconstructionHypothesis> hyps = event.get(h_hyps);
  const SingleTthReconstructionHypothesis* hyp = get_best_hypothesis( hyps, "Chi2" );

  double m_tprime = 0.;
  if( (hyp->Tprime_v4()).isTimelike() ) {m_tprime = (hyp->Tprime_v4()).M();}
  else {m_tprime = sqrt( -(hyp->Tprime_v4()).mass2());}

  if(take_ntupleweights){
    for(int i=0; i<100; i++){

      double pdf_weight = event.genInfo->systweights().at(i+9);
      double fillweight = weight * pdf_weight/event.genInfo->originalXWGTUP();
      TString name1 = histo_names1[i];
      TString name2 = histo_names2[i];

      hist(name1)->Fill(m_tprime,fillweight);
      hist(name2)->Fill(m_tprime,fillweight);
      // event.set(h_pdfweights[i], fillweight);
    }
    // event.set(h_pdfweights, 1.);
  }
  else{
    std::vector<double> weights = m_pdfweights->GetWeightList(event);
    for(int i=0; i<100; i++){
      double fillweight = weight*weights[i];
      TString name1 = histo_names1[i];
      TString name2 = histo_names2[i];

      hist(name1)->Fill(m_tprime,fillweight);
      hist(name2)->Fill(m_tprime,fillweight);
      // event.set(h_pdfweights[i], fillweight);
    }
    // event.set(h_pdfweights, 1.);

  }



}

SingleTthPDFHists::~SingleTthPDFHists(){}
