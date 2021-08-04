#include "UHH2/SingleTth/include/SingleTthReconstructionHypothesisDiscriminators.h"
#include "UHH2/core/include/Utils.h"

#include <set>

using namespace uhh2;
using namespace std;

namespace {

  // invariant mass of a lorentzVector, but save for timelike / spacelike vectors
  float inv_mass(const LorentzVector & p4){
    if(p4.isTimelike()) return p4.mass();
    else return -sqrt(-p4.mass2());
  }
}


const SingleTthReconstructionHypothesis * get_best_hypothesis(const std::vector<SingleTthReconstructionHypothesis> & hyps, const std::string & label){
  const SingleTthReconstructionHypothesis * best = nullptr;
  float current_best_disc = numeric_limits<float>::infinity();
  for(const auto & hyp : hyps){
    if(!hyp.has_discriminator(label)) continue;
    auto disc = hyp.discriminator(label);
    if(disc < current_best_disc){
      best = &hyp;
      current_best_disc = disc;
    }
  }
  if(std::isfinite(current_best_disc)){
    return best;
  }
  else{
    return nullptr;
  }
}

SingleTthChi2Discriminator::SingleTthChi2Discriminator(Context & ctx,Year year_ ,const cfg & config_): config(config_){
  h_hyps = ctx.get_handle<vector<SingleTthReconstructionHypothesis>>("TprimeHypotheses");
  year= year_;
}


