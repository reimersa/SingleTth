#include "TH1.h"
#include <fstream>
#include "func.C"
#include "helpers.C"

using namespace std;
TF1* one_fit(Eregion region, Echannel channel, bool dodata, bool all_bkgds, TH1F*& cl68, TH1F*& cl95, TString year = "2016v3");
void PlotFuncComparison(std::vector<TF1*> funcs, TH1F* cl68, TH1F* cl95, TString name);

enum EFitFunction {eFunc2p, eFunc3p, eFuncAlt3p, eFunc4p, eFuncAlt4p, eFunc5p, eFuncExp2};

//EFitFunction FitFunc = eFunc3p;
//EFitFunction FitFunc = eFunc4p;
// EFitFunction FitFunc = eFuncExp2;

 EFitFunction FitFunc = eFunc2p;
// EFitFunction FitFunc = eFuncAlt3p;
//  EFitFunction FitFunc = eFuncAlt4p;

void bkg_fit()
{

  //TString year = "2016v3";
  //TString year = "2017v2";
  //  TString year = "2018";
  TString year = "allyears";
  TH1F* cl68 = NULL; 
  TH1F* cl95 = NULL;
  TH1F* dum = NULL; 

  // ------------ Control Region --------------

  TF1* cr_mc_ele = one_fit(eCR, eEle, false, true, dum, dum,year);
  TF1* cr_mc_muo = one_fit(eCR, eMuon, false, true, dum, dum,year);

  TF1* cr_data_ele = one_fit(eCR, eEle, true, true, cl68, cl95,year);  
  TF1* cr_data_muo = one_fit(eCR, eMuon, true, true, dum, dum,year);  

  cr_mc_ele->SetLineStyle(kDashed);
  cr_mc_muo->SetLineStyle(kDashed);
  std::vector<TF1*> funcs; 
  funcs.push_back(cr_mc_ele);
  funcs.push_back(cr_mc_muo);
  funcs.push_back(cr_data_ele);
  funcs.push_back(cr_data_muo);

  // plot comparison of fit functions
  PlotFuncComparison(funcs, cl68, cl95, "CR_fitcomparison");

  // ------------ Signal Region --------------

  TF1* sr_mc_ele = one_fit(eSR, eEle, false, true, cl68, cl95,year);
  TF1* sr_mc_muo = one_fit(eSR, eMuon, false, true, dum, dum,year);

  funcs.clear();
  funcs.push_back(sr_mc_ele);
  funcs.push_back(sr_mc_muo);

  PlotFuncComparison(funcs, cl68, cl95, "SR_MC_fitcomparison");


  // BLINDED:
  //one_fit(eSR, eEle, true, true, dum, dum);  
  //one_fit(eSR, eMuon, true, true, dum, dum);  

}

