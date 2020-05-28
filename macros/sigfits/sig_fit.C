#include "TH1.h"
#include <fstream>
#include "sigfunc.C"
#include "../bkgfits/func.C"
#include "../bkgfits/helpers.C"

using namespace std;
  bool b_test = false;

void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,TString unc="", TString year = "2016v3");
void draw_eff_unc(TGraphErrors* geff, TGraphErrors* geff_up, TGraphErrors* geff_dn, TString name);

void sig_fit()
{
  TString year =  "2016v3";
  // TString year =  "2017v2";
  // TString year =  "2018";
  // TString year =  "allyears";

  //  TString postfix = "_HEMIssue_LH";
  TString postfix = "_comb";



  std::ofstream infotofile("SignalFitOutput_"+year+postfix+".txt", std::ios::out | std::ios::trunc);
  // decide which channel to do (eEle, eMuon, eComb)
    Echannel ch = eComb;
  //  Echannel ch = eEle;
  //  Echannel ch = eMuon;

  std::vector<TString> uncertainties ={}; // no syst.
	if(!b_test) {
	  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","prefiring","btag_bc","btag_udsg","eletrigger","mutrigger","scale"}; // 2016 & 2017 
	  if(year.Contains("2018"))  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","btag_bc","btag_udsg","eletrigger","mutrigger","scale"};
	  if(year.Contains("allyears")) uncertainties = {};
	}

  std::vector<double> MTs = {650};// 2016
  std::vector<double> MTs_backup = {650};// 2016


  if (!b_test){

    MTs = {700, 800,  900, 1000, 1200};// 2016
    MTs_backup = {700,800,900, 1000, 1200};// 2016

    if(year.Contains("2018") or year.Contains("2017")){
      MTs = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017
      MTs_backup = {600, 650, 700, 800, 900, 1000, 1100, 1200};// 2017
    }
    if(year.Contains("allyears")){
      MTs = {600,650, 700,800,900, 1000,1100, 1200};// 2017
      MTs_backup = {600, 650, 700,800,900, 1000,1100, 1200};// 2017
    }
  }

  std::vector<double> means;
  std::vector<double> means_err;
  std::vector<double> widths;  
  std::vector<double> widths_err;  
  std::vector<double> effs;  
  std::vector<double> effs_err;  
  std::vector<double> zeros;

  // do the fits, fill results into graph
  for (int i=0; i<MTs.size(); ++i){
    fitsignal(ch,  (int) MTs[i], means, means_err, widths, widths_err, effs, effs_err,"",year);
    zeros.push_back(0);
  }
  TGraphErrors* gmean    = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  TGraphErrors* gsigma   = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());    
  TGraphErrors* geff     = new TGraphErrors(MTs.size(), MTs.data(), effs.data(), zeros.data(), effs_err.data()); 
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

  //MTs[0] = 550;
  //MTs[6] = 1250;
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
  can->SaveAs(fname+postfix+".eps");
  can->SaveAs(fname+postfix+".pdf");

  ////// add uncertainties
  // for each uncertainties fit linear function and draw it
  std::vector<double> means_unc;
  std::vector<double> means_err_unc;
  std::vector<double> widths_unc;  
  std::vector<double> widths_err_unc;  
  std::vector<double> effs_unc;  
  std::vector<double> effs_err_unc;  

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
    for(int j=0; j<2; j++){
      TString direction = "_up";
      if(j==1) direction = "_down";

      means_unc.clear();
      means_err_unc.clear();
      widths_unc.clear();  
      widths_err_unc.clear();  
      effs_unc.clear();  
      effs_err_unc.clear();  

      for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, unc+direction,year);
      }
    TGraphErrors* gmean_unc  = new TGraphErrors(MTs.size(), MTs.data(), means_unc.data(), zeros.data(), means_err_unc.data());  
    TF1* lin = new TF1("meanfit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
    TFitResultPtr r_unc = gmean_unc->Fit(lin, "0");
    lin->SetLineColor(kBlue+jj);
    lin->SetLineStyle(jj); 
    can->cd();
    if(unc.Contains("JER") or unc.Contains("JEC")){
      lin->DrawCopy("same");
      //jj+=4;
    }
    if(unc.Contains("JEC")&&direction.Contains("up")){
      infotofile<< "JECmfitup param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JECmfitup param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
    }
    if(unc.Contains("JER")&&direction.Contains("up")){
      infotofile<< "JERmfitup param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JERmfitup param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
    }
    if(unc.Contains("JEC")&&direction.Contains("down")){
      infotofile<< "JECmfitdown param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JECmfitdown param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
    }
    if(unc.Contains("JER")&&direction.Contains("down")){
      infotofile<< "JERmfitdown param0 \t"<< lin->GetParameter(0)<<"\t"<<lin->GetParError(0)<<std::endl;
      infotofile<< "JERmfitdown param1 \t"<< lin->GetParameter(1)<<"\t"<<  lin->GetParError(1)<<std::endl;
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
  can->SaveAs(fname+postfix+"_unc.eps");
  can->SaveAs(fname+postfix+"_unc.pdf");
  ///////////
  ///
  /// test different fit ranges
  ///
  //////////

  std::vector<double> means_fitrange;
  std::vector<double> means_err_fitrange;
  std::vector<double> widths_fitrange;  
  std::vector<double> widths_err_fitrange;  
  std::vector<double> effs_fitrange;  
  std::vector<double> effs_err_fitrange;  

  TLegend *leg_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  leg_fitrange->SetBorderSize(0);
  leg_fitrange->SetFillStyle(0);
  leg_fitrange->SetTextSize(0.035);
  leg_fitrange->SetFillColor(0);
  leg_fitrange->SetLineColor(1);
  leg_fitrange->SetTextFont(42);

  MTs = MTs_backup;//{600, 650, 800, 900, 1000, 1100,1200};
  std::vector<float> sigma_ranges = {1,1.5,2,2.5};
  for(int j=0; j<sigma_ranges.size(); j++){
      means_fitrange.clear();
      means_err_fitrange.clear();
      widths_fitrange.clear();  
      widths_err_fitrange.clear();  
      effs_fitrange.clear();  
      effs_err_fitrange.clear();  

      for (int i=0; i<MTs.size(); ++i){
	fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "",year,sigma_ranges[j],sigma_ranges[j]);
      }
    TGraphErrors* gmean_fitrange  = new TGraphErrors(MTs.size(), MTs.data(), means_fitrange.data(), zeros.data(), means_err_fitrange.data());  
    TF1* lin = new TF1("meanfit"+TString::Format("%d",j), "[0]+[1]*(x-600)", 550, 1250);
    TFitResultPtr r_fitrange = gmean_fitrange->Fit(lin, "0");
    lin->SetLineColor(kBlue+j+1);
    lin->SetLineStyle(j+1); 
    can->cd();
    lin->DrawCopy("same");
    leg_fitrange->AddEntry(lin,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%.3f", lin->GetParameter(1)),"l");

    }

  can->cd();
  leg_fitrange->Draw();
  can->SaveAs(fname+postfix+"_fitrange.eps");
  can->SaveAs(fname+postfix+"_fitrange.pdf");



  //-------------------------------------------------
  // Widths with fit
  //-------------------------------------------------
  TCanvas *can2 = new TCanvas("widths_can","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
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

  //MTs[0] = 550;
  //MTs[6] = 1250;
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
  can2->SaveAs(fname2+postfix + ".eps");
  can2->SaveAs(fname2+postfix + ".pdf");

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
    for(int j=0; j<2; j++){
      TString direction = "_up";
      if(j==1) direction = "_down";
      means_unc.clear();
      means_err_unc.clear();
      widths_unc.clear();  
      widths_err_unc.clear();  
      effs_unc.clear();  
      effs_err_unc.clear();  

      for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, unc+direction, year);
      }
      TGraphErrors* gsigma_unc = new TGraphErrors(MTs.size(), MTs.data(), widths_unc.data(), zeros.data(), widths_err_unc.data());    
      TF1* lin2 = new TF1("sigmafit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
      TFitResultPtr r_unc = gsigma_unc->Fit(lin2, "0");
      lin2->SetLineColor(kBlue+jj);
      lin2->SetLineStyle(jj);
      can2->cd();
      gsigma_unc->SetMarkerStyle(24);
      if(unc.Contains("PDF"))gsigma_unc->Draw("PE Same");
      if(unc.Contains("JEC")&&direction.Contains("up")){
	      infotofile<< "JECwfitup param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitup param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JER")&&direction.Contains("up")){
	      infotofile<< "JERwfitup param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitup param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JEC")&&direction.Contains("down")){
	      infotofile<< "JECwfitdown param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitdown param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
      }
      if(unc.Contains("JER")&&direction.Contains("down")){
	      infotofile<< "JERwfitdown param0 \t"<< lin2->GetParameter(0)<<"\t"<<lin2->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitdown param1 \t"<< lin2->GetParameter(1)<<"\t"<<  lin2->GetParError(1)<<std::endl;
      }

      if(j==0)lin2->Draw("same");
      if(j==0)leg2->AddEntry(lin2,unc+" Slope "+TString::Format("%.3f", lin2->GetParameter(1)),"l");
      if(j==0)jj++;
      if(jj==5)jj++;
    }
  }

  can2->cd();
  leg2->Draw();
  can2->SaveAs(fname2+postfix+"_unc.eps");
  can2->SaveAs(fname2+postfix+"_unc.pdf");

  ///////
  ///
  /// testing the fit range
  ///
  //////

  TLegend *leg2_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  leg2_fitrange->SetBorderSize(0);
  leg2_fitrange->SetFillStyle(0);
  leg2_fitrange->SetTextSize(0.035);
  leg2_fitrange->SetFillColor(0);
  leg2_fitrange->SetLineColor(1);
  leg2_fitrange->SetTextFont(42);

  MTs = MTs_backup;//{600, 650, 800, 900, 1000,1100, 1200};

  for(int j=0; j<sigma_ranges.size(); j++){
      means_fitrange.clear();
      means_err_fitrange.clear();
      widths_fitrange.clear();  
      widths_err_fitrange.clear();  
      effs_fitrange.clear();  
      effs_err_fitrange.clear();  

      for (int i=0; i<MTs.size(); ++i){
	fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "", year,sigma_ranges[j],sigma_ranges[j]);
      }
      TGraphErrors* gsigma_fitrange = new TGraphErrors(MTs.size(), MTs.data(), widths_fitrange.data(), zeros.data(), widths_err_fitrange.data());    
      TF1* lin2 = new TF1("sigmafit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
      TFitResultPtr r_fitrange = gsigma_fitrange->Fit(lin2, "0");
      lin2->SetLineColor(kBlue+j+1);
      lin2->SetLineStyle(j+1);
      can2->cd();
      gsigma_fitrange->SetMarkerStyle(24);

     lin2->Draw("same");
     leg2_fitrange->AddEntry(lin2,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%.3f", lin2->GetParameter(1)),"l");
  }

  can2->cd();
  leg2_fitrange->Draw();
  can2->SaveAs(fname2+postfix+"_fitrange.eps");
  can2->SaveAs(fname2+postfix+"_fitrange.pdf");


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
  if(year.Contains("allyears"))  painter3->GetYaxis()->SetRangeUser(0.006, 0.02);
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

  infotofile<<"efficiency estimate  "<<lin3->GetParameter(0)<<std::endl;

  //MTs[0] = 550;
  //MTs[6] = 1250;
  //TGraphErrors* fit3_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  //fitter = (TFitter*) TVirtualFitter::GetFitter();
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

  infotofile<< "efffit param0 \t"<<lin3->GetParameter(0)<<"\t"<<lin3->GetParError(0)<<std::endl;
  infotofile<< "efffit param1 \t"<<lin3->GetParameter(1)<<"\t"<<lin3->GetParError(1)<<std::endl;
  infotofile<< "efffit param2 \t"<<lin3->GetParameter(2)<<"\t"<<lin3->GetParError(2)<<std::endl;

  double nom_eff = lin3->GetParameter(0);

  lin3->SetParameter(0,lin3->GetParameter(0)+lin3->GetParError(0));
  //  lin3->SetParameter(1,lin3->GetParameter(1)+0.5*lin3->GetParError(1));
  lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");
  infotofile<<"efficiency estimate +1sigma  "<<lin3->GetParameter(0)<<std::endl;

  lin3->SetParameter(0,lin3->GetParameter(0)-2*lin3->GetParError(0));
  //  lin3->SetParameter(1,lin3->GetParameter(1)-lin3->GetParError(1));
  lin3->SetLineColor(kBlue);
  //  lin3->DrawCopy("same");
  infotofile<<"efficiency estimate -1sigma  "<<lin3->GetParameter(0)<<std::endl;

  // draw some info
  info = TString::Format("Signal efficiencies, ");
  info.Append(info2);
  text->DrawLatex(0.16, 0.92, info.Data());

  TString fname3 = "results/signal_eff_values_" + channel_name+"_"+year; 

  can3->RedrawAxis();
  can3->SaveAs(fname3+postfix+ ".eps");
  can3->SaveAs(fname3+postfix+ ".pdf");

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
    if (unc == "scale") continue;
    std::cout << "   ===============================    uncertainty        " << unc << std::endl;

    means_unc.clear();
    means_err_unc.clear();
    widths_unc.clear();  
    widths_err_unc.clear();  
    effs_unc.clear();  
    effs_err_unc.clear();  
    for (int i=0; i<MTs.size(); ++i){
      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, unc+"_up", year);
    }
    TGraphErrors* geff_unc_up = new TGraphErrors(MTs.size(), MTs.data(), effs_unc.data(), zeros.data(), effs_err_unc.data());          

    means_unc.clear();
    means_err_unc.clear();
    widths_unc.clear();  
    widths_err_unc.clear();  
    effs_unc.clear();  
    effs_err_unc.clear();      
    for (int i=0; i<MTs.size(); ++i){
      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, unc+"_down", year);
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
      // ----------------- WARNING: remove after testing -------------------
      // for now, testing: ignore uncertainties larger than 20%
      if (relup>0.2) relup=0;
      if (reldn>0.2) reldn=0;
      // ----------------- END WARNING: remove after testing -------------------
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

    if(unc.Contains("btag_bc")) {
      //geff_unc_up->SetMarkerStyle(24);
      //geff_unc_up->SetLineWidth(2);     
      //geff_unc->Draw("P same");
      infotofile<<"efficiency estimate btag_bc  "<<lin3->GetParameter(0)<<std::endl;
    }
    if(unc.Contains("eletrigger")){
      infotofile<<"efficiency estimate eletrigger  "<<lin3->GetParameter(0)<<std::endl;
    }
    if(unc.Contains("PDF")){
      infotofile<<"efficiency estimate PDF  "<<lin3->GetParameter(0)<<std::endl;
      geff_unc->SetMarkerStyle(24);
      geff_unc->SetLineWidth(2);     
      //      geff_unc->Draw("P same");

    }
    // leg3->AddEntry(lin3,unc+" Slope "+TString::Format("%f", lin3->GetParameter(1)),"l");
    leg3->AddEntry(lin3,unc,"l");
    jj++;
    if (jj==5)jj++;
    
  }

  for (int i=0; i<MTs.size(); ++i)
  {
    cout << "MT = " << MTs[i] << " rel err up = " << sqrt(effs_uperr2[i]) << " rel err down = " << sqrt(effs_dnerr2[i]) << endl;
  }

  // ///////
  // ///
  // /// testing fit range
  // ///
  // /////
  // TLegend *leg3_fitrange = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
  // leg3_fitrange->SetBorderSize(0);
  // leg3_fitrange->SetFillStyle(0);
  // leg3_fitrange->SetTextSize(0.035);
  // leg3_fitrange->SetFillColor(0);
  // leg3_fitrange->SetLineColor(1);
  // leg3_fitrange->SetTextFont(42);

  // MTs = MTs_backup;// {600, 650, 800, 900, 1000, 1100,1200};
  // for(int j=0; j<sigma_ranges.size(); j++){
  //   means_fitrange.clear();
  //   means_err_fitrange.clear();
  //   widths_fitrange.clear();  
  //   widths_err_fitrange.clear();  
  //   effs_fitrange.clear();  
  //   effs_err_fitrange.clear();  

  //   for (int i=0; i<MTs.size(); ++i){
  //     fitsignal(ch,  (int) MTs[i], means_fitrange, means_err_fitrange, widths_fitrange, widths_err_fitrange, effs_fitrange, effs_err_fitrange, "",year,sigma_ranges[j],sigma_ranges[j]);
  //   }
  //   TGraphErrors* geff_fitrange   = new TGraphErrors(MTs.size(), MTs.data(), effs_fitrange.data(), zeros.data(), effs_err_fitrange.data());      
  //   TF1* lin3 = new TF1("efffit"+TString::Format("%d",jj), "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  //   TFitResultPtr r_fitrange = geff_fitrange->Fit(lin3, "0");
  //   lin3->SetLineColor(kBlue+j+1);
  //   lin3->SetLineStyle(j+1);
  //   can3->cd();
  //   lin3->Draw("same");
  //   leg3_fitrange->AddEntry(lin3,TString::Format("%.2f",sigma_ranges[j])+" Slope "+TString::Format("%f", lin3->GetParameter(1)),"l");
    
  // }

  // can3->cd();
  // leg3_fitrange->Draw();
  // can3->SaveAs(fname3+postfix+"_fitrange.eps");
  // can3->SaveAs(fname3+postfix+"_fitrange.pdf");


  //---------------------------------------------------------
  // Draw efficiencies with fit and total uncertainties
  //---------------------------------------------------------
  gStyle->SetStatX(0.63);
  gStyle->SetStatW(0.24);
  gStyle->SetStatY(0.85);  
  //gStyle->SetStatX(0);
  //gStyle->SetStatW(0);
  //gStyle->SetStatY(0);  
  gStyle->SetEndErrorSize(4.0);


  TCanvas *can4 = new TCanvas("eff_can_unc","",10,10,700,700);
  gPad->SetTickx();
  gPad->SetTicky();
  can4->SetLeftMargin(0.15);
  can4->SetRightMargin(0.05);
  can4->SetTopMargin(0.10);
  can4->SetBottomMargin(0.12);

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
  if(year.Contains("allyears"))  painter4->GetYaxis()->SetRangeUser(0.006, 0.02);
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

    geff_totunc->SetPointError(i, 0, tot_err_rel*y); 
  }  

  // fit the efficiency with total uncertainty
  TF1* lin_tot = new TF1("efffit_tot", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  lin_tot->SetParameter(0, effs[0]);
  lin_tot->SetParameter(1, 0);  
  lin_tot->SetParName(0, "#varepsilon at 600 GeV");
  lin_tot->SetParName(1, "Slope");
  TFitResultPtr rtot = geff_totunc->Fit(lin_tot, "0");

  //infotofile<<"efficiency estimate  "<<lin3->GetParameter(0)<<std::endl;
  //TGraphErrors* fit_toterr_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
  //fitter = (TFitter*) TVirtualFitter::GetFitter();
  //fitter->GetConfidenceIntervals(fit_toterr_68, 0.68);
  //fit_toterr_68->SetFillColor(kOrange-4);
  //fit_toterr_68->Draw("L3 same");

  // use uncertainty in p0 to estimate 1sigma uncertainty
  int Npoints = 100;
  double MTmin = 550;
  double MTmax = 1250; 
  double MTrange = MTmax - MTmin; 
  double step = MTrange/Npoints;
  TGraphErrors* eff_err_tot = new TGraphErrors(Npoints+1);
  TF1* lin_tot_unc = (TF1*) lin_tot->Clone();
  lin_tot_unc->SetParameter(0, lin_tot->GetParameter(0)+lin_tot->GetParError(0));
  for (int i=0; i<Npoints+1; ++i){
    double x = MTmin + step*i;
    double y = lin_tot->Eval(x); 
    double yerr = fabs(y-lin_tot_unc->Eval(x)); 
    eff_err_tot->SetPoint(i, x, y);
    eff_err_tot->SetPointError(i, 0, yerr); 
    //cout << "MT = " << x << " eff = " << y << " +- " << yerr << endl;
  }
  eff_err_tot->SetFillColor(kOrange-4);
  eff_err_tot->Draw("L3 same");


  // fit the efficiency with statistical uncertainty
  TF1* lin_stat = new TF1("efffit_stat", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
  lin_stat->SetParameter(0, effs[0]);
  lin_stat->SetParameter(1, 0);  
  lin_stat->SetParName(0, "#varepsilon at 600 GeV");
  lin_stat->SetParName(1, "Slope");
  TFitResultPtr r6 = geff->Fit(lin_stat, "0");

  // use uncertainty in p0 to estimate 1sigma uncertainty
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
  eff_err_stat->SetFillColor(kOrange+1);
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


  infotofile<< "efffit param0 \t"<<lin_tot->GetParameter(0)<<"\t"<<lin_tot->GetParError(0)<<std::endl;
  infotofile<< "efffit param1 \t"<<lin_tot->GetParameter(1)<<"\t"<<lin_tot->GetParError(1)<<std::endl;
  infotofile<< "efffit param2 \t"<<lin_tot->GetParameter(2)<<"\t"<<lin_tot->GetParError(2)<<std::endl;

  infotofile<<"efficiency estimate +1sigma (total) "<< lin_tot->GetParameter(0)+lin_tot->GetParError(0) <<std::endl;
  infotofile<<"efficiency estimate -1sigma (total) "<< lin_tot->GetParameter(0)-lin_tot->GetParError(0) <<std::endl;


  // draw some info
  info = TString::Format("Signal efficiencies, ");
  info.Append(info2);
  //text->DrawLatex(0.16, 0.92, info.Data());

  TString fname4 = "results/signal_eff_values_" + channel_name+"_"+year; 

  can4->RedrawAxis();
  can4->SaveAs(fname4+postfix+"_unc.eps");
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

  TCanvas* c = new TCanvas("unc_can", "", 800, 600);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.13); 
  gPad->SetLeftMargin(0.15); 


  TH1F* plotter = new TH1F("plotter", "", 1, xmin, xmax);
  plotter->GetXaxis()->SetRangeUser(xmin,xmax);
  plotter->GetYaxis()->SetRangeUser(-100,100);  
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

void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,TString unc="", TString year = "2016v3", float factormin=2, float factormax=2.5)
{
  
  // set fit regions (crude estimate)
  double fit_xmin = MT-20-150.;
  double fit_xmax = MT-20+150.;  

 
  // a bit more precise 
  double mean = MT-20;
  double sigma = 61.5+0.0033*(MT-600.);
  fit_xmin = mean - factormin*sigma;
  fit_xmax = mean + factormax*sigma;

  // if(b_test){
  //   fit_xmin = MT-300.; //600
  //   fit_xmax = MT+300.; //600
  //   // fit_xmin = MT-400.; //1000
  //   // fit_xmax = MT+100.;  //1000
  
  // }


  gROOT->SetBatch(kTRUE);

  //setOptFit( pcev (default = 0111)) Probability; Chisquare/Number of degrees of freedom; errors ;values of parameters 
  //gStyle->SetOptFit(1111);
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
  signalfunction_gauss sigfuncobj(xmin, xmax);
  //   signalfunction_doublegauss sigfuncobj(xmin, xmax);
  sigfuncobj.SetNorm(norm);    

  //  TCanvas c = TCanvas();
  TF1* fitmodel = new TF1("fitmodel", sigfuncobj, xmin, xmax, 2);
  fitmodel->SetParameter(0, MT-20.);  
  fitmodel->SetParameter(1, 60);


  fitmodel->SetParName(0,"#mu");
  fitmodel->SetParName(1,"#sigma");
  Int_t linecol = kRed+2; 
  Int_t col68 = kOrange; 
  Int_t col95 = kOrange+7;
  TString fdesc = "Gaussian PDF, 2 pars";
  TString ffile = TString::Format("gaus_MT%d", MT);

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
  double Nevents = sigh->IntegralAndError(sigh->GetXaxis()->FindBin(xmin), sigh->GetXaxis()->FindBin(xmax), Nevents_err);
  double Ntot = sigh->Integral(sigh->GetXaxis()->GetXmin(), sigh->GetXaxis()->GetXmax());
  double efferr;
  double eff = CalcEff(fitmodel, Nevents, Nevents_err, Ntot, MT, efferr, year);

  cout << "\n" << "Total number of expected events (1pb?) = " << sigh->GetSumOfWeights() << " and within fit range: " << sigh->Integral(sigh->FindBin(xmin), sigh->FindBin(xmax)) << endl << endl;

  // store the results
  means.push_back(fitmodel->GetParameter(0));
  means_err.push_back(fitmodel->GetParError(0));
  widths.push_back(fitmodel->GetParameter(1));
  widths_err.push_back(fitmodel->GetParError(1));
  effs.push_back(eff);
  effs_err.push_back(efferr);

  // draw the result
  sigh->GetYaxis()->SetRangeUser(0,100);
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
  text->DrawLatex(0.13, 0.96, info.Data());
  text->DrawLatex(0.13, 0.92, info2.Data());
  //text->DrawLatex(0.13, 0.92, info3.Data());
  text->SetTextFont(62);
  text->DrawLatex(0.17, 0.83, info4.Data());
  text->SetTextFont(42);
  TString info5 = "Fit:";
  text->DrawLatex(0.17, 0.76, info5.Data());
  info5 = TString::Format("x_{min} = %3.0f GeV", xmin);
  text->DrawLatex(0.17, 0.72, info5.Data());
  info5 = TString::Format("x_{max} = %3.0f GeV", xmax);
  text->DrawLatex(0.17, 0.67, info5.Data());

  TString info6 = TString::Format("#varepsilon_{sig} = %4.2f #pm %4.2f", eff*100, efferr*100);
  info6.Append("%");
  text->DrawLatex(0.17, 0.60, info6.Data());  

  double corr = sigh->Integral(sigh->GetXaxis()->GetXmin(), sigh->GetXaxis()->GetXmax(), "width") / norm;
  cout << "corr = " << corr << endl;
  TString info7 = TString::Format("f_{norm} = %4.3f", sigfuncobj.GetFunc()->GetParameter(0)*corr );
  text->DrawLatex(0.17, 0.55, info7.Data());  

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

