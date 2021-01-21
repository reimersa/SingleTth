#include "UHH2/SingleTth/include/SingleTthGenInfoHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include <math.h>

#include "TH1F.h"
#include "TH1D.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

SingleTthGenInfoHists::SingleTthGenInfoHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here

  // Hists for TPrime
  book<TH1D>("Tprime_mass", "Gen Mass of Tprime", 200, 0, 2000);
  book<TH1D>("Tprime_PDGID_daughters", "absolut PDG IDs of the two daughters", 30, 0, 30);

  // Hist of decay products
  book<TH1D>("Higgs_mass", "Gen Mass of Higgs/A", 200, 0, 600);
  book<TH1D>("Top_mass", "Gen Mass of Top", 20, 100, 300);

  book<TH1D>("Higgs_pt", "Gen pT of Higgs/A", 100, 0, 1000);
  book<TH1D>("Top_pt", "Gen pT of Top", 100, 0, 1000);

  book<TH1D>("Higgs_PDGID_daughters", "PDG IDs of the two daughters of the Higgs", 12, -6, 6);

  book<TH1D>("invMass_Ht", "invMass of Higgs and top", 200, 0, 2000);

  // forward jet kinematics
  book<TH1D>("forwardjet_pt", "Gen pT of forward jet", 100, 0, 1000);
  book<TH1D>("forwardjet_eta", "Gen eta of forward jet", 100, -5, 5);
  book<TH1D>("forwardjet_eta_ptg30", "Gen eta of forward jet with pT > 30 GeV", 100, -5, 5);

  book<TH1D>("bquarkjet_pt", "Gen pT of bquark jet", 100, 0, 1000);
  book<TH1D>("bquarkjet_eta", "Gen eta of bquark jet", 100, -5, 5);
  book<TH1D>("bquarkjet_eta_ptg30", "Gen eta of bquark jet with pT > 30 GeV", 100, -5, 5);

  //// H->bb kinematics
  book<TH1D>("Higgs_bquark_pt", "Gen pT of bquark from Higgs", 100, 0, 1000);
  book<TH1D>("Higgs_antibquark_pt", "Gen pT of antibquark from Higgs", 100, 0, 1000);
  book<TH1D>("DeltaR_bantib_Higgs", "Delta R between b and antib from Higgs", 100, 0, 4);


}


void SingleTthGenInfoHists::fill(const Event & event){

  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  std::vector<GenParticle>* genp = event.genparticles;
  GenParticle tprime;
  GenParticle forwardjet;
  GenParticle bquarkjet;
  int Ngenp = genp->size();

  for(auto gen:*genp){
    if(abs(gen.pdgId()) == 8000001) {
      hist("Tprime_mass") -> Fill((gen.v4()).M(),weight);
      int  daughter1 = gen.daughter1();
      int daughter2 = gen.daughter2();

      hist("Tprime_PDGID_daughters")->Fill(abs(event.genparticles->at(daughter1-1).pdgId()),weight);
      hist("Tprime_PDGID_daughters")->Fill(abs(event.genparticles->at(daughter2-1).pdgId()),weight);
      tprime = gen;
    }
    if(abs(gen.pdgId())<5 && gen.mother1() <3 && gen.mother2() <3 && gen.status() ==1){
      forwardjet = gen;
    }
    if(abs(gen.pdgId())==5 && gen.mother1() <3 && gen.mother2() <3 && gen.status() ==1){
      bquarkjet = gen;
    }
  }

  GenParticle Higgs;
  GenParticle Top;

  if(abs(event.genparticles->at(tprime.daughter1()-1).pdgId())==25) Higgs = event.genparticles->at(tprime.daughter1()-1);
  else Higgs = event.genparticles->at(tprime.daughter2()-1);

  if(abs(event.genparticles->at(tprime.daughter1()-1).pdgId())==6) Top = event.genparticles->at(tprime.daughter1()-1);
  else Top = event.genparticles->at(tprime.daughter2()-1);


  // Hists for decay products
  hist("Higgs_mass")->Fill(Higgs.v4().M(), weight);
  hist("Higgs_pt")->Fill(Higgs.pt(), weight);
  GenParticle Hdaughter1 = event.genparticles->at(Higgs.daughter1()-1);
  GenParticle Hdaughter2 = event.genparticles->at(Higgs.daughter2()-1);
  hist("Higgs_PDGID_daughters")->Fill(Hdaughter1.pdgId(),weight);
  hist("Higgs_PDGID_daughters")->Fill(Hdaughter2.pdgId(),weight);

  if (Hdaughter1.pdgId() < 0) {
    hist("Higgs_antibquark_pt")->Fill(Hdaughter1.pt(),weight);
    hist("Higgs_bquark_pt")->Fill(Hdaughter2.pt(),weight);
  }else{
    hist("Higgs_antibquark_pt")->Fill(Hdaughter2.pt(),weight);
    hist("Higgs_bquark_pt")->Fill(Hdaughter1.pt(),weight);
  }

  hist("DeltaR_bantib_Higgs")->Fill(deltaR(Hdaughter1.v4(),Hdaughter2.v4()),weight);

  hist("Top_mass")->Fill(Top.v4().M(), weight);
  hist("Top_pt")->Fill(Top.pt(), weight);
 
  hist("invMass_Ht")->Fill((Higgs.v4()+Top.v4()).M(),weight);


  // Hists forward kinematics
  hist("forwardjet_pt")->Fill(forwardjet.pt());
  hist("forwardjet_eta")->Fill(forwardjet.eta());
  if(forwardjet.pt()>30) hist("forwardjet_eta_ptg30")->Fill(forwardjet.eta());


  // Hists bquark kinematics
  hist("bquarkjet_pt")->Fill(bquarkjet.pt());
  hist("bquarkjet_eta")->Fill(bquarkjet.eta());
  if(bquarkjet.pt()>30) hist("bquarkjet_eta_ptg30")->Fill(bquarkjet.eta());



} 


SingleTthGenInfoHists::~SingleTthGenInfoHists(){}
