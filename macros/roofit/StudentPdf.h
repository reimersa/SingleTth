/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * Copyright (c) 2000-2007, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

#ifndef STUDENTPDF
#define STUDENTPDF

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class StudentPdf : public RooAbsPdf {
public:
  StudentPdf() {} ; 
  StudentPdf(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _mean,
	      RooAbsReal& _sigma,
	      RooAbsReal& _nu,
	      RooAbsReal& _c1,
	      RooAbsReal& _frac);
  StudentPdf(const StudentPdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new StudentPdf(*this,newname); }
  inline virtual ~StudentPdf() { }

protected:

  RooRealProxy x ;
  RooRealProxy mean ;
  RooRealProxy sigma ;
  RooRealProxy nu ;
  RooRealProxy c1 ;
  RooRealProxy frac ;
  
  Double_t evaluate() const ;

private:

  ClassDef(StudentPdf,1) // Your description goes here...
};
 
#endif
