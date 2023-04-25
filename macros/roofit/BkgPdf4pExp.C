// PDF of a dijet function for the background parametrization
// using 4 free parameters 
#include "BkgPdf4pExp.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(BkgPdf4pExp); 
  
BkgPdf4pExp::BkgPdf4pExp(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				   RooAbsReal& _p1,
				   RooAbsReal& _p2,
				   RooAbsReal& _p3) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1),
    p2(" p2"," p_{2}",this, _p2),
    p3(" p3"," p_{3}",this, _p3)
{ 
} 


BkgPdf4pExp::BkgPdf4pExp(const BkgPdf4pExp& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1),
  p2(" p2", this, other.p2),
  p3(" p3", this, other.p3)
{ 
} 



Double_t BkgPdf4pExp::evaluate() const 
{ 
  // Actual implementation
  //  Double_t xp = x/1000.; // scale x to TeV
  // here is the result:
  Double_t result = p0 + TMath::Exp(p1 + p2*x + p3*TMath::Power(x,2));
  // Double_t result = p0 + TMath::Exp(p1 + p2*xp + p3*TMath::Power(xp,2));
  

  return result; 
} 




