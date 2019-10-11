#include <TF1.h>

class signalfunction {

 protected: 
  double f_xmin; 
  double f_xmax;
  double f_norm;
  TF1* f;

 public:
   
   signalfunction(double xmin, double xmax){
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     f = new TF1("signal", "[0]", f_xmin, f_xmax); // dummy function

   }
   
   void SetNorm(double norm)
   {
     f_norm = norm;
   }

   double GetNorm()
   {
     return f_norm;
   }

   TF1* GetFunc()
   {
     return f;
   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0, 1.);
     double in = f->Integral(f_xmin, f_xmax, 10e-3);
     f->SetParameter(0, f_norm/in);

     return f->Eval(x);

   }
};


class signalfunction_gauss : public signalfunction {
 
 public:
   signalfunction_gauss(double xmin, double xmax) : signalfunction(xmin, xmax) {
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

     f = new TF1("gauss", "gaus(0)", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, 10.);
   	 f->SetParameter(1, p[0]);
   	 f->SetParameter(2, p[1]); 

   	 double in = f->Integral(f_xmin, f_xmax, 10e-3);
   	 f->SetParameter(0, 10.*f_norm/in);

     return f->Eval(x);

   }
};

