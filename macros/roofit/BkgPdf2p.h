#ifndef BKGPDF2p
#define BKGPDF2p

#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <RooCategoryProxy.h>
#include <RooAbsReal.h>
#include <RooAbsCategory.h>
 
class BkgPdf2p : public RooAbsPdf {
public:
  BkgPdf2p() {} ; 
  BkgPdf2p(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _p0,
	      RooAbsReal& _p1);
  BkgPdf2p(const BkgPdf2p& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new BkgPdf2p(*this,newname); }
  inline virtual ~BkgPdf2p() { }

protected:

  RooRealProxy x;
  RooRealProxy p0;
  RooRealProxy p1;
  
  Double_t evaluate() const ;

private:

  ClassDef(BkgPdf2p,1) // PDF of dijet function with 2 free parameters
};
 
#endif
