#include "CreateRooWorkspace.h"
#include "BkgPdfExp2.h" 
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

CreateRooWorkspace::CreateRooWorkspace(TString year) : infotofile("datacards/AnalysisOutput_"+year+".txt", std::ios::out | std::ios::trunc)
{
   fWS = new RooWorkspace("SingleTth");
   gSystem->Exec("mkdir -p datacards");
   debug_histos = new TFile("debug_histos.root","RECREATE");
}

TH1F* CreateRooWorkspace::GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds, TString year)
{
	  using namespace defs;
    using namespace std;    


	  // folder where the analysis output files are 
    TString anaoutputfolder;

    char *val = getenv( "ROM_SYS" );
    if (val!=NULL){
      cout << "Using Roman's setup." << endl;
      anaoutputfolder = "../../../AnalysisOutput_102X/"; 
      year = "2016";
    } else {
      cout << "Using NAF setup." << endl;
      if (year.Contains("2016")){
	    anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/NOMINAL/"; 
      } else if(year.Contains("2017")){
	//	anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/NOMINAL_NoBTagSF/"; 
	anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/"; 
      } else if(year.Contains("2018")){
	    anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/NOMINAL_NoBTagSF/"; 
      }else if(year.Contains("andrea")){ 
	anaoutputfolder = "/nfs/dust/cms/user/amalara/WorkingArea/File/Analysis/2016/SignalRegion/Puppi/muonchannel/";
      }else{
	throw runtime_error("Year not possible.");
      }
    }


  	//All files are read in
  	bool b_error=true;
  	TString unc_name = ""; // "jersmear_up" , "jersmear_down" ,"jecsmear_up" , "jecsmear_down" , "none"
  	// TString folder ="~/ownCloud/masterarbeit/tex/plots/efficiency/master_";
  	// TString unc_folder = "hists";
  	TFile * data_f = NULL;
  	if (ch == eMuon){
    	data_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.DATA.DATA_Muon_" + year + ".root", "READ");
	if(year.Contains("andrea"))     	data_f = new TFile(anaoutputfolder+"uhh2.AnalysisModuleRunner.DATA.DATA_SingleMuon_noTree.root", "READ");
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
	if(year.Contains("andrea"))hist_name = "ZprimeCandidate_btag_DeepBoosted_H4qvsQCD_CR/Zprime_mass_rebin2";
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


  	// zero out bins with little MC stats 
  	for (int i=1; i<back->GetNbinsX()+1; ++i){
    	if (back->GetBinContent(i) < 0.1){ 
      		back->SetBinContent(i, 0);
      		back->SetBinError(i,0);
    	}
	// if(back->GetBinLowEdge(i)<380){
	//   back->SetBinContent(i,0);
	//   back->SetBinError(i,0);
	// }
	}
  	back->Draw("E1");

  	return back;

}

void CreateRooWorkspace::PrintWorkspace()
{
  fWS->Print();
}

void CreateRooWorkspace::StoreWorkspace(TString year)
{
  fWS->writeToFile("datacards/ws_SingleTth_"+year+".root");
  //infotofile.close();
  debug_histos->Close();
}

void CreateRooWorkspace::SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel channel, bool dodata, bool all_bkgds, TString year)
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
  TH1F* h_data = GetAnalysisOutput(region, channel, dodata, all_bkgds,year); 


  //RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", plot_low, plot_high);
  RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", fit_xmin, fit_xmax);
  x->setBins(81);
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

  // 2 parameter exponential function 
  RooRealVar* bgexp2_p0 = new RooRealVar("bgexp2_p0"+ch_name, "bgexp2_p0"+ch_name, 8.2, -100, 100);
  RooRealVar* bgexp2_p1 = new RooRealVar("bgexp2_p1"+ch_name, "bgexp2_p1"+ch_name, 4.3, -100, 100);

  BkgPdfExp2* bgfunc_exp = new BkgPdfExp2("Bkgfunc_Exp2p_"+ch_name,"Bkgfunc_Exp2p_"+ch_name, *x, *bgexp2_p0, *bgexp2_p1);

  // nominal fit
  RooFitResult *r_bg = bgfunc->fitTo(*dataSR, RooFit::Range(fit_xmin, fit_xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  std::cout << "Testing BKG values postfit" << '\n';
  bg3p_p0->Print(); 
  bg3p_p1->Print();
  bg3p_p2->Print(); 

  // systematic fit
  RooFitResult *r_bg_4p = bgfunc_4p->fitTo(*dataSR, RooFit::Range(fit_xmin,fit_xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  std::cout << "Testing BKG systematic variation (4p) values postfit" << '\n';
  bg4p_p0->Print(); 
  bg4p_p1->Print();
  bg4p_p2->Print(); 
  bg4p_p3->Print(); 

  RooFitResult *r_bg_exp = bgfunc_exp->fitTo(*dataSR, RooFit::Range(fit_xmin,fit_xmax), RooFit::Save(), RooFit::Verbose(kFALSE));

  //create a list with all alt and nominal functions
  RooArgList mypdfs;
  mypdfs.add(*bgfunc);
  mypdfs.add(*bgfunc_4p);
  mypdfs.add(*bgfunc_exp);

  RooCategory cat("pdf_index_"+ch_name,"Index of Pdf which is active");
  RooMultiPdf multipdf("roomultipdf_"+ch_name,"All Pdfs",cat,mypdfs);
  RooRealVar norm("roomultipdf_"+ch_name+"_norm","Number of background events",10000,100000);

  // converting function into hist to debug
  RooDataSet* data1 = bgfunc->generate(RooArgSet(*x), 1000000);
  RooDataHist *hist1 = data1->binnedClone();
  TH1* myhist = dataSR->createHistogram("myhist",*x);
  TH1* myhist2 =  hist1->createHistogram("myhist2",*x);



  if(ch_name.Contains("much")){
    //    myhist->Scale(2268/myhist->Integral());
    myhist->SetName("data_obs_much");
    myhist->SetLineColor(kRed);
    myhist2->Scale(myhist->Integral()/myhist2->Integral());
    myhist2->SetName("background_much");

  }else{
    myhist->Scale(1548/myhist->Integral());
    myhist->SetName("data_obs_ech");
    myhist2->Scale(1548/myhist2->Integral());
    myhist2->SetName("background_ech");
  }

  TCanvas *test_c = new TCanvas("test_c","test fit",10,10,700,700); 
  myhist->Draw();
  myhist2->Draw("same");
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
  background_c->SaveAs("plots/nominal_bkg_3pfit_"+ch_name+"_"+year+".pdf");

  // save the data to the workspace
  fWS->import(*dataSR);

  // save the bkg fit to the workspace
  //fWS->import(*bgfunc);

  // save the bkg systematic fit to the workspace
  //  fWS->import(*bgfunc_4p);
  fWS->import(cat);
  //  fWS->import(norm);
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

void CreateRooWorkspace::SaveSignals(defs::Echannel ch, TString year)
{
  // set up name
  TString ch_name; 
  if (ch==defs::eMuon){
    ch_name = "much";
  } else {
    ch_name = "ech";
  }

  std::ifstream infile("/nfs/dust/cms/user/abenecke/CMSSW_10_2_10/CMSSW_10_2_10/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_"+year+".txt");
  std::string line;

  string fitname, paramname;
  double param0_mean_value,param0_mean_error,param1_mean_value,param1_mean_error,param0_sigma_value,param0_sigma_error,param1_sigma_value,param1_sigma_error,param0_eff_value,param1_eff_value,param2_eff_value;
  double param0_JECmeanup_value,param1_JECmeanup_value,param0_JECsigmaup_value,param1_JECsigmaup_value;
  double param0_JERmeanup_value,param1_JERmeanup_value,param0_JERsigmaup_value,param1_JERsigmaup_value;

  double param0_JECmeandown_value,param1_JECmeandown_value,param0_JECsigmadown_value,param1_JECsigmadown_value;
  double param0_JERmeandown_value,param1_JERmeandown_value,param0_JERsigmadown_value,param1_JERsigmadown_value;

  while (infile)
    {
      string c,d;
      infile>>fitname>>paramname>>c>>d;
      if((fitname.find("mean")!=std::string::npos) && (paramname.find("param0")!=std::string::npos)) {
	param0_mean_value = std::stod(c); 
	param0_mean_error = std::stod(d);
      }
      if(fitname.find("mean")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
	param1_mean_value = std::stod(c); 
	param1_mean_error = std::stod(d);
      }
      if((fitname.find("mfitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECmeanup_value = std::stod(c); 
      }
      if(fitname.find("mfitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECmeanup_value = std::stod(c); 
      }
      if((fitname.find("mfitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERmeanup_value = std::stod(c); 
      }
      if(fitname.find("mfitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERmeanup_value = std::stod(c); 
      }
      if((fitname.find("mfitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECmeandown_value = std::stod(c); 
      }
      if(fitname.find("mfitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECmeandown_value = std::stod(c); 
      }
      if((fitname.find("mfitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERmeandown_value = std::stod(c); 
      }
      if(fitname.find("mfitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERmeandown_value = std::stod(c); 
      }
      if(fitname.find("width")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
	param0_sigma_value = std::stod(c); 
	param0_sigma_error = std::stod(d);
      }
      if(fitname.find("width")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
	param1_sigma_value = std::stod(c); 
	param1_sigma_error = std::stod(d);
      }
      if((fitname.find("wfitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECsigmaup_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECsigmaup_value = std::stod(c); 
      }
      if((fitname.find("wfitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERsigmaup_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERsigmaup_value = std::stod(c); 
      }
      if((fitname.find("wfitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECsigmadown_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECsigmadown_value = std::stod(c); 
      }
      if((fitname.find("wfitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERsigmadown_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERsigmadown_value = std::stod(c); 
      }
      // if(fitname.find("eff")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
      // 	param0_eff_value = std::stod(c); 
      // }
      // if(fitname.find("eff")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
      // 	param1_eff_value = std::stod(c); 
      // }
      // if(fitname.find("eff")!=std::string::npos && paramname.find("param2")!=std::string::npos) {
      // 	param2_eff_value = std::stod(c); 
      // }

    }


  // from the fit to signals: get mean and width from combined fit (higher stats)
  TF1* mean = new TF1("meanfit", "[0]+[1]*(x-600)", 500, 1250);
  // mean->SetParameter(0, 584.9);
  // mean->SetParameter(1, 0.9755);
  mean->SetParameter(0, param0_mean_value);
  mean->SetParameter(1, param1_mean_value);

  TF1* mean_error = new TF1("meanfit_error", "[0]+[1]*(x-600)", 500, 1250);
  mean_error->SetParameter(0, param0_mean_value+2*param0_mean_error);
  mean_error->SetParameter(1, param1_mean_value+2*0.5 *param1_mean_error);

  TF1* JECmeanup_error = new TF1("JECmeanfit_error", "[0]+[1]*(x-600)", 500, 1250);
  JECmeanup_error->SetParameter(0, param0_JECmeanup_value);
  JECmeanup_error->SetParameter(1, param1_JECmeanup_value);

  TF1* JERmeanup_error = new TF1("JERmeanfitup_error", "[0]+[1]*(x-600)", 500, 1250);
  JERmeanup_error->SetParameter(0, param0_JERmeanup_value);
  JERmeanup_error->SetParameter(1, param1_JERmeanup_value);

  TF1* JECmeandown_error = new TF1("JECmeanfit_error", "[0]+[1]*(x-600)", 500, 1250);
  JECmeandown_error->SetParameter(0, param0_JECmeandown_value);
  JECmeandown_error->SetParameter(1, param1_JECmeandown_value);

  TF1* JERmeandown_error = new TF1("JERmeanfitdown_error", "[0]+[1]*(x-600)", 500, 1250);
  JERmeandown_error->SetParameter(0, param0_JERmeandown_value);
  JERmeandown_error->SetParameter(1, param1_JERmeandown_value);


  TF1* sigma = new TF1("sigmafit", "[0]+[1]*(x-600)", 500, 1250);
  // sigma->SetParameter(0, 59.04);
  // sigma->SetParameter(1, 0.04125);
  sigma->SetParameter(0, param0_sigma_value);
  sigma->SetParameter(1, param1_sigma_value);

  TF1* sigma_error = new TF1("sigmafit_error", "[0]+[1]*(x-600)", 500, 1250);
  sigma_error->SetParameter(0, param0_sigma_value+param0_sigma_error);
  sigma_error->SetParameter(1, param1_sigma_value+0.5*param1_sigma_error);

  TF1* JECsigmaup_error = new TF1("JECsigmafitup_error", "[0]+[1]*(x-600)", 500, 1250);
  JECsigmaup_error->SetParameter(0, param0_JECsigmaup_value);
  JECsigmaup_error->SetParameter(1, param1_JECsigmaup_value);


  TF1* JERsigmaup_error = new TF1("JERsigmafitup_error", "[0]+[1]*(x-600)", 500, 1250);
  JERsigmaup_error->SetParameter(0, param0_JERsigmaup_value);
  JERsigmaup_error->SetParameter(1, param1_JERsigmaup_value);

  TF1* JECsigmadown_error = new TF1("JECsigmafitdown_error", "[0]+[1]*(x-600)", 500, 1250);
  JECsigmadown_error->SetParameter(0, param0_JECsigmadown_value);
  JECsigmadown_error->SetParameter(1, param1_JECsigmadown_value);


  TF1* JERsigmadown_error = new TF1("JERsigmafitdown_error", "[0]+[1]*(x-600)", 500, 1250);
  JERsigmadown_error->SetParameter(0, param0_JERsigmadown_value);
  JERsigmadown_error->SetParameter(1, param1_JERsigmadown_value);


  TF1* eff_ele = new TF1("eff_ele", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
  eff_ele->SetParameter(0, 0.001498);
  eff_ele->SetParameter(1, 5.889e-06);
  eff_ele->SetParameter(2, -7.832e-09);
  if(year.Contains("2017")){
    // eff_ele->SetParameter(0, 0.00215);
    // eff_ele->SetParameter(1, 4.938e-06);
    // eff_ele->SetParameter(2, -5.968e-09);
    
    //medium WP
    eff_ele->SetParameter(0, 0.0043);
    eff_ele->SetParameter(1, 9.97e-06);
    eff_ele->SetParameter(2, -1.004e-08);

  }
  if(year.Contains("2018")){
    eff_ele->SetParameter(0, 0.00299);
    eff_ele->SetParameter(1, 4.882e-06);
    eff_ele->SetParameter(2, -6.394e-08);
  }


  TF1* eff_muon = new TF1("eff_muon", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
  eff_muon->SetParameter(0, 0.002258);
  eff_muon->SetParameter(1, 6.304e-06);
  eff_muon->SetParameter(2, -9.69e-09);
  if(year.Contains("2017")){
    // eff_muon->SetParameter(0, 0.002875);
    // eff_muon->SetParameter(1, 5.804e-06);
    // eff_muon->SetParameter(2, -7.862e-09);

    //medium WP
    eff_ele->SetParameter(0, 0.006);
    eff_ele->SetParameter(1, 1.24e-05);
    eff_ele->SetParameter(2, -1.443e-08);
    
  }
  if(year.Contains("2018")){
    eff_muon->SetParameter(0, 0.0031);
    eff_muon->SetParameter(1, 5.437e-06);
    eff_muon->SetParameter(2, -7.136e-08);
  }

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

    //       RooConstVar* sg_mean  =new RooConstVar("sg_mean",  "sg_mean",  mean->Eval(MT));
    RooRealVar* sg_mean  =new RooRealVar("sg_mean",  "sg_mean",  mean->Eval(MT));
    //    RooConstVar* sg_sigma =new RooConstVar("sg_sigma", "sg_sigma", sigma->Eval(MT));
     RooRealVar* sg_sigma =new RooRealVar("sg_sigma", "sg_sigma", sigma->Eval(MT));

    RooConstVar* sg_JECmeanup  =new RooConstVar("sg_JECmeanup",  "sg_JECmeanup",  JECmeanup_error->Eval(MT));
    RooConstVar* sg_JECsigmaup =new RooConstVar("sg_JECsigmaup", "sg_JECsigmaup", JECsigmaup_error->Eval(MT));

    RooConstVar* sg_JERmeanup  =new RooConstVar("sg_JERmeanup",  "sg_JERmeanup",  JERmeanup_error->Eval(MT));
    RooConstVar* sg_JERsigmaup =new RooConstVar("sg_JERsigmaup", "sg_JERsigmaup", JERsigmaup_error->Eval(MT));

    RooConstVar* sg_JECmeandown  =new RooConstVar("sg_JECmeandown",  "sg_JECmeandown",  JECmeandown_error->Eval(MT));
    RooConstVar* sg_JECsigmadown =new RooConstVar("sg_JECsigmadown", "sg_JECsigmadown", JECsigmadown_error->Eval(MT));

    RooConstVar* sg_JERmeandown  =new RooConstVar("sg_JERmeandown",  "sg_JERmeandown",  JERmeandown_error->Eval(MT));
    RooConstVar* sg_JERsigmadown =new RooConstVar("sg_JERsigmadown", "sg_JERsigmadown", JERsigmadown_error->Eval(MT));


    RooGaussian* ModelSg_Gauss = new RooGaussian(SgName, SgName, *x, *sg_mean, *sg_sigma);
    RooGaussian* ModelSg_JECup_Gauss = new RooGaussian(SgName+"_JECup", SgName+"_JECup", *x, *sg_JECmeanup, *sg_JECsigmaup);
    RooGaussian* ModelSg_JERup_Gauss = new RooGaussian(SgName+"_JERup", SgName+"_JERup", *x, *sg_JERmeanup, *sg_JERsigmaup);
    RooGaussian* ModelSg_JECdown_Gauss = new RooGaussian(SgName+"_JECdown", SgName+"_JECdown", *x, *sg_JECmeandown, *sg_JECsigmadown);
    RooGaussian* ModelSg_JERdown_Gauss = new RooGaussian(SgName+"_JERdown", SgName+"_JERdown", *x, *sg_JERmeandown, *sg_JERsigmadown);


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
  

    TCanvas* c_sg = new TCanvas(SgName, SgName, 10, 10, 700, 700);
    RooPlot* plotter=x->frame();
    ModelSg_Gauss->plotOn(plotter, RooFit::LineColor(kRed));
    ModelSg_JECup_Gauss->plotOn(plotter, RooFit::LineColor(kBlue));
    // ModelSg_JECdown_Gauss->plotOn(plotter, RooFit::LineColor(kBlue));
    // ModelSg_JERdown_Gauss->plotOn(plotter, RooFit::LineColor(kBlack));
    ModelSg_JERup_Gauss->plotOn(plotter, RooFit::LineColor(kBlack));
    plotter->Draw();
    c_sg->SaveAs("plots/Fit_Sg"+SgName+"_"+year+".pdf");
    c_sg->SaveAs("plots/Fit_Sg"+SgName+"_"+year+".eps");

    /*std::cout << "MT = " << MT << " mean = " << mean->Eval(MT) << " sigma = " << sigma->Eval(MT) << std::endl;
    delete c_sg; 
    */

    // now write out signal efficiency
    // expected number of events, based on luminosity of 35800 pb^-1:
    double eff = 0;
    if (ch==defs::eEle) eff = eff_ele->Eval(MT);
    else eff = eff_muon->Eval(MT);

    double Nevts = 35800*eff;
    if(year.Contains("2017")) Nevts = 41500*eff;
    if(year.Contains("2018")) Nevts = 59700*eff;
    infotofile << "MT = " << MT << " GeV,  N = " << Nevts <<" ,  Mean  "<< mean->Eval(MT)<<"  , Mean Error  "<<mean_error->Eval(MT)-mean->Eval(MT)<<"  Sigma  "<<sigma->Eval(MT)<<"  Sigma Error "<<sigma_error->Eval(MT)-sigma->Eval(MT)<< std::endl;

    //add uncertainties as special shape
    RooArgList mypdfs;
    mypdfs.add(*ModelSg_Gauss);
    mypdfs.add(*ModelSg_JECup_Gauss);
    mypdfs.add(*ModelSg_JERup_Gauss);
    mypdfs.add(*ModelSg_JECdown_Gauss);
    mypdfs.add(*ModelSg_JERdown_Gauss);

    RooCategory cat("pdf_index_"+(TString::Format("MT%d", (int)MT))+"_"+ch_name,"Index of Pdf which is active");
    RooMultiPdf multipdf("roomultipdf_"+(TString::Format("MT%d", (int)MT))+"_"+ch_name,"All Pdfs",cat,mypdfs);



    //    fWS->import(*ModelSg_Gauss);
    fWS->import(cat);
    fWS->import(multipdf);


    MT+=25;
  }


}


