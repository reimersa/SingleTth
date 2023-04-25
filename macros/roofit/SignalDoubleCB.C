// PDF of a dijet function for the background parametrization
// using 3 free parameters 
#include "SignalDoubleCB.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(SignalDoubleCB); 
  
SignalDoubleCB::SignalDoubleCB(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				   RooAbsReal& _p1,
			           RooAbsReal& _p2,
                  		   RooAbsReal& _p3,
				   RooAbsReal& _p4,
				   RooAbsReal& _p5) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1),
    p2(" p2"," p_{2}",this, _p2),
    p3(" p3"," p_{3}",this, _p3),
    p4(" p4"," p_{4}",this, _p4),
    p5(" p5"," p_{5}",this, _p5)

{ 
} 


SignalDoubleCB::SignalDoubleCB(const SignalDoubleCB& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1),
  p2(" p2", this, other.p2),
  p3(" p3", this, other.p3),
  p4(" p4", this, other.p4),
  p5(" p5", this, other.p5)
{ 
} 



Double_t SignalDoubleCB::evaluate() const 
{ 

  // // Actual implementation
  Double_t u   = (x-p0)/p1;
  Double_t A1  = TMath::Power(p3/TMath::Abs(p2),p3)*TMath::Exp(-p2*p2/2);
  Double_t A2  = TMath::Power(p5/TMath::Abs(p4),p5)*TMath::Exp(-p4*p4/2);
  Double_t B1  = p3/TMath::Abs(p2) - TMath::Abs(p2);
  Double_t B2  = p5/TMath::Abs(p4) - TMath::Abs(p4);

  Double_t result = 1;
  if (u<-p2){
    result *= A1*TMath::Power(B1-u,-p3);
  }
  else if( u<p4){
    result *= TMath::Exp(-u*u/2);
  }
  else    result *= A2*TMath::Power(B2+u,-p5);

  return result; 
} 



