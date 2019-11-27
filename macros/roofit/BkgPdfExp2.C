// PDF of a dijet function for the background parametrization
// using 2 free parameters 
#include "BkgPdfExp2.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(BkgPdfExp2); 
  
BkgPdfExp2::BkgPdfExp2(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				   RooAbsReal& _p1) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1)
{ 
} 


BkgPdfExp2::BkgPdfExp2(const BkgPdfExp2& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1)
{ 
} 



Double_t BkgPdfExp2::evaluate() const 
{ 
  // Actual implementation

  Double_t xp = x/1000.; // scale x to TeV

  //Double_t arg = (-1) * p0 * xp + p1 * xp * xp; 
  Double_t arg = (-1) * (0.8026*p0 + 0.5965*p1) * xp + (0.5965*p0 - 0.8026*p1) * xp * xp; 
  Double_t result = TMath::Exp( arg );

  return result; 
} 



