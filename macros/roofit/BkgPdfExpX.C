// PDF of a dijet function for the background parametrization
// using 4 free parameters 
#include "BkgPdfExpX.h" 

#include <Riostream.h>
#include <RooAbsReal.h> 
#include <RooAbsCategory.h>
#include <TMath.h>

ClassImp(BkgPdfExpX); 
  
BkgPdfExpX::BkgPdfExpX(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _p0,
				   RooAbsReal& _p1,
				   RooAbsReal& _p2) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    p0(" p0"," p_{0}",this, _p0),
    p1(" p1"," p_{1}",this, _p1),
    p2(" p2"," p_{2}",this, _p2)
{ 
} 


BkgPdfExpX::BkgPdfExpX(const BkgPdfExpX& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x", this, other.x),
  p0(" p0", this, other.p0),
  p1(" p1", this, other.p1),
  p2(" p2", this, other.p2)
{ 
} 



Double_t BkgPdfExpX::evaluate() const 
{ 
  // Actual implementation
   Double_t xp = x/1000.; // scale x to TeV
  // here is the result:
   Double_t result =TMath::Exp(p0*xp + p1*TMath::Power(xp,2)) / TMath::Power(xp,p2);
//  Double_t result =TMath::Exp(p0*x + p1*TMath::Power(x,2)) / TMath::Power(x,p2);
  //  Double_t result =TMath::Exp((-5.2e-03)*x + (4.1e-07)*TMath::Power(x,2)) / TMath::Power(x,(6.2e-01))

  //  Double_t result =TMath::Exp(p0*x + (4.1e-07)*TMath::Power(x,2)) / TMath::Power(x,p2);
  //  Double_t result =TMath::Exp((p0*(0.015704)+p1*(0.999876)+p2*(-0.001138))*x + (p0*(-0.000008)+p1*(-0.001138)+p2*(-0.999999))*TMath::Power(x,2)) / TMath::Power(x,(p0*(0.999877)+p1*(-0.015704)+p2*(0.000010)));

  return result; 
} 




