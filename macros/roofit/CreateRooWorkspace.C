#include "CreateRooWorkspace.h"
#include "BkgPdf3p.h" 
#include "BkgPdf4p.h" 


#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooMultiPdf.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"


#include <iostream>
#include <fstream>

#include <RooRealVar.h> 
#include <RooDataHist.h> 
#include <RooPlot.h> 
#include <RooFitResult.h>
#include <RooGaussian.h>

#include <TFile.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TF1.h>

CreateRooWorkspace::CreateRooWorkspace() : infotofile("datacards/AnalysisOutput2016.txt", std::ios::out | std::ios::trunc)
{
   fWS = new RooWorkspace("SingleTth");
   gSystem->Exec("mkdir -p datacards");
   debug_histos = new TFile("debug_histos.root","RECREATE");
}

TH1F* CreateRooWorkspace::GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds)
{
	  using namespace defs;
    using namespace std;    


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
      		//back->Add(WJets);  // do not consider minor backgrounds (spiky)
      		//back->Add(DYJets);
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

void CreateRooWorkspace::PrintWorkspace()
{
  fWS->Print();
}

void CreateRooWorkspace::StoreWorkspace()
{
  fWS->writeToFile("datacards/ws_SingleTth.root");
  //infotofile.close();
  debug_histos->Close();
}

void CreateRooWorkspace::SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel channel, bool dodata, bool all_bkgds)
{
  using namespace std;
  using namespace RooFit;

  // set fit regions
  double fit_xmin = 0;
  double fit_xmax = 0;  
  if (region==defs::eSR){
    fit_xmin = 380;
    fit_xmax = 2000;    
  } else {
    fit_xmin = 500;
    fit_xmax = 2000;       
  }

  // set up name
  TString ch_name; 
  if (channel==defs::eMuon){
    ch_name = "much";
  } else {
    ch_name = "ech";
  }

  // plot regions
  double plot_low = 200;
  double plot_high = 2000; 

  // get data or MC histogram
  TH1F* h_data = GetAnalysisOutput(region, channel, dodata, all_bkgds); 

  //RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", plot_low, plot_high);
  RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", fit_xmin, fit_xmax);
  RooDataHist* dataSR = new RooDataHist("data_obs_"+ch_name, "data_obs_"+ch_name, RooArgList(*x), h_data);

  // important: get xmin and xmax from bin edges!
  // needed for normalization, otherwise the fit quality is bad
  int Nbins = 0;
  double xmin = 1e6;
  double xmax = 0;  
  for (int i=0; i<h_data->GetNbinsX()+1; ++i){
    if (h_data->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = h_data->GetXaxis()->GetBinLowEdge(i);
      if (h_data->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = h_data->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }


  // control plots
  RooPlot *plotter=x->frame();

  // 3 parameter function for nominal result
  RooRealVar* bg3p_p0 = new RooRealVar("bg3p_p0"+ch_name, "bg3p_p0"+ch_name, -13.2, -1000,  1000);
  RooRealVar* bg3p_p1 = new RooRealVar("bg3p_p1"+ch_name, "bg3p_p1"+ch_name,   9.1, -1000,  1000);
  RooRealVar* bg3p_p2 = new RooRealVar("bg3p_p2"+ch_name, "bg3p_p2"+ch_name,   2.5, -100,   100 );

  BkgPdf3p* bgfunc = new BkgPdf3p("Bkgfunc_"+ch_name,"Bkgfunc_"+ch_name, *x, *bg3p_p0, *bg3p_p1, *bg3p_p2);

  // 4 parameter function for systematics
  RooRealVar* bg4p_p0 = new RooRealVar("bg4p_p0"+ch_name, "bg4p_p0"+ch_name, 66.45, -1000,  1000);
  RooRealVar* bg4p_p1 = new RooRealVar("bg4p_p1"+ch_name, "bg4p_p1"+ch_name, -12.6, -1000,  1000);
  RooRealVar* bg4p_p2 = new RooRealVar("bg4p_p2"+ch_name, "bg4p_p2"+ch_name,  -9.6, -100,   100 );
  RooRealVar* bg4p_p3 = new RooRealVar("bg4p_p3"+ch_name, "bg4p_p3"+ch_name,  -5.3, -100,   100 );

  BkgPdf4p* bgfunc_4p = new BkgPdf4p("Bkgfunc_4p_"+ch_name,"Bkfunc_4p_"+ch_name, *x, *bg4p_p0, *bg4p_p1, *bg4p_p2, *bg4p_p3);

  // nominal fit
  RooFitResult *r_bg = bgfunc->fitTo(*dataSR, RooFit::Range(xmin, xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  std::cout << "Testing BKG values postfit" << '\n';
  bg3p_p0->Print(); 
  bg3p_p1->Print();
  bg3p_p2->Print(); 

  // systematic fit
  RooFitResult *r_bg_4p = bgfunc_4p->fitTo(*dataSR, RooFit::Range(xmin, xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  std::cout << "Testing BKG systematic variation (4p) values postfit" << '\n';
  bg4p_p0->Print(); 
  bg4p_p1->Print();
  bg4p_p2->Print(); 
  bg4p_p3->Print(); 

  //create a list with all alt and nominal functions
  RooArgList mypdfs;
  mypdfs.add(*bgfunc);
  mypdfs.add(*bgfunc_4p);

  RooCategory cat("pdf_index","Index of Pdf which is active");
  RooMultiPdf multipdf("roomultipdf_"+ch_name,"All Pdfs",cat,mypdfs);

  // converting function into hist to debug
  RooDataSet* data1 = bgfunc->generate(RooArgSet(*x), 1000000);
  RooDataHist *hist1 = data1->binnedClone();
  TH1* myhist = hist1->createHistogram("myhist",*x);
  TH1* myhist2 =  hist1->createHistogram("myhist2",*x);
  std::cout<<"bin content  "<<myhist->GetBinContent(20)<<std::endl;
  if(ch_name.Contains("much")){
    myhist->Scale(2268/myhist->Integral());
    myhist->SetName("data_obs_much");
    myhist2->Scale(2268/myhist2->Integral());
    myhist2->SetName("background_much");

  }else{
    myhist->Scale(1548/myhist->Integral());
    myhist->SetName("data_obs_ech");
    myhist2->Scale(1548/myhist2->Integral());
    myhist2->SetName("background_ech");
  }
  TCanvas *test_c = new TCanvas("test_c","test fit",10,10,700,700); 
  myhist->Draw();
  test_c->SaveAs("test_hist"+ch_name+".eps");
  debug_histos->cd();
  myhist->Write();
  myhist2->Write();


  dataSR->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
  bgfunc->plotOn(plotter);
  bgfunc_4p->plotOn(plotter);
  plotter->getAttLine()->SetLineColor(kRed);
  plotter->getAttLine()->SetLineStyle(kDotted);  
  //  hist1->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));  
  cout << "chi^2 = " << plotter->chiSquare() << endl;

  TCanvas *background_c = new TCanvas("background_c","background fit",10,10,700,700);
  background_c->cd();
  //gPad->SetLogy();
  plotter->Draw();
  background_c->SaveAs("nominal_bkg_3pfit_"+ch_name+".pdf");

  // save the data to the workspace
  fWS->import(*dataSR);

  // save the bkg fit to the workspace
  //  fWS->import(*bgfunc);

  // save the bkg systematic fit to the workspace
  //  fWS->import(*bgfunc_4p);
  fWS->import(multipdf);

  // sum up number of events in fit region
  double Ntot = 0;
  int lowbin = h_data->FindBin(xmin);
  int highbin = h_data->FindBin(xmax);
  for (int i=1; i<h_data->GetNbinsX()+1; ++i){
    if ((i>=lowbin) && (i<=highbin)) Ntot += h_data->GetBinContent(i); 
  }
  
  infotofile << "===== Number of events for SR in the "; 
  if (channel==defs::eEle) infotofile << "Electron channel =====" << std::endl;
  else infotofile << "Muon channel =====" << std::endl;
  infotofile << "In interval: xmin = " << xmin << " xmax = " << xmax << "  ";
  infotofile << "N = " << Ntot << endl << endl;

  // clean up
  delete x; delete dataSR; 
  delete plotter;
  delete bgfunc;
  delete bgfunc_4p;  
  delete bg3p_p0; delete bg3p_p1; delete bg3p_p2; 
  delete bg4p_p0; delete bg4p_p1; delete bg4p_p2; delete bg4p_p3; 
  delete r_bg;
  delete background_c;

  return;

}

void CreateRooWorkspace::SaveSignals(defs::Echannel ch)
{
  // set up name
  TString ch_name; 
  if (ch==defs::eMuon){
    ch_name = "much";
  } else {
    ch_name = "ech";
  }

  // from the fit to signals: get mean and width from combined fit (higher stats)
  TF1* mean = new TF1("meanfit", "[0]+[1]*(x-600)", 500, 1250);
  mean->SetParameter(0, 584.9);
  mean->SetParameter(1, 0.9755);

  TF1* sigma = new TF1("sigmafit", "[0]+[1]*(x-600)", 500, 1250);
  sigma->SetParameter(0, 59.04);
  sigma->SetParameter(1, 0.04125);

  TF1* eff_ele = new TF1("eff_ele", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
  eff_ele->SetParameter(0, 0.001498);
  eff_ele->SetParameter(1, 5.889e-06);
  eff_ele->SetParameter(2, -7.832e-09);

  TF1* eff_muon = new TF1("eff_muon", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
  eff_muon->SetParameter(0, 0.002258);
  eff_muon->SetParameter(1, 6.304e-06);
  eff_muon->SetParameter(2, -9.69e-09);

  infotofile << "===== Number of events for Signal in the "; 
  if (ch==defs::eEle) infotofile << "Electron channel =====" << std::endl;
  else infotofile << "Muon channel =====" << std::endl;

  // loop over mass points, create PDFs and store them
  double MT = 550; 
  RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", 200, 2000);
  while (MT<1250)
  {
    TString SgName = TString::Format("MT%d_", (int)MT);
    SgName.Append(ch_name);

       RooConstVar* sg_mean  =new RooConstVar("sg_mean",  "sg_mean",  mean->Eval(MT));
    // RooRealVar* sg_mean  =new RooRealVar("sg_mean",  "sg_mean",  mean->Eval(MT));
    RooConstVar* sg_sigma =new RooConstVar("sg_sigma", "sg_sigma", sigma->Eval(MT));
    // RooRealVar* sg_sigma =new RooRealVar("sg_sigma", "sg_sigma", sigma->Eval(MT));


    RooGaussian* ModelSg_Gauss = new RooGaussian(SgName, SgName, *x, *sg_mean, *sg_sigma);


    // converting function into hist to debug
    RooDataSet* data1 = ModelSg_Gauss->generate(RooArgSet(*x), 1000000);
    RooDataHist *hist1 = data1->binnedClone();
    TH1* myhist = hist1->createHistogram("myhist",*x);
    TString signalmass = Form("%d",(int)MT);
    if(ch_name.Contains("much")){
      myhist->Scale(80/myhist->Integral());
      myhist->SetName("signal_"+signalmass+"_much");
    }else{
      myhist->Scale(53/myhist->Integral());
      myhist->SetName("signal_"+signalmass+"_ech");
    }
    debug_histos->cd();
    myhist->Write();
  
    /*
    TCanvas* c_sg = new TCanvas(SgName, SgName, 10, 10, 700, 700);
    RooPlot* plotter=x->frame();
    ModelSg_Gauss->plotOn(plotter, RooFit::LineColor(kRed));
    plotter->Draw();
    c_sg->SaveAs("Fit_Sg"+SgName+".pdf");

    std::cout << "MT = " << MT << " mean = " << mean->Eval(MT) << " sigma = " << sigma->Eval(MT) << std::endl;
    delete c_sg; 
    */

    // now write out signal efficiency
    // expected number of events, based on luminosity of 35800 pb^-1:
    double eff = 0;
    if (ch==defs::eEle) eff = eff_ele->Eval(MT);
    else eff = eff_muon->Eval(MT);

    double Nevts = 35800*eff;
    infotofile << "MT = " << MT << " GeV,  N = " << Nevts << std::endl;


    fWS->import(*ModelSg_Gauss);

    MT+=25;
  }


}


