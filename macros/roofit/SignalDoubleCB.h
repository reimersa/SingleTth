#ifndef SIGNALDoubleCB
#define SIGNALDoubleCB

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class SignalDoubleCB : public RooAbsPdf {
public:
  SignalDoubleCB() {} ; 
  SignalDoubleCB(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1,
	      RooAbsReal& _p2,
	      RooAbsReal& _p3,
	      RooAbsReal& _p4,
	      RooAbsReal& _p5);
  SignalDoubleCB(const SignalDoubleCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new SignalDoubleCB(*this,newname); }
  inline virtual ~SignalDoubleCB() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  RooRealProxy p3;
  RooRealProxy p4;
  RooRealProxy p5;
  
  Double_t evaluate() const ;

private:

  ClassDef(SignalDoubleCB,1) // PDF of dijet function with 3 free parameters
};
 
#endif
