#ifndef BKGPDFexpX
#define BKGPDFexpX

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdfExpX : public RooAbsPdf {
public:
  BkgPdfExpX() {} ; 
  BkgPdfExpX(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1,
	      RooAbsReal& _p2);
  BkgPdfExpX(const BkgPdfExpX& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdfExpX(*this,newname); }
  inline virtual ~BkgPdfExpX() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  RooRealProxy p2;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdfExpX,1) // PDF of dijet function with 4 free parameters
};
 
#endif
