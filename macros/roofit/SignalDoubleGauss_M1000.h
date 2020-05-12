#ifndef SIGNALDOUBLEGAUSS_M1000
#define SIGNALDOUBLEGAUSS_M1000

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class SignalDoubleGauss_M1000 : public RooAbsPdf {
public:
  SignalDoubleGauss_M1000() {} 
  SignalDoubleGauss_M1000(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1);
  SignalDoubleGauss_M1000(const SignalDoubleGauss_M1000& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new SignalDoubleGauss_M1000(*this,newname); }
  inline virtual ~SignalDoubleGauss_M1000() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  
  Double_t evaluate() const ;

private:

  ClassDef(SignalDoubleGauss_M1000,1) // PDF of dijet function with 3 free parameters
};
 
#endif
