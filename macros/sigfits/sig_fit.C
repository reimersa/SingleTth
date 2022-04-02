#include "TH1.h"
#include <fstream>
#include "sigfunc.C"
#include "../bkgfits/func.C"
#include "../bkgfits/helpers.C"

using namespace std;
bool b_test = false;
bool b_fitrange = false;
bool bunc = true;
bool yearflag = true;
std::vector<float> sigma_ranges = {1.5,2,2.5};

TF1 * mean_param = new TF1("mean_param","[0]+[1]*(x-600)",550,1250);
TF1 * sigma_param = new TF1("sigma_param","[0]+[1]*(x-600)",550,1250);
TF1 * n_param = new TF1("n_param","[0]+[1]*(x-600)",550,1250);
//TF1 * alpha_param = new TF1("alpha_param","[0]+[1]*(x-600)",550,1250);
TF1 * alpha_param = new TF1("alpha_param","[0]+[1]*(x-600) + [2]*(x-600)**2",550,1250);
bool debug = false;
bool b_test_param = false;

//write out the number of total events vs events in fitrange
std::ofstream Nevttofile("Nevents_test.txt", std::ios::out | std::ios::trunc);


void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,std::vector<double>& means2, std::vector<double>& means2_err, std::vector<double>& widths2, std::vector<double>& widths2_err, std::vector<double>& fnorm, std::vector<double>& fnorm_err,TString unc="", TString year = "2016v3", TString cat = "chi2h_2",TString MA = "99999",TString func = "simplegauss",float factormin=2, float factormax=2.5, bool write_Nevet = false);
void draw_eff_unc(TGraphErrors* geff, TGraphErrors* geff_up, TGraphErrors* geff_dn, TString name);

