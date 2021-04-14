#include "TH1.h"
#include <fstream>
#include "sigfunc.C"
#include "../bkgfits/func.C"
#include "../bkgfits/helpers.C"

using namespace std;
bool b_test = true;
bool b_fitrange = false;
std::vector<float> sigma_ranges = {1.5,2,2.5};

//write out the number of total events vs events in fitrange
std::ofstream Nevttofile("Nevents_test.txt", std::ios::out | std::ios::trunc);


void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,std::vector<double>& means2, std::vector<double>& means2_err, std::vector<double>& widths2, std::vector<double>& widths2_err, std::vector<double>& fnorm, std::vector<double>& fnorm_err,TString unc="", TString year = "2016v3", float factormin=2, float factormax=2.5, bool write_Nevet = false);
void draw_eff_unc(TGraphErrors* geff, TGraphErrors* geff_up, TGraphErrors* geff_dn, TString name);

void sig_fit()
{
  //TString year =  "2016v3";
  TString year =  "2017v2";
  //TString year =  "2018";
  //TString year =  "allyears";

  //  TString postfix = "_HEMIssue_LH";
  TString postfix = "";


  // decide which channel to do (eEle, eMuon, eComb)
  //  Echannel ch = eComb;
  //  Echannel ch = eEle;
  Echannel ch = eMuon;

  TString outfile_name = "SignalFitOutput_"+year+postfix+".txt";
  if(ch==eEle) outfile_name = "SignalFitOutput_"+year+"_ech"+postfix+".txt";
  if(ch==eMuon) outfile_name = "SignalFitOutput_"+year+"_much"+postfix+".txt";

  std::ofstream infotofile(outfile_name, std::ios::out | std::ios::trunc);

  std::vector<TString> uncertainties ={}; // no syst.
  if(!b_test) {
       uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","prefiring","btag_bc","btag_udsg","eletrigger","mutrigger"}; // 2016 
       if(year.Contains("2017"))  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","prefiring","btag_bc","btag_udsg","eletrigger","mutrigger","scale"}; // 2017 
    if(year.Contains("2018"))  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","btag_bc","btag_udsg","eletrigger","mutrigger","scale"};
    //    if(year.Contains("allyears")) uncertainties = {"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","btag_udsg","eletrigger","mutrigger","btag_bc","scale"}; // prefiring missing
    if(year.Contains("allyears")) uncertainties = {"JEC","JER"}; // prefiring missing
  }

  std::vector<double> MTs = {600};// 2016
  std::vector<double> MTs_backup = {600};// 2016
  //hier
  MTs = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017
  MTs_backup = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017


  if (!b_test){

    //MTs = {700,800, 900, 1000, 1200};// 2016
    //MTs_backup = {700,800, 900, 1000, 1200};// 2016

    // Roman's setup
    MTs = {700, 900, 1000, 1200};// 2016
    MTs_backup = {700, 900, 1000, 1200};// 2016

    if(year.Contains("2018") or year.Contains("2017")){
      MTs = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017
      MTs_backup = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017

      // MTs = {600};// 2017
      // MTs_backup = {600};// 2017

    }
    if(year.Contains("allyears")){
      MTs = { 600,650,700,800,900, 1000,1100, 1200};// 2017
      MTs_backup = { 600,650,700,800,900, 1000,1100, 1200};// 2017
    }
  }

  std::vector<double> means;
  std::vector<double> means_err;
  std::vector<double> widths;  
  std::vector<double> widths_err;  
  std::vector<double> effs;  
  std::vector<double> effs_err;  
  std::vector<double> means2;
  std::vector<double> means2_err;
  std::vector<double> widths2;  
  std::vector<double> widths2_err;  
  std::vector<double> fnorm;  
  std::vector<double> fnorm_err;  
  std::vector<double> zeros;

  // do the fits, fill results into graph
  for (int i=0; i<MTs.size(); ++i){
    fitsignal(ch,  (int) MTs[i], means, means_err, widths, widths_err, effs, effs_err, means2, means2_err, widths2, widths2_err, fnorm, fnorm_err,"",year,2,2.5,true);

    zeros.push_back(0);
  }
  TGraphErrors* gmean    = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  TGraphErrors* gsigma   = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());    
  TGraphErrors* geff     = new TGraphErrors(MTs.size(), MTs.data(), effs.data(), zeros.data(), effs_err.data()); 

  ///second gauss
  TGraphErrors* gmean2    = new TGraphErrors(MTs.size(), MTs.data(), means2.data(), zeros.data(), means2_err.data());  
  TGraphErrors* gsigma2   = new TGraphErrors(MTs.size(), MTs.data(), widths2.data(), zeros.data(), widths2_err.data());    
  TGraphErrors* gfnorm     = new TGraphErrors(MTs.size(), MTs.data(), fnorm.data(), zeros.data(), fnorm_err.data()); 

  // efficiency with total (stat+syst) uncertainty, is calculated below
  TGraphErrors* geff_totunc = new TGraphErrors(MTs.size(), MTs.data(), effs.data(), zeros.data(), effs_err.data()); 

  // some information text
  TString info, info2;
  if (ch==eMuon){
    info2 = "#mu+jets";
  } else if (ch==eEle) {
    info2 = "e+jets";
  } else if (ch==eComb) {
    info2 = "l+jets";
  }
  TLatex* text = new TLatex();
  text->SetTextFont(42);
  text->SetNDC();
  text->SetTextColor(kBlack);
  text->SetTextSize(0.040);  

  //-------------------------------------------------
  // Mean values with fit
  //-------------------------------------------------
  TCanvas *can = new TCanvas("mean_can","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);
  can->SetLeftMargin(0.15);
  can->SetRightMargin(0.05);
  can->SetTopMargin(0.10);
  can->SetBottomMargin(0.12);

  // cosmetics
  TH1* painter = new TH1F("painter", "", 1, 550, 1250);
  painter->SetXTitle("Generated M_{T} [GeV]");
  painter->SetYTitle("#mu [GeV]");
  painter->SetTitleSize(0.045);
  painter->GetYaxis()->SetTitleSize(0.045);
  painter->GetYaxis()->SetTitleOffset(1.4);
  painter->GetXaxis()->SetTitleOffset(1.2);
  painter->SetTitle("");
  painter->GetYaxis()->SetRangeUser(550, 1250);
  painter->Draw();
  gmean->SetMarkerStyle(20);
  gmean->SetLineWidth(2);

  gStyle->SetStatX(0.63);
  gStyle->SetStatW(0.24);
  gStyle->SetStatY(0.85);  
  TF1* lin = new TF1("meanfit", "[0]+[1]*(x-600)", 550, 1250);
  lin->SetParName(0, "#mu at 600 GeV");
  lin->SetParName(1, "Slope");
  TFitResultPtr r = gmean->Fit(lin, "0");

  infotofile<< "meanfit param0 \t"<<lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
  infotofile<< "meanfit param1 \t"<<lin->GetParameter(1)<<"\t"<<lin->GetParError(1)<<std::endl;

  TGraphErrors* fit_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  TFitter* fitter = (TFitter*) TVirtualFitter::GetFitter();
  fitter->GetConfidenceIntervals(fit_err_95, 0.95);
  fit_err_95->SetFillColor(kOrange-4);
  fit_err_95->Draw("L3 same");

  TGraphErrors* fit_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  fitter->GetConfidenceIntervals(fit_err_68, 0.68);
  fit_err_68->SetFillColor(kOrange+1);
  fit_err_68->Draw("L3 same");

  lin->SetLineColor(kRed+1);
  lin->DrawCopy("same");
  gmean->Draw("P same");

  lin->SetParameter(0, lin->GetParameter(0)+2 * lin->GetParError(0));
  lin->SetParameter(1, lin->GetParameter(1)+2*0.5* lin->GetParError(1));
  lin->SetLineColor(kBlue);
  //lin->DrawCopy("same");

  lin->SetParameter(0, lin->GetParameter(0)-2*lin->GetParError(0));
  lin->SetParameter(1, lin->GetParameter(1)- lin->GetParError(1));
  lin->SetLineColor(kBlue);
  //  lin->DrawCopy("same");


  TString channel_name = "";
  if (ch==eMuon){
    channel_name = "much";
  } else if (ch==eEle) {
    channel_name = "ech";
  } else if (ch==eComb) {
    channel_name = "comb";
  }
  TString fname = "results/signal_mean_values_" + channel_name +"_"+year; 


  // draw some info
  info = TString::Format("Signal mean values, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  can->RedrawAxis();
  //  can->SaveAs(fname+postfix+".eps");
  can->SaveAs(fname+postfix+".pdf");

  ////////second gauss
  //-------------------------------------------------
  // Mean values with fit
  //-------------------------------------------------
  TCanvas *can_mean2 = new TCanvas("mean_can_mean2","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);
  can_mean2->SetLeftMargin(0.15);
  can_mean2->SetRightMargin(0.05);
  can_mean2->SetTopMargin(0.10);
  can_mean2->SetBottomMargin(0.12);

  // cosmetics
  painter = new TH1F("painter", "", 1, 550, 1250);
  painter->SetXTitle("Generated M_{T} [GeV]");
  painter->SetYTitle("#mu_{2} [GeV]");
  painter->SetTitleSize(0.045);
  painter->GetYaxis()->SetTitleSize(0.045);
  painter->GetYaxis()->SetTitleOffset(1.4);
  painter->GetXaxis()->SetTitleOffset(1.2);
  painter->SetTitle("");
  painter->GetYaxis()->SetRangeUser(300, 1250);
  painter->Draw();
  gmean2->SetMarkerStyle(20);
  gmean2->SetLineWidth(2);

  gStyle->SetStatX(0.63);
  gStyle->SetStatW(0.24);
  gStyle->SetStatY(0.85);  
  lin = new TF1("meanfit", "[0]+[1]*(x-600)", 550, 1250);
  lin->SetParName(0, "#mu at 600 GeV");
  lin->SetParName(1, "Slope");
  r = gmean2->Fit(lin, "0");

  infotofile<< "mean2fit param0 \t"<<lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
  infotofile<< "mean2fit param1 \t"<<lin->GetParameter(1)<<"\t"<<lin->GetParError(1)<<std::endl;

  fit_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means2.data(), zeros.data(), means2_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  fitter->GetConfidenceIntervals(fit_err_95, 0.95);
  fit_err_95->SetFillColor(kOrange-4);
  fit_err_95->Draw("L3 same");

  fit_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means2.data(), zeros.data(), means2_err.data());  
  fitter->GetConfidenceIntervals(fit_err_68, 0.68);
  fit_err_68->SetFillColor(kOrange+1);
  fit_err_68->Draw("L3 same");

  lin->SetLineColor(kRed+1);
  lin->DrawCopy("same");
  gmean2->Draw("P same");

  lin->SetParameter(0, lin->GetParameter(0)+2 * lin->GetParError(0));
  lin->SetParameter(1, lin->GetParameter(1)+2*0.5* lin->GetParError(1));
  lin->SetLineColor(kBlue);
  //lin->DrawCopy("same");

  lin->SetParameter(0, lin->GetParameter(0)-2*lin->GetParError(0));
  lin->SetParameter(1, lin->GetParameter(1)- lin->GetParError(1));
  lin->SetLineColor(kBlue);
  //  lin->DrawCopy("same");


  fname = "results/signal_mean2_values_" + channel_name +"_"+year; 


  // draw some info
  info = TString::Format("Signal mean values, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  can_mean2->RedrawAxis();
  //  can_mean2->SaveAs(fname+postfix+".eps");
  can_mean2->SaveAs(fname+postfix+".pdf");

  ////// add uncertainties
  // for each uncertainties fit linear function and draw it
  std::vector<double> means_unc;
  std::vector<double> means_err_unc;
  std::vector<double> widths_unc;  
  std::vector<double> widths_err_unc;  
  std::vector<double> effs_unc;  
  std::vector<double> effs_err_unc;  

  std::vector<double> means2_unc;
  std::vector<double> means2_err_unc;
  std::vector<double> widths2_unc;  
  std::vector<double> widths2_err_unc;  
  std::vector<double> fnorm_unc;  
  std::vector<double> fnorm_err_unc;  

  TLegend *leg = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.035);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->SetTextFont(42);

  int jj=0;
  MTs = MTs_backup;//{600, 650, 800, 900, 1000, 1100,1200};
  for(TString unc:uncertainties){

    //    if (ch==eEle && !unc.Contains("ele")) continue;
    //    if (ch==eMuon && !unc.Contains("mu")) continue;

    for(int j=0; j<2; j++){
      TString direction = "_up";
      if(j==1) direction = "_down";

      means_unc.clear();
      means_err_unc.clear();
      widths_unc.clear();  
      widths_err_unc.clear();  
      effs_unc.clear();  
      effs_err_unc.clear();  
      means2_unc.clear();
      means2_err_unc.clear();
      widths2_unc.clear();  
      widths2_err_unc.clear();  
      fnorm_unc.clear();  
      fnorm_err_unc.clear();  

      for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, means2_unc, means2_err_unc, widths2_unc, widths2_err_unc, fnorm_unc, fnorm_err_unc, unc+direction,year);
      }
    TGraphErrors* gmean_unc  = new TGraphErrors(MTs.size(), MTs.data(), means_unc.data(), zeros.data(), means_err_unc.data());  
    TF1* lin = new TF1("meanfit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
    TFitResultPtr r_unc = gmean_unc->Fit(lin, "0");
    lin->SetLineColor(kBlue+jj);
    lin->SetLineStyle(jj); 
    can->cd();

    TGraphErrors* gmean2_unc  = new TGraphErrors(MTs.size(), MTs.data(), means2_unc.data(), zeros.data(), means2_err_unc.data());  
    TF1* lin_mean2 = new TF1("meanfit2"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
    TFitResultPtr r_mean2_unc = gmean2_unc->Fit(lin_mean2, "0");

    if(unc.Contains("JER") or unc.Contains("JEC")){
      lin->DrawCopy("same");
      //jj+=4;
    }
    if(unc.Contains("JEC")&&direction.Contains("up")){
      infotofile<< "JECmfitup param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JECmfitup param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
      infotofile<< "JECmfitup param2 \t"<< lin_mean2->GetParameter(0)<<"\t"<<lin_mean2->GetParError(0)<<std::endl;
      infotofile<< "JECmfitup param3 \t"<< lin_mean2->GetParameter(1)<<"\t"<<  lin_mean2->GetParError(1)<<std::endl;

    }
    if(unc.Contains("JER")&&direction.Contains("up")){
      infotofile<< "JERmfitup param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JERmfitup param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
      infotofile<< "JERmfitup param2 \t"<< lin_mean2->GetParameter(0)<<"\t"<<lin_mean2->GetParError(0)<<std::endl;
      infotofile<< "JERmfitup param3 \t"<< lin_mean2->GetParameter(1)<<"\t"<<  lin_mean2->GetParError(1)<<std::endl;
    }
    if(unc.Contains("JEC")&&direction.Contains("down")){
      infotofile<< "JECmfitdown param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JECmfitdown param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
      infotofile<< "JECmfitdown param2 \t"<< lin_mean2->GetParameter(0)<<"\t"<<lin_mean2->GetParError(0)<<std::endl;
      infotofile<< "JECmfitdown param3 \t"<< lin_mean2->GetParameter(1)<<"\t"<<  lin_mean2->GetParError(1)<<std::endl;
    }
    if(unc.Contains("JER")&&direction.Contains("down")){
      infotofile<< "JERmfitdown param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JERmfitdown param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
      infotofile<< "JERmfitdown param2 \t"<< lin_mean2->GetParameter(0)<<"\t"<<lin_mean2->GetParError(0)<<std::endl;
      infotofile<< "JERmfitdown param3 \t"<< lin_mean2->GetParameter(1)<<"\t"<<  lin_mean2->GetParError(1)<<std::endl;
    }

    if(j==0){
      leg->AddEntry(lin,unc+" Slope "+TString::Format("%.3f", lin->GetParameter(1)),"l");
      jj+=1;
      if (jj == 5) jj++;
    }
    }

  }

  can->cd();
  leg->Draw();
  //  can->SaveAs(fname+postfix+"_unc.eps");
  can->SaveAs(fname+postfix+"_unc.pdf");


  // std::vector<double> means_fitrange;
  // std::vector<double> means_err_fitrange;
  // std::vector<double> widths_fitrange;  
  // std::vector<double> widths_err_fitrange;  
  // std::vector<double> effs_fitrange;  
  // std::vector<double> effs_err_fitrange;  

  // if (b_fitrange){
  // // ///////////
  // // ///
  // // /// test different fit ranges
  // // ///
  // // //////////

    
  //   TLegend *leg_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  //   leg_fitrange->SetBorderSize(0);
  //   leg_fitrange->SetFillStyle(0);
  //   leg_fitrange->SetTextSize(0.035);
  //   leg_fitrange->SetFillColor(0);
  //   leg_fitrange->SetLineColor(1);
  //   leg_fitrange->SetTextFont(42);

  //   MTs = MTs_backup;//{600, 650, 800, 900, 1000, 1100,1200};

  //   for(int j=0; j<sigma_ranges.size(); j++){
  //     means_fitrange.clear();
  //     means_err_fitrange.clear();
  //     widths_fitrange.clear();  
  //     widths_err_fitrange.clear();  
  //     effs_fitrange.clear();  
  //     effs_err_fitrange.clear();  
      
  //     for (int i=0; i<MTs.size(); ++i){
  // 	      fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "",year,sigma_ranges[j],sigma_ranges[j]);
  //     }
  //     TGraphErrors* gmean_fitrange  = new TGraphErrors(MTs.size(), MTs.data(), means_fitrange.data(), zeros.data(), means_err_fitrange.data());  
  //     TF1* lin = new TF1("meanfit"+TString::Format("%d",j), "[0]+[1]*(x-600)", 550, 1250);
  //     TFitResultPtr r_fitrange = gmean_fitrange->Fit(lin, "0");
  //     lin->SetLineColor(kBlue+j+1);
  //     lin->SetLineStyle(j+1); 
  //     can->cd();
  //     lin->DrawCopy("same");
  //     leg_fitrange->AddEntry(lin,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%.3f", lin->GetParameter(1)),"l");
      
  //   }
    
  //   can->cd();
  //   leg_fitrange->Draw();
  //   can->SaveAs(fname+postfix+"_fitrange.eps");
  //   can->SaveAs(fname+postfix+"_fitrange.pdf");
    
  // }

  //-------------------------------------------------
  // Widths with fit
  //-------------------------------------------------
  TCanvas *can2 = new TCanvas("widths_can","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);
  can2->SetLeftMargin(0.15);
  can2->SetRightMargin(0.05);
  can2->SetTopMargin(0.10);
  can2->SetBottomMargin(0.12);

  // cosmetics
  TH1* painter2 = new TH1F("painter2", "", 1, 550, 1250);
  painter2->SetXTitle("Generated M_{T} [GeV]");
  painter2->SetYTitle("#sigma [GeV]");
  painter2->SetTitleSize(0.045);
  painter2->GetYaxis()->SetTitleSize(0.045);
  painter2->GetYaxis()->SetTitleOffset(1.4);
  painter2->GetXaxis()->SetTitleOffset(1.2);
  painter2->SetTitle("");
  painter2->GetYaxis()->SetRangeUser(0, 130);
  if(year.Contains("allyear"))   painter2->GetYaxis()->SetRangeUser(40, 130);
  painter2->Draw();
  gsigma->SetMarkerStyle(20);
  gsigma->SetLineWidth(2);

  gStyle->SetStatX(0.70);
  gStyle->SetStatW(0.27);
  gStyle->SetStatY(0.85);  
  TF1* lin2 = new TF1("sigmafit", "[0]+[1]*(x-600)", 550, 1250);
  lin2->SetParName(0, "#sigma at 600 GeV");
  lin2->SetParName(1, "Slope");
  TFitResultPtr r2 = gsigma->Fit(lin2, "0");

  TGraphErrors* fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
  fit2_err_95->SetFillColor(kOrange-4);
  fit2_err_95->Draw("L3 same");

  TGraphErrors* fit2_err_68 = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());  
  fitter->GetConfidenceIntervals(fit2_err_68, 0.68);
  fit2_err_68->SetFillColor(kOrange+1);
  fit2_err_68->Draw("L3 same");

  lin2->SetLineColor(kRed+1);
  lin2->DrawCopy("same");
  gsigma->Draw("P same");

  infotofile<< "widthfit param0 \t"<<lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
  infotofile<< "widthfit param1 \t"<<lin2->GetParameter(1)<<"\t"<<lin2->GetParError(1)<<std::endl;


  lin2->SetParameter(0,lin2->GetParameter(0)+lin2->GetParError(0));
  lin2->SetParameter(1,lin2->GetParameter(1)+0.5*lin2->GetParError(1));
  lin2->SetLineColor(kBlue);
  //  lin2->DrawCopy("same");

  lin2->SetParameter(0,lin2->GetParameter(0)-2*lin2->GetParError(0));
  lin2->SetParameter(1,lin2->GetParameter(1)-lin2->GetParError(1));
  lin2->SetLineColor(kBlue);
  //  lin2->DrawCopy("same");


  TString fname2 = "results/signal_sigma_values_" + channel_name+"_"+year; 

  // draw some info
  info = TString::Format("Signal widths, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  can2->RedrawAxis();
  //  can2->SaveAs(fname2+postfix + ".eps");
  can2->SaveAs(fname2+postfix + ".pdf");

  ///////second gauss
  //-------------------------------------------------
  // Widths with fit
  //-------------------------------------------------
  TCanvas *can2_width = new TCanvas("widths_can2","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  gStyle->SetOptStat(0);
  can2_width->SetLeftMargin(0.15);
  can2_width->SetRightMargin(0.05);
  can2_width->SetTopMargin(0.10);
  can2_width->SetBottomMargin(0.12);

  // cosmetics
  painter2 = new TH1F("painter2", "", 1, 550, 1250);
  painter2->SetXTitle("Generated M_{T} [GeV]");
  painter2->SetYTitle("#sigma_{2} [GeV]");
  painter2->SetTitleSize(0.045);
  painter2->GetYaxis()->SetTitleSize(0.045);
  painter2->GetYaxis()->SetTitleOffset(1.4);
  painter2->GetXaxis()->SetTitleOffset(1.2);
  painter2->SetTitle("");
  painter2->GetYaxis()->SetRangeUser(0, 300);
  if(year.Contains("allyear"))   painter2->GetYaxis()->SetRangeUser(40, 300);
  painter2->Draw();
  gsigma2->SetMarkerStyle(20);
  gsigma2->SetLineWidth(2);

  gStyle->SetStatX(0.70);
  gStyle->SetStatW(0.27);
  gStyle->SetStatY(0.85);  
  lin2 = new TF1("sigmafit", "[0]+[1]*(x-600)", 550, 1250);
  lin2->SetParName(0, "#sigma at 600 GeV");
  lin2->SetParName(1, "Slope");
  r2 = gsigma2->Fit(lin2, "0");

  fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), widths2.data(), zeros.data(), widths2_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
  fit2_err_95->SetFillColor(kOrange-4);
  fit2_err_95->Draw("L3 same");

  fit2_err_68 = new TGraphErrors(MTs.size(), MTs.data(), widths2.data(), zeros.data(), widths2_err.data());  
  fitter->GetConfidenceIntervals(fit2_err_68, 0.68);
  fit2_err_68->SetFillColor(kOrange+1);
  fit2_err_68->Draw("L3 same");

  lin2->SetLineColor(kRed+1);
  lin2->DrawCopy("same");
  gsigma2->Draw("P same");

  infotofile<< "width2fit param0 \t"<<lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
  infotofile<< "width2fit param1 \t"<<lin2->GetParameter(1)<<"\t"<<lin2->GetParError(1)<<std::endl;


  lin2->SetParameter(0,lin2->GetParameter(0)+lin2->GetParError(0));
  lin2->SetParameter(1,lin2->GetParameter(1)+0.5*lin2->GetParError(1));
  lin2->SetLineColor(kBlue);
  //  lin2->DrawCopy("same");

  lin2->SetParameter(0,lin2->GetParameter(0)-2*lin2->GetParError(0));
  lin2->SetParameter(1,lin2->GetParameter(1)-lin2->GetParError(1));
  lin2->SetLineColor(kBlue);
  //  lin2->DrawCopy("same");


  fname2 = "results/signal_sigma2_values_" + channel_name+"_"+year; 

  // draw some info
  info = TString::Format("Signal widths, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  can2_width->RedrawAxis();
  //  can2_width->SaveAs(fname2+postfix + ".eps");
  can2_width->SaveAs(fname2+postfix + ".pdf");



  ////// add uncertainties
  // for each uncertainties fit linear function and draw it
  TLegend *leg2 = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->SetTextSize(0.035);
  leg2->SetFillColor(0);
  leg2->SetLineColor(1);
  leg2->SetTextFont(42);

  jj=0;
  MTs = MTs_backup;//{600, 650, 800, 900, 1000,1100, 1200};
  for(TString unc:uncertainties){

    //    if (ch==eEle && !unc.Contains("ele")) continue;
    //    if (ch==eMuon && unc.Contains("mu")) continue;

    for(int j=0; j<2; j++){
      TString direction = "_up";
      if(j==1) direction = "_down";
      means_unc.clear();
      means_err_unc.clear();
      widths_unc.clear();  
      widths_err_unc.clear();  
      effs_unc.clear();  
      effs_err_unc.clear();  

      means2_unc.clear();
      means2_err_unc.clear();
      widths2_unc.clear();  
      widths2_err_unc.clear();  
      fnorm_unc.clear();  
      fnorm_err_unc.clear();  

      for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, means2_unc, means2_err_unc, widths2_unc, widths2_err_unc, fnorm_unc, fnorm_err_unc, unc+direction, year);
      }
      TGraphErrors* gsigma_unc = new TGraphErrors(MTs.size(), MTs.data(), widths_unc.data(), zeros.data(), widths_err_unc.data());    
      TF1* lin2 = new TF1("sigmafit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
      TFitResultPtr r_unc = gsigma_unc->Fit(lin2, "0");
      lin2->SetLineColor(kBlue+jj);
      lin2->SetLineStyle(jj);
      can2->cd();
      TGraphErrors* gsigma2_unc = new TGraphErrors(MTs.size(), MTs.data(), widths2_unc.data(), zeros.data(), widths2_err_unc.data());    
      TF1* lin2_sigma = new TF1("sigmafit2"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
      TFitResultPtr r_unc_sigma = gsigma2_unc->Fit(lin2_sigma, "0");

      TGraphErrors* gfnorm_unc = new TGraphErrors(MTs.size(), MTs.data(), fnorm_unc.data(), zeros.data(), fnorm_err_unc.data());    
      TF1* lin2_fnorm = new TF1("fnorm"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
      TFitResultPtr r_unc_fnorm = gfnorm_unc->Fit(lin2_fnorm, "0");


      gsigma_unc->SetMarkerStyle(24);
      if(unc.Contains("PDF"))gsigma_unc->Draw("PE Same");
      if(unc.Contains("JEC")&&direction.Contains("up")){
	      infotofile<< "JECwfitup param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitup param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitup param2 \t"<< lin2_sigma->GetParameter(0)<<"\t"<<lin2_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitup param3 \t"<< lin2_sigma->GetParameter(1)<<"\t"<<  lin2_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JECffitup param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<  lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JECffitup param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JER")&&direction.Contains("up")){
	      infotofile<< "JERwfitup param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitup param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitup param2 \t"<< lin2_sigma->GetParameter(0)<<"\t"<<lin2_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitup param3 \t"<< lin2_sigma->GetParameter(1)<<"\t"<<  lin2_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JERffitup param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<  lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JERffitup param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JEC")&&direction.Contains("down")){
	      infotofile<< "JECwfitdown param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitdown param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitdown param2 \t"<< lin2_sigma->GetParameter(0)<<"\t"<<lin2_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitdown param3 \t"<< lin2_sigma->GetParameter(1)<<"\t"<<  lin2_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JECffitdown param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JECffitdown param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JER")&&direction.Contains("down")){
	      infotofile<< "JERwfitdown param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitdown param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitdown param2 \t"<< lin2_sigma->GetParameter(0)<<"\t"<<lin2_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitdown param3 \t"<< lin2_sigma->GetParameter(1)<<"\t"<<  lin2_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JERffitdown param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JERffitdown param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
      }

      if(j==0)lin2->Draw("same");
      if(j==0)leg2->AddEntry(lin2,unc+" Slope "+TString::Format("%.3f", lin2->GetParameter(1)),"l");
      if(j==0)jj++;
      if(jj==5)jj++;
    }
  }

  can2->cd();
  leg2->Draw();
  //  can2->SaveAs(fname2+postfix+"_unc.eps");
  can2->SaveAs(fname2+postfix+"_unc.pdf");


  // if(b_fitrange){
  // // ///////
  // // ///
  // // /// testing the fit range
  // // ///
  // // //////

  //   TLegend *leg2_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  //   leg2_fitrange->SetBorderSize(0);
  //   leg2_fitrange->SetFillStyle(0);
  //   leg2_fitrange->SetTextSize(0.035);
  //   leg2_fitrange->SetFillColor(0);
  //   leg2_fitrange->SetLineColor(1);
  //   leg2_fitrange->SetTextFont(42);

  //   MTs = MTs_backup;//{600, 650, 800, 900, 1000,1100, 1200};

  //   for(int j=0; j<sigma_ranges.size(); j++){
  //     means_fitrange.clear();
  //     means_err_fitrange.clear();
  //     widths_fitrange.clear();  
  //     widths_err_fitrange.clear();  
  //     effs_fitrange.clear();  
  //     effs_err_fitrange.clear();  

  //     for (int i=0; i<MTs.size(); ++i){
  // 	fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "", year,sigma_ranges[j],sigma_ranges[j]);
  //     }
  //     TGraphErrors* gsigma_fitrange = new TGraphErrors(MTs.size(), MTs.data(), widths_fitrange.data(), zeros.data(), widths_err_fitrange.data());    
  //     TF1* lin2 = new TF1("sigmafit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
  //     TFitResultPtr r_fitrange = gsigma_fitrange->Fit(lin2, "0");
  //     lin2->SetLineColor(kBlue+j+1);
  //     lin2->SetLineStyle(j+1);
  //     can2->cd();
  //     gsigma_fitrange->SetMarkerStyle(24);
      
  //     lin2->Draw("same");
  //     leg2_fitrange->AddEntry(lin2,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%.3f", lin2->GetParameter(1)),"l");
  //   }
    
  //   can2->cd();
  //   leg2_fitrange->Draw();
  //   can2->SaveAs(fname2+postfix+"_fitrange.eps");
  //   can2->SaveAs(fname2+postfix+"_fitrange.pdf");

  // }

  //-------------------------------------------------
  // Efficiencies with fit
  //-------------------------------------------------
  double   result = 0;

  TCanvas *can3 = new TCanvas("eff_can","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  can3->SetLeftMargin(0.15);
  can3->SetRightMargin(0.05);
  can3->SetTopMargin(0.10);
  can3->SetBottomMargin(0.12);

  // cosmetics
  TH1* painter3 = new TH1F("painter", "", 1, 550, 1250);
  painter3->SetXTitle("Generated M_{T} [GeV]");
  painter3->SetYTitle("#varepsilon");
  painter3->SetTitleSize(0.045);
  painter3->GetYaxis()->SetTitleSize(0.045);
  painter3->GetYaxis()->SetTitleOffset(1.4);
  painter3->GetXaxis()->SetTitleOffset(1.2);
  painter3->SetTitle("");
  painter3->GetYaxis()->SetRangeUser(0, 0.02);
  if(year.Contains("allyears"))  painter3->GetYaxis()->SetRangeUser(0.006, 0.016);
  painter3->Draw();
  geff->SetMarkerStyle(20);
  geff->SetLineWidth(2);

  gStyle->SetStatX(0.63);
  gStyle->SetStatW(0.24);
  gStyle->SetStatY(0.85);  
  TF1* lin3 = new TF1("efffit", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  lin3->SetParameter(0, effs[0]);
  lin3->SetParameter(1, 0);  
  lin3->SetParName(0, "#varepsilon at 600 GeV");
  lin3->SetParName(1, "Slope");
  TFitResultPtr r3 = geff->Fit(lin3, "0");

  infotofile << "eff param0  "<<lin3->GetParameter(0)<<"  "<<0<<endl;
  infotofile << "eff param1  "<<lin3->GetParameter(1)<<"  "<<0<<endl;
  infotofile << "eff param2  "<<lin3->GetParameter(2)<<"  "<<0<<endl;

  //TGraphErrors* fit3_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  //fitter->GetConfidenceIntervals(fit3_err_95, 0.95);
  //fit3_err_95->SetFillColor(kOrange-4);
  //fit3_err_95->Draw("L3 same");

  TGraphErrors* fit3_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  fitter->GetConfidenceIntervals(fit3_err_68, 0.68);
  fit3_err_68->SetFillColor(kOrange+1);
  fit3_err_68->Draw("L3 same");

  lin3->SetLineColor(kRed+1);
  lin3->DrawCopy("same");
  geff->Draw("P same");

  double nom_eff = lin3->GetParameter(0);

  lin3->SetParameter(0,lin3->GetParameter(0)+lin3->GetParError(0));
  lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");

  lin3->SetParameter(0,lin3->GetParameter(0)-2*lin3->GetParError(0));
    lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");

  // draw some info
  info = TString::Format("Signal efficiencies, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  TString fname3 = "results/signal_eff_values_" + channel_name+"_"+year; 

  can3->RedrawAxis();
  //  can3->SaveAs(fname3+postfix+ ".eps");
  can3->SaveAs(fname3+postfix+ ".pdf");

  ///// second gauss
  TCanvas *can3_fnorm = new TCanvas("eff_can","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  can3_fnorm->SetLeftMargin(0.15);
  can3_fnorm->SetRightMargin(0.05);
  can3_fnorm->SetTopMargin(0.10);
  can3_fnorm->SetBottomMargin(0.12);

  // cosmetics
  painter3 = new TH1F("painter", "", 1, 550, 1250);
  painter3->SetXTitle("Generated M_{T} [GeV]");
  painter3->SetYTitle("f_{norm}");
  painter3->SetTitleSize(0.045);
  painter3->GetYaxis()->SetTitleSize(0.045);
  painter3->GetYaxis()->SetTitleOffset(1.4);
  painter3->GetXaxis()->SetTitleOffset(1.2);
  painter3->SetTitle("");
  painter3->GetYaxis()->SetRangeUser(0, 0.3);
  if(year.Contains("allyears"))  painter3->GetYaxis()->SetRangeUser(0, 0.3);
  painter3->Draw();
  gfnorm->SetMarkerStyle(20);
  gfnorm->SetLineWidth(2);

  gStyle->SetStatX(0.63);
  gStyle->SetStatW(0.24);
  gStyle->SetStatY(0.85);  
  lin3 = new TF1("efffit", "[0]+[1]*(x-600)", 550, 1250);
  lin3->SetParameter(0, 0.17);
  lin3->SetParameter(1, 0);  
  lin3->SetParName(0, "f_{norm} at 600 GeV");
  lin3->SetParName(1, "Slope");
  r3 = gfnorm->Fit(lin3, "0");

  infotofile<< "fnormfit param0 \t"<<lin3->GetParameter(0)<<"\t"<<lin3->GetParError(0)<<std::endl;
  infotofile<< "fnormfit param1 \t"<<lin3->GetParameter(1)<<"\t"<<lin3->GetParError(1)<<std::endl;

  //TGraphErrors* fit3_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  //fitter->GetConfidenceIntervals(fit3_err_95, 0.95);
  //fit3_err_95->SetFillColor(kOrange-4);
  //fit3_err_95->Draw("L3 same");

  fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), widths2.data(), zeros.data(), widths2_err.data());  
  fitter = (TFitter*) TVirtualFitter::GetFitter();
  fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
  fit2_err_95->SetFillColor(kOrange-4);
  fit2_err_95->Draw("L3 same");



  fit3_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means2.data(), zeros.data(), means2_err.data());  
  fitter->GetConfidenceIntervals(fit3_err_68, 0.68);
  fit3_err_68->SetFillColor(kOrange+1);
  fit3_err_68->Draw("L3 same");



  lin3->SetLineColor(kRed+1);
  lin3->DrawCopy("same");
  gfnorm->Draw("P same");

  nom_eff = lin3->GetParameter(0);

  lin3->SetParameter(0,lin3->GetParameter(0)+lin3->GetParError(0));
  lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");

  lin3->SetParameter(0,lin3->GetParameter(0)-2*lin3->GetParError(0));
    lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");

  // draw some info
  info = TString::Format("Signal efficiencies, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  fname3 = "results/signal_fnorm_values_" + channel_name+"_"+year; 

  can3_fnorm->RedrawAxis();
  //  can3_fnorm->SaveAs(fname3+postfix+ ".eps");
  can3_fnorm->SaveAs(fname3+postfix+ ".pdf");



  ////// add uncertainties
  // for each uncertainties fit linear function and draw it
  TLegend *leg3 = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  leg3->SetBorderSize(0);
  leg3->SetFillStyle(0);
  leg3->SetTextSize(0.035);
  leg3->SetFillColor(0);
  leg3->SetLineColor(1);
  leg3->SetTextFont(42);

  jj=0;
  MTs = MTs_backup;// {600, 650, 800, 900, 1000, 1100,1200};

  std::vector<double> effs_uperr2(MTs.size());  
  std::vector<double> effs_dnerr2(MTs.size());  
  for (int i=0; i<MTs.size(); ++i)
  {
    effs_uperr2[i] = 0;
    effs_dnerr2[i] = 0;
  }

  for(TString unc:uncertainties){
    std::cout << "   ===============================    uncertainty        " << unc << std::endl;

    if (ch==eEle && !unc.Contains("ele")) continue;
    if (ch==eMuon && !unc.Contains("mu")) continue;
    if (ch==eComb && (unc.Contains("ele") || unc.Contains("mu"))) continue;

    means_unc.clear();
    means_err_unc.clear();
    widths_unc.clear();  
    widths_err_unc.clear();  
    effs_unc.clear();  
    effs_err_unc.clear();  
    means2_unc.clear();
    means2_err_unc.clear();
    widths2_unc.clear();  
    widths2_err_unc.clear();  
    fnorm_unc.clear();  
    fnorm_err_unc.clear();  


    for (int i=0; i<MTs.size(); ++i){
      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, means2_unc, means2_err_unc, widths2_unc, widths2_err_unc, fnorm_unc, fnorm_err_unc, unc+"_up", year,2,2.5,true);
    }
    TGraphErrors* geff_unc_up = new TGraphErrors(MTs.size(), MTs.data(), effs_unc.data(), zeros.data(), effs_err_unc.data());          

    means_unc.clear();
    means_err_unc.clear();
    widths_unc.clear();  
    widths_err_unc.clear();  
    effs_unc.clear();  
    effs_err_unc.clear();      
    means2_unc.clear();
    means2_err_unc.clear();
    widths2_unc.clear();  
    widths2_err_unc.clear();  
    fnorm_unc.clear();  
    fnorm_err_unc.clear();      
    for (int i=0; i<MTs.size(); ++i){
      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc,means2_unc, means2_err_unc, widths2_unc, widths2_err_unc, fnorm_unc, fnorm_err_unc, unc+"_down", year,2,2.5,true);
    }

    TGraphErrors* geff_unc_dn = new TGraphErrors(MTs.size(), MTs.data(), effs_unc.data(), zeros.data(), effs_err_unc.data());          

    // sum up all uncertainties in quadrature to get the total uncertainty
    for (int i=0; i<MTs.size(); ++i){
      double x, y, yup, ydn; 
      geff->GetPoint(i, x, y);
      geff_unc_up->GetPoint(i, x, yup);
      geff_unc_dn->GetPoint(i, x, ydn);
      if (yup<y){
        double tmp = yup;
        yup = ydn;
        ydn = tmp;
      }
      double relup = fabs(y-yup)/y;
      double reldn = fabs(y-ydn)/y;
      effs_uperr2[i] += relup*relup;
      effs_dnerr2[i] += reldn*reldn;
    }

    //TF1* lin3 = new TF1("efffit"+TString::Format("%d",jj), "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
    //TFitResultPtr r_unc = geff_unc_up->Fit(lin3, "0");
    //lin3->SetLineColor(kBlue+jj);
    //lin3->SetLineStyle(jj);
    //can3->cd();
    //lin3->Draw("same");

    TString uncname = unc + "_" + channel_name + "_" + year;
    draw_eff_unc(geff, geff_unc_up, geff_unc_dn, uncname);


    // leg3->AddEntry(lin3,unc+" Slope "+TString::Format("%f", lin3->GetParameter(1)),"l");
    leg3->AddEntry(lin3,unc,"l");
    jj++;
    if (jj==5)jj++;
    
  }

  for (int i=0; i<MTs.size(); ++i)
  {
    cout << "MT = " << MTs[i] << " rel err up = " << sqrt(effs_uperr2[i]) << " rel err down = " << sqrt(effs_dnerr2[i]) << endl;
  }

  // if(b_fitrange){
  // // // ///////
  // // // ///
  // // // /// testing fit range
  // // // ///
  // // // /////
  //   TLegend *leg3_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  //   leg3_fitrange->SetBorderSize(0);
  //   leg3_fitrange->SetFillStyle(0);
  //   leg3_fitrange->SetTextSize(0.035);
  //   leg3_fitrange->SetFillColor(0);
  //   leg3_fitrange->SetLineColor(1);
  //   leg3_fitrange->SetTextFont(42);

  //   MTs = MTs_backup;// {600, 650, 800, 900, 1000, 1100,1200};
  //   for(int j=0; j<sigma_ranges.size(); j++){
  //     means_fitrange.clear();
  //     means_err_fitrange.clear();
  //     widths_fitrange.clear();  
  //     widths_err_fitrange.clear();  
  //     effs_fitrange.clear();  
  //     effs_err_fitrange.clear();  
      
  //     for (int i=0; i<MTs.size(); ++i){
  // 	fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "",year,sigma_ranges[j],sigma_ranges[j]);
  //     }
  //     TGraphErrors* geff_fitrange   = new TGraphErrors(MTs.size(), MTs.data(), effs_fitrange.data(), zeros.data(), effs_err_fitrange.data());      
  //     TF1* lin3 = new TF1("efffit"+TString::Format("%d",jj), "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  //     TFitResultPtr r_fitrange = geff_fitrange->Fit(lin3, "0");
  //     lin3->SetLineColor(kBlue+j+1);
  //     lin3->SetLineStyle(j+1);
  //     can3->cd();
  //     lin3->Draw("same");
  //     leg3_fitrange->AddEntry(lin3,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%f", lin3->GetParameter(1)),"l");
      
  //   }

  //   can3->cd();
  //   leg3_fitrange->Draw();
  //   can3->SaveAs(fname3+postfix+"_fitrange.eps");
  //   can3->SaveAs(fname3+postfix+"_fitrange.pdf");
  // }

  //---------------------------------------------------------
  // Draw efficiencies with fit and total uncertainties
  //---------------------------------------------------------
  if(year.Contains("allyears")){
    gStyle->SetStatX(0);
    gStyle->SetStatW(0);
    gStyle->SetStatY(0);  
    gStyle->SetOptFit(0);
    //    gStyle->SetOptStat(0);
  }else{
    gStyle->SetStatX(0.63);
    gStyle->SetStatW(0.24);
    gStyle->SetStatY(0.85);  
    gStyle->SetEndErrorSize(4.0);
  }


  TCanvas *can4 = new TCanvas("eff_can_unc","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  can4->SetLeftMargin(0.15);
  can4->SetRightMargin(0.05);
  can4->SetTopMargin(0.10);
  can4->SetBottomMargin(0.12);
  
  //legend
  TLegend *leg_eff = new TLegend(0.2,0.15,0.55,0.45, "","brNDC");
  leg_eff->SetBorderSize(0);	
  leg_eff->SetFillStyle(0);
  leg_eff->SetTextSize(0.035);
  leg_eff->SetTextFont(42);
  


  // cosmetics
  TH1* painter4 = new TH1F("painter", "", 1, 550, 1250);
  painter4->SetXTitle("Generated M_{T} [GeV]");
  painter4->SetYTitle("Signal efficiency");
  painter4->SetTitleSize(0.045);
  painter4->GetYaxis()->SetTitleSize(0.045);
  painter4->GetYaxis()->SetTitleOffset(1.6);
  painter4->GetXaxis()->SetTitleOffset(1.2);
  painter4->SetTitle("");
  painter4->GetYaxis()->SetRangeUser(0, 0.02);
  if(year.Contains("allyears"))  painter4->GetYaxis()->SetRangeUser(0., 0.016);
  painter4->Draw();
  geff->SetMarkerStyle(20);
  geff->SetLineWidth(2);

  // fill graph with total uncertainty
  for (int i=0; i<MTs.size(); ++i)
  {
    double x, y; 
    geff->GetPoint(i, x, y);
    double y_err = geff->GetErrorY(i);
    double y_err_rel = y_err / y;

    // symmetrize systematic uncertainty
    double y_err_rel_sys = (sqrt(effs_uperr2[i]) + sqrt(effs_dnerr2[i]))/2. ;

    // add stat and sys in quadrature
    double tot_err_rel = sqrt( y_err_rel*y_err_rel + y_err_rel_sys*y_err_rel_sys );

    cout << "MT = " << MTs[i] << " stat err = " << y_err_rel << " tot err = " << tot_err_rel << endl;
    if(year.Contains("2016")) tot_err_rel = sqrt(tot_err_rel * tot_err_rel + 0.02*0.02);
    //    infotofile<<"total rate unc  "<<"MT = " << MTs[i] << " tot err = " << tot_err_rel << endl;
    geff_totunc->SetPointError(i, 0, tot_err_rel*y); 
  }  

  // fill two graphs: one with up- and one with down variation of 
  // total uncertainty, to determine parametric total uncertainty 
  TGraphErrors* gtot_up = (TGraphErrors*) geff_totunc->Clone();
  TGraphErrors* gtot_dn = (TGraphErrors*) geff_totunc->Clone();
  for (int i=0; i<MTs.size(); ++i)
  {
    double x, y; 
    geff->GetPoint(i, x, y);
    double y_err = geff->GetErrorY(i);

    double yup = y + geff_totunc->GetErrorY(i);
    double ydn = y - geff_totunc->GetErrorY(i);

    gtot_up->SetPoint(i, x, yup);
    gtot_dn->SetPoint(i, x, ydn);

    // set uncertainties to stat only
    gtot_up->SetPointError(i, 0, y_err);
    gtot_dn->SetPointError(i, 0, y_err);
  }  

  // fit the up and down variations to get functions 
  // parametrizing the total uncertainties
  TF1* eff_tot_up = new TF1("eff_tot_up", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  TF1* eff_tot_dn = new TF1("eff_tot_dn", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  double x, y;
  gtot_up->GetPoint(0, x, y);
  eff_tot_up->SetParameter(0, y);
  eff_tot_up->SetParameter(1, 0);   
  gtot_dn->GetPoint(0, x, y);
  eff_tot_dn->SetParameter(0, y);
  eff_tot_dn->SetParameter(1, 0); 
  gtot_up->Fit(eff_tot_up, "0");
  gtot_dn->Fit(eff_tot_dn, "0");


  // fit the efficiency with total uncertainty
  TF1* lin_tot = new TF1("efffit_tot", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  lin_tot->SetParameter(0, effs[0]);
  lin_tot->SetParameter(1, 0);  
  // lin_tot->SetParName(0, "#varepsilon at 600 GeV");
  // lin_tot->SetParName(1, "Slope");
  TFitResultPtr rtot = geff_totunc->Fit(lin_tot, "0");

  //infotofile<<"efficiency estimate  "<<lin3->GetParameter(0)<<std::endl;
  //TGraphErrors* fit_toterr_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  //fitter = (TFitter*) TVirtualFitter::GetFitter();
  //fitter->GetConfidenceIntervals(fit_toterr_68, 0.68);
  //fit_toterr_68->SetFillColor(kOrange-4);
  //fit_toterr_68->Draw("L3 same");


  // fit the efficiency with statistical uncertainty
  TF1* lin_stat = new TF1("efffit_stat", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  lin_stat->SetParameter(0, effs[0]);
  lin_stat->SetParameter(1, 0);  
  // lin_stat->SetParName(0, "#varepsilon at 600 GeV");
  // lin_stat->SetParName(1, "Slope");
  TFitResultPtr r6 = geff->Fit(lin_stat, "0");

  //  if (year.Contains("allyears")) {TPaveStats *ps = (TPaveStats *)geff->GetListOfFunctions()->FindObject("stats"); ps->SetX1NDC(0); ps->SetX2NDC(0);}

  // use uncertainty in p0 to estimate 1sigma uncertainty
  int Npoints = 100;
  double MTmin = 550;
  double MTmax = 1250; 
  double MTrange = MTmax - MTmin; 
  double step = MTrange/Npoints;  
  TGraphErrors* eff_err_stat = new TGraphErrors(Npoints+1);
  TF1* lin_stat_unc = (TF1*) lin_stat->Clone();
  lin_stat_unc->SetParameter(0, lin_stat->GetParameter(0)+lin_stat->GetParError(0));  
  for (int i=0; i<Npoints+1; ++i){
    double x = MTmin + step*i;
    double y = lin_stat->Eval(x); 
    double yerr = fabs(y-lin_stat_unc->Eval(x)); 
    eff_err_stat->SetPoint(i, x, y);
    eff_err_stat->SetPointError(i, 0, yerr); 
  }


  // use fits of up and down variations to get the total uncertainty
  TGraphAsymmErrors* eff_err_tot = new TGraphAsymmErrors(Npoints+1);
  for (int i=0; i<Npoints+1; ++i){
    double x = MTmin + step*i;
    double y = lin_stat->Eval(x);
    double yup = eff_tot_up->Eval(x); 
    double ydn = eff_tot_dn->Eval(x); 

    double yerr_up = fabs(yup - y); 
    double yerr_dn = fabs(y - ydn);

    eff_err_tot->SetPoint(i, x, y);
    eff_err_tot->SetPointEYhigh(i, yerr_up); 
    eff_err_tot->SetPointEYlow(i, yerr_dn); 
    //cout << "MT = " << x << " eff = " << y << " + " << yerr_up << " - " << yerr_dn << endl;
  }

  // now draw the statistical and total uncertainties
  eff_err_tot->SetFillColor(kOrange-4);
  eff_err_tot->SetLineColor(kOrange-4);
  eff_err_tot->Draw("L3 same");
  

  //eff_err_tot->Draw("P same");
  eff_err_stat->SetFillColor(kOrange+1);
  eff_err_stat->SetLineColor(kOrange+1);
  eff_err_stat->Draw("L3 same");
  

  //TGraphErrors* fit6_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  //fitter = (TFitter*) TVirtualFitter::GetFitter();
  //fitter->GetConfidenceIntervals(fit6_err_68, 0.68);
  //fit6_err_68->SetFillColor(kOrange+1);
  //fit6_err_68->Draw("L3 same");

  lin_stat->SetLineColor(kRed+1);
  lin_stat->DrawCopy("same");
  geff->Draw("P same");
  
  geff_totunc->SetMarkerStyle(20);
  geff_totunc->SetLineWidth(2);
  geff_totunc->Draw("PZ same");
  

  // draw up and down variations to check that it works
  // gtot_up->SetMarkerStyle(22);
  // gtot_up->SetLineWidth(2);
  // gtot_up->SetMarkerColor(kRed+2);
  // gtot_up->SetLineColor(kRed+2);
  // gtot_up->Draw("PZ same");
  // eff_tot_up->SetLineColor(kRed+2);
  // eff_tot_up->Draw("same");

  // gtot_dn->SetMarkerStyle(22);
  // gtot_dn->SetLineWidth(2);
  // gtot_dn->SetMarkerColor(kBlue+2);
  // gtot_dn->SetLineColor(kBlue+2);
  // gtot_dn->Draw("PZ same");
  // eff_tot_dn->SetLineColor(kBlue+2);
  // eff_tot_dn->Draw("same");

  // @ANNA: now we can use the two functions (with three free parameters): 
  // eff_tot_up
  // eff_tot_dn
  // to have the up-variation on the signal efficiency 
  // and the down-variation of hte signal efficiency
  // the central value is obtained from 
  // lin_stat
  // if we want to have a symmetric uncertainty, 
  // we should just symmetrize: 

  double MT_unc = 550; 
  while (MT_unc<1250){
    double y = lin_stat->Eval(MT_unc);
    double yup = eff_tot_up->Eval(MT_unc);
    double ydn = eff_tot_dn->Eval(MT_unc);
    double err_up = fabs( y - yup );
    double err_dn = fabs( y - ydn );
    double err = ( err_up + err_dn ) / 2.;
    infotofile<<"total rate unc  "<<"MT = " << MT_unc << " tot err = " << err/y << endl;
    MT_unc+=25;
  }

  // draw some info
  info = TString::Format("Signal efficiencies, ");
  info.Append(info2);
  //  text->DrawLatex(0.16, 0.92, info.Data());

  TString fname4 = "results/signal_eff_values_" + channel_name+"_"+year; 



  leg_eff->SetHeader(info2);
  leg_eff->AddEntry(geff_totunc,"Simulated efficiency","PE");
  TString fitfunc = Form("f(x) = %.2e + %.2e x+ %.2e x^{2}", lin_stat->GetParameter(0),lin_stat->GetParameter(1),lin_stat->GetParameter(2));
  leg_eff->AddEntry(lin_stat,"Parametrisation","l");
leg_eff->AddEntry(eff_err_stat,"Stat. uncertainty","f");
leg_eff->AddEntry(eff_err_tot,"Tot. uncertainty","f");

  leg_eff->Draw();
  can4->RedrawAxis();

  ////// CMS tags
  TString infotext = "13 TeV";
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(42);
  text1->SetTextSize(0.045);
  text1->SetY(.95);
  text1->Draw();
  
  TString cmstext = "CMS";
  TLatex *text2 = new TLatex(3.5, 24, cmstext);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.2);
  text2->SetTextFont(62);
  text2->SetTextSize(0.072);
  text2->SetY(0.87);
  text2->Draw();
 
  TString simtext = "Simulation";
  TLatex *text4 = new TLatex(3.5, 24, simtext);
  text4->SetNDC();
  text4->SetTextAlign(13);
  text4->SetX(0.2);
  text4->SetTextFont(52);
  text4->SetTextSize(0.05);
  text4->SetY(0.8);
  text4->Draw();


  TString preltext = "Preliminary";
  TLatex *text3 = new TLatex(3.5, 24, preltext);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(0.2);
  text3->SetTextFont(52);
  text3->SetTextSize(0.05);
  text3->SetY(0.75);
  text3->Draw();




  //  can4->SaveAs(fname4+postfix+"_unc.eps");
  can4->SaveAs(fname4+postfix+"_unc.pdf");


}

void draw_eff_unc(TGraphErrors* geff, TGraphErrors* geff_up, TGraphErrors* geff_dn, TString name)
{

  double xmin = 550;
  double xmax = 1250;

  cout << "\n\nShowing uncertainties for variation in " << name << endl;
  TGraphErrors* gunc_up = new TGraphErrors(geff->GetN());
  TGraphErrors* gunc_dn = new TGraphErrors(geff->GetN());  
  for (int i=0; i<geff->GetN(); ++i){
    double x, y, yup, ydn; 
    geff->GetPoint(i, x, y);
    geff_up->GetPoint(i, x, yup);
    geff_dn->GetPoint(i, x, ydn);
    double relup = fabs(y-yup)/y;
    double reldn = fabs(y-ydn)/y;
    gunc_up->SetPoint(i, x, relup*100);
    gunc_dn->SetPoint(i, x, (-1)*reldn*100);
    cout << "MT = " << x << " eff = " << y << " + " << relup*y << " - " << reldn*y << endl;
  }
  cout << "----------------------------------------------------------------\n" << endl;
  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("unc_can", "", 800, 600);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.13); 
  gPad->SetLeftMargin(0.15); 


  TH1F* plotter = new TH1F("plotter", "", 1, xmin, xmax);
  plotter->GetXaxis()->SetRangeUser(xmin,xmax);
  plotter->GetYaxis()->SetRangeUser(-20,20);  
  if(name.Contains("btag_bc"))  plotter->GetYaxis()->SetRangeUser(-30,30);  
  if(name.Contains("scale"))  plotter->GetYaxis()->SetRangeUser(-20,20);  
  plotter->GetXaxis()->SetTitleSize(0.05);
  plotter->GetYaxis()->SetTitleSize(0.05);
  plotter->GetXaxis()->SetLabelSize(0.05);
  plotter->GetYaxis()->SetLabelSize(0.05);
  plotter->GetXaxis()->SetTitleOffset(1.1);
  plotter->GetYaxis()->SetTitleOffset(1.3); 
  plotter->GetXaxis()->SetTitle("M_{T} [GeV]");
  plotter->GetYaxis()->SetTitle("Relative Uncertainty [%]");  
  plotter->SetLineColor(kBlack);
  plotter->SetTitle("");

  plotter->Draw();
  
  // some lines to guide the eye
  TLine* l = new TLine();
  l->SetLineColor(kBlack);
  l->SetLineStyle(kDotted);
  l->SetLineWidth(1);
  l->DrawLine(xmin, 5, xmax, 5);
  l->DrawLine(xmin, -5, xmax, -5);
  l->SetLineStyle(kDashed);
  l->DrawLine(xmin, 10, xmax, 10);
  l->DrawLine(xmin, -10, xmax, -10);

  gunc_up->SetMarkerStyle(20);
  gunc_up->SetMarkerColor(kRed+2);
  gunc_up->Draw("P same");

  gunc_dn->SetMarkerStyle(20);
  gunc_dn->SetMarkerColor(kBlue+2);
  gunc_dn->Draw("P same");

  TString fname = "results/signal_eff_unc_" + name + ".pdf";
  c->SaveAs(fname);

}

void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,std::vector<double>& means2, std::vector<double>& means2_err, std::vector<double>& widths2, std::vector<double>& widths2_err, std::vector<double>& fnorm, std::vector<double>& fnorm_err,TString unc, TString year, float factormin, float factormax, bool write_Nevet)
{
  
  // set fit regions (crude estimate)
  double fit_xmin = MT-20-150.;
  double fit_xmax = MT-20+150.;  

 
  // a bit more precise 
  double mean = MT-20;
  double sigma = 61.5+0.06*(MT-600.);
  //  fit_xmin = mean - factormin*sigma;
  fit_xmax = mean + factormax*sigma;
  fit_xmin = 300;
  //  fit_xmax = 1600;
  

  // if(b_test){
  //   fit_xmin = MT-300.; //600
  //   fit_xmax = MT+300.; //600
  //   // fit_xmin = MT-400.; //1000
  //   // fit_xmax = MT+100.;  //1000
  
  // }


  gROOT->SetBatch(kTRUE);

  //setOptFit( pcev (default = 0111)) Probability; Chisquare/Number of degrees of freedom; errors ;values of parameters 
  
  gStyle->SetStatX(0.98);
  gStyle->SetStatW(0.20);
  gStyle->SetStatY(0.95);

  //  gStyle->SetOptFit(1111);
  gStyle->SetOptStat(0);

 
  // -----------------------------------------
  // Canvas for Background Fit without Ratios
  // -----------------------------------------
  TCanvas *can = new TCanvas("sig_c","signal fit",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();


  can->Clear();
  can->cd();
  can->SetLeftMargin(0.12);
  can->SetRightMargin(0.05);
  can->SetTopMargin(0.10);
  can->SetBottomMargin(0.12);



  // get data or MC
  TH1F* sigh = GetAnalysisOutputSignal(MT, channel,unc, year); 
  sigh->SetTitle("");

  // important: get xmin and xmax from bin edges!
  // needed for normalization, otherwise the fit quality is bad
  int Nbins = 0;
  double xmin = 1e6;
  double xmax = 0;  
  for (int i=0; i<sigh->GetNbinsX()+1; ++i){
    if (sigh->GetXaxis()->GetBinLowEdge(i)>=fit_xmin){
      if (xmin>1e5) xmin = sigh->GetXaxis()->GetBinLowEdge(i);
      if (sigh->GetXaxis()->GetBinUpEdge(i)<=fit_xmax){ 
        xmax = sigh->GetXaxis()->GetBinUpEdge(i);
        ++Nbins;
      }
    }
  }
  cout << "Nbins = " << Nbins << endl;
  cout << "xmin = " << xmin << endl;
  cout << "xmax = " << xmax << endl;

  // calculate the integral to set the normalisation
  double norm = sigh->Integral(sigh->GetXaxis()->FindBin(xmin), sigh->GetXaxis()->FindBin(xmax), "width");
  cout << "Norm from hist = " << norm << endl;

  //-----------------------------------
  // set up function for fit
  //-----------------------------------
  //signalfunction_gauss sigfuncobj(xmin, xmax);
  signalfunction_doublegauss sigfuncobj(xmin, xmax);
  sigfuncobj.SetNorm(norm);    

  //  TCanvas c = TCanvas();
  TF1* fitmodel = new TF1("fitmodel", sigfuncobj, xmin, xmax, 5);
  fitmodel->SetParameter(0, mean);  
  fitmodel->SetParameter(1, sigma);
  
  fitmodel->SetParameter(2,0.7*mean);
  fitmodel->SetParLimits(2,xmin,mean-2*sigma);
  if(year.Contains("2016v3"))  fitmodel->SetParLimits(2,xmin,mean-1.5*sigma);

  fitmodel ->SetParameter(3,sigma);
  fitmodel ->SetParameter(4,0.15);
  
  fitmodel->SetNpx(200); 

  fitmodel->SetParName(0,"#mu");
  fitmodel->SetParName(1,"#sigma");
  fitmodel->SetParName(2, "n");  
  fitmodel->SetParName(3, "#alpha");

  Int_t linecol = kRed+2; 
  Int_t col68 = kOrange; 
  Int_t col95 = kOrange+7;
  //TString fdesc = "Gaussian PDF, 2 pars";
  //TString ffile = TString::Format("gaus_MT%d", MT);

  TString fdesc = "CB PDF, 4 pars";
  TString ffile = TString::Format("cb_MT%d", MT);

  cout << "Norm in function = " << fitmodel->Integral(xmin, xmax, 1e-3) << endl;

  // some cosmetics
  fitmodel->SetLineColor(linecol);
  fitmodel->SetLineStyle(kSolid);
  fitmodel->SetLineWidth(2);

  sigh->Draw();

  // FIT!
  //  TFitResultPtr r = sigh->Fit(fitmodel, "RS 0");
  TFitResultPtr r = sigh->Fit(fitmodel, "RS");
  TMatrixDSym covmatr = r->GetCovarianceMatrix();
  TMatrixDSym rho = r->GetCorrelationMatrix();
  covmatr.Print();
  rho.Print();

  //  c.SaveAs("test.eps");
  // efficiency calculation
  double Nevents_err; 
  double Nevents = 0;
  for(int bin=1;bin < sigh->GetNbinsX()+1;bin++){
    double bin_cen = sigh->GetBinCenter(bin);
    double value = fitmodel->Eval(bin_cen);
    Nevents+=value;
  }

  double Ntot = sigh->Integral(sigh->GetXaxis()->GetXmin(), sigh->GetXaxis()->GetXmax());

  TString unc_name = unc;
  if (unc=="") unc_name = "nominal";
  if(write_Nevet) Nevttofile <<unc_name<<"  MT  "<<MT<< "  1-(Nevt/Ntot)  "<<1-(Nevents/Ntot)<< "  Ntot  "<< Ntot <<"  Nevt  "<<Nevents <<endl;
  cout << "\n" << "Total number of expected events (1pb?) = " << sigh->GetSumOfWeights() << " and within fit range: " << sigh->Integral(sigh->FindBin(xmin), sigh->FindBin(xmax)) << endl << endl;
  
  Nevents = sigh->IntegralAndError(sigh->GetXaxis()->FindBin(xmin), sigh->GetXaxis()->FindBin(xmax), Nevents_err);

  double efferr;
  double eff = CalcEff(fitmodel, Nevents, Nevents_err, Ntot, MT, efferr, year);


  // store the results
  means.push_back(fitmodel->GetParameter(0));
  means_err.push_back(fitmodel->GetParError(0));
  widths.push_back(fitmodel->GetParameter(1));
  widths_err.push_back(fitmodel->GetParError(1));
  effs.push_back(eff);
  effs_err.push_back(efferr);

  //second gaussian
  means2.push_back(fitmodel->GetParameter(2));
  means2_err.push_back(fitmodel->GetParError(2));
  widths2.push_back(fitmodel->GetParameter(3));
  widths2_err.push_back(fitmodel->GetParError(3));
  fnorm.push_back(fitmodel->GetParameter(4));
  fnorm_err.push_back(fitmodel->GetParError(4));


  // draw the result
  //  sigh->GetYaxis()->SetRangeUser(0,100);
  
  sigh->Draw("PZ");
  //gPad->SetLogy(true);


  // draw 95% CL
  TH1F* clhist = ComputeHistWithCL(fitmodel, r, sigh, 0.95);
  clhist->SetStats(kFALSE);
  clhist->SetLineColor(col95);
  clhist->SetFillColor(col95);
  clhist->SetMarkerColor(col95);
  clhist->Draw("e3 same");

  // draw 68% CL
  TH1F* clhist2 = ComputeHistWithCL(fitmodel, r, sigh, 0.68);
  clhist2->SetStats(kFALSE);
  clhist2->SetLineColor(col68);
  clhist2->SetFillColor(col68);
  clhist2->SetMarkerColor(col68);
  clhist2->Draw("e3 same");

  //fitmodel->DrawClone("same");
  sigh->DrawClone("PZ same");
  fitmodel->DrawClone("same");


  // some information
  TString info, info2;
  info = TString::Format("Signal with M_{T} = %d GeV", MT);
  if (channel==eMuon){
    info2 = "#mu+jets";
  } else if (channel==eEle) {
    info2 = "e+jets";
  } else if (channel==eComb) {
    info2 = "l+jets";
  }
  TString info4 = fdesc;
  TLatex* text = new TLatex();
  text->SetTextFont(42);
  text->SetNDC();
  text->SetTextColor(kBlack);
  text->SetTextSize(0.035);
  if(!year.Contains("allyears"))  text->DrawLatex(0.13, 0.96, info.Data());
  if(!year.Contains("allyears"))  text->DrawLatex(0.13, 0.92, info2.Data());
  //text->DrawLatex(0.13, 0.92, info3.Data());
  text->SetTextFont(62);
  if(!year.Contains("allyears"))text->DrawLatex(0.65, 0.83, info4.Data());
  else text->DrawLatex(0.65, 0.81, TString::Format("#splitline{%s",info2.Data())+"}{"+TString::Format("M_{T} = %i",(MT))+" GeV}");
  text->SetTextFont(42);

  double xvalue = 0.65;
  double yoffset = 0.03;

  TString info5 = "Fit:";
  text->DrawLatex(xvalue, 0.76 -yoffset, info5.Data());
  info5 = TString::Format("x_{min} = %3.0f GeV", xmin);
  text->DrawLatex(xvalue, 0.72-yoffset, info5.Data());
  info5 = TString::Format("x_{max} = %3.0f GeV", xmax);
  text->DrawLatex(xvalue, 0.67-yoffset, info5.Data());

  TString info6 = TString::Format("#varepsilon_{sig} = %4.2f #pm %4.2f", eff*100, efferr*100);
  info6.Append("%");
  text->DrawLatex(xvalue, 0.60-yoffset, info6.Data());  

  double corr = sigh->Integral(sigh->GetXaxis()->GetXmin(), sigh->GetXaxis()->GetXmax(), "width") / norm;
  cout << "corr = " << corr << endl;
  TString info7 = TString::Format("f_{norm} = %4.3f", sigfuncobj.GetFunc()->GetParameter(0)*corr );
  //  text->DrawLatex(xvalue, 0.55, info7.Data());  

  TString info8 = TString::Format("#chi^{2}/ndf = %.2f / %i", fitmodel->GetChisquare(),fitmodel->GetNDF() );
   text->DrawLatex(xvalue, 0.26-yoffset, info8.Data());  

  TString info9 = TString::Format("#mu = %.0f #pm %.0f GeV", fitmodel->GetParameter(0), fitmodel->GetParError(0) );
  text->DrawLatex(xvalue, 0.53-yoffset, info9.Data());  

  TString info10 = TString::Format("#sigma = %.0f #pm %.0f GeV",  fitmodel->GetParameter(1), fitmodel->GetParError(1));
  text->DrawLatex(xvalue, 0.47-yoffset, info10.Data());  


  TString info92 = TString::Format("#mu_{2} = %.0f #pm %.0f GeV", fitmodel->GetParameter(2), fitmodel->GetParError(2) );
  text->DrawLatex(xvalue, 0.43-yoffset, info92.Data());  

  TString info102 = TString::Format("#sigma_{2} = %.0f #pm %.0f GeV",  fitmodel->GetParameter(3), fitmodel->GetParError(3));
  text->DrawLatex(xvalue, 0.37-yoffset, info102.Data());  

  TString info11 = TString::Format("f_{norm} = %.2f #pm %.2f",  fitmodel->GetParameter(4), fitmodel->GetParError(4));
  text->DrawLatex(xvalue, 0.33-yoffset, info11.Data());  


  ////// CMS tags
  TString infotext = "13 TeV";
  TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.94);
  text1->SetTextFont(42);
  text1->SetTextSize(0.045);
  text1->SetY(.95);
  text1->Draw();
  
  TString cmstext = "CMS";
  TLatex *text2 = new TLatex(3.5, 24, cmstext);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.2);
  text2->SetTextFont(62);
  text2->SetTextSize(0.072);
  text2->SetY(0.85);
  text2->Draw();
 
  TString simtext = "Simulation";
  TLatex *text4 = new TLatex(3.5, 24, simtext);
  text4->SetNDC();
  text4->SetTextAlign(13);
  text4->SetX(0.2);
  text4->SetTextFont(52);
  text4->SetTextSize(0.05);
  text4->SetY(0.78);
  text4->Draw();


  TString preltext = "Preliminary";
  TLatex *text3 = new TLatex(3.5, 24, preltext);
  text3->SetNDC();
  text3->SetTextAlign(13);
  text3->SetX(0.2);
  text3->SetTextFont(52);
  text3->SetTextSize(0.05);
  text3->SetY(0.73);
  text3->Draw();


  can->RedrawAxis();

  TString folder = "results";
  TString channel_name = "";
  if (channel==eMuon){
    channel_name = "much";
  } else if (channel==eEle) {
    channel_name = "ech";
  } else if (channel==eComb) {
    channel_name = "comb";
  }

  can->Print(folder + "/signalfit_" + channel_name + "_" + ffile + + "_" +year+"_"+unc+".pdf");

  // save ratios and all infos
/*  
  std::vector<TH1F*> err_hists;
  err_hists.push_back(clhist);
  err_hists.push_back(clhist2);  
  plot_ratio(back, fitmodel, err_hists, region, channel, dodata, all_bkgds, fdesc, ffile);
*/

}

