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
  book<TH1D>("Tprime_PDGID_daugthers", "absolt PDG IDs of the two daugthers", 30, 0, 30);

  // Hist of decay products
  book<TH1D>("Higgs_mass", "Gen Mass of Higgs/A", 150, 0, 300);
  book<TH1D>("Top_mass", "Gen Mass of Top", 20, 100, 300);

  book<TH1D>("Higgs_pt", "Gen pT of Higgs/A", 100, 0, 1000);
  book<TH1D>("Top_pt", "Gen pT of Top", 100, 0, 1000);

  book<TH1D>("invMass_Ht", "invMass of Higgs and top", 200, 0, 2000);

  // forward jet kinematics
  book<TH1D>("forwardjet_pt", "Gen pT of forward jet", 100, 0, 1000);

}


void SingleTthGenInfoHists::fill(const Event & event){

  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  std::vector<GenParticle>* genp = event.genparticles;
  GenParticle tprime;
  int Ngenp = genp->size();

  for(auto gen:*genp){
    if(abs(gen.pdgId()) == 8000001) {
      hist("Tprime_mass") -> Fill((gen.v4()).M(),weight);
      hist("Tprime_PDGID_daugthers")->Fill(abs(event.genparticles->at(gen.daughter1()).pdgId()),weight);
      hist("Tprime_PDGID_daugthers")->Fill(abs(event.genparticles->at(gen.daughter2()).pdgId()),weight);
      tprime = gen;
      break;
    }
  }

  GenParticle Higgs;
  GenParticle Top;

  if(abs(event.genparticles->at(tprime.daughter1()).pdgId())==25) Higgs = event.genparticles->at(tprime.daughter1());
  else Higgs = event.genparticles->at(tprime.daughter2());

  if(abs(event.genparticles->at(tprime.daughter1()).pdgId())==6) Top = event.genparticles->at(tprime.daughter1());
  else Top = event.genparticles->at(tprime.daughter2());


  // Hists for decay products
  hist("Higgs_mass")->Fill(Higgs.v4().M(), weight);
  hist("Higgs_pt")->Fill(Higgs.pt(), weight);

  hist("Top_mass")->Fill(Top.v4().M(), weight);
  hist("Top_pt")->Fill(Top.pt(), weight);
 
  hist("invMass_Ht")->Fill((Higgs.v4()+Top.v4()).M(),weight);


  // Hists forward kinematics
  // hist("forwardjet_pt")->Fill();



} 


SingleTthGenInfoHists::~SingleTthGenInfoHists(){}
