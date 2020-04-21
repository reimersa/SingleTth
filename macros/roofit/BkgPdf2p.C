// PDF of a dijet function for the background parametrization
// using 2 free parameters 
#include "BkgPdf2p.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(BkgPdf2p); 
  
BkgPdf2p::BkgPdf2p(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				   RooAbsReal& _p1) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1)
{ 
} 


BkgPdf2p::BkgPdf2p(const BkgPdf2p& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1)
{ 
} 



Double_t BkgPdf2p::evaluate() const 
{ 
  // Actual implementation

  Double_t xp = x/1000.; // scale x to TeV

  // numerator 
  Double_t num = TMath::Power(5-xp, p0);

  // denominator, 2rd order polynomial in log(x) in exponent
  Double_t d1 = p1;
  Double_t denom = TMath::Power(xp, d1); 

  // here is the result:
  Double_t result = num/denom;

  return result; 
} 



