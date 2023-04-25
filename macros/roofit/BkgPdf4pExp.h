#ifndef BKGPDF4pexp
#define BKGPDF4pexp

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdf4pExp : public RooAbsPdf {
public:
  BkgPdf4pExp() {} ; 
  BkgPdf4pExp(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1,
	      RooAbsReal& _p2,
	      RooAbsReal& _p3);
  BkgPdf4pExp(const BkgPdf4pExp& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdf4pExp(*this,newname); }
  inline virtual ~BkgPdf4pExp() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  RooRealProxy p3;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdf4pExp,1) // PDF of dijet function with 4 free parameters
};
 
#endif
