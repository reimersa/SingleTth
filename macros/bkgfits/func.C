#include <TF1.h>

class dijetfunction {

 protected: 
  double f_xmin; 
  double f_xmax;
  double f_norm;
  TF1* f;

 public:
   
   dijetfunction(double xmin, double xmax){
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     f = new TF1("dijet", "[0]", f_xmin, f_xmax); // dummy function

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

class expfunction_p2 : public dijetfunction {
 
 public:
   expfunction_p2(double xmin, double xmax) : dijetfunction(xmin, xmax) {
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     TString func = "[2]*TMath::Exp(-(_p1_)*x/1000 + (_p2_)*x*x/1e6)";
     func.ReplaceAll("_p1_", "0.8026*[0] + 0.5965*[1]");
     func.ReplaceAll("_p2_", "0.5965*[0] - 0.8026*[1]");     

     //f = new TF1("exp2", "[2]*( TMath::Exp(-[0]*x/1000 + [1]*x*x/1e6) )", f_xmin, f_xmax);
     f = new TF1("exp2", func.Data(), f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0, p[0]);
     f->SetParameter(1, p[1]);
     f->SetParameter(2, 1000.); // set it to 1000 to avoid small numbers

     double in = f->Integral(f_xmin, f_xmax, 10e-3);
     f->SetParameter(2, 1000.*f_norm/in);

     return f->Eval(x);

   }
};


class dijetfunction_p2 : public dijetfunction {
 
 public:
   dijetfunction_p2(double xmin, double xmax) : dijetfunction(xmin, xmax) {
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

     f = new TF1("dijet2", "[2]*( TMath::Power(5.-x/1000.,[0])/TMath::Power(x/1000.,[1]) )", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, p[0]);
   	 f->SetParameter(1, p[1]);
   	 f->SetParameter(2, 1000.); // set it to 1000 to avoid small numbers

   	 double in = f->Integral(f_xmin, f_xmax, 10e-3);
   	 f->SetParameter(2, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

class dijetfunction_p3 : public dijetfunction {
 
 public:
   dijetfunction_p3(double xmin, double xmax) : dijetfunction(xmin, xmax){
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

     f = new TF1("dijet3", "[3]*( TMath::Power(5.-x/1000.,[0])/(TMath::Power(x/1000.,[1]+[2]*TMath::Log(x/1000.)) ))", f_xmin, f_xmax);

     // some sensible starting values
     f->SetParameter(0, 0);
     f->SetParameter(1, 0);
     f->SetParameter(2, 0);
     f->SetParameter(3, 1.); 

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, p[0]);
   	 f->SetParameter(1, p[1]);
   	 f->SetParameter(2, p[2]);
  	 f->SetParameter(3, 1000.); // set it to 1000 to avoid small numbers

   	 double in = f->Integral(f_xmin, f_xmax, 10e-2);
   	 f->SetParameter(3, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

class dijetfunction_altp3 : public dijetfunction {
 
 public:
   dijetfunction_altp3(double xmin, double xmax) : dijetfunction(xmin, xmax){
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

     f = new TF1("dijetalt3", "[3]*( TMath::Power(5.-x/1000. + [2]*x*x/1e6,[0])/(TMath::Power(x/1000.,[1]) ))", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, p[0]);
   	 f->SetParameter(1, p[1]);
   	 f->SetParameter(2, p[2]);
  	 f->SetParameter(3, 1000.); // set it to 1000 to avoid small numbers

   	 double in = f->Integral(f_xmin, f_xmax, 10e-2);
   	 f->SetParameter(3, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

class dijetfunction_p4 : public dijetfunction {
 
 public:
   dijetfunction_p4(double xmin, double xmax) : dijetfunction(xmin, xmax){
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     f = new TF1("dijet4", "[4]*( TMath::Power(5.-x/1000.,[0])/(TMath::Power(x/1000.,[1] + [2]*TMath::Log(x/1000.) + [3]*TMath::Log(x/1000.)*TMath::Log(x/1000.) ) ) )", f_xmin, f_xmax);

     // some sensible starting values
     f->SetParameter(0, 0);
     f->SetParameter(1, 0);
     f->SetParameter(2, 0);
     f->SetParameter(3, 0);      
     f->SetParameter(4, 1.); 

   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0, p[0]);
     f->SetParameter(1, p[1]);
     f->SetParameter(2, p[2]);
     f->SetParameter(3, p[3]); // set it to 1000 to avoid small numbers
     f->SetParameter(4, 1000.); // set it to 1000 to avoid small numbers

     double in = f->Integral(f_xmin, f_xmax, 10e-2);
     f->SetParameter(4, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

class dijetfunction_altp4 : public dijetfunction {
 
 public:
   dijetfunction_altp4(double xmin, double xmax) : dijetfunction(xmin, xmax){
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     f = new TF1("dijetalt4", "[4]*( TMath::Power(5.-x/1000. + [2]*x*x/1e6,[0])/(TMath::Power(x/1000.,[1]+[3]*TMath::Log(x/1000.)) ))", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0, p[0]);
     f->SetParameter(1, p[1]);
     f->SetParameter(2, p[2]);
     f->SetParameter(3, p[3]);
     f->SetParameter(4, 1000.); // set it to 1000 to avoid small numbers

     double in = f->Integral(f_xmin, f_xmax, 10e-2);
     f->SetParameter(4, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

class dijetfunction_p5 : public dijetfunction {
 
 public:
   dijetfunction_p5(double xmin, double xmax) : dijetfunction(xmin, xmax){
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;

     f = new TF1("dijet5", "[5]*( TMath::Power(5.-x/1000.,[0])) / (TMath::Power(x/1000.,[1] + [2]*TMath::Log(x/1000.) + [3]*TMath::Log(x/1000.)*TMath::Log(x/1000.) + [4]*TMath::Power(TMath::Log(x/1000.),3) ) )", f_xmin, f_xmax);

     // some sensible starting values
     f->SetParameter(0, 0);
     f->SetParameter(1, 0);
     f->SetParameter(2, 0);
     f->SetParameter(3, 0);      
     f->SetParameter(4, 0.); 
     f->SetParameter(5, 1.); 

   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0, p[0]);
     f->SetParameter(1, p[1]);
     f->SetParameter(2, p[2]);
     f->SetParameter(3, p[3]); // set it to 1000 to avoid small numbers
     f->SetParameter(4, p[4]); // set it to 1000 to avoid small numbers
     f->SetParameter(5, 1000.); // set it to 1000 to avoid small numbers

     double in = f->Integral(f_xmin, f_xmax, 10e-2);
     f->SetParameter(5, 1000.*f_norm/in);

     return f->Eval(x);

   }
};

