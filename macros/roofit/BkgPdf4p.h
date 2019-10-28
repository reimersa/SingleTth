#ifndef BKGPDF4p
#define BKGPDF4p

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdf4p : public RooAbsPdf {
public:
  BkgPdf4p() {} ; 
  BkgPdf4p(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1,
	      RooAbsReal& _p2,
	      RooAbsReal& _p3);
  BkgPdf4p(const BkgPdf4p& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdf4p(*this,newname); }
  inline virtual ~BkgPdf4p() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  RooRealProxy p3;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdf4p,1) // PDF of dijet function with 4 free parameters
};
 
#endif
