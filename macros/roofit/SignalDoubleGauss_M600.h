#ifndef SIGNALDOUBLEGAUSS_M600
#define SIGNALDOUBLEGAUSS_M600

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class SignalDoubleGauss_M600 : public RooAbsPdf {
public:
  SignalDoubleGauss_M600() {} 
  SignalDoubleGauss_M600(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1);
  SignalDoubleGauss_M600(const SignalDoubleGauss_M600& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new SignalDoubleGauss_M600(*this,newname); }
  inline virtual ~SignalDoubleGauss_M600() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  
  Double_t evaluate() const ;

private:

  ClassDef(SignalDoubleGauss_M600,1) // PDF of dijet function with 3 free parameters
};
 
#endif
