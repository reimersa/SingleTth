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

  return true;

}


