#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TMatrixDSym.h>
#include <TFitResult.h>
#include <TLatex.h>

// some definitions

enum Eregion {eCR, eSR};
enum Echannel {eMuon, eEle, eComb};

// folder where the analysis output files are 
// TString anaoutputfolder = "../../../AnalysisOutput_102X/"; 

TH1F* GetAnalysisOutput(Eregion region, Echannel ch, bool dodata, bool all_bkgds)
{

  // folder where the analysis output files are 
  TString anaoutputfolder;
  TString year;
  char *val = getenv( "ROM_SYS" );
  if (val!=NULL){
     cout << "Using Roman's setup." << endl;
     anaoutputfolder = "../../../AnalysisOutput_102X/"; 
     year = "2016";
  } else {
     cout << "Using NAF setup." << endl;
     anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/NOMINAL/"; 
     year = "2016v3";
  }
	
  //All files are read in
  bool b_error=true;
  TString unc_name = ""; // "jersmear_up" , "jersmear_down" ,"jecsmear_up" , "jecsmear_down" , "none"
  // TString folder ="~/ownCloud/masterarbeit/tex/plots/efficiency/master_";
  // TString unc_folder = "hists";
  TFile * data_f = NULL;
  if (ch == eMuon){
    data_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.DATA.DATA_Muon_" + year + ".root", "READ");
  } else {
    data_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.DATA.DATA_Electron_" + year + ".root", "READ");
  }
  TFile * ttbar_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.TTbar_" + year + ".root", "READ");
  TFile * singlet_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.SingleTop_" + year + ".root", "READ");
  TFile * WJets_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.WJets_" + year + ".root", "READ");
  TFile * DYJets_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.DYJets_" + year + ".root", "READ");
  TFile * DIB_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.Diboson_" + year + ".root", "READ");
  TFile * ttV_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.TTV_" + year + ".root", "READ");

  //Get all hist from the string hist_name
  TString channel_name = "";
  TString region_name = "";
  if (ch==eMuon){
    channel_name = "much";
  } else {
    channel_name = "ech";
  }
  if (region==eCR){
  	region_name = "cr";
  } else {
  	region_name = "sr";
  }
  TString hist_name = "chi2h_2_" + channel_name + "_" + region_name + "/M_Tprime";
  TH1F* data = (TH1F*)data_f->Get(hist_name);
  TH1F* ttbar = (TH1F*)ttbar_f->Get(hist_name);
  TH1F* singlet = (TH1F*)singlet_f->Get(hist_name);
  TH1F* WJets = (TH1F*)WJets_f->Get(hist_name);
  TH1F* DYJets = (TH1F*)DYJets_f->Get(hist_name);
  TH1F* DIB = (TH1F*)DIB_f->Get(hist_name);
  TH1F* ttV = (TH1F*)ttV_f->Get(hist_name);

  // add all backgrounds to ttbar
  TH1F* back = NULL;

  if (dodata){
    back = (TH1F*)data->Clone();
  } else {
    back = (TH1F*)ttbar->Clone();
    if (all_bkgds){
      back->Add(singlet);
      //back->Add(WJets);
      back->Add(DYJets);
      back->Add(DIB);
      back->Add(ttV);
    }
  }
  back->Rebin(2);

  // cosmetics
  back->SetXTitle("M_{T}^{rec} [GeV]");
  back->SetYTitle("Events");
  back->SetTitleSize(0.045);
  back->GetYaxis()->SetTitleSize(0.045);
  back->GetYaxis()->SetTitleOffset(1.1);
  back->SetTitle("");
  if (dodata){
    back->SetMarkerStyle(20);
  } else {
    back->SetMarkerStyle(21);    
  }
  back->SetMarkerSize(1.);
  back->SetLineColor(kBlack);
  back->SetMarkerColor(kBlack);
  if (region_name=="cr"){
    back->GetYaxis()->SetRangeUser(1., 1000);
  } else {
    back->GetYaxis()->SetRangeUser(0.05, 1000);
  }
  back->GetXaxis()->SetRangeUser(200, 2000);
  back->Draw("E1");

  // zero out bins with little MC stats 
  for (int i=1; i<back->GetNbinsX()+1; ++i){
    if (back->GetBinContent(i) < 0.1){ 
      back->SetBinContent(i, 0);
      back->SetBinError(i,0);
    }
  }

  return back;

}

