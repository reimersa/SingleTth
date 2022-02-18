#include "CreateRooWorkspace.h"
#include "BkgPdfExp2.h" 
#include "BkgPdf3p.h" 
#include "BkgPdf2p.h" 
#include "BkgPdf4p.h" 
#include "SignalDoubleGauss.h" 


#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
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

CreateRooWorkspace::CreateRooWorkspace(TString year, TString cat) : infotofile("datacards/AnalysisOutput_"+year+"_"+cat+".txt", std::ios::out | std::ios::trunc)
{
   fWS = new RooWorkspace("SingleTth"+cat);
   gSystem->Exec("mkdir -p datacards");
   debug_histos = new TFile("debug_histos.root","RECREATE");
   xmin = 0;
   xmax = 0;
}

TH1F* CreateRooWorkspace::GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds, TString year, TString cat)
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
	    anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mavariable/NOMINAL/"; 
      } else if(year.Contains("2017")){
	anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mavariable/NOMINAL/"; 
      } else if(year.Contains("2018")){
	    anaoutputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mavariable/NOMINAL/"; 
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
  	TString hist_name = cat+"_" + channel_name + "_" + region_name + "/M_Tprime";
	//	if(region_name.Contains("cr")) hist_name = "chi2_10_" + channel_name + "_" + region_name + "/M_Tprime";
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

void CreateRooWorkspace::StoreWorkspace(TString year,TString cat, TString MA)
{
  fWS->writeToFile("datacards/ws_SingleTth_"+year+"_"+cat+"_"+MA+".root");
  //infotofile.close();
  debug_histos->Close();
}

void CreateRooWorkspace::SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel channel, bool dodata, bool all_bkgds, TString year, TString cat)
{
  using namespace std;
  using namespace RooFit;

  // set fit regions
  double fit_xmin = 0;
  double fit_xmax = 0;  
  if (region==defs::eSR){
    fit_xmin = 450;
    fit_xmax = 2000;    

    if(cat.Contains("catma300")) fit_xmin = 560;
    if(cat.Contains("chi2h_2")) fit_xmin = 520;
    //    if(cat.Contains("chi2h_2")) fit_xmin = 480;
    //    if(cat.Contains("chi2h_2") && year.Contains("2017")) fit_xmax = 1900;
    //    if(cat.Contains("chi2h_2")&&year.Contains("2018")) fit_xmin = 490;
    if(cat.Contains("catma175")) fit_xmin = 590;
    if(cat.Contains("ma175")&& channel==defs::eEle && year.Contains("2018"))fit_xmax = 1999;
    if(cat.Contains("ma90") && channel==defs::eEle) fit_xmax = 1999;

  } else {
    fit_xmin = 560;
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
  TH1F* h_data = GetAnalysisOutput(region, channel, dodata, all_bkgds,year, cat); 



  // important: get xmin and xmax from bin edges!
  // needed for normalization, otherwise the fit quality is bad
  int Nbins = 0;
  // double xmin = 1e6;
  // double xmax = 0;  
  xmin=1e6;
  xmax = 0;
  for (int i=0; i<h_data->GetNbinsX()+1; ++i){
    if (h_data->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = h_data->GetXaxis()->GetBinLowEdge(i);
      if (h_data->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = h_data->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }


  //RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", plot_low, plot_high);
  RooRealVar* x = new RooRealVar("x"+cat+year, "m_{T} [GeV]", xmin, xmax);
  // RooRealVar* x = new RooRealVar("x"+cat, "m_{T} [GeV]", xmin, xmax);
  x->setBins(Nbins);
  RooDataHist* dataSR = new RooDataHist("data_obs_"+ch_name+"_"+year+"_"+cat, "data_obs_"+ch_name+"_"+year+"_"+cat, RooArgList(*x), h_data);



  // control plots
  RooPlot *plotter=x->frame();


  // 3 parameter function for nominal result
  RooRealVar* bg3p_p0 = new RooRealVar("bg3p_p0"+ch_name+"_"+year+"_"+cat, "bg3p_p0"+ch_name, -13.2, -100,  100);
  RooRealVar* bg3p_p1 = new RooRealVar("bg3p_p1"+ch_name+"_"+year+"_"+cat, "bg3p_p1"+ch_name,   9.1, -100,  100);
  RooRealVar* bg3p_p2 = new RooRealVar("bg3p_p2"+ch_name+"_"+year+"_"+cat, "bg3p_p2"+ch_name,   2.5, -100,   100 );


  BkgPdf3p* bgfunc = new BkgPdf3p("Bkgfunc_"+ch_name+"_"+year+"_"+cat,"Bkgfunc_"+ch_name, *x, *bg3p_p0, *bg3p_p1, *bg3p_p2);

  // 2 parameter exponential function 
  RooRealVar* bgexp2_p0 = new RooRealVar("bgexp2_p0"+ch_name+"_"+year+"_"+cat, "bgexp2_p0"+ch_name, 8.2, -100, 100);
  RooRealVar* bgexp2_p1 = new RooRealVar("bgexp2_p1"+ch_name+"_"+year+"_"+cat, "bgexp2_p1"+ch_name, 4.3, -100, 100);

  BkgPdfExp2* bgfunc_exp = new BkgPdfExp2("Bkgfunc_Exp2p_"+ch_name+"_"+year+"_"+cat,"Bkgfunc_Exp2p_"+ch_name, *x, *bgexp2_p0, *bgexp2_p1);

  //  nominal fit
   RooFitResult *r_bg = bgfunc->fitTo(*dataSR, RooFit::Range(xmin, xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  std::cout << "Testing BKG values postfit" << '\n';
  bg3p_p0->Print(); 
  bg3p_p1->Print();
  bg3p_p2->Print(); 

  infotofile << "---------  Bg3p  "+ch_name+"  ---------"<<std::endl;
  infotofile << "bg3p_p0"<<ch_name<<"_"<<year<<"_"<<cat<<"  " << bg3p_p0->getValV() <<"  error  "<<   bg3p_p0->getError()<<std::endl;
  infotofile << "bg3p_p1"<<ch_name<<"_"<<year<<"_"<<cat<<"   " << bg3p_p1->getValV() <<"  error  "<<    bg3p_p1->getError()<<std::endl;
  infotofile << "bg3p_p2"<<ch_name<<"_"<<year<<"_"<<cat<<"   " << bg3p_p2->getValV() <<"  error  "<<   bg3p_p2->getError()<<std::endl;


  RooFitResult *r_bg_exp1 = bgfunc_exp->fitTo(*dataSR, RooFit::Range(xmin,xmax), RooFit::Save(), RooFit::Verbose(kFALSE));

  infotofile << "---------  Bgexp  "+ch_name+"  - ---------"<<std::endl;
  infotofile << "bgexp2_p0"<<ch_name<<"_"<<year<<"_"<<cat<<"   " << bgexp2_p0->getValV() <<"  error  "<<   bgexp2_p0->getError()<<std::endl;
  infotofile << "bgexp2_p1"<<ch_name<<"_"<<year<<"_"<<cat<<"   " << bgexp2_p1->getValV() <<"  error  "<<   bgexp2_p1->getError()<<std::endl;
  

  //create a list with all alt and nominal functions
  RooArgList mypdfs;
  mypdfs.add(*bgfunc);
  //  mypdfs.add(*bgfunc_4p);
  mypdfs.add(*bgfunc_exp);
  //  mypdfs.add(*bgfunc_2p);

  RooCategory category("pdf_index_"+ch_name+"_"+year+"_"+cat,"Index of Pdf which is active");
  RooMultiPdf multipdf("roomultipdf_"+ch_name+"_"+year+"_"+cat,"All Pdfs",category,mypdfs);
  RooRealVar norm("roomultipdf_"+ch_name+"_"+year+"_"+cat+"_norm","Number of background events",12014,0,1000000);


  // convert exp function to pseudo data:
  RooDataSet* data1 = bgfunc_exp->generate(RooArgSet(*x),5000);
  RooDataHist *hist1 = data1->binnedClone();
  TH1* myhist2 =  hist1->createHistogram("myhist2",*x);
  TH1* myhist = dataSR->createHistogram("myhist",*x);
  myhist2->Scale(myhist->Integral()/myhist2->Integral());

    RooDataHist* dataSR_generated = new RooDataHist("data_obs_"+ch_name+"_"+year+"_"+cat, "data_obs_"+ch_name+"_"+year+"_"+cat, RooArgList(*x), myhist2);
 
  // RooFitResult *r_bg_exp = bgfunc_exp->fitTo(*dataSR_generated, RooFit::Range(xmin,xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  // infotofile << "---------  Bgexp  "+ch_name+"  - ---------"<<std::endl;
  // infotofile << "bgexp2_p0"<<ch_name<<"_"<<year<<"   " << bgexp2_p0->getValV() <<"  error  "<< 100* bgexp2_p0->getError()<<std::endl;
  // infotofile << "bgexp2_p1"<<ch_name<<"_"<<year<<"   " << bgexp2_p1->getValV() <<"  error  "<< 100*bgexp2_p1->getError()<<std::endl;

  //  RooFitResult *r_bg = bgfunc->fitTo(*dataSR_generated, RooFit::Range(xmin, xmax), RooFit::Save(), RooFit::Verbose(kFALSE));
  // infotofile << "---------  Bg3p  "+ch_name+"  ---------"<<std::endl;
  // infotofile << "bg3p_p0"<<ch_name<<"_"<<year<<"  " << bg3p_p0->getValV() <<"  error  "<< 100*bg3p_p0->getError()<<std::endl;
  // infotofile << "bg3p_p1"<<ch_name<<"_"<<year<<"   " << bg3p_p1->getValV() <<"  error  "<< 100*bg3p_p1->getError()<<std::endl;
  // infotofile << "bg3p_p2"<<ch_name<<"_"<<year<<"   " << bg3p_p2->getValV() <<"  error  "<< 100*bg3p_p2->getError()<<std::endl;


  // save the data to the workspace
    fWS->import(*dataSR);
    //     fWS->import(*dataSR_generated);

  // save the bkg fit to the workspace
  //fWS->import(*bgfunc);

  // save the bkg systematic fit to the workspace
  //  fWS->import(*bgfunc_4p);
  fWS->import(category);
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
  infotofile << "Cat: "+cat+" N = " << Ntot << endl << endl;

  //  RooPlot *plotter=x->frame();
  //  dataSR->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
  bgfunc->plotOn(plotter);
  plotter->getAttLine()->SetLineColor(kRed);
  plotter->getAttLine()->SetLineStyle(kDotted);  
  bgfunc_exp->plotOn(plotter);

  TCanvas *background_c = new TCanvas("background_c","background fit",10,10,700,700);
  background_c->cd();
  gPad->SetLogy();
  plotter->Draw();
  background_c->SaveAs("plots/nominal_bkg_3pfit_"+ch_name+"_"+year+"_"+cat+".pdf");


  // clean up
  delete x; delete dataSR; 
   delete plotter;
  delete bgfunc;
  delete bgfunc_exp;
  delete bg3p_p0; delete bg3p_p1; delete bg3p_p2; 
  delete r_bg;
  //  delete background_c;

  return;

}

void CreateRooWorkspace::SaveSignals(defs::Echannel ch, TString year, TString cat, TString MA)
{
  // set up name
  TString ch_name; 
  if (ch==defs::eMuon){
    ch_name = "much";
  } else {
    ch_name = "ech";
  }


  std::ifstream infile("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_"+year+"_"+cat+"_"+MA+".txt");
  std::ifstream infile_much("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_"+year+"_"+cat+"_"+MA+"_much.txt");
  std::ifstream infile_ech("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_"+year+"_"+cat+"_"+MA+"_ech.txt");
  //  std::ifstream infile_allyears("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_allyears_"+ch_name+".txt");
  std::ifstream infile_allyears("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput_"+year+"_"+cat+"_"+MA+".txt");

  std::string line;

  string fitname, paramname;
  double param0_mean_value,param0_mean_error,param1_mean_value,param1_mean_error,param0_sigma_value,param0_sigma_error,param1_sigma_value,param1_sigma_error;
  double param0_eff_value_much,param1_eff_value_much,param2_eff_value_much,param0_eff_value_ech,param1_eff_value_ech,param2_eff_value_ech;
  double param0_mean2_value,param0_mean2_error,param1_mean2_value,param1_mean2_error,param0_sigma2_value,param0_sigma2_error,param1_sigma2_value,param1_sigma2_error,param2_sigma2_value,param2_sigma2_error,param0_fnorm_value,param1_fnorm_value,param0_fnorm_error,param1_fnorm_error;
  double param0_JECmeanup_value,param1_JECmeanup_value,param0_JECsigmaup_value,param1_JECsigmaup_value,param0_JECmeanup2_value,param1_JECmeanup2_value,param0_JECsigmaup2_value,param1_JECsigmaup2_value,param2_JECsigmaup2_value,param0_JECfnormup_value,param1_JECfnormup_value;
  double param0_JERmeanup_value,param1_JERmeanup_value,param0_JERsigmaup_value,param1_JERsigmaup_value,param0_JERmeanup2_value,param1_JERmeanup2_value,param0_JERsigmaup2_value,param1_JERsigmaup2_value,param2_JERsigmaup2_value,param0_JERfnormup_value,param1_JERfnormup_value;

  double param0_JECmeandown_value,param1_JECmeandown_value,param0_JECsigmadown_value,param1_JECsigmadown_value,param0_JECmeandown2_value,param1_JECmeandown2_value,param0_JECsigmadown2_value,param1_JECsigmadown2_value,param2_JECsigmadown2_value,param0_JECfnormdown_value,param1_JECfnormdown_value;
  double param0_JERmeandown_value,param1_JERmeandown_value,param0_JERsigmadown_value,param1_JERsigmadown_value,param0_JERmeandown2_value,param1_JERmeandown2_value,param0_JERsigmadown2_value,param1_JERsigmadown2_value,param2_JERsigmadown2_value,param0_JERfnormdown_value,param1_JERfnormdown_value;

  while (infile)
    {
      string c,d;
      infile>>fitname>>paramname>>c>>d;
      if((fitname.find("meanfit")!=std::string::npos) && (paramname.find("param0")!=std::string::npos)) {
	param0_mean_value = std::stod(c); 
	param0_mean_error = std::stod(d);
      }
      if(fitname.find("meanfit")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
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
      if(fitname.find("widthfit")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
	param0_sigma_value = std::stod(c); 
	param0_sigma_error = std::stod(d);
      }
      if(fitname.find("widthfit")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
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
    }

  while(infile_much){
      string c,d;
      infile_much>>fitname>>paramname>>c>>d;

      if(fitname.find("eff")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
      	param0_eff_value_much = std::stod(c); 
      }
      if(fitname.find("eff")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
      	param1_eff_value_much = std::stod(c); 
      }
      if(fitname.find("eff")!=std::string::npos && paramname.find("param2")!=std::string::npos) {
      	param2_eff_value_much = std::stod(c); 
      }

  }

  while(infile_ech){
      string c,d;
      infile_ech>>fitname>>paramname>>c>>d;

      if(fitname.find("eff")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
      	param0_eff_value_ech = std::stod(c); 
      }
      if(fitname.find("eff")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
      	param1_eff_value_ech = std::stod(c); 
      }
      if(fitname.find("eff")!=std::string::npos && paramname.find("param2")!=std::string::npos) {
      	param2_eff_value_ech = std::stod(c); 
      }

  }

  while (infile_allyears)
    {
      string c,d;
      infile_allyears>>fitname>>paramname>>c>>d;
      //      std::cout<<"fitname  " << fitname << "  paramname  "<< paramname <<"  c  "<<c<<"  d  "<<d<<std::endl;

      if((fitname.find("mean2fit")!=std::string::npos) && (paramname.find("param0")!=std::string::npos)) {
	param0_mean2_value = std::stod(c); 
	param0_mean2_error = std::stod(d);
      }
      if(fitname.find("mean2fit")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
	param1_mean2_value = std::stod(c); 
	param1_mean2_error = std::stod(d);
      }
      if((fitname.find("mfitup")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECmeanup2_value = std::stod(c); 
      }
      if(fitname.find("mfitup")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECmeanup2_value = std::stod(c); 
      }
      if((fitname.find("mfitup")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERmeanup2_value = std::stod(c); 
      }
      if(fitname.find("mfitup")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERmeanup2_value = std::stod(c); 
      }
      if((fitname.find("mfitdown")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECmeandown2_value = std::stod(c); 
      }
      if(fitname.find("mfitdown")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECmeandown2_value = std::stod(c); 
      }
      if((fitname.find("mfitdown")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERmeandown2_value = std::stod(c); 
      }
      if(fitname.find("mfitdown")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERmeandown2_value = std::stod(c); 
      }
      if(fitname.find("width2fit")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
	param0_sigma2_value = std::stod(c); 
	param0_sigma2_error = std::stod(d);
      }
      if(fitname.find("width2fit")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
	param1_sigma2_value = std::stod(c); 
	param1_sigma2_error = std::stod(d);
      }
      if(fitname.find("width2fit")!=std::string::npos && paramname.find("param2")!=std::string::npos) {
	param2_sigma2_value = std::stod(c); 
	param2_sigma2_error = std::stod(d);
      }
      if((fitname.find("wfitup")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECsigmaup2_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECsigmaup2_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param4")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param2_JECsigmaup2_value = std::stod(c); 
      }
      if((fitname.find("wfitup")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERsigmaup2_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERsigmaup2_value = std::stod(c); 
      }
      if(fitname.find("wfitup")!=std::string::npos && paramname.find("param4")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param2_JERsigmaup2_value = std::stod(c); 
      }
      if((fitname.find("wfitdown")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECsigmadown2_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECsigmadown2_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param4")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param2_JECsigmadown2_value = std::stod(c); 
      }
      if((fitname.find("wfitdown")!=std::string::npos) && (paramname.find("param2")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERsigmadown2_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param3")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERsigmadown2_value = std::stod(c); 
      }
      if(fitname.find("wfitdown")!=std::string::npos && paramname.find("param4")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param2_JERsigmadown2_value = std::stod(c); 
      }
      if(fitname.find("fnormfit")!=std::string::npos && paramname.find("param0")!=std::string::npos) {
	param0_fnorm_value = std::stod(c); 
	param0_fnorm_error = std::stod(d);
      }
      if(fitname.find("fnormfit")!=std::string::npos && paramname.find("param1")!=std::string::npos) {
	param1_fnorm_value = std::stod(c); 
	param1_fnorm_error = std::stod(d);
      }
      if((fitname.find("ffitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECfnormup_value = std::stod(c); 
      }
      if(fitname.find("ffitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECfnormup_value = std::stod(c); 
      }
      if((fitname.find("ffitup")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERfnormup_value = std::stod(c); 
      }
      if(fitname.find("ffitup")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERfnormup_value = std::stod(c); 
      }
      if((fitname.find("ffitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JEC")!=std::string::npos)) {
	param0_JECfnormdown_value = std::stod(c); 
      }
      if(fitname.find("ffitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JEC")!=std::string::npos)) {
	param1_JECfnormdown_value = std::stod(c); 
      }
      if((fitname.find("ffitdown")!=std::string::npos) && (paramname.find("param0")!=std::string::npos) && (fitname.find("JER")!=std::string::npos)) {
	param0_JERfnormdown_value = std::stod(c); 
      }
      if(fitname.find("ffitdown")!=std::string::npos && paramname.find("param1")!=std::string::npos&& (fitname.find("JER")!=std::string::npos)) {
	param1_JERfnormdown_value = std::stod(c); 
      }

    }


  // from the fit to signals: get mean and width from combined fit (higher stats)
  TF1* mean = new TF1("meanfit", "[0]+[1]*(x-600)", 500, 1250);
  // mean->SetParameter(0, 584.9);
  // mean->SetParameter(1, 0.9755);
  mean->SetParameter(0, param0_mean_value);
  mean->SetParameter(1, param1_mean_value);

  TF1* mean_error = new TF1("meanfit_error", "[0]+[1]*(x-600)", 500, 1250);
  //mean_error->SetParameter(0, param0_mean_value+2*param0_mean_error);
  //  mean_error->SetParameter(1, param1_mean_value+2*0.5 *param1_mean_error);
  mean_error->SetParameter(0, param0_mean_value+param0_mean_error);
  mean_error->SetParameter(1, param1_mean_value+0.5 *param1_mean_error);


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

  ////////second mean
  TString mean2functionstring = "1";
  TF1* mean2 = new TF1("mean2fit", mean2functionstring, 500, 1250);
  // mean2->SetParameter(0, 584.9);
  // mean2->SetParameter(1, 0.9755);
  // mean2->SetParameter(0, param0_mean2_value);
  // mean2->SetParameter(1, param1_mean2_value);

  TF1* mean2_error = new TF1("mean2fit_error", mean2functionstring, 500, 1250);
  //mean2_error->SetParameter(0, param0_mean2_value+2*param0_mean2_error);
  //  mean2_error->SetParameter(1, param1_mean2_value+2*0.5 *param1_mean2_error);
  // mean2_error->SetParameter(0, param0_mean2_value+param0_mean2_error);
  // mean2_error->SetParameter(1, param1_mean2_value+0.5 *param1_mean2_error);


  TF1* JECmeanup2_error = new TF1("JECmean2fit_error", mean2functionstring, 500, 1250);
  // JECmeanup2_error->SetParameter(0, param0_JECmeanup2_value);
  // JECmeanup2_error->SetParameter(1, param1_JECmeanup2_value);

  TF1* JERmeanup2_error = new TF1("JERmean2fitup_error", mean2functionstring, 500, 1250);
  // JERmeanup2_error->SetParameter(0, param0_JERmeanup2_value);
  // JERmeanup2_error->SetParameter(1, param1_JERmeanup2_value);

  TF1* JECmeandown2_error = new TF1("JECmean2fit_error", mean2functionstring, 500, 1250);
  // JECmeandown2_error->SetParameter(0, param0_JECmeandown2_value);
  // JECmeandown2_error->SetParameter(1, param1_JECmeandown2_value);

  TF1* JERmeandown2_error = new TF1("JERmean2fitdown_error", mean2functionstring, 500, 1250);
  // JERmeandown2_error->SetParameter(0, param0_JERmeandown2_value);
  // JERmeandown2_error->SetParameter(1, param1_JERmeandown2_value);


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

  /////////second sigma
  TF1* sigma2 = new TF1("sigma2fit", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  sigma2->SetParameter(0, param0_sigma2_value);
  sigma2->SetParameter(1, param1_sigma2_value);
  sigma2->SetParameter(2, param2_sigma2_value);

  TF1* sigma2_error = new TF1("sigma2fit_error", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  sigma2_error->SetParameter(0, param0_sigma2_value+param0_sigma2_error);
  sigma2_error->SetParameter(1, param1_sigma2_value+0.5*param1_sigma2_error);
  sigma2_error->SetParameter(2, param2_sigma2_value+0.5*param2_sigma2_error);

  TF1* JECsigmaup2_error = new TF1("JECsigma2fitup_error", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  JECsigmaup2_error->SetParameter(0, param0_JECsigmaup2_value);
  JECsigmaup2_error->SetParameter(1, param1_JECsigmaup2_value);
  JECsigmaup2_error->SetParameter(2, param2_JECsigmaup2_value);


  TF1* JERsigmaup2_error = new TF1("JERsigma2fitup_error", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  JERsigmaup2_error->SetParameter(0, param0_JERsigmaup2_value);
  JERsigmaup2_error->SetParameter(1, param1_JERsigmaup2_value);
  JERsigmaup2_error->SetParameter(2, param2_JERsigmaup2_value);

  TF1* JECsigmadown2_error = new TF1("JECsigma2fitdown_error", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  JECsigmadown2_error->SetParameter(0, param0_JECsigmadown2_value);
  JECsigmadown2_error->SetParameter(1, param1_JECsigmadown2_value);
  JECsigmadown2_error->SetParameter(2, param2_JECsigmadown2_value);


  TF1* JERsigmadown2_error = new TF1("JERsigma2fitdown_error", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 500, 1250);
  JERsigmadown2_error->SetParameter(0, param0_JERsigmadown2_value);
  JERsigmadown2_error->SetParameter(1, param1_JERsigmadown2_value);
  JERsigmadown2_error->SetParameter(2, param2_JERsigmadown2_value);

  /////////second fnorm
  TF1* fnorm = new TF1("fnormfit", "[0]+[1]*(x-600)", 500, 1250);
  fnorm->SetParameter(0, param0_fnorm_value);
  fnorm->SetParameter(1, param1_fnorm_value);

  TF1* fnorm_error = new TF1("fnormfit_error", "[0]+[1]*(x-600)", 500, 1250);
  fnorm_error->SetParameter(0, param0_fnorm_value+param0_fnorm_error);
  fnorm_error->SetParameter(1, param1_fnorm_value+0.5*param1_fnorm_error);

  TF1* JECfnormup_error = new TF1("JECfnormfitup_error", "[0]+[1]*(x-600)", 500, 1250);
  JECfnormup_error->SetParameter(0, param0_JECfnormup_value);
  JECfnormup_error->SetParameter(1, param1_JECfnormup_value);


  TF1* JERfnormup_error = new TF1("JERfnormfitup_error", "[0]+[1]*(x-600)", 500, 1250);
  JERfnormup_error->SetParameter(0, param0_JERfnormup_value);
  JERfnormup_error->SetParameter(1, param1_JERfnormup_value);

  TF1* JECfnormdown_error = new TF1("JECfnormfitdown_error", "[0]+[1]*(x-600)", 500, 1250);
  JECfnormdown_error->SetParameter(0, param0_JECfnormdown_value);
  JECfnormdown_error->SetParameter(1, param1_JECfnormdown_value);


  TF1* JERfnormdown_error = new TF1("JERfnormfitdown_error", "[0]+[1]*(x-600)", 500, 1250);
  JERfnormdown_error->SetParameter(0, param0_JERfnormdown_value);
  JERfnormdown_error->SetParameter(1, param1_JERfnormdown_value);


  TF1* eff_ele = new TF1("eff_ele", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
    // eff_ele->SetParameter(0, 0.00387224);
    // eff_ele->SetParameter(1, 7.31594e-06);
    // eff_ele->SetParameter(2, -8.14789e-09);
    eff_ele->SetParameter(0, param0_eff_value_ech);
    eff_ele->SetParameter(1, param1_eff_value_ech);
    eff_ele->SetParameter(2, param2_eff_value_ech);

  if(year.Contains("2017")){
    // eff_ele->SetParameter(0, 0.00190037);
    // eff_ele->SetParameter(1,  3.58322e-06);
    // eff_ele->SetParameter(2, -5.20436e-09);
    eff_ele->SetParameter(0, param0_eff_value_ech);
    eff_ele->SetParameter(1, param1_eff_value_ech);
    eff_ele->SetParameter(2, param2_eff_value_ech);

  }
  if(year.Contains("2018")){ 
    eff_ele->SetParameter(0, param0_eff_value_ech);
    eff_ele->SetParameter(1, param1_eff_value_ech);
    eff_ele->SetParameter(2, param2_eff_value_ech);    
  }


  TF1* eff_muon = new TF1("eff_muon", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 500, 1250);
  eff_muon->SetParameter(0, param0_eff_value_much);
  eff_muon->SetParameter(1, param1_eff_value_much);
  eff_muon->SetParameter(2, param2_eff_value_much);


  if(year.Contains("2017")){
    eff_muon->SetParameter(0, param0_eff_value_much);
    eff_muon->SetParameter(1, param1_eff_value_much);
    eff_muon->SetParameter(2, param2_eff_value_much);

  }
  if(year.Contains("2018")){
    eff_muon->SetParameter(0, param0_eff_value_much);
    eff_muon->SetParameter(1, param1_eff_value_much);
    eff_muon->SetParameter(2, param2_eff_value_much);
  }

  infotofile << "===== Number of events for Signal in the "; 
  if (ch==defs::eEle) infotofile << "Electron channel =====" << std::endl;
  else infotofile << "Muon channel =====" << std::endl;

  // loop over mass points, create PDFs and store them
  double MT = 550; 
  if(cat.Contains("ma300")) MT = 700;
  //  if(cat.Contains("ma300") && year.Contains("2018")) MT = 800;
  if(cat.Contains("ma175")) MT = 700;
  //  RooRealVar* x = new RooRealVar("x"+cat+year, "m_{T} [GeV]", 200, 2000);
  RooRealVar* x = new RooRealVar("x"+cat+year, "m_{T} [GeV]", xmin, xmax);
  // RooRealVar* x = new RooRealVar("x"+cat, "m_{T} [GeV]", 200, 2000);

  while (MT<1200)
      //  while (MT<810)
  {
    TString SgName = TString::Format("MT%d_", (int)MT);
    SgName.Append(ch_name);
    SgName.Append(year);
    SgName.Append("_");
    SgName.Append(cat);

    //       RooConstVar* sg_mean  =new RooConstVar("sg_mean",  "sg_mean",  mean->Eval(MT));
    RooRealVar* sg_mean  =new RooRealVar("sg_mean_"+year+"_"+cat,  "sg_mean",  mean->Eval(MT));

    RooRealVar* sg_mean_variation  =new RooRealVar("sg_mean_variation",  "sg_mean_variation",  mean_error->Eval(MT));

    RooRealVar* sg_mean2  =new RooRealVar("sg_mean2_"+year+"_"+cat,  "sg_mean2",  mean2->Eval(MT));

    //    RooConstVar* sg_sigma =new RooConstVar("sg_sigma", "sg_sigma", sigma->Eval(MT));
    RooRealVar* sg_sigma =new RooRealVar("sg_sigma_"+year+"_"+cat, "sg_sigma", sigma->Eval(MT));
    RooRealVar* sg_sigma2 =new RooRealVar("sg_sigma2_"+year+"_"+cat, "sg_sigma2", sigma2->Eval(MT));
    
    RooRealVar* sg_fnorm =new RooRealVar("sg_fnorm_"+year+"_"+cat, "sg_fnorm", fnorm->Eval(MT));

    RooConstVar* sg_JECmeanup  =new RooConstVar("sg_JECmeanup_"+year+"_"+cat,  "sg_JECmeanup",  JECmeanup_error->Eval(MT));
    RooConstVar* sg_JECsigmaup =new RooConstVar("sg_JECsigmaup_"+year+"_"+cat, "sg_JECsigmaup", JECsigmaup_error->Eval(MT));
    RooConstVar* sg_JECmeanup2  =new RooConstVar("sg_JECmeanup2_"+year+"_"+cat,  "sg_JECmeanup2",  JECmeanup2_error->Eval(MT));
    RooConstVar* sg_JECsigmaup2 =new RooConstVar("sg_JECsigmaup2_"+year+"_"+cat, "sg_JECsigmaup2", JECsigmaup2_error->Eval(MT));
    RooConstVar* sg_JECfnormup  =new RooConstVar("sg_JECfnormup_"+year+"_"+cat,  "sg_JECfnormup",  JECfnormup_error->Eval(MT));

    RooConstVar* sg_JERmeanup  =new RooConstVar("sg_JERmeanup_"+year+"_"+cat,  "sg_JERmeanup",  JERmeanup_error->Eval(MT));
    RooConstVar* sg_JERsigmaup =new RooConstVar("sg_JERsigmaup_"+year+"_"+cat, "sg_JERsigmaup", JERsigmaup_error->Eval(MT));
    RooConstVar* sg_JERmeanup2  =new RooConstVar("sg_JERmeanup2_"+year+"_"+cat,  "sg_JERmeanup2",  JERmeanup2_error->Eval(MT));
    RooConstVar* sg_JERsigmaup2 =new RooConstVar("sg_JERsigmaup2_"+year+"_"+cat, "sg_JERsigmaup2", JERsigmaup2_error->Eval(MT));
    RooConstVar* sg_JERfnormup  =new RooConstVar("sg_JERfnormup_"+year+"_"+cat,  "sg_JERfnormup",  JERfnormup_error->Eval(MT));

    RooConstVar* sg_JECmeandown  =new RooConstVar("sg_JECmeandown_"+year+"_"+cat,  "sg_JECmeandown",  JECmeandown_error->Eval(MT));
    RooConstVar* sg_JECsigmadown =new RooConstVar("sg_JECsigmadown_"+year+"_"+cat, "sg_JECsigmadown", JECsigmadown_error->Eval(MT));
    RooConstVar* sg_JECmeandown2  =new RooConstVar("sg_JECmeandown2_"+year+"_"+cat,  "sg_JECmeandown2",  JECmeandown2_error->Eval(MT));
    RooConstVar* sg_JECsigmadown2 =new RooConstVar("sg_JECsigmadown2_"+year+"_"+cat, "sg_JECsigmadown2", JECsigmadown2_error->Eval(MT));
    RooConstVar* sg_JECfnormdown  =new RooConstVar("sg_JECfnormdown_"+year+"_"+cat,  "sg_JECfnormdown",  JECfnormdown_error->Eval(MT));

    RooConstVar* sg_JERmeandown  =new RooConstVar("sg_JERmeandown_"+year+"_"+cat,  "sg_JERmeandown",  JERmeandown_error->Eval(MT));
    RooConstVar* sg_JERsigmadown =new RooConstVar("sg_JERsigmadown_"+year+"_"+cat, "sg_JERsigmadown", JERsigmadown_error->Eval(MT));
    RooConstVar* sg_JERmeandown2  =new RooConstVar("sg_JERmeandown2_"+year+"_"+cat,  "sg_JERmeandown2",  JERmeandown2_error->Eval(MT));
    RooConstVar* sg_JERsigmadown2 =new RooConstVar("sg_JERsigmadown2_"+year+"_"+cat, "sg_JERsigmadown2", JERsigmadown2_error->Eval(MT));
    RooConstVar* sg_JERfnormdown  =new RooConstVar("sg_JERfnormdown_"+year+"_"+cat,  "sg_JERfnormdown",  JERfnormdown_error->Eval(MT));

    
    std::cout<<"Cat:  "<< cat <<"  MT "<<MT<< "  sg_mean  "<<sg_mean->getValV()<<"  sg_sigma  "<<sg_sigma->getValV()<<"  sg_mean2  "<<sg_mean2->getValV()<<"  sg_sigma2  "<<sg_sigma2->getValV()<<std::endl;
    //Hier
    RooCBShape* ModelSg_Gauss = new RooCBShape(SgName, SgName, *x, *sg_mean, *sg_sigma,*sg_sigma2,*sg_mean2);
    // SignalDoubleGauss* ModelSg_JECup_Gauss = new SignalDoubleGauss(SgName+"_JECup", SgName+"_JECup", *x, *sg_JECmeanup, *sg_JECsigmaup,*sg_JECmeanup2, *sg_JECsigmaup2, *sg_JECfnormup);
    // SignalDoubleGauss* ModelSg_JERup_Gauss = new SignalDoubleGauss(SgName+"_JERup", SgName+"_JERup", *x, *sg_JERmeanup, *sg_JERsigmaup,*sg_JERmeanup2, *sg_JERsigmaup2, *sg_JERfnormup);
    // SignalDoubleGauss* ModelSg_JECdown_Gauss = new SignalDoubleGauss(SgName+"_JECdown", SgName+"_JECdown", *x, *sg_JECmeandown, *sg_JECsigmadown,*sg_JECmeandown2, *sg_JECsigmadown2, *sg_JECfnormdown);
    // SignalDoubleGauss* ModelSg_JERdown_Gauss = new SignalDoubleGauss(SgName+"_JERdown", SgName+"_JERdown", *x, *sg_JERmeandown, *sg_JERsigmadown,*sg_JERmeandown2, *sg_JERsigmadown2, *sg_JERfnormdown);


    RooCBShape* ModelSg_JECup_Gauss = new RooCBShape(SgName+"_JECup", SgName+"_JECup", *x, *sg_JECmeanup, *sg_JECsigmaup, *sg_JECsigmaup2,*sg_JECmeanup2);
    RooCBShape* ModelSg_JERup_Gauss = new RooCBShape(SgName+"_JERup", SgName+"_JERup", *x, *sg_JERmeanup, *sg_JERsigmaup, *sg_JERsigmaup2,*sg_JERmeanup2);
    RooCBShape* ModelSg_JECdown_Gauss = new RooCBShape(SgName+"_JECdown", SgName+"_JECdown", *x, *sg_JECmeandown, *sg_JECsigmadown, *sg_JECsigmadown2,*sg_JECmeandown2);
    RooCBShape* ModelSg_JERdown_Gauss = new RooCBShape(SgName+"_JERdown", SgName+"_JERdown", *x, *sg_JERmeandown, *sg_JERsigmadown, *sg_JERsigmadown2,*sg_JERmeandown2);


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
    ModelSg_JECdown_Gauss->plotOn(plotter, RooFit::LineColor(kTeal));
    ModelSg_JERdown_Gauss->plotOn(plotter, RooFit::LineColor(kGray));
    ModelSg_JERup_Gauss->plotOn(plotter, RooFit::LineColor(kBlack));
    plotter->Draw();
    c_sg->SaveAs("plots/Fit_Sg"+SgName+"_"+year+"_"+cat+".pdf");
    c_sg->SaveAs("plots/Fit_Sg"+SgName+"_"+year+"_"+cat+".eps");

    /*std::cout << "MT = " << MT << " mean = " << mean->Eval(MT) << " sigma = " << sigma->Eval(MT) << std::endl;
    delete c_sg; 
    */

    // now write out signal efficiency
    // expected number of events, based on luminosity of 35800 pb^-1:
    double eff = 0;
    if (ch==defs::eEle) eff = eff_ele->Eval(MT);
    else eff = eff_muon->Eval(MT);
    std::cout<<"================ eff  "<< eff<<std::endl;

    double BR = 0.3258;
    double Nevts = 36330*eff*BR;
    if(year.Contains("2017")) Nevts = 41530*eff*BR;
    if(year.Contains("2018")) Nevts = 59740*eff*BR;
    //    infotofile << "MT = " << MT << " GeV,  N = " << Nevts <<" ,  Mean  "<< mean->Eval(MT)<<"  , Mean Error  "<<mean_error->Eval(MT)-mean->Eval(MT)<<"  Sigma  "<<sigma->Eval(MT)<<"  Sigma Error "<<sigma_error->Eval(MT)-sigma->Eval(MT)<< std::endl;
    infotofile << "MT = " << MT << " GeV,  N = " << Nevts <<" ,  Mean  "<< mean->Eval(MT)<<"  , Mean Error  "<<(mean_error->Eval(MT)-mean->Eval(MT))<<"  Sigma  "<<sigma->Eval(MT)<<"  Sigma Error "<<sigma_error->Eval(MT)-sigma->Eval(MT);
    infotofile << "  JERupmean   "<<JERmeanup_error->Eval(MT)<<"   JERupsigma   "<<JERsigmaup_error->Eval(MT)<<"  JERdownmean   "<<JERmeandown_error->Eval(MT)<<"   JERdownsigma   "<<JERsigmadown_error->Eval(MT)<< "  JECupmean   "<<JECmeanup_error->Eval(MT)<<"   JECupsigma   "<<JECsigmaup_error->Eval(MT)<<"  JECdownmean   "<<JECmeandown_error->Eval(MT)<<"   JECdownsigma   "<<JECsigmadown_error->Eval(MT)<<" ,  Mean2  "<< mean2->Eval(MT)<<"  , Mean2 Error  "<<(mean2_error->Eval(MT)-mean2->Eval(MT))<<"  Sigma2  "<<sigma2->Eval(MT)<<"  Sigma2 Error "<<sigma2_error->Eval(MT)-sigma2->Eval(MT);
    infotofile << "  JERupmean2   "<<JERmeanup2_error->Eval(MT)<<"   JERupsigma2   "<<JERsigmaup2_error->Eval(MT)<<"  JERdownmean2   "<<JERmeandown2_error->Eval(MT)<<"   JERdownsigma2   "<<JERsigmadown2_error->Eval(MT)<< "  JECupmean2   "<<JECmeanup2_error->Eval(MT)<<"   JECupsigma2   "<<JECsigmaup2_error->Eval(MT)<<"  JECdownmean2   "<<JECmeandown2_error->Eval(MT)<<"   JECdownsigma2   "<<JECsigmadown2_error->Eval(MT)<<" ,  Fnorm  "<< fnorm->Eval(MT)<<"  , fnorm Error  "<<(fnorm_error->Eval(MT)-fnorm->Eval(MT));
    infotofile << "  JERupfnorm   "<<JERfnormup_error->Eval(MT)<<"  JERdownfnorm   "<<JERfnormdown_error->Eval(MT)<< "  JECupfnorn   "<<JECfnormup_error->Eval(MT)<<"  JECdownfnorm   "<<JECfnormdown_error->Eval(MT)<<std::endl;


    //add uncertainties as special shape
    RooArgList mypdfs;
    mypdfs.add(*ModelSg_Gauss);
    //mypdfs.add(*ModelSg_Gauss_variation);

    mypdfs.add(*ModelSg_JECup_Gauss);
    mypdfs.add(*ModelSg_JERup_Gauss);
    mypdfs.add(*ModelSg_JECdown_Gauss);
    mypdfs.add(*ModelSg_JERdown_Gauss);

    RooCategory category("pdf_index_"+(TString::Format("MT%d", (int)MT))+"_"+ch_name+"_"+year+"_"+cat,"Index of Pdf which is active");
    RooMultiPdf multipdf("roomultipdf_"+(TString::Format("MT%d", (int)MT))+"_"+ch_name+"_"+year+"_"+cat,"All Pdfs",category,mypdfs);



    //    fWS->import(*ModelSg_Gauss);
    fWS->import(category);
    fWS->import(multipdf);


    MT+=25;
  }


}