void sig_fit()
{
  //  TString year =  "2016v3";
  //  TString year =  "2017v2";
  //  TString year =  "2018";
  TString year =  "allyears";

  TString postfix = "";


  //run over all ma, if ma=99999 GeV do the old stuff for the moment
  //  std::vector<TString>MAs = {"75","125","175","250","350","450","99999"};
  //  std::vector<TString>MAs = {"99999"};
  //  std::vector<TString>MAs = {"75","100","125","175","200","250","350","450","500"};
  std::vector<TString>MAs = {"125","175","200","250","350","450","500"};
  //  std::vector<TString>MAs = {"250","350","450","500"};
  //std::vector<TString>MAs = {"125"};

  for(unsigned int ima = 0; ima < MAs.size();ima++){

    TString MA = MAs[ima];

    ///// run over all channels
    //    std::vector<Echannel> channels = {eComb,eEle,eMuon};
    std::vector<Echannel> channels = {eMuon, eEle};
    //  std::vector<Echannel> channels = {eComb};
    for(unsigned int ich = 0; ich < channels.size();ich++){

      Echannel ch =channels[ich];

      ///// run over all cat
      std::vector<TString> categories = {"chi2h_2","catma60","catma90","catma175","catma300"};
      if(MA=="75") categories = {"catma90","catma60","catma300"};
      if(MA=="100") categories = {"catma90","catma300","chi2h_2"};
      if(MA=="175") categories = {"chi2h_2","catma175","catma300"};
      if(MA=="200") categories = {"catma175","catma300","chi2h_2"};
      if(MA=="250") categories = {"catma175","catma300"};
      if(MA=="350") categories = {"catma175","catma300"};
      if(MA=="450") categories = {"catma175","catma300"};
      if(MA=="500") categories = {"catma175","catma300"};
      if(MA.Contains("999")) categories = {"catma300","chi2h_2","catma175","catma90"};
      //      categories = {"catma300"};

      for(unsigned int icat=0;icat < categories.size();icat++){

	TString cat = categories[icat];

	TString func = "cb";
	
	TString outfile_name = "SignalFitOutput_"+year+"_"+cat+"_"+MA+postfix+".txt";
	if(ch==eEle) outfile_name = "SignalFitOutput_"+year+"_"+cat+"_"+MA+"_ech"+postfix+".txt";
	if(ch==eMuon) outfile_name = "SignalFitOutput_"+year+"_"+cat+"_"+MA+"_much"+postfix+".txt";

	// //read in parametrisation only for cb
	// if(func.Contains("cb")){
	//   //first chekc if file exists
	//   float c,d;
	//   TString a,b;
	//   float mean_param0,mean_param1,sigma_param0,sigma_param1,n_param0,n_param1,alpha_param0,alpha_param1,alpha_param2;

	  
	//   std::ifstream infile(outfile_name);
	//   while(infile >>a>>b>>c>>d){
	//     //	    std::cout<<" a "<<a<<" b "<<b<<" c "<<c<< " d "<<d<<std::endl;
	//     if(a.Contains("mean") && !a.Contains("2") && b.Contains("param0")) mean_param0=c;
	//     if(a.Contains("mean") && !a.Contains("2") && b.Contains("param1")) mean_param1=c;

	//     if(a.Contains("width") && !a.Contains("2") && b.Contains("param0")) sigma_param0=c;
	//     if(a.Contains("width") && !a.Contains("2") && b.Contains("param1")) sigma_param1=c;
	    

	//     if(a.Contains("mean2") && b.Contains("param0")) n_param0=c;
	//     if(a.Contains("mean2") && b.Contains("param1")) n_param1=c;
	    
	//     if(a.Contains("width2") && b.Contains("param0")) alpha_param0=c;
	//     if(a.Contains("width2") && b.Contains("param1")) alpha_param1=c;
	//     if(a.Contains("width2") && b.Contains("param2")) alpha_param2=c;

	//   }


	//   mean_param->FixParameter(0,mean_param0);
	//   mean_param->FixParameter(1,mean_param1);

	//   sigma_param->FixParameter(0,sigma_param0);
	//   sigma_param->FixParameter(1,sigma_param1);

	//   n_param->FixParameter(0,n_param0);
	//   n_param->FixParameter(1,n_param1);

	//   alpha_param->FixParameter(0,alpha_param0);
	//   alpha_param->FixParameter(1,alpha_param1);
	//   alpha_param->FixParameter(2,alpha_param2);
	  	  
	// }

    
	std::ofstream infotofile(outfile_name, std::ios::out | std::ios::trunc);
    
	std::vector<TString> uncertainties ={}; // no syst.
	if(!b_test) {
	  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","prefiring","btag_bc","btag_udsg","eletrigger","mutrigger","scale"}; // 2016 
	  //	  if(year.Contains("2016"))  uncertainties ={"JER","JEC","muid","pu","eleid","elereco","muiso",};
	  if(year.Contains("2017"))  uncertainties ={"JEC","JER","muid","muiso","mutrigger","eleid","elereco","eletrigger","PDF","prefiring","scale","pu","btag_bc","btag_udsg"};
	  //	  if(year.Contains("2017"))  uncertainties ={};
	  if(year.Contains("2018"))  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","btag_bc","btag_udsg","eletrigger","mutrigger","scale"};
	  //	  if(year.Contains("2018"))  uncertainties ={};

	  //	  if(year.Contains("allyears")) uncertainties = {}; // prefiring missing
	  if(year.Contains("allyears"))  uncertainties ={"muid","pu","eleid","elereco","muiso","PDF","JEC","JER","btag_bc","btag_udsg","eletrigger","mutrigger","scale"};
	}
    
	std::vector<double> MTs = {600};// 2016
	std::vector<double> MTs_backup = {600};// 2016

	// MTs = {600, 650, 700, 800, 900, 1000, 1100, 1200};
	// MTs_backup = {600, 650, 700, 800, 900, 1000, 1100, 1200};
    
	if (!b_test){
            
	  // Roman's setup
	  MTs = {700, 800, 900, 1000, 1100,1200};// 2016
	  MTs_backup = {700,800, 900, 1000,1100, 1200};// 2016
      
	  if(year.Contains("2016") && MA.Contains("125")){
	    MTs = {700, 800, 900, 1000, 1200};// 2016
	    MTs_backup = {700,800, 900, 1000, 1200};// 2016
	  }

	  if(year.Contains("2018") || year.Contains("2017")){
	    MTs = {600, 700, 800, 900, 1000, 1100,1200};// 2017
	    MTs_backup = {600, 700, 800, 900, 1000, 1100,1200};// 2017
	    if(cat.Contains("ma300")){
	      MTs = {700, 800, 900, 1000, 1100,1200};// 2017
	      MTs_backup = { 700, 800, 900, 1000, 1100,1200};// 2017	  
	    }
	    if(year.Contains("2017") && cat.Contains("ma175")){
	      MTs = {600,700, 800, 900, 1000, 1100,1200};// 2017
	      MTs_backup = { 600,700, 800, 900, 1000, 1100,1200};// 2017	  

	    }
	    if(year.Contains("2017") && cat.Contains("ma175") && MA.Contains("450")){
	      MTs = {600,700, 800, 900,  1100,1200};// 2017
	      MTs_backup = { 600,700, 800, 900, 1100,1200};// 2017	  

	    }
	    if(year.Contains("2017") && cat.Contains("chi2") && MA.Contains("200")){
	      MTs = {600,700, 800, 900, 1000,1100};// 2017
	      MTs_backup = { 600,700, 800, 900, 1000, 1100};// 2017	  
	    }
	    if(year.Contains("2017") && cat.Contains("catma90") && MA.Contains("75")){
	      MTs = {600,700, 800, 1000,1100,1200};// 2017
	      MTs_backup = { 600,700, 800, 1000, 1100, 1200};// 2017	  
	    }
	    // if(year.Contains("2018") && cat.Contains("catma60") && MA.Contains("75")){
	    //   MTs = {600,700, 800, 900, 1000};// 2017
	    //   MTs_backup = { 600,700, 800, 900, 1000};// 2017	  
	    // }
	    if(year.Contains("2018") && cat.Contains("catma300") && MA.Contains("200")){
	      MTs = {600,700, 800, 900, 1000,1100,1200};// 2018
	      MTs_backup = { 600,700, 800, 900, 1000, 1100,1200};// 2018
	    }

	  }
	  if(year.Contains("allyears")){
	    MTs = { 700,800,900, 1000, 1200};// 2017
	    MTs_backup = { 700,800,900, 1000, 1200};// 2017
	  }
	}


    
	std::vector<double> means;
	std::vector<double> means_err;
	std::vector<double> widths;  
	std::vector<double> widths_err;  
	std::vector<double> effs;  
	std::vector<double> effs_err;  
	std::vector<double> nvalues;
	std::vector<double> nvalues_err;
	std::vector<double> alpha;  
	std::vector<double> alpha_err;  
	std::vector<double> fnorm;  
	std::vector<double> fnorm_err;  
	std::vector<double> zeros;
    
	// do the fits, fill results into graph
	for (int i=0; i<MTs.size(); ++i){
	  fitsignal(ch,  (int) MTs[i], means, means_err, widths, widths_err, effs, effs_err, nvalues, nvalues_err, alpha, alpha_err, fnorm, fnorm_err,"",year,cat,MA,func,2,2.5,true);
      
	  zeros.push_back(0);
	}
	TGraphErrors* gmean    = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
	TGraphErrors* gsigma   = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());    
	TGraphErrors* geff     = new TGraphErrors(MTs.size(), MTs.data(), effs.data(), zeros.data(), effs_err.data()); 
    
	///second gauss
	TGraphErrors* gn    = new TGraphErrors(MTs.size(), MTs.data(), nvalues.data(), zeros.data(), nvalues_err.data());  
	TGraphErrors* galpha   = new TGraphErrors(MTs.size(), MTs.data(), alpha.data(), zeros.data(), alpha_err.data());    
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
	TString mamass = "125";
	if(cat.Contains("catma")) {
	  mamass = cat;
	  mamass.ReplaceAll("catma", "");
	}
	info2 +=", Cat: M_{a} = "+mamass+" GeV";
	if(yearflag) info2+=", "+year;
	TLatex* text = new TLatex();
	text->SetTextFont(42);
	text->SetNDC();
	text->SetTextColor(kBlack);
	text->SetTextSize(0.040);  
    
	cout<<"Mean values with fit"<<endl;
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
	if(MA.Contains("999") && cat.Contains("catma90"))  painter->GetYaxis()->SetRangeUser(450, 1150);
	if(MA=="175" && cat.Contains("chi2h")) 	painter->GetYaxis()->SetRangeUser(500, 1100);
	if(MA=="75" && cat.Contains("catma60")) painter->GetYaxis()->SetRangeUser(450, 1100);
	painter->Draw();
	gmean->SetMarkerStyle(20);
	gmean->SetLineWidth(2);
    
	gStyle->SetStatX(0.63);
	gStyle->SetStatW(0.24);
	gStyle->SetStatY(0.85);  
	TF1* lin_mean = new TF1("meanfit", "[0]+[1]*(x-600)", 550, 1250);
	lin_mean->SetParName(0, "#mu at 600 GeV");
	lin_mean->SetParName(1, "Slope");
	TFitResultPtr r = gmean->Fit(lin_mean, "0");
    
	infotofile<< "meanfit param0 \t"<<lin_mean->GetParameter(0)<<"\t"<<lin_mean->GetParError(0)<<std::endl;
	infotofile<< "meanfit param1 \t"<<lin_mean->GetParameter(1)<<"\t"<<lin_mean->GetParError(1)<<std::endl;
    
	TGraphErrors* fit_err_95 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
	TFitter* fitter = (TFitter*) TVirtualFitter::GetFitter();
	fitter->GetConfidenceIntervals(fit_err_95, 0.95);
	fit_err_95->SetFillColor(kOrange-4);
	fit_err_95->Draw("L3 same");
    
	TGraphErrors* fit_err_68 = new TGraphErrors(MTs.size(), MTs.data(), means.data(), zeros.data(), means_err.data());  
	fitter->GetConfidenceIntervals(fit_err_68, 0.68);
	fit_err_68->SetFillColor(kOrange+1);
	fit_err_68->Draw("L3 same");
    
	lin_mean->SetLineColor(kRed+1);
	lin_mean->DrawCopy("same");
	gmean->Draw("P same");
    
	lin_mean->SetParameter(0, lin_mean->GetParameter(0)+2 * lin_mean->GetParError(0));
	lin_mean->SetParameter(1, lin_mean->GetParameter(1)+2*0.5* lin_mean->GetParError(1));
	lin_mean->SetLineColor(kBlue);
	//lin_mean->DrawCopy("same");
    
	lin_mean->SetParameter(0, lin_mean->GetParameter(0)-2*lin_mean->GetParError(0));
	lin_mean->SetParameter(1, lin_mean->GetParameter(1)- lin_mean->GetParError(1));
	lin_mean->SetLineColor(kBlue);
	//  lin_mean->DrawCopy("same");
    
    
	TString channel_name = "";
	if (ch==eMuon){
	  channel_name = "much";
	} else if (ch==eEle) {
	  channel_name = "ech";
	} else if (ch==eComb) {
	  channel_name = "comb";
	}
	TString fname = "results/signal_mean_values_" + channel_name +"_"+year+"_"+cat+"_"+MA; 
    
    
	// draw some info
	info = TString::Format("Signal mean values, ");
	info.Append(info2);
	text->DrawLatex(0.16, 0.92, info.Data());
    
	can->RedrawAxis();
	//  can->SaveAs(fname+postfix+".eps");
	can->SaveAs(fname+postfix+".pdf");
    
	cout<<"Mean values with fit: second gauss"<<endl;
	////////second gauss
	//-------------------------------------------------
	// Mean values with fit
	//-------------------------------------------------
	TCanvas *can_n = new TCanvas("mean_can_n","",10,10,700,700);
	gPad->SetTickx();
	gPad->SetTicky();
	gStyle->SetOptStat(0);
	can_n->SetLeftMargin(0.15);
	can_n->SetRightMargin(0.05);
	can_n->SetTopMargin(0.10);
	can_n->SetBottomMargin(0.12);
    
	// cosmetics
	painter = new TH1F("painter", "", 1, 550, 1250);
	painter->SetXTitle("Generated M_{T} [GeV]");
	painter->SetYTitle("#mu_{2} [GeV]");
	if(func.Contains("cb"))	painter->SetYTitle("n");
	if(func.Contains("lin"))	painter->SetYTitle("a");
	painter->SetTitleSize(0.045);
	painter->GetYaxis()->SetTitleSize(0.045);
	painter->GetYaxis()->SetTitleOffset(1.4);
	painter->GetXaxis()->SetTitleOffset(1.2);
	painter->SetTitle("");
	painter->GetYaxis()->SetRangeUser(300, 1250);
	if(func.Contains("cb")) painter->GetYaxis()->SetRangeUser(-2,40);
	if(func.Contains("lin")) painter->GetYaxis()->SetRangeUser(0,100);
	if(cat.Contains("175")) painter->GetYaxis()->SetRangeUser(-2,10);
	painter->Draw();
	gn->SetMarkerStyle(20);
	gn->SetLineWidth(2);
    
	gStyle->SetStatX(0.63);
	gStyle->SetStatW(0.24);
	gStyle->SetStatY(0.85);  
	TF1* lin_n = new TF1("meanfit", "[0]+[1]*(x-600)", 550, 1250);
	if(cat.Contains("ma300"))lin_n->SetParLimits(0,0,999999999);
	if(cat.Contains("ma60"))lin_n->SetParLimits(0,0,999999999);
	if(cat.Contains("ma90")&&year.Contains("allyears"))lin_n->SetParLimits(0,0,999999999);
	if(cat.Contains("chi2h")&&MA.Contains("175"))lin_n->FixParameter(1,0);
	if(cat.Contains("catma300")&&MA.Contains("75"))lin_n->FixParameter(1,0);
	if(cat.Contains("catma60")&&MA.Contains("75"))lin_n->FixParameter(1,0);
	if(cat.Contains("catma90")&&MA.Contains("75"))lin_n->FixParameter(1,0);
	if(cat.Contains("catma175")&&MA.Contains("450"))lin_n->FixParameter(1,0);
	if(year.Contains("2017")&&cat.Contains("catma300")&&MA.Contains("450"))lin_n->FixParameter(1,0);
	lin_n->SetParName(0, "#mu at 600 GeV");
	lin_n->SetParName(1, "Slope");
	r = gn->Fit(lin_n, "0");
    
	infotofile<< "mean2fit param0 \t"<<lin_n->GetParameter(0)<<"\t"<<lin_n->GetParError(0)<<std::endl;
	infotofile<< "mean2fit param1 \t"<<lin_n->GetParameter(1)<<"\t"<<lin_n->GetParError(1)<<std::endl;
    
	fit_err_95 = new TGraphErrors(MTs.size(), MTs.data(), nvalues.data(), zeros.data(), nvalues_err.data());  
	fitter = (TFitter*) TVirtualFitter::GetFitter();
	fitter->GetConfidenceIntervals(fit_err_95, 0.95);
	fit_err_95->SetFillColor(kOrange-4);
	fit_err_95->Draw("L3 same");
    
	fit_err_68 = new TGraphErrors(MTs.size(), MTs.data(), nvalues.data(), zeros.data(), nvalues_err.data());  
	fitter->GetConfidenceIntervals(fit_err_68, 0.68);
	fit_err_68->SetFillColor(kOrange+1);
	fit_err_68->Draw("L3 same");
    
	lin_n->SetLineColor(kRed+1);
	lin_n->DrawCopy("same");
	gn->Draw("P same");
    
	lin_n->SetParameter(0, lin_n->GetParameter(0)+2 * lin_n->GetParError(0));
	lin_n->SetParameter(1, lin_n->GetParameter(1)+2*0.5* lin_n->GetParError(1));
	lin_n->SetLineColor(kBlue);
	//lin_n->DrawCopy("same");
    
	lin_n->SetParameter(0, lin_n->GetParameter(0)-2*lin_n->GetParError(0));
	lin_n->SetParameter(1, lin_n->GetParameter(1)- lin_n->GetParError(1));
	lin_n->SetLineColor(kBlue);
	//  lin_n->DrawCopy("same");
    
    
	fname = "results/signal_mean2_values_" + channel_name +"_"+year+"_"+cat+"_"+MA; 
    
    
	// draw some info
	info = TString::Format("Signal mean values, ");
	info.Append(info2);
	text->DrawLatex(0.16, 0.92, info.Data());
    
	can_n->RedrawAxis();
	//  can_n->SaveAs(fname+postfix+".eps");
	can_n->SaveAs(fname+postfix+".pdf");
    
	////// add uncertainties
	// for each uncertainties fit linear function and draw it
	std::vector<double> means_unc;
	std::vector<double> means_err_unc;
	std::vector<double> widths_unc;  
	std::vector<double> widths_err_unc;  
	std::vector<double> effs_unc;  
	std::vector<double> effs_err_unc;  
    
	std::vector<double> nvalues_unc;
	std::vector<double> nvalues_err_unc;
	std::vector<double> alpha_unc;  
	std::vector<double> alpha_err_unc;  
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
	  std::cout<<"============== uncertainty  "<<unc <<std::endl;
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
	    nvalues_unc.clear();
	    nvalues_err_unc.clear();
	    alpha_unc.clear();  
	    alpha_err_unc.clear();  
	    fnorm_unc.clear();  
	    fnorm_err_unc.clear();  
	
	    for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, nvalues_unc, nvalues_err_unc, alpha_unc, alpha_err_unc, fnorm_unc, fnorm_err_unc, unc+direction,year,cat,MA,func);
	    }
	    TGraphErrors* gmean_unc  = new TGraphErrors(MTs.size(), MTs.data(), means_unc.data(), zeros.data(), means_err_unc.data());  
	    if(unc.Contains("JER") && direction.Contains("up") && MA.Contains("175") && year.Contains("2017") ){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>720 && MTs[i]<880) gmean_unc->RemovePoint(i);
	      }
	    } 
	    if(unc.Contains("JER") && direction.Contains("up") && MA.Contains("75") && year.Contains("2016") && cat.Contains("ma60")){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>920 && MTs[i]<1020) gmean_unc->RemovePoint(i);
	      }
	    } 
	    TF1* lin_mean = new TF1("meanfit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
	    TFitResultPtr r_unc = gmean_unc->Fit(lin_mean, "0");
	    lin_mean->SetLineColor(kBlue+jj);
	    lin_mean->SetLineStyle(jj); 
	    can->cd();
	
	    TGraphErrors* gn_unc  = new TGraphErrors(MTs.size(), MTs.data(), nvalues_unc.data(), zeros.data(), nvalues_err_unc.data());  
	    TF1* lin_n = new TF1("meanfit2"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
	    TFitResultPtr r_n_unc = gn_unc->Fit(lin_n, "0");
	
	    if(unc.Contains("JER") or unc.Contains("JEC")){
	      lin_mean->DrawCopy("same");
	      //jj+=4;
	    }
	    if(unc.Contains("JEC")&&direction.Contains("up")){
	      infotofile<< "JECmfitup param0 \t"<< lin_mean->GetParameter(0)<<"\t"<<lin_mean->GetParError(0)<<std::endl;
	      infotofile<< "JECmfitup param1 \t"<< lin_mean->GetParameter(1)<<"\t"<<  lin_mean->GetParError(1)<<std::endl;
	      infotofile<< "JECmfitup param2 \t"<< lin_n->GetParameter(0)<<"\t"<<lin_n->GetParError(0)<<std::endl;
	      infotofile<< "JECmfitup param3 \t"<< lin_n->GetParameter(1)<<"\t"<<  lin_n->GetParError(1)<<std::endl;
	  
	    }
	    if(unc.Contains("JER")&&direction.Contains("up")){
	      infotofile<< "JERmfitup param0 \t"<< lin_mean->GetParameter(0)<<"\t"<<lin_mean->GetParError(0)<<std::endl;
	      infotofile<< "JERmfitup param1 \t"<< lin_mean->GetParameter(1)<<"\t"<<  lin_mean->GetParError(1)<<std::endl;
	      infotofile<< "JERmfitup param2 \t"<< lin_n->GetParameter(0)<<"\t"<<lin_n->GetParError(0)<<std::endl;
	      infotofile<< "JERmfitup param3 \t"<< lin_n->GetParameter(1)<<"\t"<<  lin_n->GetParError(1)<<std::endl;
	      gmean_unc->Draw("Same");
	    }
	    if(unc.Contains("JEC")&&direction.Contains("down")){
	      infotofile<< "JECmfitdown param0 \t"<< lin_mean->GetParameter(0)<<"\t"<<lin_mean->GetParError(0)<<std::endl;
	      infotofile<< "JECmfitdown param1 \t"<< lin_mean->GetParameter(1)<<"\t"<<  lin_mean->GetParError(1)<<std::endl;
	      infotofile<< "JECmfitdown param2 \t"<< lin_n->GetParameter(0)<<"\t"<<lin_n->GetParError(0)<<std::endl;
	      infotofile<< "JECmfitdown param3 \t"<< lin_n->GetParameter(1)<<"\t"<<  lin_n->GetParError(1)<<std::endl;
	    }
	    if(unc.Contains("JER")&&direction.Contains("down")){
	      infotofile<< "JERmfitdown param0 \t"<< lin_mean->GetParameter(0)<<"\t"<<lin_mean->GetParError(0)<<std::endl;
	      infotofile<< "JERmfitdown param1 \t"<< lin_mean->GetParameter(1)<<"\t"<<  lin_mean->GetParError(1)<<std::endl;
	      infotofile<< "JERmfitdown param2 \t"<< lin_n->GetParameter(0)<<"\t"<<lin_n->GetParError(0)<<std::endl;
	      infotofile<< "JERmfitdown param3 \t"<< lin_n->GetParameter(1)<<"\t"<<  lin_n->GetParError(1)<<std::endl;
	    }
	
	    if(j==0){
	      leg->AddEntry(lin_mean,unc+" Slope "+TString::Format("%.3f", lin_mean->GetParameter(1)),"l");
	      jj+=1;
	      if (jj == 5) jj++;
	    }
	  }
      
	}

	can->cd();
	leg->Draw();
	//  can->SaveAs(fname+postfix+"_unc.eps");
	if(bunc) can->SaveAs(fname+postfix+"_unc.pdf");
    
    
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
    
	cout<<"Widths values with fit"<<endl;
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
	if(year.Contains("2016"))	painter2->GetYaxis()->SetRangeUser(0, 200);
	if(year.Contains("allyear"))   painter2->GetYaxis()->SetRangeUser(40, 130);
	if(cat.Contains("175"))  painter2->GetYaxis()->SetRangeUser(40, 300);
	if(cat.Contains("300"))  painter2->GetYaxis()->SetRangeUser(40, 300);
	if(cat.Contains("90")&&year.Contains("2018"))  painter2->GetYaxis()->SetRangeUser(0, 200);
	if(cat.Contains("chi2h")&&MA.Contains("175"))  painter2->GetYaxis()->SetRangeUser(30, 200);
	if(cat.Contains("chi2h")&&MA.Contains("200"))  painter2->GetYaxis()->SetRangeUser(30, 200);
	painter2->Draw();
	gsigma->SetMarkerStyle(20);
	gsigma->SetLineWidth(2);
    
	gStyle->SetStatX(0.70);
	gStyle->SetStatW(0.27);
	gStyle->SetStatY(0.85);  
	TF1* lin_sigma = new TF1("sigmafit", "[0]+[1]*(x-600)", 550, 1250);
	lin_sigma->SetParName(0, "#sigma at 600 GeV");
	lin_sigma->SetParName(1, "Slope");
	lin_sigma->SetParameter(0,60);
	lin_sigma->SetParameter(1,0.06);
	TFitResultPtr r2 = gsigma->Fit(lin_sigma, "0");
    
	TGraphErrors* fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());  
	fitter = (TFitter*) TVirtualFitter::GetFitter();
	fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
	fit2_err_95->SetFillColor(kOrange-4);
	fit2_err_95->Draw("L3 same");
    
	TGraphErrors* fit2_err_68 = new TGraphErrors(MTs.size(), MTs.data(), widths.data(), zeros.data(), widths_err.data());  
	fitter->GetConfidenceIntervals(fit2_err_68, 0.68);
	fit2_err_68->SetFillColor(kOrange+1);
	fit2_err_68->Draw("L3 same");
    
	lin_sigma->SetLineColor(kRed+1);
	lin_sigma->DrawCopy("same");
	gsigma->Draw("P same");
    
	infotofile<< "widthfit param0 \t"<<lin_sigma->GetParameter(0)<<"\t"<<lin_sigma->GetParError(0)<<std::endl;
	infotofile<< "widthfit param1 \t"<<lin_sigma->GetParameter(1)<<"\t"<<lin_sigma->GetParError(1)<<std::endl;
    

	lin_sigma->SetParameter(0,lin_sigma->GetParameter(0)+lin_sigma->GetParError(0));
	lin_sigma->SetParameter(1,lin_sigma->GetParameter(1)+0.5*lin_sigma->GetParError(1));
	lin_sigma->SetLineColor(kBlue);
	//  lin_sigma->DrawCopy("same");
    
	lin_sigma->SetParameter(0,lin_sigma->GetParameter(0)-2*lin_sigma->GetParError(0));
	lin_sigma->SetParameter(1,lin_sigma->GetParameter(1)-lin_sigma->GetParError(1));
	lin_sigma->SetLineColor(kBlue);
	//  lin_sigma->DrawCopy("same");


	TString fname2 = "results/signal_sigma_values_" + channel_name+"_"+year+"_"+cat+"_"+MA; 
    
	// draw some info
	info = TString::Format("Signal widths, ");
	info.Append(info2);
	text->DrawLatex(0.16, 0.92, info.Data());
    
	can2->RedrawAxis();
	//  can2->SaveAs(fname2+postfix + ".eps");
	can2->SaveAs(fname2+postfix + ".pdf");
    
	cout<<"Widths with fit: second gauss"<<endl;
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
	if(func.Contains("lin"))	painter2->SetYTitle("b");
	if(func.Contains("cb"))	painter2->SetYTitle("#alpha");
	painter2->SetTitleSize(0.045);
	painter2->GetYaxis()->SetTitleSize(0.045);
	painter2->GetYaxis()->SetTitleOffset(1.4);
	painter2->GetXaxis()->SetTitleOffset(1.2);
	painter2->SetTitle("");
	painter2->GetYaxis()->SetRangeUser(0, 500);
	if(func.Contains("cb"))	painter2->GetYaxis()->SetRangeUser(0, 3);
	if(func.Contains("cb") && cat.Contains("ma300"))	painter2->GetYaxis()->SetRangeUser(-4, 6);
	if(func.Contains("lin"))	painter2->GetYaxis()->SetRangeUser(-30, 30);
	//	if(year.Contains("allyear"))   painter2->GetYaxis()->SetRangeUser(40, 300);
	if(cat.Contains("175"))   painter2->GetYaxis()->SetRangeUser(-10, 10);
	if(cat.Contains("chi2h")&&MA.Contains("175"))   painter2->GetYaxis()->SetRangeUser(-10, 10);
	painter2->Draw();
	galpha->SetMarkerStyle(20);
	galpha->SetLineWidth(2);

	gStyle->SetStatX(0.70);
	gStyle->SetStatW(0.27);
	gStyle->SetStatY(0.85);  
	TF1* lin_alpha = new TF1("sigmafit", "[0]+[1]*(x-600)", 550, 1250);
	TF1* fconst = new TF1("fconst", "[0]", 550, 1250);
	TF1* fp2 = new TF1("fp2", "[0]+[1]*(x-600)+ [2]*(x-600)**2", 550, 1250);
	TF1* flin = new TF1("flin", "[0]+[1]*(x-600)", 550, 1250);
	lin_alpha->SetParName(0, "#sigma at 600 GeV");
	lin_alpha->SetParName(1, "Slope");
	lin_alpha->SetParameter(0,50);
	lin_alpha->SetParameter(1,0.06);
	if(func.Contains("cb")) 	lin_alpha = new TF1("sigmafit", "[0]+[1]*(x-600) + [2]*(x-600)**2", 580, 1250);
	if(year.Contains("2016")&&cat.Contains("ma90"))	lin_alpha->FixParameter(2,0);//lin_alpha = new TF1("sigmafit", "[0]+[1]*(x-600) ", 580, 1250);
	if(year.Contains("2016")&&cat.Contains("ma60"))	{
	  lin_alpha->FixParameter(2,0);//lin_alpha = new TF1("sigmafit", "[0]+[1]*(x-600) ", 580, 1250);
	}
	//	if(func.Contains("cb")&&cat.Contains("catma300")) 	lin_alpha = new TF1("sigmafit", "[0]+[1]*(x-600) ", 580, 1250);
	galpha->Fit(fconst,"0R");
	galpha->Fit(fp2,"0R");
	galpha->Fit(flin,"0R");
	r2 = galpha->Fit(lin_alpha,"0R");

	fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), alpha.data(), zeros.data(), alpha_err.data());  
	fitter = (TFitter*) TVirtualFitter::GetFitter();
	fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
	fit2_err_95->SetFillColor(kOrange-4);
	fit2_err_95->Draw("L3 same");

	fit2_err_68 = new TGraphErrors(MTs.size(), MTs.data(), alpha.data(), zeros.data(), alpha_err.data());  
	fitter->GetConfidenceIntervals(fit2_err_68, 0.68);
	fit2_err_68->SetFillColor(kOrange+1);
	fit2_err_68->Draw("L3 same");

	lin_alpha->SetLineColor(kRed+1);
	lin_alpha->DrawCopy("same");
	galpha->Draw("P same");

	infotofile<< "width2fit param0 \t"<<lin_alpha->GetParameter(0)<<"\t"<<lin_alpha->GetParError(0)<<std::endl;
	infotofile<< "width2fit param1 \t"<<lin_alpha->GetParameter(1)<<"\t"<<lin_alpha->GetParError(1)<<std::endl;
	if(func.Contains("cb"))	infotofile<< "width2fit param2 \t"<<lin_alpha->GetParameter(2)<<"\t"<<lin_alpha->GetParError(2)<<std::endl;


	lin_alpha->SetParameter(0,lin_alpha->GetParameter(0)+lin_alpha->GetParError(0));
	lin_alpha->SetParameter(1,lin_alpha->GetParameter(1)+0.5*lin_alpha->GetParError(1));
	lin_alpha->SetLineColor(kBlue);
	//  lin_alpha->DrawCopy("same");

	lin_alpha->SetParameter(0,lin_alpha->GetParameter(0)-2*lin_alpha->GetParError(0));
	lin_alpha->SetParameter(1,lin_alpha->GetParameter(1)-lin_alpha->GetParError(1));
	lin_alpha->SetLineColor(kBlue);
	//  lin_alpha->DrawCopy("same");


	fname2 = "results/signal_sigma2_values_" + channel_name+"_"+year+"_"+cat+"_"+MA; 

	// draw some info
	info = TString::Format("Signal widths, ");
	info.Append(info2);
	text->DrawLatex(0.16, 0.92, info.Data());

	TString info_const = TString::Format("#chi^{2}_{const}/ndf = %.2f / %i",fconst->GetChisquare(), fconst->GetNDF());
	TString info_lin = TString::Format("#chi^{2}_{lin}/ndf = %.2f / %i",flin->GetChisquare(), flin->GetNDF());
	TString info_p2 = TString::Format("#chi^{2}_{p2}/ndf = %.2f / %i",fp2->GetChisquare(), fp2->GetNDF());
	if(b_test_param)text->DrawLatex(0.5, 0.2, info_const.Data());
	if(b_test_param)	text->DrawLatex(0.5, 0.25, info_lin.Data());
	if(b_test_param)	text->DrawLatex(0.5, 0.3, info_p2.Data());


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

	    nvalues_unc.clear();
	    nvalues_err_unc.clear();
	    alpha_unc.clear();  
	    alpha_err_unc.clear();  
	    fnorm_unc.clear();  
	    fnorm_err_unc.clear();  

	    for (int i=0; i<MTs.size(); ++i){
	      fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, nvalues_unc, nvalues_err_unc, alpha_unc, alpha_err_unc, fnorm_unc, fnorm_err_unc, unc+direction, year,cat,MA, func);
	    }
	    TGraphErrors* gsigma_unc = new TGraphErrors(MTs.size(), MTs.data(), widths_unc.data(), zeros.data(), widths_err_unc.data());    
	    if(unc.Contains("JER") && direction.Contains("up")&& MA.Contains("175") && year.Contains("2017") ){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>720 && MTs[i]<880) gsigma_unc->RemovePoint(i);
	      }
	    } 
	    if(unc.Contains("JER") && direction.Contains("up") && MA.Contains("75") && year.Contains("2016") && cat.Contains("ma60")){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>920 && MTs[i]<1020) gsigma_unc->RemovePoint(i);
		//		if(MTs[i]>720 && MTs[i]<820) gsigma_unc->RemovePoint(i);
	      }
	    } 

	    TF1* lin_sigma = new TF1("sigmafit"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
	    TFitResultPtr r_unc = gsigma_unc->Fit(lin_sigma, "0");
	    lin_sigma->SetLineColor(kBlue+jj);
	    lin_sigma->SetLineStyle(jj);
	    can2->cd();
	    TGraphErrors* galpha_unc = new TGraphErrors(MTs.size(), MTs.data(), alpha_unc.data(), zeros.data(), alpha_err_unc.data());    
	    if(unc.Contains("JER") && direction.Contains("up")&& MA.Contains("175") && year.Contains("2017") ){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>720 && MTs[i]<880) galpha_unc->RemovePoint(i);
	      }
	    } 
	    if(unc.Contains("JER") && direction.Contains("up") && MA.Contains("75") && year.Contains("2016") && cat.Contains("ma60")){
	      for (int i=MTs.size()-1; i>=0; --i){
		if(MTs[i]>920 && MTs[i]<1020) galpha_unc->RemovePoint(i);
	      }
	    } 

	    TF1* lin_alpha = new TF1("sigmafit2"+TString::Format("%d",jj), "[0]+[1]*(x-600)+[2]*(x-600)**2", 550, 1250);
	    TFitResultPtr r_unc_sigma = galpha_unc->Fit(lin_alpha, "0");

	    TGraphErrors* gfnorm_unc = new TGraphErrors(MTs.size(), MTs.data(), fnorm_unc.data(), zeros.data(), fnorm_err_unc.data());    
	    TF1* lin2_fnorm = new TF1("fnorm"+TString::Format("%d",jj), "[0]+[1]*(x-600)", 550, 1250);
	    TFitResultPtr r_unc_fnorm = gfnorm_unc->Fit(lin2_fnorm, "0");


	    gsigma_unc->SetMarkerStyle(24);
	    if(unc.Contains("PDF"))gsigma_unc->Draw("PE Same");
	    if(unc.Contains("JEC")&&direction.Contains("up")){
	      infotofile<< "JECwfitup param0 \t"<< lin_sigma->GetParameter(0)<<"\t"<<lin_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitup param1 \t"<< lin_sigma->GetParameter(1)<<"\t"<<  lin_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitup param2 \t"<< lin_alpha->GetParameter(0)<<"\t"<<lin_alpha->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitup param3 \t"<< lin_alpha->GetParameter(1)<<"\t"<<  lin_alpha->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitup param4 \t"<< lin_alpha->GetParameter(2)<<"\t"<<  lin_alpha->GetParError(2)<<std::endl;
	      infotofile<< "JECffitup param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<  lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JECffitup param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
	    }
	    if(unc.Contains("JER")&&direction.Contains("up")){
	      infotofile<< "JERwfitup param0 \t"<< lin_sigma->GetParameter(0)<<"\t"<<lin_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitup param1 \t"<< lin_sigma->GetParameter(1)<<"\t"<<  lin_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitup param2 \t"<< lin_alpha->GetParameter(0)<<"\t"<<lin_alpha->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitup param3 \t"<< lin_alpha->GetParameter(1)<<"\t"<<  lin_alpha->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitup param4 \t"<< lin_alpha->GetParameter(2)<<"\t"<<  lin_alpha->GetParError(2)<<std::endl;
	      infotofile<< "JERffitup param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<  lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JERffitup param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
	      gsigma_unc->Draw("Same");
	      //	      galpha_unc->Draw("Same");
	    }
	    if(unc.Contains("JEC")&&direction.Contains("down")){
	      infotofile<< "JECwfitdown param0 \t"<< lin_sigma->GetParameter(0)<<"\t"<<lin_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitdown param1 \t"<< lin_sigma->GetParameter(1)<<"\t"<<  lin_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitdown param2 \t"<< lin_alpha->GetParameter(0)<<"\t"<<lin_alpha->GetParError(0)<<std::endl;
	      infotofile<< "JECwfitdown param3 \t"<< lin_alpha->GetParameter(1)<<"\t"<<  lin_alpha->GetParError(1)<<std::endl;
	      infotofile<< "JECwfitdown param4 \t"<< lin_alpha->GetParameter(2)<<"\t"<<  lin_alpha->GetParError(2)<<std::endl;
	      infotofile<< "JECffitdown param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JECffitdown param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
	    }
	    if(unc.Contains("JER")&&direction.Contains("down")){
	      infotofile<< "JERwfitdown param0 \t"<< lin_sigma->GetParameter(0)<<"\t"<<lin_sigma->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitdown param1 \t"<< lin_sigma->GetParameter(1)<<"\t"<<  lin_sigma->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitdown param2 \t"<< lin_alpha->GetParameter(0)<<"\t"<<lin_alpha->GetParError(0)<<std::endl;
	      infotofile<< "JERwfitdown param3 \t"<< lin_alpha->GetParameter(1)<<"\t"<<  lin_alpha->GetParError(1)<<std::endl;
	      infotofile<< "JERwfitdown param4 \t"<< lin_alpha->GetParameter(2)<<"\t"<<  lin_alpha->GetParError(2)<<std::endl;
	      infotofile<< "JERffitdown param0 \t"<< lin2_fnorm->GetParameter(0)<<"\t"<<lin2_fnorm->GetParError(0)<<std::endl;
	      infotofile<< "JERffitdown param1 \t"<< lin2_fnorm->GetParameter(1)<<"\t"<<  lin2_fnorm->GetParError(1)<<std::endl;
	    }

	    if(j==0)lin_sigma->Draw("same");
	    if(j==0)leg2->AddEntry(lin_sigma,unc+" Slope "+TString::Format("%.3f", lin_sigma->GetParameter(1)),"l");
	    if(j==0)jj++;
	    if(jj==5)jj++;
	  }
	}

	can2->cd();
	leg2->Draw();
	//  can2->SaveAs(fname2+postfix+"_unc.eps");
	if(bunc)can2->SaveAs(fname2+postfix+"_unc.pdf");


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

	cout<<"Efficiencies with fit "<<endl;
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
	painter3->GetYaxis()->SetRangeUser(0, 0.008);
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

	TString fname3 = "results/signal_eff_values_" + channel_name+"_"+year+"_"+cat+"_"+MA; 

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

	fit2_err_95 = new TGraphErrors(MTs.size(), MTs.data(), alpha.data(), zeros.data(), alpha_err.data());  
	fitter = (TFitter*) TVirtualFitter::GetFitter();
	fitter->GetConfidenceIntervals(fit2_err_95, 0.95);
	fit2_err_95->SetFillColor(kOrange-4);
	fit2_err_95->Draw("L3 same");



	fit3_err_68 = new TGraphErrors(MTs.size(), MTs.data(), nvalues.data(), zeros.data(), nvalues_err.data());  
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

	fname3 = "results/signal_fnorm_values_" + channel_name+"_"+year+"_"+cat+"_"+MA; 

	can3_fnorm->RedrawAxis();
	//  can3_fnorm->SaveAs(fname3+postfix+ ".eps");
	//	can3_fnorm->SaveAs(fname3+postfix+ ".pdf");



	////// add uncertainties
	// for each uncertainties fit a function and draw it
	TLegend *leg3 = new TLegend(0.5,0.15,0.9,0.5,"","brNDC");
	leg3->SetBorderSize(0);
	leg3->SetFillStyle(0);
	leg3->SetTextSize(0.035);
	leg3->SetFillColor(0);
	leg3->SetLineColor(1);
	leg3->SetTextFont(42);

	jj=0;
	MTs = MTs_backup;// {600, 650, 800, 900, 1000, 1100,1200};

	// vector with eff up/dn values
	std::vector<double> effs_uperr2(MTs.size());  
	std::vector<double> effs_dnerr2(MTs.size());  
	for (int i=0; i<MTs.size(); ++i)
	  {
	    effs_uperr2[i] = 0;
	    effs_dnerr2[i] = 0;
	  }

	for(TString unc:uncertainties){
	  std::cout << "   ===============================    uncertainty        " << unc << std::endl;
	  //// loop over all uncertainties but mu-unc only in mu channel, e-unc only in e channel and all others in comb channel
	  if (ch==eEle && !unc.Contains("ele")) continue;
	  if (ch==eMuon && !unc.Contains("mu")) continue;
	  if (ch==eComb && (unc.Contains("ele") || unc.Contains("mu"))) continue;
	  if(year.Contains("2017")&&cat.Contains("ma90") && unc.Contains("PDF")){
	    MTs={600, 700,800,900,1100,1200};
	  }else{MTs=MTs_backup;}
	  means_unc.clear();
	  means_err_unc.clear();
	  widths_unc.clear();  
	  widths_err_unc.clear();  
	  effs_unc.clear();  
	  effs_err_unc.clear();  
	  nvalues_unc.clear();
	  nvalues_err_unc.clear();
	  alpha_unc.clear();  
	  alpha_err_unc.clear();  
	  fnorm_unc.clear();  
	  fnorm_err_unc.clear();  


	  // for each unc fit all signal mass points
	  for (int i=0; i<MTs.size(); ++i){
	    fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc, nvalues_unc, nvalues_err_unc, alpha_unc, alpha_err_unc, fnorm_unc, fnorm_err_unc, unc+"_up", year,cat,MA,func,2,2.5,true);
	  }
	  // generate TGraph with the eff for each up unc
	  TGraphErrors* geff_unc_up = new TGraphErrors(MTs.size(), MTs.data(), effs_unc.data(), zeros.data(), effs_err_unc.data());          

	  // clear all variable and do it again for down
	  means_unc.clear();
	  means_err_unc.clear();
	  widths_unc.clear();  
	  widths_err_unc.clear();  
	  effs_unc.clear();  
	  effs_err_unc.clear();      
	  nvalues_unc.clear();
	  nvalues_err_unc.clear();
	  alpha_unc.clear();  
	  alpha_err_unc.clear();  
	  fnorm_unc.clear();  
	  fnorm_err_unc.clear();      

	  // for each unc fit all signal mass points
	  for (int i=0; i<MTs.size(); ++i){
	    fitsignal(ch,  (int) MTs[i], means_unc, means_err_unc, widths_unc, widths_err_unc, effs_unc, effs_err_unc,nvalues_unc, nvalues_err_unc, alpha_unc, alpha_err_unc, fnorm_unc, fnorm_err_unc, unc+"_down", year,cat,MA,func,2,2.5,true);
	  }
	  
	  // generate TGraph with the eff for each dn unc
	  TGraphErrors* geff_unc_dn = new TGraphErrors(MTs.size(), MTs.data(), effs_unc.data(), zeros.data(), effs_err_unc.data());          

	  // sum up all uncertainties in quadrature to get the total uncertainty
	  for (int i=0; i<MTs.size(); ++i){
	    double x, y, yup, ydn; 
	    geff->GetPoint(i, x, y); // nominal eff
	    geff_unc_up->GetPoint(i, x, yup); // up variation
	    geff_unc_dn->GetPoint(i, x, ydn); // down variation
	    // if up variation is small than nominal switch it to down variation and vice versa
	    if (yup<y){
	      double tmp = yup;
	      yup = ydn;
	      ydn = tmp;
	    }
	    
	    // calculate relativ up and down error from actual points
	    double relup = fabs(y-yup)/y;
	    double reldn = fabs(y-ydn)/y;
	    // and add it in quadrature to the other unc for ech mass point, [i] is MT
	    effs_uperr2[i] += relup*relup;
	    effs_dnerr2[i] += reldn*reldn;
	  }

	  TF1* lin3 = new TF1("efffit"+TString::Format("%d",jj), "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
	  // TFitResultPtr r_unc = geff_unc_up->Fit(lin3, "0");
	  // lin3->SetLineColor(kBlue+jj);
	  // lin3->SetLineStyle(jj);
	  // can3->cd();
	  //	  lin3->Draw("same");

	  // Draw eff for each unc
	  TString uncname = unc + "_" + channel_name + "_" + year+"_"+cat+"_"+MA;
	  draw_eff_unc(geff, geff_unc_up, geff_unc_dn, uncname);


	  // leg3->AddEntry(lin3,unc+" Slope "+TString::Format("%f", lin3->GetParameter(1)),"l");
	  leg3->AddEntry(lin3,unc,"l");
	  // jj++;
	  // if (jj==5)jj++;
    
	}
	
	/// print the error for each MT mass point available in MC
	for (int i=0; i<MTs.size(); ++i)
	  {
	    cout << "MT = " << MTs[i] << " rel err up = " << sqrt(effs_uperr2[i]) << " rel err down = " << sqrt(effs_dnerr2[i]) << endl;
	  }

	cout<<"Draw efficiencies with fit and total uncertainties "<<endl;
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
	
	/// draw eff with sigma bands
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
	painter4->GetYaxis()->SetRangeUser(0, 0.08);
	if(cat.Contains("catma300") && MA.Contains("350"))	painter4->GetYaxis()->SetRangeUser(0, 0.1);
	if(cat.Contains("catma175") && MA.Contains("350"))	painter4->GetYaxis()->SetRangeUser(0, 0.01);
	if(cat.Contains("catma175") && MA.Contains("450"))	painter4->GetYaxis()->SetRangeUser(0, 0.01);
	if(cat.Contains("catma175") && MA.Contains("500"))	painter4->GetYaxis()->SetRangeUser(0, 0.01);
	// if(MA.Contains("999") && cat.Contains("chi2h") && year.Contains("2018")) painter4->GetYaxis()->SetRangeUser(0, 0.01);
	// if(MA.Contains("999") && cat.Contains("catma300")) painter4->GetYaxis()->SetRangeUser(0, 0.005);
	// if(MA.Contains("999") && cat.Contains("catma175")) painter4->GetYaxis()->SetRangeUser(0, 0.001);
	// if(MA.Contains("999") && cat.Contains("catma175")&& year.Contains("2018")) painter4->GetYaxis()->SetRangeUser(0, 0.0018);
	// if(MA.Contains("999") && cat.Contains("catma90")) painter4->GetYaxis()->SetRangeUser(0, 0.003);
	// if(MA.Contains("75")){
	//   if(cat.Contains("catma60")) painter4->GetYaxis()->SetRangeUser(0, 0.005);
	// }
	// if(MA.Contains("175")){
	//   if(cat.Contains("catma175")) painter4->GetYaxis()->SetRangeUser(0.005, 0.012);
	//   if(cat.Contains("catma300")) painter4->GetYaxis()->SetRangeUser(0., 0.01);
	//   if(cat.Contains("chi2h")) painter4->GetYaxis()->SetRangeUser(0., 0.004);
	// }
	// if(MA.Contains("450")){
	//   if(cat.Contains("catma300")) painter4->GetYaxis()->SetRangeUser(0., 0.03);
	//   if(year.Contains("2016") && cat.Contains("catma300")) painter4->GetYaxis()->SetRangeUser(0., 0.015);
	//   if(year.Contains("2016") && cat.Contains("catma175")) painter4->GetYaxis()->SetRangeUser(0., 0.003);
	//   if(year.Contains("2018") && cat.Contains("catma300")) painter4->GetYaxis()->SetRangeUser(0., 0.02);
	//   if(year.Contains("2018") && cat.Contains("catma175")) painter4->GetYaxis()->SetRangeUser(0., 0.003);
	// }
	if(year.Contains("allyears"))  painter4->GetYaxis()->SetRangeUser(0., 0.016);
	painter4->Draw();
	geff->SetMarkerStyle(20);
	geff->SetLineWidth(2);

	// fill two graphs: one with up- and one with down variation of 
	// total uncertainty, to determine parametric total uncertainty 
	TGraphErrors* gtot_up = (TGraphErrors*) geff_totunc->Clone();
	TGraphErrors* gtot_dn = (TGraphErrors*) geff_totunc->Clone();

	// fill graph with total uncertainty
	for (int i=0; i<MTs.size(); ++i)
	  {
	    double x, y; 
	    geff->GetPoint(i, x, y);
	    double y_err = geff->GetErrorY(i);
	    double y_err_rel = y_err / y; // stat. error

	    // symmetrize systematic uncertainty
	    double y_err_rel_sys = (sqrt(effs_uperr2[i]) + sqrt(effs_dnerr2[i]))/2. ;

	    // add stat and sys in quadrature
	    double tot_err_rel = sqrt( y_err_rel*y_err_rel + y_err_rel_sys*y_err_rel_sys );

	    if(year.Contains("2016")) tot_err_rel = sqrt(tot_err_rel * tot_err_rel + 0.02*0.02);
	    // print out stat and tot error for each mass point
	    cout << "MT = " << MTs[i] << " stat err = " << y_err_rel << " tot err = " << tot_err_rel << endl;

	    //if(tot_err_rel > 1) continue;
	    // Create a Graph containing the tot unc for each MC mass point but rejecting points where the fit did not work
	    geff_totunc->SetPointError(i, 0, tot_err_rel*y); // this graph contains the nominal value and we set the error to the total error here


	    double yup = y + geff_totunc->GetErrorY(i);
	    double ydn = y - geff_totunc->GetErrorY(i);

	    gtot_up->SetPoint(i, x, yup);
	    gtot_dn->SetPoint(i, x, ydn);

	    // set error of graph to stat only
	    gtot_up->SetPointError(i, 0, y_err);
	    gtot_dn->SetPointError(i, 0, y_err);

	  }  

	for (int i=MTs.size()-1; i>=0; --i)
          {
	    double y_err,x,y;
	    geff->GetPoint(i,x,y);
	    y_err = geff_totunc->GetErrorY(i);

	    if(y_err/y > 1 || isnan(y_err/y)){
	      //	      std::cout<<" MT:  "<< MTs[i] << "  y_err/y  "<<y_err/y <<std::endl;
	      double y_m1, x_m1;
	      geff->GetPoint(i-1,x_m1,y_m1);
	      double estimation = (geff_totunc->GetErrorY(i-1) / y_m1) * y;
	      cout<< "in estimation: "<<i<< "  estimation  "<<estimation<<endl;
	      if(i==0){
		double y_m1, x_m1;
		geff->GetPoint(i+1,x_m1,y_m1);
		cout<<"ym1 "<<y_m1 <<" tot unc  "<<geff_totunc->GetErrorY(i+1)<<"  y  "<<y<<endl;
		estimation = (geff_totunc->GetErrorY(i+1) / y_m1) * y;
		cout<< estimation <<endl;
	      }
	      TString error_mess = "Signal fit in cat. "+cat+" of MT=";
	      error_mess+=MTs[i];
	      error_mess+=" did not work and the efficiency is nan";
	      if (isnan(y)) throw runtime_error(error_mess);
	      double j=2;
	      while(isnan(estimation)){
		double y_mj, x_mj;
		geff->GetPoint(i-j,x_mj,y_mj);
		std::cout<<"in while loop "<< i-j<<" x_mj "<< x_mj<<"   y_mj  "<<y_mj<<std::endl;
		estimation = (geff_totunc->GetErrorY(i-j) / y_mj) * y;
		if(i==0){
		  geff->GetPoint(i+j,x_mj,y_mj);
		  estimation = (geff_totunc->GetErrorY(i+j) / y_mj) * y;
		}	
		j+=1;
	      }

	      //	      std::cout<<"estimation  "<< estimation <<std::endl;
	      geff_totunc->SetPointError(i,0,estimation);
	      gtot_up->SetPoint(i,x,y+estimation);
	      gtot_up->SetPointError(i,0,geff->GetErrorY(i)*2);
	      gtot_dn->SetPoint(i,x,y-estimation);
	      gtot_dn->SetPointError(i,0,geff->GetErrorY(i)*2);

	      // geff_totunc->RemovePoint(i);
	      // gtot_up->RemovePoint(i);
	      // gtot_dn->RemovePoint(i);
	    }
	    
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


	// fit the efficiency with statistical uncertainty
	TF1* lin_stat = new TF1("efffit_stat", "[0]+[1]*(x-600)+[2]*(x-600)*(x-600)", 550, 1250);
	lin_stat->SetParameter(0, effs[0]);
	lin_stat->SetParameter(1, 0);  
	// lin_stat->SetParName(0, "#varepsilon at 600 GeV");
	// lin_stat->SetParName(1, "Slope");
	TFitResultPtr r6 = geff->Fit(lin_stat, "0");

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
	cout<< "Eff pluts tot up and down variation" <<std::endl;
	for (int i=0; i<Npoints+1; ++i){
	  double x = MTmin + step*i;
	  double y = lin_stat->Eval(x);
	  double yup = eff_tot_up->Eval(x); 
	  double ydn = eff_tot_dn->Eval(x); 

	  double yerr_up = fabs(yup - y); 
	  double yerr_dn = fabs(y - ydn);
	  
	  double err = (yerr_up + yerr_dn)/2;

	  eff_err_tot->SetPoint(i, x, y);
	  eff_err_tot->SetPointEYhigh(i, err); 
	  eff_err_tot->SetPointEYlow(i, err); 

	  cout << "MT = " << x << " eff = " << y << " + " << yerr_up << " - " << yerr_dn << endl;
	}

	// now draw the statistical and total uncertainties
	eff_err_tot->SetFillColor(kOrange-4);
	eff_err_tot->SetLineColor(kOrange-4);
	eff_err_tot->Draw("L3 same");
  

	//	eff_err_tot->Draw("P same"); // see below
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
  
	// to see the up variation with its fit
	// eff_tot_up->SetLineColor(kBlue);
	// eff_tot_up->Draw("same");
	// gtot_up ->SetMarkerColor(kBlue);
	// gtot_up ->SetMarkerStyle(20);
	// gtot_up->Draw("P same");

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
	  double y = lin_stat->Eval(MT_unc); // nominal value from the fit to eff with stat. error only
	  double yup = eff_tot_up->Eval(MT_unc); // up variation from the fit to total up varied eff
	  double ydn = eff_tot_dn->Eval(MT_unc); // down variation from the fit to total up varied eff
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

	TString fname4 = "results/signal_eff_values_" + channel_name+"_"+year+"_"+cat+"_"+MA; 


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
	if(bunc)can4->SaveAs(fname4+postfix+"_unc.pdf");
      }
    }
  }
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
  if(bunc)  c->SaveAs(fname);

}

