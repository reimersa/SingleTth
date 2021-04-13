#include "UHH2/SingleTth/include/SingleTthSelections.h"
#include "UHH2/core/include/Event.h"


#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}
    
bool DijetSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 2) return false;
    const auto & jet0 = event.jets->at(0);
    const auto & jet1 = event.jets->at(1);
    auto dphi = deltaPhi(jet0, jet1);
    if(dphi < dphi_min) return false;
    if(event.jets->size() == 2) return true;
    const auto & jet2 = event.jets->at(2);
    auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
    return third_jet_frac < third_frac_max;
}


HEMIssueSelection::HEMIssueSelection(JetId btag_):btag(btag_){}
    
bool HEMIssueSelection::passes(const Event & event){
  std::vector<Jet> btags;
  for (unsigned int i =0; i<event.jets->size(); ++i) {
    if(btag(event.jets->at(i),event)) btags.push_back(event.jets->at(i));
  }
  
  for(Jet jet:btags){
    if(-3.2 < jet.eta()  &&  jet.eta() < -1.3 && -1.57<jet.phi()  &&  jet.phi() < -0.87 ) return false;
  }

  if(event.electrons ->size()){
    double ele_eta = event.electrons ->at(0).eta();
    double ele_phi = event.electrons ->at(0).phi();
    if(-3.2 < ele_eta  &&  ele_eta < -1.3 && -1.57<ele_phi  &&  ele_phi < -0.87) return false;
  }

  return true;

}


JetLeptonOverlapRemoval::JetLeptonOverlapRemoval(double deltaRmin):
deltaRmin_(deltaRmin){}

bool JetLeptonOverlapRemoval::process(Event & event){
   
   assert(event.muons);
   Particle lepton;
   std::vector<Jet> result;
   if(event.muons->size()){

     lepton =event.muons->at(0);
   }else{
     lepton =event.electrons->at(0);
   }
   for(const auto & jet : *event.jets){
     if(deltaR(jet, lepton) > deltaRmin_){
       result.push_back(jet);
     }
   }
   std::swap(result, *event.jets);
   
   return true;
}


LowestChi2Value::LowestChi2Value(Context & ctx,std::vector<double> chi2_values_):chi2_values(chi2_values_) {
  h_lowest_chi2= ctx.declare_event_output< double > ("lowest_Chi2");
  h_lowest_chi2_index= ctx.declare_event_output< int > ("lowest_Chi2_index");
}

bool LowestChi2Value::process(Event & event){
  double lowest_chi2 = 9999;
  double index = 9999;

  for(int i = 0; i<chi2_values.size(); i++){
    std::cout<<"chi2 value "<< chi2_values.at(i) <<std::endl;
    std::cout<<"index "<< i <<std::endl;
    if(chi2_values.at(i) < lowest_chi2) {
      lowest_chi2 = chi2_values.at(i);
      index = i;
    }
  }
  /// set handle with lowest chi2 and index of lowest chi2
  event.set(h_lowest_chi2,lowest_chi2);
  event.set(h_lowest_chi2_index,index);
  return true;
}
