#ifndef BKGPDFEXP2
#define BKGPDFEXP2

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdfExp2 : public RooAbsPdf {
public:
  BkgPdfExp2() {} 
  BkgPdfExp2(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1);
  BkgPdfExp2(const BkgPdfExp2& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdfExp2(*this,newname); }
  inline virtual ~BkgPdfExp2() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdfExp2,1) // PDF of dijet function with 3 free parameters
};
 
#endif