TH1F* GetAnalysisOutputSignal(int MT, Echannel ch, TString unc = "")
{

  // folder where the analysis output files are 
  TString anaoutputfolder;
  TString year;
  TString systfolder; 
  char *val = getenv( "ROM_SYS" );
  if (val!=NULL){
     cout << "Using Roman's setup." << endl;
     anaoutputfolder = "../../../AnalysisOutput_102X/"; 
     year = "2016";
     systfolder = ""; 
  } else {
     cout << "Using NAF setup." << endl;
     anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/NOMINAL/"; 
     year = "2016v3";
     systfolder = "/nfs/dust/cms/user/reimersa/SingleTth/Finalselection/NOMINAL/"; 
  }

  
  //All files are read in
  bool b_error=true;

  std::cout<<"MT  "<< TString::Format("%d", MT)<<std::endl;

  TString MT_name = TString::Format("%d", MT);
  TFile * sig_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_" + year + ".root", "READ");
  if ((MT <700) && (val==NULL))     sig_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_2016v2.root", "READ");

  if(unc!=""){
    TString subfolder = "NOMINAL/";
      sig_f = new TFile(systfolder+subfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_" + year + ".root", "READ");
    if ((MT <700) && (val==NULL))     sig_f = new TFile(systfolder+subfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_2016v2.root", "READ");
    if(unc.Contains("PDF")) {
      subfolder = "PDF_up/";
      sig_f = new TFile(systfolder+subfolder+"VLQ_RH_" + MT_name + "_" + year + ".root", "READ");
      if ((MT <700) && (val==NULL))     sig_f = new TFile(systfolder+subfolder+"VLQ_RH_" + MT_name + "_2016v2.root", "READ");
    }

    if(unc.Contains("JEC")||unc.Contains("JER")) {
      sig_f = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/"+unc+"/uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_" + year + ".root", "READ");
      if ((MT <700) && (val==NULL))     sig_f = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/"+unc+"/uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_2016v2.root", "READ");
    }

  }  

  TString region_name = "sr";
  TString ech_name  = "chi2h_2_ech_"  + region_name + "/M_Tprime";
  TString much_name = "chi2h_2_much_" + region_name + "/M_Tprime";

  if(unc!=""){
    if(unc.Contains("PDF")) unc.ReplaceAll("PDF_up","pdf");
    ech_name  = "chi2h_2_ech_"  + region_name +"_"+unc +"/M_Tprime";
    much_name = "chi2h_2_much_" + region_name +"_"+unc + "/M_Tprime";
    if(unc.Contains("JEC") || unc.Contains("JER")){
      ech_name  = "chi2h_2_ech_"  + region_name +"/M_Tprime";
      much_name = "chi2h_2_much_" + region_name +"/M_Tprime";
    }
  }

  //Get all hists
  TH1F* sigh = NULL;

  if (ch==eMuon){
    sigh = (TH1F*) sig_f->Get(much_name);
  } else if (ch == eEle) {
    sigh = (TH1F*) sig_f->Get(ech_name);
  } else if (ch == eComb) {
    TH1F* eh = (TH1F*) sig_f->Get(ech_name);
    TH1F* mh = (TH1F*) sig_f->Get(much_name);
    sigh = (TH1F*) eh->Clone();
    sigh->Add(mh);
  }
  sigh->Rebin(2);

  // cosmetics
  sigh->SetXTitle("M_{T}^{rec} [GeV]");
  sigh->SetYTitle("Events");
  sigh->SetTitleSize(0.045);
  sigh->GetYaxis()->SetTitleSize(0.045);
  sigh->GetYaxis()->SetTitleOffset(1.1);
  sigh->SetTitle("");
  sigh->SetMarkerStyle(21);    
  sigh->SetMarkerSize(1.);
  sigh->SetLineColor(kBlack);
  sigh->SetMarkerColor(kBlack);
  sigh->GetYaxis()->SetRangeUser(0.01, sigh->GetMaximum()*1.3);
  sigh->GetXaxis()->SetRangeUser(MT-400, MT+400);
  //sigh->Draw("E1");

  // zero out bins with little MC stats 
  //for (int i=1; i<back->GetNbinsX()+1; ++i){
  //  if (back->GetBinContent(i) < 0.1){ 
  //    back->SetBinContent(i, 0);
  //    back->SetBinError(i,0);
  //  }
  //}

  return sigh;

}

double CalcEff(TF1* sigf, double Npeak, double Npeak_err, double NSRtot, int MT, double& err)
{
  // folder where the analysis output files are 
  TString anaoutputfolder;
  TString year;
  char *val = getenv( "ROM_SYS" );
  if (val!=NULL){
     cout << "Using Roman's setup." << endl;
     anaoutputfolder = "../../../AnalysisOutput_102X/"; 
     year = "2016";
  } else {
     cout << "Using NAF setup." << endl;
     anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/NOMINAL/"; 
     year = "2016v3";
  }

  double gauss_norm_interval = sigf->Integral(sigf->GetXmin(), sigf->GetXmax(), 1e-3 );
  double f = Npeak / gauss_norm_interval; 
  double f_err = Npeak_err / Npeak; 

  double xmin = sigf->GetParameter(0) - 3*fabs(sigf->GetParameter(1));
  double xmax = sigf->GetParameter(0) + 3*fabs(sigf->GetParameter(1));
  double gauss_norm = sigf->Integral(xmin, xmax, 1e-3 );  

  double Nevts_peak = gauss_norm * f; 

  // get total number of events before any selection
  TString MT_name = TString::Format("%d", MT);
  TFile * sig_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_" + year + ".root", "READ");
  if ((MT <700) && (val==NULL)) sig_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.MC.VLQ_RH_" + MT_name + "_2016v2.root", "READ");
  TString hname = "cleaner/sum_event_weights";
  TH1F* h = (TH1F*) sig_f->Get(hname);  
  double Ntot = h->GetBinContent(1); 

  // this is not correct, as Ntot is already after the preselection! 
  // we should use the total generated number of events, scaled by the cross section (1pb)
  // todo: read in the preselection file to get this number from the file instead of hard-coding it
  Ntot = 35800.;

  cout << "\nNevents before selection = " << Ntot << endl;
  cout << "Sum of weights = " << h->GetSumOfWeights() << endl;
  cout << "Check: Is this equal to Ntot = L*xs = " << 35.8*1000 << " ? " << endl;
  cout << "Sum of weights after full selection = " << NSRtot << endl;

  //cout << "N events under peak = " << Nevts_peak << endl;
  //cout << "N total before selection = " << Ntot << endl;

  double eff = Nevts_peak / Ntot;
  err = eff*f_err;

  cout << "efficiency under peak = " << eff << " +- " << err << endl;
  cout << "efficiency total SR = " << NSRtot / Ntot << endl;

  return eff;

}

// WARNING: this function ignores correlations, 
// display of fit uncertainties only works for uncorrelated parameters
void DrawFitVariations(TF1* f, TH1* h, double fit_xmin, double fit_xmax)
{
  // create a histogram for the fit region only
  int Nbins = 0;
  double xmin = 1e6;
  double xmax = 0;  
  for (int i=0; i<h->GetNbinsX()+1; ++i){
    if (h->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = h->GetXaxis()->GetBinLowEdge(i);
      if (h->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = h->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }
  TH1F* hfitregion = new TH1F("hfitregion", "", Nbins, xmin, xmax);
  for (int i=1; i<Nbins+1; ++i){
    int ibin = h->GetXaxis()->FindBin(hfitregion->GetXaxis()->GetBinCenter(i));
    //hfitregion->SetBinContent(i, h->GetBinContent(ibin) );
    hfitregion->SetBinError(i, 0);
    hfitregion->SetBinContent(i, f->Eval(hfitregion->GetXaxis()->GetBinCenter(i)));
  }
  hfitregion->SetLineColor(kAzure-1);  

  //hfitregion->Draw("same");

  // now vary the parameters to get the uncertainty 
  // up variations
  std::vector<TF1*> fs; 
  for (int i=0; i<f->GetNpar(); ++i){
    dijetfunction_p3 d2obj(xmin, xmax); 
    double norm = h->Integral(h->GetXaxis()->FindBin(xmin), h->GetXaxis()->FindBin(xmax), "width");
    d2obj.SetNorm(norm);
    TF1* fm = new TF1("fm", d2obj, xmin, xmax, 3);
    fm->SetParameter(0, f->GetParameter(0));  
    fm->SetParameter(1, f->GetParameter(1));  
    fm->SetParameter(2, f->GetParameter(2));      
    fs.push_back( fm ); 
    //cout << "original:     par " << i << " = " << fs[i]->GetParameter(i) << " +- " << fs[i]->GetParError(i) << endl;
    fs[i]->SetParameter(i, f->GetParameter(i) + f->GetParError(i));
    //cout << "up variation: par " << i << " = " << fs[i]->GetParameter(i) << " +- " << fs[i]->GetParError(i) << endl;
  }

  // down variations
  for (int i=0; i<f->GetNpar(); ++i){
    dijetfunction_p3 d2obj(xmin, xmax); 
    double norm = h->Integral(h->GetXaxis()->FindBin(xmin), h->GetXaxis()->FindBin(xmax), "width");
    d2obj.SetNorm(norm);
    TF1* fm = new TF1("fm", d2obj, xmin, xmax, 3);
    fm->SetParameter(0, f->GetParameter(0));  
    fm->SetParameter(1, f->GetParameter(1));  
    fm->SetParameter(2, f->GetParameter(2));      
    fs.push_back( fm ); 
    int index = i+f->GetNpar(); 
    fs[index]->SetParameter(i, f->GetParameter(i) - f->GetParError(i));
    //cout << "dn variation: par " << index << " = " << fs[index]->GetParameter(i) << " +- " << fs[index]->GetParError(i) << endl;    
  }

  for (int i=0; i<fs.size(); ++i){
    fs[i]->SetLineColor(kRed);    
    fs[i]->SetLineStyle(kDotted);
    fs[i]->DrawClone("same");
  }
}

void GetConfidenceIntervals(TF1* f, TFitResultPtr fr, Int_t n, Int_t ndim, Double_t *x, Double_t *ci, Double_t cl)
 {
    Int_t npar = f->GetNumberFreeParameters();
    Int_t npar_real = f->GetNpar();
    Double_t *grad = new Double_t[npar_real];
    Double_t *sum_vector = new Double_t[npar];
    Bool_t *fixed=0;
    Double_t al, bl;
    if (npar_real != npar){
       fixed = new Bool_t[npar_real];
       memset(fixed,0,npar_real*sizeof(Bool_t));
 
       for (Int_t ipar=0; ipar<npar_real; ipar++){
          fixed[ipar]=0;
          f->GetParLimits(ipar,al,bl);
          if (al*bl != 0 && al >= bl) {
             //this parameter is fixed
             fixed[ipar]=1;
          }
       }
    }
    Double_t c=0;
 
    //fr->Print();
    TMatrixDSym covmatr = fr->GetCovarianceMatrix();
    TMatrixDSym rho = fr->GetCorrelationMatrix();
    //covmatr.Print();
    //rho.Print();

    Double_t t = TMath::StudentQuantile(0.5 + cl/2, f->GetNDF());
    Double_t chidf = TMath::Sqrt(f->GetChisquare()/f->GetNDF());
    Int_t igrad, ifree=0;
    for (Int_t ipoint=0; ipoint<n; ipoint++){
       c=0;
       f->GradientPar(x+ndim*ipoint, grad);
       //multiply the covariance matrix by gradient
       for (Int_t irow=0; irow<npar; irow++){
          sum_vector[irow]=0;
          igrad = 0;
          for (Int_t icol=0; icol<npar; icol++){
             igrad = 0;
             ifree=0;
             if (fixed) {
                //find the free parameter #icol
                while (ifree<icol+1){
                   if (fixed[igrad]==0) ifree++;
                   igrad++;
                }
                igrad--;
                //now the [igrad] element of gradient corresponds to [icol] element of cov.matrix
             } else {
                igrad = icol;
             }
             sum_vector[irow]+=covmatr[irow][icol]*grad[igrad];
             //cout << "irow = " << irow << " icol = " << icol << " matr index = " << irow*npar_real+icol 
             //     << " mat element = " << covmatr[irow][icol] << " grad = " << grad[igrad] << endl;
          }
       }
       igrad = 0;
       for (Int_t i=0; i<npar; i++){
          igrad = 0; ifree=0;
          if (fixed) {
             //find the free parameter #icol
             while (ifree<i+1){
                if (fixed[igrad]==0) ifree++;
                igrad++;
             }
             igrad--;
          } else {
             igrad = i;
          }
          c+=grad[igrad]*sum_vector[i];
       }
 
       c=TMath::Sqrt(c);
       ci[ipoint]=c*t*chidf;
       //cout << "c = " << c << " ci = " << ci[ipoint] << endl;
    }
 
    delete [] grad;
    delete [] sum_vector;
    if (fixed)
       delete [] fixed;

}


TH1F* ComputeHistWithCL(TF1* f, TFitResultPtr fr, TH1F* h, double cl)
{

  // create a histogram for the fit region only
  int Nbins = 0;
  double fit_xmin = f->GetXmin();
  double fit_xmax = f->GetXmax();
  cout << "xmin = " << fit_xmin << " xmax = " << fit_xmax << endl;
  double xmin = 1e6;
  double xmax = 0;  
  for (int i=0; i<h->GetNbinsX()+1; ++i){
    if (h->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = h->GetXaxis()->GetBinLowEdge(i);
      if (h->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = h->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }
  

  TH1F* hfitregion = new TH1F("hfitregion", "", Nbins, xmin, xmax);

  // now get an array with the bin centers and compute the CLs
  Double_t* x = new Double_t[Nbins];
  Double_t* ci = new Double_t[Nbins];
  for (int i=1; i<Nbins+1; ++i){
    x[i-1] = hfitregion->GetXaxis()->GetBinCenter(i);
  }

  GetConfidenceIntervals(f, fr, Nbins, 1, x, ci, cl);

  for (int i=1; i<Nbins+1; ++i){
    int ibin = h->GetXaxis()->FindBin(hfitregion->GetXaxis()->GetBinCenter(i));
    //hfitregion->SetBinContent(i, h->GetBinContent(ibin) );
    hfitregion->SetBinError(i, ci[i-1]);
    //cout << "ci = " << ci[i-1] << endl;
    hfitregion->SetBinContent(i, f->Eval(hfitregion->GetXaxis()->GetBinCenter(i)));
  }

  return hfitregion;

}


void plot_ratio(TH1F* hist, TF1* func, std::vector<TH1F*> err_hists, Eregion region, Echannel ch, bool dodata, bool all_bkgds, TString funcdesc, TString funcfilename)
{
  static int i = 0;
  ++i;
  TString cname = Form ("can_%d", i);
  TCanvas *c1 = new TCanvas(cname,"",10,10,700,800);
  TPad *m_rp_top, *m_rp, *pe;

  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  c1->Clear();
  c1->cd();
  gPad->SetTickx();
  gPad->SetTicky();
 
  Float_t yplot = 0.72;
  Float_t yratio = 0.27;

  //  coordinates:
  //
  // set up the coordinates of the two pads:    //  y6 +-------------+
  Float_t y1, y2, y3, y4, y5, y6;               //     |             |
  y6 = 0.995;                                   //     |     pad1    |
  y5 = y6-yplot;                                //  y5 |-------------|
  y4 = y5-yratio;                               //     |     rp1     |
  Float_t x1, x2;                               //  y4 +-------------+
  x1 = 0.01;
  x2 = 0.995;
  double factor = 1;

  m_rp_top = new TPad("pad", "Control Plots", x1, y5, x2, y6);
  m_rp_top->SetTopMargin( 0.11 );
  m_rp_top->SetBottomMargin(0.025);
  m_rp_top->SetLeftMargin(0.15);
  m_rp_top->SetRightMargin(0.045);
  m_rp_top->Draw();

  m_rp = new TPad("rp", "Ratio", x1, y4, x2, y5);
  m_rp->SetTopMargin(0);
  m_rp->SetBottomMargin(0.35);
  m_rp->SetLeftMargin(0.15);
  m_rp->SetRightMargin(0.045);
  m_rp->Draw();
      

  m_rp_top->cd();
  m_rp_top->SetLogy();
  m_rp_top->SetTickx();
  m_rp_top->SetTicky();

  //START TEST
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetTickx(0);
  c1->SetTicky(0);
  //END TEST

  hist->GetXaxis()->SetNdivisions(6,5,0);
  hist->GetYaxis()->SetNdivisions(6,5,0);

  //hist->SetMaximum(max);
  //hist->SetMinimum(0.05);

  hist->GetXaxis()->SetTitleSize(0);
  hist->GetXaxis()->SetTitleOffset(0);
  hist->GetXaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetLabelSize(0);

  hist->GetYaxis()->SetTitleSize(0.07);
  hist->GetYaxis()->SetTitleOffset(1.1);
  hist->GetYaxis()->SetLabelFont(42);
  hist->GetYaxis()->SetLabelSize(0.055);
  hist->Draw("PZ");

  for (int i=0;i<err_hists.size();++i){
  	err_hists[i]->Draw("e3 same");
  }
  hist->Draw("PZ same");
  func->Draw("same");

  // some information
  TString info, info2;
  if (region==eSR){
    info = "Signal Region";
  } else {
    info = "Control Region";
  }
  if (ch==eMuon){
    info2 = "#mu + jets, ";
  } else {
    info2 = "e + jets, ";
  }
  info2 += info; 
  TString info3 = "Backgrounds from MC";
  if (dodata){
    info3 = "Data";
  }
  TLatex* text = new TLatex();
  text->SetTextFont(42);
  text->SetNDC();
  text->SetTextColor(kBlack);
  text->SetTextSize(0.05);
  text->DrawLatex(0.15, 0.96, info2.Data());
  text->DrawLatex(0.15, 0.905, info3.Data());

  TString lumiText = "35.9 fb^{-1}";
  lumiText += " (13 TeV)";
  text->DrawLatex(0.665, 0.905, lumiText.Data());

  TString info4 = funcdesc;
  text->SetTextSize(0.04);
  text->SetTextAlign(32);
  text->SetTextFont(62);
  text->DrawLatex(0.90, 0.83, info4.Data());  
  TString info5 = "#chi^{2} / ndf";
  text->SetTextFont(42);
  text->SetTextAlign(12);
  text->DrawLatex(0.60, 0.82-0.05, info5.Data());  
  info5 = TString::Format("%4.2f / %d", func->GetChisquare(), func->GetNDF() );
  text->SetTextAlign(32);
  text->DrawLatex(0.90, 0.82-0.05, info5.Data());    
  TString info6 = "Prob";
  text->SetTextAlign(12);  
  text->DrawLatex(0.60, 0.82-0.10, info6.Data());  
  info6 = TString::Format("%4.3f", TMath::Prob(func->GetChisquare(), func->GetNDF()) );
  text->SetTextAlign(32);
  text->DrawLatex(0.90, 0.82-0.10, info6.Data());    
  for (int i=0; i<func->GetNpar(); ++i){
    TString info7 = "p_{";
    info7.Append(TString::Format("%d",i));
    info7.Append("}");
    text->SetTextAlign(12);
    text->DrawLatex(0.60, 0.82-0.15-0.05*i, info7.Data());     
    info7 = TString::Format("%4.2f",func->GetParameter(i)); 
    info7.Append(" #pm ");
    info7.Append(TString::Format("%4.2f",func->GetParError(i)));
    text->SetTextAlign(32);
    text->DrawLatex(0.90, 0.82-0.15-0.05*i, info7.Data()); 
  }

  c1->Update();
  gPad->RedrawAxis();

  m_rp->cd();
  gPad->SetTickx();
  gPad->SetTicky();

  TH1F *ratio1 = (TH1F*)hist->Clone();
  ratio1->Divide(func);

  // delete entries outside of fit range
  for (int i=1; i<ratio1->GetNbinsX()+1;++i){
  	if (ratio1->GetXaxis()->GetBinLowEdge(i)<func->GetXmin()){
  		ratio1->SetBinContent(i,0);
  		ratio1->SetBinError(i,0);
  	}
  	if (ratio1->GetXaxis()->GetBinUpEdge(i)>func->GetXmax()){
  		ratio1->SetBinContent(i,0);
  		ratio1->SetBinError(i,0);
  	}
  }

  ratio1->GetYaxis()->SetRangeUser(0.4,1.6);
  ratio1->GetXaxis()->SetNdivisions(6,5,0);
  ratio1->GetYaxis()->SetNdivisions(4,5,0);
    
  //    ratio1->GetXaxis()->SetTitleSize(45);
  ratio1->GetXaxis()->SetTitleSize(0.17);
  ratio1->GetXaxis()->SetTitleOffset(0.95);
  ratio1->GetXaxis()->SetLabelSize(0.15);
  ratio1->GetXaxis()->SetTitleFont(42);
  ratio1->GetXaxis()->SetLabelFont(42);

  //Y-axis
  ratio1->GetYaxis()->SetTitle("Ratio");
  ratio1->GetYaxis()->CenterTitle();
  ratio1->GetYaxis()->SetTitleSize(0.17);
  ratio1->GetYaxis()->SetTitleOffset(0.44);
  ratio1->GetYaxis()->SetTitleFont(42);
  ratio1->GetYaxis()->SetLabelFont(42);
  ratio1->GetYaxis()->SetLabelSize(0.15);
  ratio1->GetXaxis()->SetTickLength(0.08);
  ratio1->GetYaxis()->SetTickLength(0.03);
    
  ratio1->SetTitle("");
  ratio1->Draw("PZ");
    
 
  TF1* unity = new TF1("unity", "[0]", func->GetXmin(), func->GetXmax());
  unity->SetLineColor(func->GetLineColor());
  unity->SetLineStyle(kSolid);
  unity->SetLineWidth(2);
  unity->SetParameter(0, 1);
    
  for (int i=0; i<err_hists.size(); ++i){
    TH1F *rh = (TH1F*)err_hists[i]->Clone();
    rh->Divide(func);
    rh->Draw("e3 same");
  }

  ratio1->Draw("PZ same");
  unity->Draw("same");

  gPad->RedrawAxis();

  TString folder = "results";
  TString channel_name = "";
  TString region_name = "";
  if (ch==eMuon){
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
    c1->Print(folder + "/ratio_" + region_name + "_" + channel_name + "_data_fit_" + funcfilename + ".pdf");
  } else {

    if (all_bkgds){
      c1->Print(folder + "/ratio_" + region_name + "_" + channel_name + "_allbkgds_fit_" + funcfilename + ".pdf");
    } else {
      c1->Print(folder + "/ratio_" + region_name + "_" + channel_name + "_ttbar_fit_" + funcfilename + ".pdf");
    }

  }

}









