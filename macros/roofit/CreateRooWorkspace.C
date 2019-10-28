#include "CreateRooWorkspace.h"
#include "BkgPdf4p.h" 

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

}

TH1F* CreateRooWorkspace::GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds)
{
	using namespace defs;

	// folder where the analysis output files are 
	TString anaoutputfolder = "../../../AnalysisOutput_102X/"; 
	TString year = "2016";
	
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
}

void CreateRooWorkspace::SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel channel, bool dodata, bool all_bkgds)
{
  using namespace std;

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

  RooRealVar* x = new RooRealVar("x", "m_{T} [GeV]", plot_low, plot_high);
  RooDataHist* dataSR = new RooDataHist("dataSR_"+ch_name, "dataSR_"+ch_name, RooArgList(*x), h_data);

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

  RooRealVar* bg_p0 = new RooRealVar("bg_p0", "bg_p0", 66.45, -1000,  1000);
  RooRealVar* bg_p1 = new RooRealVar("bg_p1", "bg_p1", -12.6, -1000,  1000);
  RooRealVar* bg_p2 = new RooRealVar("bg_p2", "bg_p2",  -9.6, -100,   100 );
  RooRealVar* bg_p3 = new RooRealVar("bg_p3", "bg_p3",  -5.3, -100,   100 );

  BkgPdf4p* bgfunc = new BkgPdf4p("Dijet4p_"+ch_name,"Dijet4p_"+ch_name, *x, *bg_p0, *bg_p1, *bg_p2, *bg_p3);

  RooFitResult *r_bg = bgfunc->fitTo(*dataSR, RooFit::Range(xmin, xmax), RooFit::Save(), RooFit::Verbose(kFALSE));

  std::cout << "Testing BKG values postfit" << '\n';
  bg_p0->Print(); 
  bg_p1->Print();
  bg_p2->Print(); 
  bg_p3->Print(); 

  dataSR->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
  bgfunc->plotOn(plotter);
  cout << "chi^2 = " << plotter->chiSquare() << endl;

  TCanvas *background_c = new TCanvas("background_c","background fit",10,10,700,700);
  background_c->cd();
  //gPad->SetLogy();
  plotter->Draw();
  background_c->SaveAs("test_"+ch_name+".pdf");

  // save the data to the workspace
  fWS->import(*dataSR);

  // save the bkg fit to the workspace
  fWS->import(*bgfunc);

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
  delete bg_p0; delete bg_p1; delete bg_p2; delete bg_p3; 
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

    RooRealVar* sg_mean  =new RooRealVar("sg_mean",  "sg_mean",  mean->Eval(MT),  200, 2000);
    RooRealVar* sg_sigma =new RooRealVar("sg_sigma", "sg_sigma", sigma->Eval(MT),  10, 1000);

    RooGaussian* ModelSg_Gauss = new RooGaussian(SgName, SgName, *x, *sg_mean, *sg_sigma);

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

    MT+=50;
  }


}