TF1* one_fit(Eregion region, Echannel channel, bool dodata, bool all_bkgds, TH1F*& cl68, TH1F*& cl95, TString year)
{
  
  // set fit regions
  double fit_xmin = 0;
  double fit_xmax = 0;  
  if (region==eSR){
    fit_xmin = 400;
    fit_xmax = 2000;    
    //    if(FitFunc == eFuncExp2 or FitFunc == eFuncAlt4p)fit_xmin =380;

  } else {
    fit_xmin = 560;
    fit_xmax = 2000;       
  }

  gROOT->SetBatch(kTRUE);

  //setOptFit( pcev (default = 0111)) Probability; Chisquare/Number of degrees of freedom; errors ;values of parameters 
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);

 
  // -----------------------------------------
  // Canvas for Background Fit without Ratios
  // -----------------------------------------
  TCanvas *background_c = new TCanvas("background_c","background fit",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  background_c->Clear();
  background_c->cd();
  background_c->SetLeftMargin(0.12);
  background_c->SetRightMargin(0.05);
  background_c->SetTopMargin(0.10);
  background_c->SetBottomMargin(0.12);

  // get data or MC
  TH1F* back = GetAnalysisOutput(region, channel, dodata, all_bkgds,year); 

  // important: get xmin and xmax from bin edges!
  // needed for normalization, otherwise the fit quality is bad
  int Nbins = 0;
  double xmin = 1e6;
  double xmax = 0;  
  for (int i=0; i<back->GetNbinsX()+1; ++i){
    if (back->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = back->GetXaxis()->GetBinLowEdge(i);
      if (back->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = back->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }
  cout << "Nbins = " << Nbins << endl;
  cout << "xmin = " << xmin << endl;
  cout << "xmax = " << xmax << endl;

  // calculate the integral to set the normalisation
  double norm = back->Integral(back->GetXaxis()->FindBin(xmin), back->GetXaxis()->FindBin(xmax), "width");
  cout << "Norm from hist = " << norm << endl;

  //-----------------------------------
  // set up function for fit
  //-----------------------------------
  TF1* fitmodel = NULL;
  Int_t linecol = 0;
  Int_t col68 = 0; 
  Int_t col95 = 0;
  TString fdesc = "";
  TString ffile = "";
  if (FitFunc==eFunc2p){
    dijetfunction_p2 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 2);
    fitmodel->SetParameter(0, 14);  
    fitmodel->SetParameter(1, 0.8);
    linecol = kBlue+1; 
    col68 = kAzure-4; 
    col95 = kAzure-9;
    fdesc = "Dijet function, 2 pars";    
    ffile = "dijet2p";
  } else if (FitFunc==eFuncExp2){
    expfunction_p2 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 2);
    fitmodel->SetParameter(0, 5.3);  
    fitmodel->SetParameter(1, 0.59);
    linecol = kBlue+1; 
    col68 = kAzure-4; 
    col95 = kAzure-9;
    fdesc = "Exp, 2 pars";
    ffile = "exp2p";
  } else if (FitFunc==eFunc3p){
    dijetfunction_p3 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 3);
    fitmodel->SetParameter(0, -13);  
    fitmodel->SetParameter(1, 8);
    fitmodel->SetParameter(2, 3.8);
    linecol = kRed+2; 
    col68 = kOrange; 
    col95 = kOrange+7;
    fdesc = "Dijet function, 3 pars";
    ffile = "dijet3p";
  } else if (FitFunc==eFuncAlt3p){ 
    dijetfunction_altp3 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 3);
    fitmodel->SetParameter(0, 69);  
    fitmodel->SetParameter(1, -4);
    fitmodel->SetParameter(2, 0.21);
    linecol = kViolet-6; 
    col68 = kViolet-2; 
    col95 = kViolet-9;
    fdesc = "Alt. dijet function, 3 pars";
    ffile = "dijet_alt3p";
  } else if (FitFunc==eFunc4p){ 
    dijetfunction_p4 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 4);
    fitmodel->SetParameter(0, 66.45);  
    fitmodel->SetParameter(1, -12.6);
    fitmodel->SetParameter(2, -9.6);
    fitmodel->SetParameter(3, -5.3);
    linecol = kGreen+2; 
    col68 = kSpring-1; 
    col95 = kSpring-4;
    fdesc = "Dijet function, 4 pars";
    ffile = "dijet4p";
  } else if (FitFunc==eFuncAlt4p){ 
    dijetfunction_altp4 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 4);
    fitmodel->SetParameter(0, 10.6);  
    fitmodel->SetParameter(1, 12.8);
    fitmodel->SetParameter(2, 3.19);
    fitmodel->SetParameter(3, 6.9);
    linecol = kPink+8; 
    col68 = kPink+6; 
    col95 = kPink+1;
    fdesc = "Alt. dijet function, 4 pars";
    ffile = "dijet_alt4p";
  } else if (FitFunc==eFunc5p){ 
    dijetfunction_p5 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    fitmodel = new TF1("fitmodel", fitfuncobj, xmin, xmax, 5);
    fitmodel->SetParameter(0, 69);  
    fitmodel->SetParameter(1, -4);
    fitmodel->SetParameter(2, 0);
    fitmodel->SetParameter(3, 0);
    fitmodel->SetParameter(4, 0);
    linecol = kBlue+1; 
    col68 = kAzure-4; 
    col95 = kAzure-9;
    fdesc = "Dijet function, 5 pars";
    ffile = "dijet5p";
  }

  cout << "Norm in function = " << fitmodel->Integral(xmin, xmax, 1e-3) << endl;

  // some cosmetics
  fitmodel->SetLineColor(linecol);
  fitmodel->SetLineStyle(kSolid);
  fitmodel->SetLineWidth(2);

  // FIT!
  TFitResultPtr r = back->Fit(fitmodel, "RS 0");
  TMatrixDSym covmatr = r->GetCovarianceMatrix();
  TMatrixDSym rho = r->GetCorrelationMatrix();
  covmatr.Print();
  rho.Print();

  // draw the result
  back->Draw("PZ");
  gPad->SetLogy(true);

  // draw 95% CL
  TH1F* clhist = ComputeHistWithCL(fitmodel, r, back, 0.95);
  clhist->SetStats(kFALSE);
  clhist->SetLineColor(col95);
  clhist->SetFillColor(col95);
  clhist->SetMarkerColor(col95);
  clhist->Draw("e3 same");
  cl95 = clhist;

  // draw 68% CL
  TH1F* clhist2 = ComputeHistWithCL(fitmodel, r, back, 0.68);
  clhist2->SetStats(kFALSE);
  clhist2->SetLineColor(col68);
  clhist2->SetFillColor(col68);
  clhist2->SetMarkerColor(col68);
  clhist2->Draw("e3 same");
  cl68 = clhist2;

  //fitmodel->DrawClone("same");
  back->DrawClone("PZ same");
  fitmodel->DrawClone("same");

  //DrawFitVariations(fitmodel, back, fit_xmin, fit_xmax);  // doesn't work for correlated parameters


  // some information
  TString info, info2;
  if (region==eSR){
    info = "Signal Region";
  } else {
    info = "Control Region";
  }
  if (channel==eMuon){
    info2 = "#mu + jets,";
  } else {
    info2 = "e + jets,";
  }
  TString info3 = "Backgrounds from MC";
  if (dodata){
    info3 = "Data";
  }
  TString info4 = fdesc;
  TLatex* text = new TLatex();
  text->SetTextFont(42);
  text->SetNDC();
  text->SetTextColor(kBlack);
  text->SetTextSize(0.035);
  text->DrawLatex(0.25, 0.96, info.Data());
  text->DrawLatex(0.13, 0.96, info2.Data());
  text->DrawLatex(0.13, 0.92, info3.Data());
  text->SetTextFont(62);
  text->DrawLatex(0.17, 0.83, info4.Data());
  //text2->DrawLatex(0.15, 0.67, info5.Data());
  
  if (FitFunc==eFunc3p){
    dijetfunction_p3 fitfuncobj(xmin, xmax);
    fitfuncobj.SetNorm(norm);    
    TF1* df = new TF1("df", fitfuncobj, xmin, xmax, 3);
    df->SetParameter(0, fitmodel->GetParameter(0));  
    df->SetParameter(1, fitmodel->GetParameter(1));  
    df->SetParameter(2, fitmodel->GetParameter(2));  
    df->Eval(600);
    TString info7 = TString::Format("f_{norm} = %.4e", fitfuncobj.GetFunc()->GetParameter(3) );
    text->SetTextFont(42);
    text->DrawLatex(0.64, 0.65, info7.Data());    
  }

  background_c->RedrawAxis();

  TString folder = "results";
  TString channel_name = "";
  TString region_name = "";
  if (channel==eMuon){
    channel_name = "much";
  } else {
    channel_name = "ech";
  }
  if (region==eCR){
    region_name = "cr";
  } else {
    region_name = "sr";
  }  
  if (dodata){
    background_c->Print(folder + "/" + region_name + "_" + channel_name + "_data_fit_" + ffile +"_"+year+ ".pdf");
  } else {

    if (all_bkgds){
      background_c->Print(folder + "/" + region_name + "_" + channel_name + "_allbkgds_fit_" + ffile + "_"+year+".pdf");
    } else {
      background_c->Print(folder + "/" + region_name + "_" + channel_name + "_ttbar_fit_" + ffile + "_"+year+".pdf");
    }

  }

  // save ratios and all infos
  std::vector<TH1F*> err_hists;
  err_hists.push_back(clhist);
  err_hists.push_back(clhist2);  
  plot_ratio(back, fitmodel, err_hists, region, channel, dodata, all_bkgds, fdesc, ffile,year);

  return fitmodel;

}