void fitsignal(Echannel channel, int MT, std::vector<double>& means, std::vector<double>& means_err, std::vector<double>& widths, std::vector<double>& widths_err, std::vector<double>& effs, std::vector<double>& effs_err,std::vector<double>& nvalues, std::vector<double>& nvalues_err, std::vector<double>& alpha, std::vector<double>& alpha_err, std::vector<double>& fnorm, std::vector<double>& fnorm_err,TString unc, TString year, TString cat, TString MA, TString func, float factormin, float factormax, bool write_Nevet)
{
  
  // set fit regions (crude estimate)
  double fit_xmin = MT-20-150.;
  double fit_xmax = MT-20+150.;  

 
  // a bit more precise 
  double mean = MT-20;
  double sigma = 61.5+0.06*(MT-600.);
  //  fit_xmin = mean - factormin*sigma;
  //  fit_xmax = mean + factormax*sigma;
  fit_xmax = 2000;
  fit_xmin = 300;


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
  TH1F* sigh = GetAnalysisOutputSignal(MT, channel,unc, year, cat,MA); 
  sigh->SetTitle("");
  if(cat.Contains("300")&&MA.Contains("9999")) sigh->Rebin(2);
  if(cat.Contains("175")&&MA.Contains("9999")) sigh->Rebin(2);
  //  if(cat.Contains("90")&&MA.Contains("9999")&&year.Contains("2016")) sigh->Rebin(2);

  std::cout<<"fit_xmax "<<fit_xmax<<std::endl;
  if(fit_xmin < sigh->GetBinLowEdge(sigh->FindFirstBinAbove(0))) fit_xmin = sigh->GetBinLowEdge(sigh->FindFirstBinAbove(0));

  if(fit_xmax > sigh->GetBinLowEdge(sigh->FindLastBinAbove(0)+1)) fit_xmax = sigh->GetBinLowEdge(sigh->FindLastBinAbove(0)+1);
  std::cout<<"fit_xmax 2 "<<fit_xmax<<std::endl;
  std::cout<<"last bin above 0 "<< sigh->FindLastBinAbove(0)<<std::endl;
  std::cout<<"Nbins of hist  "<< sigh->GetNbinsX()<<std::endl;

  if(fit_xmax > sigh->GetMean() + 3*sigh->GetRMS()) fit_xmax = sigh->GetMean() + 3*sigh->GetRMS();
  if(MA.Contains("200"))if(fit_xmin < sigh->GetMean() - 4*sigh->GetRMS()) fit_xmin = sigh->GetMean() - 4*sigh->GetRMS();
  std::cout<<"fit_xmax 3 "<<fit_xmax<<std::endl;

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
  signalfunction_doublegauss sigfuncobj_dg(xmin, xmax);
  signalfunction_cb sigfuncobj_cb(xmin, xmax);
  signalfunction_cb sigfuncobj_cb_param(xmin, xmax);
  signalfunction_cb sigfuncobj_lin(xmin, xmax);
  sigfuncobj.SetNorm(norm);    
  sigfuncobj_dg.SetNorm(norm);    
  sigfuncobj_cb.SetNorm(norm);    
  sigfuncobj_cb_param.SetNorm(norm);    
  sigfuncobj_lin.SetNorm(norm);    

  
  TF1* fitmodel_dg = new TF1("fitmodel_dg", sigfuncobj_dg, xmin, xmax, 5); //doublegauss
  TF1* fitmodel = new TF1("fitmodel", sigfuncobj, xmin, xmax, 2);//simple gauss
  TF1* fitmodel_cb = new TF1("fitmodel_cb", sigfuncobj_cb, xmin, xmax, 4); // cb
  TF1* fitmodel_lin = new TF1("fitmodel_lin", sigfuncobj_lin, xmin, xmax, 4); // lin
  fitmodel->SetParameter(0, mean);  
  fitmodel->SetParameter(1, sigma);

  fitmodel_cb->SetParameter(0, mean);  
  fitmodel_cb->SetParameter(1, sigma);
  fitmodel_cb->SetParameter(2, 1);  
  fitmodel_cb->SetParameter(3, 2);

  fitmodel_lin->SetParameter(0, mean);  
  fitmodel_lin->SetParameter(1, sigma);
  fitmodel_lin->SetParameter(2, 1);  
  //  fitmodel_lin->SetParameter(3, 0.2);
  fitmodel_lin->SetParameter(4, 0.2);


  fitmodel_dg->SetParameter(0, mean);  
  fitmodel_dg->SetParameter(1, sigma);
  fitmodel_dg->SetParameter(2,0.7*mean);
  fitmodel_dg->SetParLimits(2,xmin,mean-2*sigma);
  if(year.Contains("2016v3"))  fitmodel_dg->SetParLimits(2,xmin,mean-1.5*sigma);

  fitmodel_dg ->SetParameter(3,sigma);
  fitmodel_dg ->SetParameter(4,0.15);
  fitmodel_dg ->SetParLimits(4, 0,0.30);

  fitmodel_dg->SetNpx(200); 

  // if (cat.Contains("ma60")){
  //   fitmodel_cb->SetParameter(0, mean);  
  //   fitmodel_cb->SetParameter(1, sigma);
  //   fitmodel_cb->SetParameter(2, 2);  
  //   fitmodel_cb->SetParLimits(2, 0,100);  
  //   fitmodel_cb->SetParameter(3, 2);
  // }


  // if (cat.Contains("ma90")){
  //   fitmodel->SetParameter(0, 0.9*MT - 20);  
  //   fitmodel->SetParameter(1, 60);

  //   fitmodel_dg->SetParameter(0, 0.9*MT - 20);  
  //   fitmodel_dg->SetParameter(1, 60);
  //   fitmodel_dg->SetParameter(2, 0.7*MT - 80);
  //   fitmodel_dg->SetParameter(3, 100 + 0.1*MT);
  //   fitmodel_dg->SetParLimits(3,100,10000);
  //   fitmodel_dg->SetParameter(4, 0.15);
  //   fitmodel_dg->SetParLimits(4, 0.0,0.30);

  //   fitmodel_cb->SetParameter(0, mean);  
  //   fitmodel_cb->SetParameter(1, sigma);
  //   fitmodel_cb->SetParameter(2, 2);  
  //   fitmodel_cb->SetParLimits(2, 0,100);  
  //   fitmodel_cb->SetParameter(3, 2);
  //   if(year.Contains("2016")) fitmodel_cb->SetParLimits(3, 0,10);
  // }
  // if (cat.Contains("chi2h_2")){
  //   fitmodel->SetParameter(0, 0.9*MT - 20);  
  //   fitmodel->SetParameter(1, 60);

  //   fitmodel_dg->SetParameter(0, 0.9*MT - 20);  
  //   fitmodel_dg->SetParameter(1, 60);
  //   fitmodel_dg->SetParameter(2, 0.7*MT - 80);
  //   fitmodel_dg->SetParameter(3, 50 + 0.1*MT);
  //   fitmodel_dg->SetParLimits(3,50,10000);
  //   fitmodel_dg->SetParameter(4, 0.15);
  //   fitmodel_dg->SetParLimits(4, 0.0,0.30);

  //   fitmodel_cb->SetParameter(0, mean);  
  //   fitmodel_cb->SetParameter(1, sigma);
  //   fitmodel_cb->SetParameter(2, 2);  
  //   fitmodel_cb->SetParLimits(2, 0,100);  
  //   // fitmodel_cb->SetParameter(3, 2);

  //   if(MA.Contains("175")){
  //     fitmodel_cb->SetParameter(3, -2);
  //   }

  //   fitmodel_lin->SetParameter(0, mean);  
  //   fitmodel_lin->SetParameter(1, sigma);
  //   fitmodel_lin->SetParameter(2, 1);  
  //   fitmodel_lin->SetParLimits(2, -100,100);  
  //   fitmodel_lin->SetParameter(3, 0.2);
  //   // fitmodel_lin->SetParameter(4, 0.2);
  //   //    fitmodel_lin->SetParLimits(4,0,0.5);


  // }
  double nval = 1;
  double nvalmin = 0;
  double nvalmax = 100;
  double alphaval = 2;
  double alphavalmin = 0.5;
  double alphavalmax = 10;

  if (cat.Contains("ma175")){
    // fitmodel->SetParameter(0, MT + 40);  
    // fitmodel->SetParameter(1, 60);

    // fitmodel_dg->SetParameter(0, MT + 40);  
    // fitmodel_dg->SetParameter(1, 60);
    // fitmodel_dg->SetParameter(2, MT - 200);
    // fitmodel_dg->SetParameter(3, 60);
    // fitmodel_dg->SetParameter(4, 0.30);
    // fitmodel_dg ->SetParLimits(4, 0,0.50);

    nval = 1;
    alphaval = 2;
    alphavalmin = 0.5;
    alphavalmax = 10;
  }
  if (cat.Contains("ma300")){
    alphaval = 2;
    alphavalmin = 0.5;
    alphavalmax = 10;
  }
    fitmodel_cb->SetParameter(0, mean);  
    fitmodel_cb->SetParameter(1, sigma);
    fitmodel_cb->FixParameter(2, nval);  
    // fitmodel_cb->SetParameter(2, nval);  
    // fitmodel_cb->SetParLimits(2,nvalmin,nvalmax);  
    fitmodel_cb->SetParameter(3, alphaval);
    fitmodel_cb->SetParLimits(3,alphavalmin,alphavalmax);  




  Int_t linecol = kRed+2; 
  Int_t col68 = kOrange; 
  Int_t col95 = kOrange+7;
  // TString fdesc = "Guassian PDF, 2 pars";
  // TString ffile = TString::Format("gauss_MT%d", MT);

  TString fdesc = "CB PDF, 4 pars";
  TString ffile = TString::Format("cb_MT%d", MT);

  cout << "Norm in function = " << fitmodel->Integral(xmin, xmax, 1e-3) << endl;

  // some cosmetics
  fitmodel->SetLineColor(linecol);
  fitmodel->SetLineStyle(kSolid);
  fitmodel->SetLineWidth(2);

  fitmodel_dg->SetLineColor(linecol);
  fitmodel_dg->SetLineStyle(kSolid);
  fitmodel_dg->SetLineWidth(2);

  fitmodel_cb->SetLineColor(linecol);
  fitmodel_cb->SetLineStyle(kSolid);
  fitmodel_cb->SetLineWidth(2);

  fitmodel_lin->SetLineColor(linecol);
  fitmodel_lin->SetLineStyle(kSolid);
  fitmodel_lin->SetLineWidth(2);

  sigh->Draw();

  // FIT!
  //  TFitResultPtr r = sigh->Fit(fitmodel, "RS 0");
  TFitResultPtr r;
  if(func.Contains("simplegauss")) r = sigh->Fit(fitmodel, "RS");
  if(func.Contains("double")) r = sigh->Fit(fitmodel_dg, "RS");
  if(func.Contains("cb")) r = sigh->Fit(fitmodel_cb, "RS");
  if(func.Contains("lin")) r = sigh->Fit(fitmodel_lin, "RS");
  TMatrixDSym covmatr = r->GetCovarianceMatrix();
  TMatrixDSym rho = r->GetCorrelationMatrix();
  covmatr.Print();
  rho.Print();

  ////// If the fit is not good enough redo the fit but with shrinked x range
  if(func.Contains("cb") ){
    for(unsigned int i=0; i<10;i++){
      cout << "Checking chi2  "<< fitmodel_cb->GetChisquare()/ fitmodel_cb->GetNDF()<<endl;
      if ((fitmodel_cb->GetChisquare()/ fitmodel_cb->GetNDF())<1.5) break;
      if(cat.Contains("300") || cat.Contains("175")){
	if(i%2==0 ){
	  cout<<"defining new bin "<<endl;
	  unsigned int xbin = sigh->GetXaxis()->FindBin(xmin);
	  xbin+=1;
	  xmin = sigh->GetXaxis()->GetBinLowEdge(xbin);
	}else{
	  cout<<"defining new bin "<<endl;
	  unsigned int xbin = sigh->GetXaxis()->FindBin(xmax);
	  xbin-=1;
	  xmax = sigh->GetXaxis()->GetBinLowEdge(xbin);

	}
      } // TODO else case

      cout <<"new range is xmin = "<<xmin<<"  xmax = "<<xmax<<endl;
      norm = sigh->Integral(sigh->GetXaxis()->FindBin(xmin), sigh->GetXaxis()->FindBin(xmax), "width");
      cout<<"new norm is "<<norm<<endl;
      sigfuncobj_cb.set_xmin_xmax(xmin,xmax);
      sigfuncobj_cb.SetNorm(norm);
      cout<<"Norm of function"<<sigfuncobj_cb.GetNorm()<<endl;
      cout<<"after setting norm"<<endl;
      fitmodel_cb = new TF1("fitmodel_cb", sigfuncobj_cb, xmin, xmax, 4); // cb
      fitmodel_cb->SetParameter(0, mean);  
      fitmodel_cb->SetParameter(1, sigma);
      fitmodel_cb->FixParameter(2, nval);  
      //      fitmodel_cb->SetParameter(2, nval);  
      //      fitmodel_cb->SetParLimits(2,nvalmin,nvalmax);
      fitmodel_cb->SetParameter(3, alphaval);
      fitmodel_cb->SetParLimits(3,alphavalmin,alphavalmax);  

      cout<<"before fitting"<<endl;
      r = sigh->Fit(fitmodel_cb, "RS");
      
    }
  }


  //  c.SaveAs("test.eps");
  // efficiency calculation
  // TODO: - ist es null wenn die Funktion nicht definiert ist
  // - nicht bei bin 1 sonder wo der backgroundfit anfaengt
  double Nevents_err; 
  double Nevents = 0;
  for(int bin=1;bin < sigh->GetNbinsX()+1;bin++){
    double bin_cen = sigh->GetBinCenter(bin);
    double value = fitmodel->Eval(bin_cen);
    if(func.Contains("double")) value = fitmodel_dg->Eval(bin_cen);
    if(func.Contains("cb")) value = fitmodel_cb->Eval(bin_cen);
    if(func.Contains("lin")) value = fitmodel_lin->Eval(bin_cen);
    Nevents+=value;
  }


  double Ntot = sigh->Integral(sigh->GetXaxis()->GetXmin(), sigh->GetXaxis()->GetXmax());

  TString unc_name = unc;
  if (unc=="") unc_name = "nominal";
  if(write_Nevet) Nevttofile <<unc_name<<"  MT  "<<MT<< "  1-(Nevt/Ntot)  "<<1-(Nevents/Ntot)<< "  Ntot  "<< Ntot <<"  Nevt  "<<Nevents <<endl;
  cout << "\n" << "Total number of expected events (1pb?) = " << sigh->GetSumOfWeights() << " and within fit range: " << sigh->Integral(sigh->FindBin(xmin), sigh->FindBin(xmax)) << endl << endl;
  
  Nevents = sigh->IntegralAndError(sigh->GetXaxis()->FindBin(xmin), sigh->GetXaxis()->FindBin(xmax), Nevents_err);

  double efferr;
  double eff;
  if(func.Contains("simplegauss"))eff = CalcEff(fitmodel, Nevents, Nevents_err, Ntot, MT, efferr, year);
  if(func.Contains("double")) eff = CalcEff(fitmodel_dg, Nevents, Nevents_err, Ntot, MT, efferr, year);
  if(func.Contains("cb")) eff = CalcEff(fitmodel_cb, Nevents, Nevents_err, Ntot, MT, efferr, year);
  if(func.Contains("lin")) eff = CalcEff(fitmodel_lin, Nevents, Nevents_err, Ntot, MT, efferr, year);


  if(func.Contains("double")){
    means.push_back(fitmodel_dg->GetParameter(0));
    means_err.push_back(fitmodel_dg->GetParError(0));
    widths.push_back(fitmodel_dg->GetParameter(1));
    widths_err.push_back(fitmodel_dg->GetParError(1));
    effs.push_back(eff);
    effs_err.push_back(efferr);

    //second gaussian
    nvalues.push_back(fitmodel_dg->GetParameter(2));
    nvalues_err.push_back(fitmodel_dg->GetParError(2));
    alpha.push_back(fitmodel_dg->GetParameter(3));
    alpha_err.push_back(fitmodel_dg->GetParError(3));
    fnorm.push_back(fitmodel_dg->GetParameter(4));
    fnorm_err.push_back(fitmodel_dg->GetParError(4));
  }
  else if(func.Contains("cb")){
    means.push_back(fitmodel_cb->GetParameter(0));
    means_err.push_back(fitmodel_cb->GetParError(0));
    widths.push_back(fitmodel_cb->GetParameter(1));
    widths_err.push_back(fitmodel_cb->GetParError(1));
    effs.push_back(eff);
    effs_err.push_back(efferr);

    //second gaussian
    nvalues.push_back(fitmodel_cb->GetParameter(2));
    nvalues_err.push_back(fitmodel_cb->GetParError(2));
    alpha.push_back(fitmodel_cb->GetParameter(3));
    alpha_err.push_back(fitmodel_cb->GetParError(3));
  }
  else if(func.Contains("lin")){
    means.push_back(fitmodel_lin->GetParameter(0));
    means_err.push_back(fitmodel_lin->GetParError(0));
    widths.push_back(fitmodel_lin->GetParameter(1));
    widths_err.push_back(fitmodel_lin->GetParError(1));
    effs.push_back(eff);
    effs_err.push_back(efferr);

    //second gaussian
    nvalues.push_back(fitmodel_lin->GetParameter(2));
    nvalues_err.push_back(fitmodel_lin->GetParError(2));
    alpha.push_back(fitmodel_lin->GetParameter(3));
    alpha_err.push_back(fitmodel_lin->GetParError(3));
    // fnorm.push_back(fitmodel_lin->GetParameter(4));
    // fnorm_err.push_back(fitmodel_lin->GetParError(4));

  }
  else{
    // store the results
    means.push_back(fitmodel->GetParameter(0));
    means_err.push_back(fitmodel->GetParError(0));
    widths.push_back(fitmodel->GetParameter(1));
    widths_err.push_back(fitmodel->GetParError(1));
    effs.push_back(eff);
    effs_err.push_back(efferr);
  }
  // draw the result
  if(cat.Contains("300")&& MA.Contains("999"))sigh->GetYaxis()->SetRangeUser(0,30);
  if(cat.Contains("175")&& MA.Contains("999"))sigh->GetYaxis()->SetRangeUser(0,10);

  sigh->Draw("PZ");
  //gPad->SetLogy(true);


  // draw 95% CL
  TH1F* clhist;
  if(func.Contains("simplegauss")) clhist= ComputeHistWithCL(fitmodel, r, sigh, 0.95);
  if(func.Contains("double"))clhist = ComputeHistWithCL(fitmodel_dg, r, sigh, 0.95); 
  if(func.Contains("cb"))clhist = ComputeHistWithCL(fitmodel_cb, r, sigh, 0.95); 
  if(func.Contains("lin"))clhist = ComputeHistWithCL(fitmodel_lin, r, sigh, 0.95); 
  clhist->SetStats(kFALSE);
  clhist->SetLineColor(col95);
  clhist->SetFillColor(col95);
  clhist->SetMarkerColor(col95);
  clhist->Draw("e3 same");

  // draw 68% CL
  TH1F* clhist2;
  if(func.Contains("simplegauss"))clhist2= ComputeHistWithCL(fitmodel, r, sigh, 0.68);
  if(func.Contains("double")) clhist2 = ComputeHistWithCL(fitmodel_dg, r, sigh, 0.68);
  if(func.Contains("cb")) clhist2 = ComputeHistWithCL(fitmodel_cb, r, sigh, 0.68);
  if(func.Contains("lin")) clhist2 = ComputeHistWithCL(fitmodel_lin, r, sigh, 0.68);
  clhist2->SetStats(kFALSE);
  clhist2->SetLineColor(col68);
  clhist2->SetFillColor(col68);
  clhist2->SetMarkerColor(col68);
  clhist2->Draw("e3 same");

  //fitmodel->DrawClone("same");
  sigh->DrawClone("PZ same");
  if(func.Contains("double"))   fitmodel_dg->DrawClone("same");
  else if(func.Contains("cb"))   fitmodel_cb->DrawClone("same");
  else if(func.Contains("lin"))   fitmodel_lin->DrawClone("same");
  else  fitmodel->DrawClone("same");

  TF1* fitmodel_param = new TF1("fitmodel_param", sigfuncobj_cb_param, xmin, xmax, 4); // cb
  if(func.Contains("cb")){
    //draw parametrisation
    fitmodel_param->FixParameter(0,mean_param->Eval(MT));
    fitmodel_param->FixParameter(1,sigma_param->Eval(MT));
    fitmodel_param->SetParameter(2,n_param->Eval(MT));
    fitmodel_param->SetParLimits(2,0,n_param->Eval(MT)+1);
    if(cat.Contains("ma300"))    fitmodel_param->SetParLimits(2,-0.5,n_param->Eval(MT)+1);
    fitmodel_param->SetParameter(3,alpha_param->Eval(MT));
    fitmodel_param->SetParLimits(3,0,alpha_param->Eval(MT)+0.3);
    if(cat.Contains("ma300"))     fitmodel_param->SetParLimits(3,alpha_param->Eval(MT)-0.3,alpha_param->Eval(MT)+0.3);



    fitmodel_param->SetLineColor(kBlue);
    fitmodel_param->SetLineWidth(2);
    if(b_test_param) sigh->Fit(fitmodel_param,"RS");
    if(b_test_param) fitmodel_param->DrawClone("same");
  }

  // some information
  TString info, info2;
  info = TString::Format("Signal with M_{T} = %d GeV", MT);
  if(MA.Contains("9999"))   info += ", M_{H} = 125 GeV";
  else info += ", M_{a} = "+MA + " GeV";
  if (channel==eMuon){
    info2 = "#mu+jets";
  } else if (channel==eEle) {
    info2 = "e+jets";
  } else if (channel==eComb) {
    info2 = "l+jets";
  }
  TString mamass = "125";
  if(cat.Contains("catma")) {
    mamass = cat;
    mamass.ReplaceAll("catma", "");
  }
  info2 +=", Cat: M_{a} = "+mamass+" GeV";
      
  TString info4 = fdesc;
  TLatex* text = new TLatex();
  text->SetTextFont(42);
  text->SetNDC();
  text->SetTextColor(kBlack);
  text->SetTextSize(0.035);
  if(!year.Contains("allyears"))  text->DrawLatex(0.13, 0.96, info.Data());
  if(!year.Contains("allyears"))  text->DrawLatex(0.13, 0.92, info2.Data());
  //text->DrawLatex(0.13, 0.92, info3.Data());
  text->SetTextFont(42);
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
  if(func.Contains("double")) info8 = TString::Format("#chi^{2}/ndf = %.2f / %i", fitmodel_dg->GetChisquare(),fitmodel_dg->GetNDF() );
  if(func.Contains("cb")) info8 = TString::Format("#chi^{2}/ndf = %.2f / %i", fitmodel_cb->GetChisquare(),fitmodel_cb->GetNDF() );
  if(func.Contains("lin")) info8 = TString::Format("#chi^{2}/ndf = %.2f / %i", fitmodel_lin->GetChisquare(),fitmodel_lin->GetNDF() );
   text->DrawLatex(xvalue, 0.26-yoffset, info8.Data());  


  TString info9 = TString::Format("#mu = %.0f #pm %.0f GeV", fitmodel->GetParameter(0), fitmodel->GetParError(0) );
  TString info10 = TString::Format("#sigma = %.0f #pm %.0f GeV",  fitmodel->GetParameter(1), fitmodel->GetParError(1));


  if (func.Contains("double")){
    TString info92 = TString::Format("#mu_{2} = %.0f #pm %.0f GeV", fitmodel_dg->GetParameter(2), fitmodel_dg->GetParError(2) );
    text->DrawLatex(xvalue, 0.43-yoffset, info92.Data());  

    TString info102 = TString::Format("#sigma_{2} = %.0f #pm %.0f GeV",  fitmodel_dg->GetParameter(3), fitmodel_dg->GetParError(3));
    text->DrawLatex(xvalue, 0.37-yoffset, info102.Data());  

    TString info11 = TString::Format("f_{norm} = %.2f #pm %.2f",  fitmodel_dg->GetParameter(4), fitmodel_dg->GetParError(4));
    text->DrawLatex(xvalue, 0.33-yoffset, info11.Data());  
  }

  if (func.Contains("cb")){
    info9 = TString::Format("#mu = %.0f #pm %.0f GeV", fitmodel_cb->GetParameter(0), fitmodel_cb->GetParError(0) );
    info10 = TString::Format("#sigma = %.0f #pm %.0f GeV",  fitmodel_cb->GetParameter(1), fitmodel_cb->GetParError(1));


    TString info92 = TString::Format("n = %.2f #pm %.2f ", fitmodel_cb->GetParameter(2), fitmodel_cb->GetParError(2) );
    text->DrawLatex(xvalue, 0.43-yoffset, info92.Data());  

    TString info102 = TString::Format("#alpha = %.2f #pm %.2f ",  fitmodel_cb->GetParameter(3), fitmodel_cb->GetParError(3));
    text->DrawLatex(xvalue, 0.37-yoffset, info102.Data());  

  }

  if (func.Contains("lin")){
    TString info92 = TString::Format("a = %.2f #pm %.2f ", fitmodel_lin->GetParameter(2), fitmodel_lin->GetParError(2) );
    text->DrawLatex(xvalue, 0.43-yoffset, info92.Data());  

    TString info102 = TString::Format("b = %.2f #pm %.0f ",  fitmodel_lin->GetParameter(3), fitmodel_lin->GetParError(3));
    text->DrawLatex(xvalue, 0.37-yoffset, info102.Data());  

  }
  
  text->DrawLatex(xvalue, 0.53-yoffset, info9.Data());  
  text->DrawLatex(xvalue, 0.47-yoffset, info10.Data());  

   if(func.Contains("cb")){
     double xvalue2 = 0.2;
     TLatex* text_param = new TLatex();
     text_param->SetTextFont(42);
     text_param->SetNDC();
     text_param->SetTextColor(kBlack);
     text_param->SetTextSize(0.035);

     TString info9 = TString::Format("#chi^{2}/ndf = %.2f / %i", fitmodel_param->GetChisquare(),fitmodel_param->GetNDF() );
     text_param->SetTextColor(kBlue);
     if (b_test_param) text_param->DrawLatex(xvalue, 0.2-yoffset, info9.Data());  

     if(debug){
       info9 = TString::Format("#mu = %.0f #pm %.0f GeV", fitmodel_param->GetParameter(0), fitmodel_param->GetParError(0) );
       info10 = TString::Format("#sigma = %.0f #pm %.0f GeV",  fitmodel_param->GetParameter(1), fitmodel_param->GetParError(1));
       text_param->DrawLatex(xvalue2, 0.53-yoffset, info9.Data());  
       text_param->DrawLatex(xvalue2, 0.47-yoffset, info10.Data());  
      
       TString info92 = TString::Format("n = %.2f #pm %.2f ", fitmodel_param->GetParameter(2), fitmodel_param->GetParError(2) );
       text_param->DrawLatex(xvalue2, 0.43-yoffset, info92.Data());  
       
       TString info102 = TString::Format("#alpha = %.2f #pm %.0f ",  fitmodel_param->GetParameter(3), fitmodel_param->GetParError(3));
       text_param->DrawLatex(xvalue2, 0.37-yoffset, info102.Data());  
     }
   }


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

  //   if(unc=="" && !bunc)  can->Print(folder + "/signalfit_" + channel_name + "_" + ffile + + "_" +year+"_"+cat+"_"+MA+"_"+unc+".pdf");

  //  if(unc=="")   can->Print(folder + "/signalfit_" + channel_name + "_" + ffile + + "_" +year+"_"+cat+"_"+MA+"_"+unc+".pdf");
  can->Print(folder + "/signalfit_" + channel_name + "_" + ffile + + "_" +year+"_"+cat+"_"+MA+"_"+unc+".pdf");

  // save ratios and all infos
/*  
  std::vector<TH1F*> err_hists;
  err_hists.push_back(clhist);
  err_hists.push_back(clhist2);  
  plot_ratio(back, fitmodel, err_hists, region, channel, dodata, all_bkgds, fdesc, ffile);
*/

}

