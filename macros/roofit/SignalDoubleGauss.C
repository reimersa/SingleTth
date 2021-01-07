// PDF of a dijet function for the background parametrization
// using 2 free parameters 
#include "SignalDoubleGauss.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(SignalDoubleGauss); 
  
SignalDoubleGauss::SignalDoubleGauss(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				     RooAbsReal& _p1,RooAbsReal& _p2,RooAbsReal& _p3,RooAbsReal& _p4) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1),
    p2(" p2"," p_{2}",this, _p2),
    p3(" p3"," p_{3}",this, _p3),
    p4(" p4"," p_{4}",this, _p4)

{ 
} 


SignalDoubleGauss::SignalDoubleGauss(const SignalDoubleGauss& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1),
  p2(" p2", this, other.p2),
  p3(" p3", this, other.p3),
  p4(" p4", this, other.p4)
{ 
} 



Double_t SignalDoubleGauss::evaluate() const 
{ 
  // Actual implementation

  //  Double_t xp = x/1000.; // scale x to TeV
  Double_t xp = x;

  //// First Gauss
  Double_t arg1 = (-0.5) * pow((xp - p0)/p1,2); 
  Double_t gauss1 = (1-p4) * TMath::Exp( arg1 );

  //  Double_t arg2 = (-0.5) * pow((xp - 390)/(50),2); 
  Double_t arg2 = (-0.5) * pow((xp - p2)/(p3),2); 
  Double_t gauss2 = p4 * TMath::Exp( arg2 );

  

  Double_t result = gauss1 + gauss2;
  //[0]*exp(-0.5*((x-[1])/[2])**2) + [5]*exp(-0.5*((x-[3])/[4])**2)

  return result; 
} 



