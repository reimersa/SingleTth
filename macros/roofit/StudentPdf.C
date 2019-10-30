// PDF of a dijet function for the background parametrization
// using 4 free parameters 

#include "Riostream.h" 

#include "StudentPdf.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <TMath.h>


ClassImp(StudentPdf) 
  
  StudentPdf::StudentPdf(const char *name, const char *title, 
				   RooAbsReal& _x,
				   RooAbsReal& _mean,
				   RooAbsReal& _sigma,
				   RooAbsReal& _nu,
				   RooAbsReal& _c1,
				   RooAbsReal& _frac) :
    RooAbsPdf(name,title), 
    x("x","x",this,_x),
    mean(" mean"," mean",this, _mean),
    sigma(" sigma"," sigma",this, _sigma),
    nu(" nu"," nu",this, _nu),
    c1(" c1"," c1",this, _c1),
    frac(" frac"," frac",this, _frac)
{ 
} 


StudentPdf::StudentPdf(const StudentPdf& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  mean(" mean",this,other. mean),
  sigma(" sigma",this,other. sigma),
  nu(" nu",this,other. nu),
  c1(" c1",this,other. c1),
  frac(" frac",this,other. frac)
{ 
} 



Double_t StudentPdf::evaluate() const 
{ 
  // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 

  Double_t pol = 1 + c1*x;

  // check for positvie definiteness in [0.5, 1.5]
  //Double_t test = frac * (1 + c1*0.5);
  //if (test < 0) return 0.0;
  //test = frac * (1 + c1*1.5);
  //if (test < 0) return 0.0;

  Double_t t = (x-mean)/sigma;

  Double_t studentt = 1 + t*t/nu;
  studentt = TMath::Power(studentt, -(nu+1)/2 );

  Double_t result = studentt + frac*pol;

  return result; 
} 



