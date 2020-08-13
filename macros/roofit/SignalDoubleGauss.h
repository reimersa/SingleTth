#ifndef SIGNALDOUBLEGAUSS
#define SIGNALDOUBLEGAUSS

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class SignalDoubleGauss : public RooAbsPdf {
public:
  SignalDoubleGauss() {} 
  SignalDoubleGauss(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
		    RooAbsReal& _p1,RooAbsReal& _p2,RooAbsReal& _p3,RooAbsReal& _p4);
  SignalDoubleGauss(const SignalDoubleGauss& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new SignalDoubleGauss(*this,newname); }
  inline virtual ~SignalDoubleGauss() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  RooRealProxy p3;
  RooRealProxy p4;


  Double_t evaluate() const ;

private:

  ClassDef(SignalDoubleGauss,1) // PDF of dijet function with 3 free parameters
};
 
#endif
