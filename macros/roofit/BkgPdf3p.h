#ifndef BKGPDF3p
#define BKGPDF3p

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdf3p : public RooAbsPdf {
public:
  BkgPdf3p() {} ; 
  BkgPdf3p(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1,
	      RooAbsReal& _p2);
  BkgPdf3p(const BkgPdf3p& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdf3p(*this,newname); }
  inline virtual ~BkgPdf3p() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdf3p,1) // PDF of dijet function with 3 free parameters
};
 
#endif