bool SingleTthChi2Discriminator::process(uhh2::Event & event){
  auto & hyps = event.get(h_hyps);

  double mass_tlep = 177.;
  double mass_tlep_sigma = 30.;
  double mass_higgs = 125.;	
  double mass_higgs_sigma = 12.5;

  double mass_higgs_ma60 = 60.;	
  double mass_higgs_sigma_ma60 = 6.;

  double mass_higgs_ma90 = 90.;	
  double mass_higgs_sigma_ma90 = 9.;

  double mass_higgs_ma175 = 175.;	
  double mass_higgs_sigma_ma175 = 17.5;

  double mass_higgs_ma300 = 300.;	
  double mass_higgs_sigma_ma300 = 30.;

  // double mass_higgs = 112.5;	
  // double mass_higgs_sigma = 11.25;


  // double mass_higgs_ma60 = 54.;	
  // double mass_higgs_sigma_ma60 = 5.4;

  // double mass_higgs_ma90 = 80.;	
  // double mass_higgs_sigma_ma90 = 8.;

  // double mass_higgs_ma175 = 157.5;	
  // double mass_higgs_sigma_ma175 = 15.75;

  // double mass_higgs_ma300 = 270.;	
  // double mass_higgs_sigma_ma300 = 27.;




  // if(year == Year::is2017v2){
  //   // mass_higgs = 121.6;	
  //   // mass_higgs_sigma = 13;

  //   mass_tlep = 172.5;
  //   mass_tlep_sigma = 27;
  // }

  // if(year == Year::is2018){
  //   // mass_higgs = 120.4;	
  //   // mass_higgs_sigma = 13.;

  //   mass_tlep = 171.4;
  //   mass_tlep_sigma = 28;

  // }

  //// test crystal ball function
  double xmin_top = 50;
  double xmax_top = 1000;
  static TF1* fcb_top = new TF1("fcb_top", "[0]*ROOT::Math::crystalball_function(x, [1], [2], [3], [4])", xmin_top, xmax_top);
  fcb_top->SetNpx(200);
  fcb_top->SetParName(0, "Peak");
  fcb_top->SetParName(1, "#alpha");
  fcb_top->SetParName(2, "n");
  fcb_top->SetParName(3, "#sigma");
  fcb_top->SetParName(4, "#mu");


  if(year ==  Year::is2016v3){
    fcb_top->SetParameter(0,1);
    fcb_top->SetParameter(1,-0.3414);
    fcb_top->SetParameter(2,0.9319);
    fcb_top->SetParameter(3,30.45);
    fcb_top->SetParameter(4,177.1);
  }
  if(year ==  Year::is2017v2){
    fcb_top->SetParameter(0,1);
    fcb_top->SetParameter(1,-0.5624);
    fcb_top->SetParameter(2,2.187);
    fcb_top->SetParameter(3,27.39);
    fcb_top->SetParameter(4,172.5);
  }
  if(year ==  Year::is2018){
    fcb_top->SetParameter(0,1);
    fcb_top->SetParameter(1,-0.6324);
    fcb_top->SetParameter(2,1.903);
    fcb_top->SetParameter(3,27.77);
    fcb_top->SetParameter(4,171.4);
  }

  double in = fcb_top -> Integral(xmin_top,xmax_top,10e-3);
  //  fcb_top->SetParameter(0,1/in);


  double xmax_higgs = 400;
  double xmin_higgs = 30;
  static  TF1* fcb_higgs = new TF1("fcb_higgs", "[0]*ROOT::Math::crystalball_function(x, [1], [2], [3], [4])", xmin_higgs,xmax_higgs);
  fcb_higgs->SetNpx(200);
  fcb_higgs->SetParName(0, "Peak");
  fcb_higgs->SetParName(1, "#alpha");
  fcb_higgs->SetParName(2, "n");
  fcb_higgs->SetParName(3, "#sigma");
  fcb_higgs->SetParName(4, "#mu");


  if(year ==  Year::is2016v3){
    fcb_higgs->SetParameter(0,1);
    fcb_higgs->SetParameter(1,0.7684);
    fcb_higgs->SetParameter(2,1.159e6);
    fcb_higgs->SetParameter(3,13.04);
    fcb_higgs->SetParameter(4,122.7);
  }
  if(year ==  Year::is2017v2){
    fcb_higgs->SetParameter(0,1);
    fcb_higgs->SetParameter(1,0.7362);
    fcb_higgs->SetParameter(2,5.862e5);
    fcb_higgs->SetParameter(3,12.91);
    fcb_higgs->SetParameter(4,121.6);
  }
  if(year ==  Year::is2018){
    fcb_higgs->SetParameter(0,1);
    fcb_higgs->SetParameter(1,0.7432);
    fcb_higgs->SetParameter(2,2.919e6);
    fcb_higgs->SetParameter(3,13.36);
    fcb_higgs->SetParameter(4,120.4);
  }

  double in_H = fcb_higgs -> Integral(xmin_higgs,xmax_higgs,10e-3);
  //  fcb_higgs->SetParameter(0,1/in_H);

  /// end





  for(auto & hyp: hyps){
    double mass_tlep_rec = inv_mass(hyp.toplep_v4());
    double mass_higgs_rec = inv_mass(hyp.higgs_v4());
    double chi2_top = pow((mass_tlep_rec - mass_tlep)/(mass_tlep_sigma),2); //original chi2
    double chi2_higgs = pow((mass_higgs_rec - mass_higgs)/(mass_higgs_sigma),2); //original chi2

    double chi2_higgs_ma60 = pow((mass_higgs_rec - mass_higgs_ma60)/(mass_higgs_sigma_ma60),2); 
    double chi2_higgs_ma90 = pow((mass_higgs_rec - mass_higgs_ma90)/(mass_higgs_sigma_ma90),2); 
    double chi2_higgs_ma175 = pow((mass_higgs_rec - mass_higgs_ma175)/(mass_higgs_sigma_ma175),2); 
    double chi2_higgs_ma300 = pow((mass_higgs_rec - mass_higgs_ma300)/(mass_higgs_sigma_ma300),2); 

    // if(mass_tlep_rec > xmax_top )mass_tlep_rec = xmax_top;
    // if(mass_tlep_rec < xmin_top) mass_tlep_rec = xmin_top;
    // if(mass_higgs_rec > xmax_higgs) mass_higgs_rec = xmax_higgs;
    // if(mass_higgs_rec < xmin_higgs) mass_higgs_rec = xmin_higgs;
    
    // double top_v = fcb_top->Eval(mass_tlep_rec);
    // double higgs_v = fcb_higgs->Eval(mass_higgs_rec);

    // double l = top_v * higgs_v;

    // double chi2_top = -2 * log(top_v);
    // double chi2_higgs = -2*log(higgs_v);
    // double chi2_total = -2 * log(l);

    hyp.set_discriminator(config.discriminator_label + "_top", chi2_top);
    hyp.set_discriminator(config.discriminator_label + "_higgs", chi2_higgs);
    hyp.set_discriminator(config.discriminator_label + "_ma60", chi2_higgs_ma60);
    hyp.set_discriminator(config.discriminator_label + "_ma90", chi2_higgs_ma90);
    hyp.set_discriminator(config.discriminator_label + "_ma175", chi2_higgs_ma175);
    hyp.set_discriminator(config.discriminator_label + "_ma300", chi2_higgs_ma300);

    hyp.set_discriminator(config.discriminator_label + "_top+ma60", chi2_higgs_ma60 + chi2_top);
    hyp.set_discriminator(config.discriminator_label + "_top+ma90", chi2_higgs_ma90 + chi2_top);
    hyp.set_discriminator(config.discriminator_label + "_top+ma175", chi2_higgs_ma175 + chi2_top);
    hyp.set_discriminator(config.discriminator_label + "_top+ma300", chi2_higgs_ma300 + chi2_top);

    hyp.set_discriminator(config.discriminator_label, chi2_higgs + chi2_top);  //original chi2
    //hyp.set_discriminator(config.discriminator_label, chi2_total); 
  }
  return true;
}