void PlotFuncComparison(std::vector<TF1*> funcs, TH1F* cl68, TH1F* cl95, TString name)
{
  TCanvas *c = new TCanvas(name, name, 10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  c->Clear();
  c->cd();
  c->SetLeftMargin(0.12);
  c->SetRightMargin(0.05);
  c->SetTopMargin(0.10);
  c->SetBottomMargin(0.12);
  //gPad->SetLogy();

  double xmax = funcs[0]->GetMinimumX();
  double xmin = funcs[0]->GetMaximumX();  
  double norm0 = funcs[0]->Integral(xmin, xmax, 1e-3); 

  // cosmetics
  TH1F* plotter = new TH1F("h", "", 100, xmin, xmax);
  plotter->SetXTitle("M_{T}^{rec} [GeV]");
  plotter->SetYTitle("Events");
  plotter->SetTitleSize(0.045);
  plotter->GetYaxis()->SetTitleSize(0.045);
  plotter->GetYaxis()->SetTitleOffset(1.1);
  //plotter->SetTitle("");
  plotter->GetYaxis()->SetRangeUser(0.1, 3000);
  plotter->Draw();

  //funcs[0]->Draw("same");
  cl95->Draw("e3same");
  cl68->Draw("e3same");
  funcs[0]->SetLineColor(kBlack);
  funcs[0]->Draw("same");

  for (int i=1; i<funcs.size(); ++i){
    double norm = funcs[i]->Integral(xmin, xmax, 1e-3); 
    double sf = norm0/norm; 
    TH1F* h = new TH1F("h", "", 100, xmin, xmax);
    h->Add(funcs[i], sf);
    h->SetLineColor(kBlack);
    h->SetLineWidth(funcs[i]->GetLineWidth());
    h->SetLineStyle(funcs[i]->GetLineStyle());
    //funcs[i]->SetNorm(funcs[0]->GetNorm());
    h->Draw("lsame");
  }

  c->SaveAs(name + ".pdf");
}

