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

  void set_xmin_xmax(double xmin, double xmax){
    f_xmin = xmin;
    f_xmax = xmax;
    f->SetRange(f_xmin,f_xmax);
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

class signalfunction_cb : public signalfunction {
 
 public:
   signalfunction_cb(double xmin, double xmax) : signalfunction(xmin, xmax) {
     f_xmin = xmin;
     f_xmax = xmax;
     f_norm = 1.;
     //f_norm = 5000.;

     f = new TF1("crystalball", "[0]*ROOT::Math::crystalball_function(x, [4], [3], [2], [1])", f_xmin, f_xmax);
     //f->SetNpx(200); 
     //f->SetParName(0, "Peak");
     //f->SetParName(1, "#mu");
     //f->SetParName(2, "#sigma");        
     //f->SetParName(3, "n");  
     //f->SetParName(4, "#alpha");

   }

   double operator() (double *xx, double *p) 
   {

     Float_t x =xx[0];
     f->SetParameter(0,10); 
     f->SetParameter(1,p[0]);
     f->SetParameter(2,p[1]);
     f->SetParameter(3,p[2]);
     f->SetParameter(4,p[3]);

     double in = f->Integral(f_xmin, f_xmax, 10e-3);
     f->SetParameter(0, 10.*f_norm/in);

     return f->Eval(x);

   }
};


class signalfunction_doublegauss : public signalfunction {
 
 public:
   signalfunction_doublegauss(double xmin, double xmax) : signalfunction(xmin, xmax) {
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

	 //	 f = new TF1("doublegauss", "[0]*exp(-0.5*((x-[1])/[2])**2) + [5]*exp(-0.5*((x-[3])/[4])**2)", f_xmin, f_xmax);
	 f = new TF1("doublegauss", "[0]*((1-[5]) * exp(-0.5*((x-[1])/[2])**2) + [5]*exp(-0.5*((x-[3])/[4])**2))", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, 10.);
   	 f->SetParameter(1, p[0]);
   	 f->SetParameter(2, p[1]); 
   	 f->SetParameter(3, p[2]); 
   	 f->SetParameter(4, p[3]);  
   	 f->SetParameter(5, p[4]);  

   	 // f->SetParameter(3, 390);  // MT 600
   	 // f->SetParameter(4, 50);   // MT 600
	 // 	 f->SetParameter(5, 3.8);   // MT 600
   	 // f->SetParameter(3, 750);  // MT 1000
   	 // f->SetParameter(4, 90);   // MT 1000
 	 // f->SetParameter(5, 3.6);   // MT 1000

	 double in = f->Integral(f_xmin, f_xmax, 10e-3);  //orig  
	 //double in = f->Integral(f_xmin+80, f_xmax, 10e-3);//600
   	 // double in = f->Integral(f_xmin+150, f_xmax, 10e-3); //1000
   	 f->SetParameter(0, 10.*f_norm/in);

     return f->Eval(x);

   }
};


class signalfunction_gausslin : public signalfunction {
 
 public:
   signalfunction_gausslin(double xmin, double xmax) : signalfunction(xmin, xmax) {
   	 f_xmin = xmin;
   	 f_xmax = xmax;
   	 f_norm = 1.;

	 //	 f = new TF1("gausslin", "[0]*((1-[5]) exp(-0.5*((x-[1])/[2])**2) +([5] * ([4] +[3] * x)))", f_xmin, f_xmax);
	 f = new TF1("gausslin", "[0]*( exp(-0.5*((x-[1])/[2])**2) +([4] +[3] * x))", f_xmin, f_xmax);

   }

   double operator() (double *xx, double *p) 
   {

   	 Float_t x =xx[0];
   	 f->SetParameter(0, 10.);
   	 f->SetParameter(1, p[0]);
   	 f->SetParameter(2, p[1]); 
   	 f->SetParameter(3, p[2]); 
	 f->SetParameter(4, p[3]);  
	 f->SetParameter(5, p[4]);  

	 double in = f->Integral(f_xmin, f_xmax, 10e-3);  //orig  
   	 f->SetParameter(0, 10.*f_norm/in);

     return f->Eval(x);

   }
};

