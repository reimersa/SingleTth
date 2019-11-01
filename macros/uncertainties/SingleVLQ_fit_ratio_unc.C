#include "TH1.h"
#include <fstream>
using namespace std;


void fit(TString channel, TString region, TString process,  TString fitfunction, TFile *outputfile, bool b_fit = true, TString unc="jec_up", TString directory = "/nfs/dust/cms/user/abenecke/ZPrimeTotTPrime/CMSSW_8X/rootfiles/uncertainties/"){
  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);

TFile * nominal_file = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/NOMINAL/uhh2.AnalysisModuleRunner.MC."+process+".root", "READ");
 TFile * up_file;


 std::cout<<"upfile: "<< directory+"/"+unc+"/uhh2.AnalysisModuleRunner.MC."+process+".root" <<std::endl;
 up_file = new TFile(directory+"/"+unc+"/uhh2.AnalysisModuleRunner.MC."+process+".root", "READ");



  TH1F * h_nominal = (TH1F *)nominal_file->Get("chi2h_2_"+channel+"_"+region+"/M_Tprime_rebinlimit");
  TH1F * h_up = (TH1F *)up_file->Get("chi2h_2_"+channel+"_"+region+"/M_Tprime_rebinlimit");

  TH1F *ratio = (TH1F*)h_up->Clone();
  ratio ->Add(h_nominal,-1);
  ratio->Divide(h_nominal);


  TCanvas *fit_c = new TCanvas("fit_c"+channel+"_"+process,"fit_"+process,10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  
  ratio->Draw();


  TF1* fit = new TF1("fit", fitfunction);  
  ratio->Fit(fit,"S");
  TFitResultPtr fit_result = ratio->Fit(fit,"S");


  fit_c->SaveAs("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/NOMINAL/MacroPlots/fit_"+unc+"_"+channel+"_"+region+"_"+process+".pdf");

  // New up histogram by scaling nominal with the fit result
  TH1F* new_hist = (TH1F*)h_nominal->Clone();
  //  fit ->SetParameter(0,fit_result->Parameter(0));

  if(b_fit){
    for(int i=0; i<new_hist->GetXaxis()->GetNbins();i++){
      double sf = fit_result->Parameter(0);
      new_hist->SetBinContent(i,new_hist->GetBinContent(i)+new_hist->GetBinContent(i) * sf);
    }
  }else{
    for(int i=0; i<new_hist->GetXaxis()->GetNbins();i++){
      new_hist->SetBinContent(i,h_up->GetBinContent(i));
    }
  }

  //Write to a new file
  outputfile->cd();
  TDirectory *cdtof= outputfile->mkdir("chi2h_2_"+channel+"_"+region+"/");
  gDirectory->Cd("chi2h_2_"+channel+"_"+region+"/");
  new_hist->SetName("M_Tprime_rebinlimit");
  new_hist->Write();


}


void SingleVLQ_fit_ratio_unc(){

  gROOT->SetBatch(kTRUE);

  TFile *  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/TTbar_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JEC_up/SingleTop_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/TTbar_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/SingleTop_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/TTbar_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/SingleTop_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/TTbar_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_up/SingleTop_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  //////und fuer down
  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JEC_down/TTbar_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JEC_down/SingleTop_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_down/TTbar_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_down/SingleTop_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_down/TTbar_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JEC_down/SingleTop_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JEC_down/TTbar_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "TTbar_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JEC_down/SingleTop_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JEC_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  //und fuer JER
   outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/TTbar_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "TTbar_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JER_up/SingleTop_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/TTbar_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "TTbar_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/SingleTop_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/TTbar_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "TTbar_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/SingleTop_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/TTbar_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "TTbar_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_up/SingleTop_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_up","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  //////und fuer down
  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JER_down/TTbar_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "TTbar_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JER_down/SingleTop_2016v3_much_sr.root","RECREATE");
  fit("much","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_down/TTbar_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "TTbar_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_down/SingleTop_2016v3_ech_sr.root","RECREATE");
  fit("ech","sr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_down/TTbar_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "TTbar_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();


  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/JER_down/SingleTop_2016v3_much_cr.root","RECREATE");
  fit("much","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JER_down/TTbar_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "TTbar_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();

  outputfile = new TFile("/nfs/dust/cms/user/reimersa/SingleTth/Fullselection//JER_down/SingleTop_2016v3_ech_cr.root","RECREATE");
  fit("ech","cr", "SingleTop_2016v3", "[0]",outputfile, true, "JER_down","/nfs/dust/cms/user/reimersa/SingleTth/Fullselection/");
  outputfile->Close();




}
