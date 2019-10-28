#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <iostream>
#include <TStyle.h>
#include <TPaveText.h>
#include <THStack.h>
#include <TF1.h>
#include <TSystem.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLine.h>


void CreateSingleWorkspace(std::string histFolder = "btag_DeepBoosted_H4qvsQCD", std::string channel = "muonchannel", std::string collection = "Puppi", bool isHbb = false) {

  std::cout << "Start of CreateWorkspace: " << histFolder << '\n';

  std::string AnalysisDir = AnalysisPath+"Limits/";
  if (isHbb)  AnalysisDir = AnalysisPath+"Limits/Hbb/";
  std::string workingDir = AnalysisDir+"/"+collection+"/"+channel+"/"+histFolder+"/";
  gSystem->Exec(("mkdir -p "+workingDir+"/datacards").c_str());

  // std::string filepath = StoragePath+"Selection/"+collection+"/"+channel+"/";
  // if (isHbb)  filepath = StoragePath+"SignalRegion/"+collection+"/"+channel+"/";
  std::string filepath = StoragePath+"SignalRegion/"+collection+"/"+channel+"/";
  std::string dataFileName= "uhh2.AnalysisModuleRunner.DATA.DATA_SingleMuon_noTree.root";
  if (channel=="electronchannel") dataFileName= "uhh2.AnalysisModuleRunner.DATA.DATA_SingleElectron_noTree.root";
  std::string BgName = "MC_DY"; // MC_DY
  std::string bkgFileName= "uhh2.AnalysisModuleRunner.MC."+BgName+"_noTree.root";

  std::string mode = histFolder;

  std::string SRname = "ZprimeCandidate_"+histFolder+"_SR/Zprime_mass";
  std::string hist_dataname="ZprimeCandidate_"+histFolder+"_CR/Zprime_mass";
  std::string hist_neventsname="ZprimeCandidate_"+histFolder+"_SR/sum_event_weights";

  // std::string SRname = "ZprimeCandidate_TriggerCut/Zprime_mass";
  // std::string hist_dataname="ZprimeCandidate_ControlRegionTrigger";

  double plot_lo = 200;
  double plot_hi = 2500;
  double fit_lo = 400;
  double fit_hi = 2000;
  double rangeLo = 950;
  double rangeHi = 1100;


  std::ofstream DataCard;
  DataCard.open (workingDir+"datacards/AnalysisOutput2016_"+mode+".txt");

  RooWorkspace *ws=new RooWorkspace("ZprimeZH");

  /**************************************************
  *             PLOTTING VARIABLES                  *
  **************************************************/
  lumi_13TeV  = "35.9 fb^{-1}";
  bool debug = false;
  debug = true;
  bool doXsecPlot = false;
  bool doBkgPlots = true;
  bool doSignalPlots = true;
  bool doSignalExtraPlots = false;
  string extraPlotsName = "H_pt"; // H_mass;

  /*
  █  ██████ ██████   ██████  ███████ ███████       ███████ ███████  ██████ ████████ ██  ██████  ███    ██ ███████
  █ ██      ██   ██ ██    ██ ██      ██            ██      ██      ██         ██    ██ ██    ██ ████   ██ ██
  █ ██      ██████  ██    ██ ███████ ███████ █████ ███████ █████   ██         ██    ██ ██    ██ ██ ██  ██ ███████
  █ ██      ██   ██ ██    ██      ██      ██            ██ ██      ██         ██    ██ ██    ██ ██  ██ ██      ██
  █  ██████ ██   ██  ██████  ███████ ███████       ███████ ███████  ██████    ██    ██  ██████  ██   ████ ███████
  */



  std::vector<double> Mass      = {600      ,800      ,1000     ,1200     ,1400     ,1600     ,1800     ,2000     ,2500     ,3000     ,3500     ,4000     ,4500     ,5000     ,5500     ,6000};
  std::vector<double> mass_err  = {10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10       ,10  };
  std::vector<double> xsec      = {1.089e+04,3.415e+03,1.505e+03,7.783e+02,4.387e+02,2.620e+02,1.626e+02,1.038e+02,3.640e+01,1.371e+01,5.330e+00,2.133e+00,8.652e-01,3.604e-01,1.535e-01,6.639e-02};
  std::vector<double> xsec_err  = {6.314e+00,4.186e+01,9.512e-01,4.371e-01,2.517e-01,1.494e-01,9.351e-02,5.563e-02,2.002e-02,7.165e-03,2.522e-03,9.392e-04,4.011e-04,1.515e-04,6.865e-05,2.653e-05};

  if (doXsecPlot) {
    TCanvas* c_xsec = tdrCanvas("xsec", 0, 6500, 1e-3, 1e7, "m(Z')", "#sigma [pb]");
    c_xsec->SetLogy(1); c_xsec->SetTickx(0); c_xsec->SetTicky(0);
    TGraphErrors* gr_xsec = new TGraphErrors(Mass.size(), &(Mass[0]), &xsec[0], &(mass_err[0]), &xsec_err[0]);
    tdrDraw(gr_xsec,  "P5", kFullDotLarge, kGreen-2, kSolid, kGreen-2, 3005, kGreen-2);
  }

  TFile *f_data=new TFile((filepath+dataFileName).c_str());
  TFile *f_data2=new TFile((filepath+bkgFileName).c_str());
  TH1F *h_data=(TH1F*)f_data->Get((hist_dataname).c_str());
  double nEvents=((TH1F*)f_data->Get((hist_neventsname).c_str()))->GetSumOfWeights();
  // double nEventsSR=((TH1F*)f_data->Get(SRname.c_str()))->GetSumOfWeights(); //TODO fix input for SR
  double nEventsSR=((TH1F*)f_data2->Get(SRname.c_str()))->GetSumOfWeights();
  std::cout << "nEvents " << nEvents << '\n';
  std::cout << "nEvents " << h_data->GetSumOfWeights() << '\n'; // TODO fix the difference
  std::cout << "nEventsSR " << nEventsSR << '\n';

  // For the datacard
  DataCard << "=== RooFit data fit result to be entered in datacard === "<<std::endl;
  DataCard << BgName+" Background number of events = "<<nEventsSR<<std::endl;

  std::cout << filepath+dataFileName << " " << f_data << '\n';
  std::cout << filepath+bkgFileName << " " << f_data2 << '\n';
  std::cout << hist_dataname << " " << h_data << '\n';
  new TCanvas;
  h_data->Draw();

  /*
  █ ██████   █████   ██████ ██   ██  ██████  ██████   ██████  ██    ██ ███    ██ ██████      ███████ ██ ████████
  █ ██   ██ ██   ██ ██      ██  ██  ██       ██   ██ ██    ██ ██    ██ ████   ██ ██   ██     ██      ██    ██
  █ ██████  ███████ ██      █████   ██   ███ ██████  ██    ██ ██    ██ ██ ██  ██ ██   ██     █████   ██    ██
  █ ██   ██ ██   ██ ██      ██  ██  ██    ██ ██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██     ██      ██    ██
  █ ██████  ██   ██  ██████ ██   ██  ██████  ██   ██  ██████   ██████  ██   ████ ██████      ██      ██    ██
  */

  std::cout << "********************" << '\n';
  std::cout << "*  Background FIT  *" << '\n';
  std::cout << "********************" << '\n';


  RooRealVar* x = new RooRealVar("x", "m_{Zprime} (GeV)", plot_lo, plot_hi);
  RooDataHist* dataSR = new RooDataHist("dataSR", "dataSR", RooArgList(*x), h_data);
  RooPlot *plotter=x->frame();



  // RooPolynomial* bg = new RooPolynomial("bg","bg",*x, RooArgList(*bg_p1, *bg_p2,*bg_p3, *bg_p4, *bg_p5, *bg_p6));
  // RooPolynomial* bg_1 = new RooPolynomial("bg_1","bg_1",*x, RooArgList(*bg_p1_1, *bg_p2_1,*bg_p3_1, *bg_p4_1, *bg_p5_1));
  // RooPolynomial* bg_1 = new RooPolynomial("bg_1","bg_1",*x, RooArgList(*bg_p1_1));
  // RooBernstein* bg = new RooBernstein("bg","bg",*x, RooArgList(*bg_p1, *bg_p2,*bg_p3, *bg_p4, *bg_p5, *bg_p6));

  RooRealVar* bg_p1_1 = new RooRealVar("bg_p1_1", "bg_p1_1",  600,  700.);
  RooRealVar* bg_p2_1 = new RooRealVar("bg_p2_1", "bg_p2_1", -100,  100.);
  RooRealVar* bg_p3_1 = new RooRealVar("bg_p3_1", "bg_p3_1", -10,   10 );
  RooRealVar* bg_p4_1 = new RooRealVar("bg_p4_1", "bg_p4_1", -10,   10 );
  RooRealVar* bg_p5_1 = new RooRealVar("bg_p5_1", "bg_p5_1", -10,   10 );
  RooRealVar* bg_p6_1 = new RooRealVar("bg_p6_1", "bg_p6_1", -10,   10 );
  //
  // RooRealVar* bg_p1_1 = new RooRealVar("bg_p1_1", "bg_p1_1", 99000);
  // RooRealVar* bg_p2_1 = new RooRealVar("bg_p2_1", "bg_p2_1", -70);
  // RooRealVar* bg_p3_1 = new RooRealVar("bg_p3_1", "bg_p3_1", -1e-5);
  // RooRealVar* bg_p4_1 = new RooRealVar("bg_p4_1", "bg_p4_1", -1e-10);
  // RooRealVar* bg_p5_1 = new RooRealVar("bg_p5_1", "bg_p5_1", 1e-10);
  // RooRealVar* bg_p6_1 = new RooRealVar("bg_p6_1", "bg_p6_1", 0);

  RooBernstein* bg_1 = new RooBernstein("bg_1","bg_1",*x, RooArgList(*bg_p1_1, *bg_p2_1,*bg_p3_1, *bg_p4_1, *bg_p5_1, *bg_p6_1));
  RooFitResult *r_bg1=bg_1->fitTo(*dataSR, RooFit::Range(600, fit_hi), RooFit::Save(), RooFit::Verbose(kFALSE));

  RooRealVar* bg_p1 = new RooRealVar("bg_p1", "mean", +400, 100, 1000);
  RooRealVar* bg_p2 = new RooRealVar("bg_p2", "sigma", +10, 10, 500);
  RooRealVar* bg_p3 = new RooRealVar("bg_p3", "e0", +12, 0, 10);
  RooRealVar* bg_p4 = new RooRealVar("bg_p4", "e1", -1e-2, -1, 1);
  RooRealVar* bg_p5 = new RooRealVar("bg_p5", "e2", +1e-6, 0, 1);
  RooRealVar* bg_p6 = new RooRealVar("bg_p6", "er0", +800, 500, 2000);
  RooRealVar* bg_p7 = new RooRealVar("bg_p7", "er1", +1, 1, 100);

  RooLandau* bg = new RooLandau(BgName.c_str(),BgName.c_str(),*x, *bg_p1, *bg_p2);
  RooFitResult *r_bg=bg->fitTo(*dataSR, RooFit::Range(fit_lo, fit_hi), RooFit::Save(), RooFit::Verbose(kFALSE));

  // TODO try revCrystalball and FlatteFunction and Novosibirsk
  // http://pprc.qmul.ac.uk/~bevan/afit/afit.pdf
  LandauExpPDF* Modelbg_LandauExp = new LandauExpPDF(BgName.c_str(),BgName.c_str(),*x, *bg_p1, *bg_p2, *bg_p3,*bg_p4, *bg_p5,*bg_p6,*bg_p7);
  RooFitResult *r_bg_LandauExp=Modelbg_LandauExp->fitTo(*dataSR, RooFit::Range(fit_lo, 2000), RooFit::Save(), RooFit::Verbose(kFALSE));
  if (debug) {
    std::cout << "Testing BKG values postfit" << '\n';
    bg_p1->Print(); bg_p2->Print();
    bg_p3->Print(); bg_p4->Print(); bg_p5->Print();
    bg_p6->Print(); bg_p7->Print();
  }


  if (doBkgPlots) {
    TCanvas* c_bg = tdrCanvas("events", plot_lo, plot_hi, 1e-3, 1e5, "m(Z')", "events");
    c_bg->SetLogy(1);
    plotter=x->frame();
    dataSR->plotOn(plotter);
    bg->plotOn(plotter, RooFit::VisualizeError(*r_bg, 10), RooFit::FillColor(kGray+1), RooFit::FillStyle(3001));
    bg->plotOn(plotter, RooFit::LineColor(kRed+1));
    // bg_1->plotOn(plotter, RooFit::LineColor(kBlue+1));
    Modelbg_LandauExp->plotOn(plotter, RooFit::VisualizeError(*r_bg, 1), RooFit::FillColor(kGray+1), RooFit::FillStyle(3001));
    Modelbg_LandauExp->plotOn(plotter, RooFit::LineColor(kGreen+1));
    dataSR->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
    c_bg->cd();
    plotter->Draw();
    c_bg->SaveAs((workingDir+"Fit_Bg_LandauExp_"+mode+".pdf").c_str());

    // double fitChi2=plotter->chiSquare();
    // RooAbsReal* chi2_data    = Modelbg_LandauExp->createChi2(*dataSR);
    // double rebin = 1;
    // double pvalue=TMath::Prob(chi2_data->getVal(),int((fit_hi-fit_lo)/rebin)-3);
    // std::cout<<"p-value = "<< chi2_data->getVal() << " " << TMath::Prob(chi2_data->getVal(),int((fit_hi-fit_lo)/rebin)-3)<<std::endl;
    // std::cout<<"p-value2= "<< fitChi2<< " " << int((fit_hi-fit_lo)/rebin) << " " << TMath::Prob(fitChi2,7)<<std::endl;
    // TLegend *leg_bg = tdrLeg(0.50,0.70,0.9,0.9, 0.035);
    // tdrHeader(leg_bg, "Data in CR", 12, 0.04, 42, kBlack, true);
    // leg_bg->AddEntry(Modelbg_LandauExp, ("LandauExp p-value="+std::to_string(TMath::Prob(fitChi2,7))).c_str() ,"l");
  }

  /*
  █ ███████ ██  ██████  ███    ██  █████  ██          ███████ ██ ████████
  █ ██      ██ ██       ████   ██ ██   ██ ██          ██      ██    ██
  █ ███████ ██ ██   ███ ██ ██  ██ ███████ ██          █████   ██    ██
  █      ██ ██ ██    ██ ██  ██ ██ ██   ██ ██          ██      ██    ██
  █ ███████ ██  ██████  ██   ████ ██   ██ ███████     ██      ██    ██
  */


  std::cout << "****************" << '\n';
  std::cout << "*  SIGNAL FIT  *" << '\n';
  std::cout << "****************" << '\n';


  std::vector<int> masses = {600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500};
  std::map<int, int> colors = {{600, kRed}, {800, kGreen}, {1000, kViolet}, {1200, kBlue}, {1400, kBlack}, {1600, kOrange}, {1800, kAzure}, {2000, kSpring}, {2500, kPink}};

  TCanvas* c_ExtraPlot; TLegend *leg_ExtraPlot = tdrLeg(0.50,0.70,0.9,0.9, 0.035);

  if (doSignalExtraPlots) c_ExtraPlot = tdrCanvas("ExtraPlot", 0, (extraPlotsName=="H_mass")? 150: 2000, 1e-3, (extraPlotsName=="H_mass")? 100:1000, extraPlotsName.c_str(), "events");

  for (int & mass : masses) {
    double rangeLo = (double)mass-100;
    double rangeHi = (double)mass+100;
    std::cout << "fit mass: " << mass << " between " << rangeLo << " and " << rangeHi << '\n';
    double xSec = xsec[std::distance(Mass.begin(), std::find(Mass.begin(), Mass.end(), (int)mass))];

    std::string SgName = "M"+std::to_string(mass);
    std::string fnameSignal = "MC_ZprimeToZHToWW_"+SgName+"_2016v2";
    if (isHbb) fnameSignal = "MC_ZprimeToZHTobb_"+SgName+"_2016v2";

    TFile *f_signal=new TFile((filepath+"uhh2.AnalysisModuleRunner.MC."+fnameSignal+"_noTree.root").c_str());
    std::cout << f_signal->GetName() << '\n';
    TH1F *h_signal=(TH1F*)f_signal->Get(SRname.c_str());

    if (doSignalExtraPlots) {
      TH1F *h_ExtraPlot=(TH1F*)f_signal->Get(("ZprimeCandidate_PTMassCut/"+extraPlotsName).c_str());
      c_ExtraPlot->cd(); tdrDraw(h_ExtraPlot,  "same", kSolid, colors[mass], kSolid, colors[mass], 3005, colors[mass]);
      leg_ExtraPlot->AddEntry(h_ExtraPlot, SgName.c_str() ,"lep");
    }

    // h_signal->Rebin(mass>1400? 2 : 1);
    std::cout << "check " << h_signal->GetSumOfWeights() << "\t" << h_signal->Integral() << "\t" <<  h_signal->Integral(h_signal->FindBin(200),h_signal->FindBin(2000)) << "\t" << h_signal->Integral("width") << "\t" << '\n';

    double xsec_ref=0.1; // this is to mantain the signal strenght close to 1; (remember to multiply for this Normalization when plotting)
    h_signal->Scale(xsec_ref);

    RooRealVar* sg_p0 =new RooRealVar("sg_p0", "sg_p0", (rangeLo+rangeHi)/2, rangeLo, rangeHi);
    RooRealVar* sg_p1 =new RooRealVar("sg_p1", "sg_p1", 40,10., 110.);

    RooGaussian* ModelSg_Gauss = new RooGaussian(SgName.c_str(), "Signal Prediction", *x, *sg_p0, *sg_p1);
    RooDataHist* Sg_Hist = new RooDataHist(SgName.c_str(), SgName.c_str(), RooArgList(*x), h_signal);
    std::cout << "check2 " << Sg_Hist->sumEntries() << '\n';
    RooFitResult *r_sg=ModelSg_Gauss->fitTo(*Sg_Hist, RooFit::Range(rangeLo, rangeHi), RooFit::Save(), RooFit::Verbose(kFALSE));

    if (doSignalPlots) {
      TCanvas* c_sg = tdrCanvas(SgName.c_str(), 0, 3000, 1e-3, 100, "m(Z')", "events");
      plotter=x->frame();
      Sg_Hist->plotOn(plotter);
      ModelSg_Gauss->plotOn(plotter, RooFit::VisualizeError(*r_sg, 1), RooFit::FillColor(kGray+1), RooFit::FillStyle(3001));
      ModelSg_Gauss->plotOn(plotter, RooFit::LineColor(kRed));
      Sg_Hist->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
      plotter->Draw();
      c_sg->SaveAs((workingDir+"Fit_Sg"+SgName+"_"+mode+".pdf").c_str());
    }

    // ws->import(*(new RooHistPdf(SgName.c_str(),"",*x,*Sg_Hist))); //TODO decide weather to go for shaped analysis or not
    ws->import(*ModelSg_Gauss);

    //TODO fix the norm of signal
    // DataCard << SgName+" signal number of events = " << h_signal->GetSumOfWeights()*xsec_ref<<""<<std::endl;
    // DataCard << SgName+" signal number of events = " << h_signal->GetSumOfWeights()<<""<<std::endl;
    DataCard << SgName+" signal number of events = " << h_signal->Integral(h_signal->FindBin(rangeLo-100),h_signal->FindBin(rangeHi+100))<<""<<std::endl;
    // DataCard << SgName+" signal number of events = " << h_signal->Integral(h_signal->FindBin(rangeLo-100),h_signal->FindBin(rangeHi+100), "width")<<""<<std::endl;

  }



  /*
  █ ██████   █████  ████████  █████
  █ ██   ██ ██   ██    ██    ██   ██
  █ ██   ██ ███████    ██    ███████
  █ ██   ██ ██   ██    ██    ██   ██
  █ ██████  ██   ██    ██    ██   ██
  */


  // // Normalize h_mX_SB to SR for pretend data
  TH1F *h_data_fake=(TH1F*)h_data->Clone("h_data_fake");
  h_data_fake->Scale(nEventsSR/h_data_fake->GetSumOfWeights());
  RooDataHist* data_obs = new RooDataHist("data_obs", "Data", RooArgList(*x), h_data_fake);

  // new TCanvas;
  // plotter=x->frame();
  // data_obs->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
  // plotter->Draw();

  // new TCanvas;
  plotter=x->frame();
  // data_obs->plotOn(plotter, RooFit::LineColor(kBlack), RooFit::MarkerColor(kBlack));
  // plotter->Draw();


  /*
  █ ███████ ████████  ██████  ██████  ███████     ██     ██  ██████  ██████  ██   ██ ███████ ██████   █████   ██████ ███████
  █ ██         ██    ██    ██ ██   ██ ██          ██     ██ ██    ██ ██   ██ ██  ██  ██      ██   ██ ██   ██ ██      ██
  █ ███████    ██    ██    ██ ██████  █████       ██  █  ██ ██    ██ ██████  █████   ███████ ██████  ███████ ██      █████
  █      ██    ██    ██    ██ ██   ██ ██          ██ ███ ██ ██    ██ ██   ██ ██  ██       ██ ██      ██   ██ ██      ██
  █ ███████    ██     ██████  ██   ██ ███████      ███ ███   ██████  ██   ██ ██   ██ ███████ ██      ██   ██  ██████ ███████
  */



  // ws->import(*bg);
  ws->import(*Modelbg_LandauExp);
  // ws->import(bg_exp);
  ws->import(*data_obs);
  ws->Print();
  ws->writeToFile((workingDir+"/datacards/ws_"+mode+".root").c_str());


  // DataCard << "sg_p0   param   "<<sg_p0->getVal()<<" "<<sg_p0->getError()<<std::endl;
  // DataCard << "sg_p1   param   "<<sg_p1->getVal()<<" "<<sg_p1->getError()<<std::endl;

  DataCard << "bg_p1   param   "<<bg_p1->getVal()<<" "<<bg_p1->getError()<<std::endl;
  DataCard << "bg_p2   param   "<<bg_p2->getVal()<<" "<<bg_p2->getError()<<std::endl;
  // std::cout<< "bg_p3   param   "<<bg_p3->getVal()<<" "<<bg_p3->getError()<<std::endl;
  // std::cout<< "bg_p4   param   "<<bg_p4->getVal()<<" "<<bg_p4->getError()<<std::endl;
  // std::cout<< "bg_p5   param   "<<bg_p5->getVal()<<" "<<bg_p5->getError()<<std::endl;
  // std::cout<< "bg_p6   param   "<<bg_p6->getVal()<<" "<<bg_p6->getError()<<std::endl;
  //

  std::cout << "End of CreateWorkspace" << '\n';

  return true;
}
