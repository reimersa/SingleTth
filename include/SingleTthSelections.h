#pragma once
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"
#include "UHH2/common/include/JetIds.h"

namespace uhh2examples {
    
/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */
class DijetSelection: public uhh2::Selection {
public:
    DijetSelection(float dphi_min = 2.7f, float third_frac_max = 0.2f);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float dphi_min, third_frac_max;
};

class HEMIssueSelection: public uhh2::Selection {
public:
    HEMIssueSelection(JetId btag);
    virtual bool passes(const uhh2::Event & event) override;
private:
    JetId btag; 
};

class JetLeptonOverlapRemoval: public uhh2::AnalysisModule {
   public:
      explicit JetLeptonOverlapRemoval(double deltaRmax);
      virtual bool process(uhh2::Event & event) override;
      
   private:
      double deltaRmin_;
 };

 class LowestChi2Value: public uhh2::AnalysisModule{
 public:
   explicit LowestChi2Value(uhh2::Context & ctx,std::vector<double> chi2_values_);
   virtual bool process(uhh2::Event & event) override;

 private:
   std::vector<double> chi2_values;
   uhh2::Event::Handle< double > h_lowest_chi2;
   uhh2::Event::Handle< int > h_lowest_chi2_index;
 };
}

