#include <TString.h>
#include <iostream>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TROOT.h>
#include <TKey.h>
#include <TLatex.h>
#include <TClass.h>
#include <TFitResult.h>
#include <fstream>

using namespace std;

void FindRMS(TString infolder, map<TString, TString> samplemap, TString sample, TString year);

void PDFRMS(){

    TString year = "2016v3";
  //  TString year = "2017v2";
  //  TString year = "2018";


  TString prodch = "";
  TString year_lowmass = "";
  if(year == "2016v3") {
    year_lowmass = "2016v2";
    prodch = "B_";
  }
  else year_lowmass = year;
  TString hand = "LH";
  if(year == "2017v2" || year == "2018") hand="LH";


  std::vector<TString> mas = {};
  if(year == "2017v2")   mas = {"_ma75","_ma125","_ma175","_ma250","_ma350","_ma450"};

  map<TString, TString> samplemap;
  samplemap["TTbar_"+year] = "uhh2.AnalysisModuleRunner.MC.TTbar_"+year+".root";
  samplemap["WJets_"+year] = "uhh2.AnalysisModuleRunner.MC.WJets_"+year+".root";
  samplemap["Diboson_"+year] = "uhh2.AnalysisModuleRunner.MC.Diboson_"+year+".root";
  samplemap["TTV_"+year] = "uhh2.AnalysisModuleRunner.MC.TTV_"+year+".root";
  samplemap["DYJets_"+year] = "uhh2.AnalysisModuleRunner.MC.DYJets_"+year+".root";
  samplemap["SingleTop_"+year] = "uhh2.AnalysisModuleRunner.MC.SingleTop_"+year+".root";
  samplemap["VLQ_"+hand+"_600_"+year_lowmass] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_600_"+prodch+year_lowmass+".root";
  samplemap["VLQ_"+hand+"_650_"+year_lowmass] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_650_"+prodch+year_lowmass+".root";
  samplemap["VLQ_"+hand+"_700_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_700_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_800_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_800_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_900_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_900_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1000_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1000_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1100_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1100_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1200_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1200_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1300_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1300_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1400_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1400_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1500_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1500_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1600_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1600_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1700_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1700_"+prodch+year+".root";
  samplemap["VLQ_"+hand+"_1800_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_"+hand+"_1800_"+prodch+year+".root";


  TString infolder = "/nfs/dust/cms/user/reimersa/SingleTth/2016/Finalselection/mavariable/";
  if (year == "2017v2") infolder = "/nfs/dust/cms/user/reimersa/SingleTth/2017/Finalselection/mavariable/";
  if (year == "2018") infolder = "/nfs/dust/cms/user/reimersa/SingleTth/2018/Finalselection/mavariable/";

  FindRMS(infolder, samplemap, "TTbar_"+year,year);
  FindRMS(infolder, samplemap, "WJets_"+year,year);
  FindRMS(infolder, samplemap, "Diboson_"+year,year);
  FindRMS(infolder, samplemap, "TTV_"+year,year);
  FindRMS(infolder, samplemap, "DYJets_"+year,year);
  FindRMS(infolder, samplemap, "SingleTop_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_600_"+year_lowmass,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_650_"+year_lowmass,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_700_"+year,year);

  FindRMS(infolder, samplemap, "VLQ_"+hand+"_800_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_900_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1000_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1100_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1200_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1300_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1400_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1500_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1600_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1700_"+year,year);
  FindRMS(infolder, samplemap, "VLQ_"+hand+"_1800_"+year,year);

  for(unsigned int ima = 0; ima < mas.size(); ima++){
    TString ma = mas[ima];
    samplemap["VLQ_LH_700"+ma+"_"+year] = "uhh2.AnalysisModuleRunner.MC.VLQ_LH_700"+prodch+ma+"_"+year+".root";
    FindRMS(infolder, samplemap, "VLQ_LH_700"+ma+"_"+year,year);
  }
}


void FindRMS(TString infolder, map<TString, TString> samplemap, TString sample, TString year){
  gStyle->SetOptStat(0);
  // gErrorIgnoreLevel = 2002;

  // Open File
  // ==========

  TString infilename = infolder +"/NOMINAL/"+ samplemap[sample];
  //  if(year =="2017v2" || year == "2018") infilename = infolder +"/NOMINAL_NoBTagSF/"+ samplemap[sample];
  //  if(year == "2018") infilename = infolder +"/NOMINAL_NoBTagSF/"+ samplemap[sample];
  cout << "infilename: " << infilename << endl;
  TFile* infile = new TFile(infilename, "READ");

  // outfiles
  // TString outfilename_up = infolder + "/PDF_up/" + sample + ".root";
  // TString outfilename_down = infolder + "/PDF_down/" + sample + ".root";
  TString outfilename = infolder + "/pdf/" + sample + ".root";
  // cout << "outfilename_up: " << outfilename_up << endl;
  // cout << "outfilename_down: " << outfilename_down << endl;
  // TFile* outfile_up = new TFile(outfilename_up, "RECREATE");
  // TFile* outfile_down = new TFile(outfilename_down, "RECREATE");
  TFile* outfile = new TFile(outfilename, "RECREATE");




  // Loop through the entire file and subtract minor backgrounds from data in every single histogram in the "General"-folder (!)
  // ===========================================================================================================================

  // get list of foldernames: The PDF folders have "_PDF" in their name
  vector<TString> histfolders = {"chi2h_2","catma90","catma175","catma300"};
  vector<TString> channel_tags = {"ech", "much"};
  vector<TString> region_tags = {"sr", "cr"};

  infile->cd();
  vector<TString> foldernames;
  TDirectory* dir = gDirectory;
  TIter iter(dir->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)iter())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if(cl->InheritsFrom("TDirectoryFile")){
      TString name = key->ReadObj()->GetName();
      if(name != "SFrame" && name.Contains("_pdf")){
        foldernames.emplace_back(name);
        std::cout<<"name  "<<name<<std::endl;
        // make dirs without "_PDF"
        TString makedirname = name;
        // makedirname.ReplaceAll("_pdf", "_pdf_up");
        // outfile_up->mkdir(makedirname);
        // makedirname.ReplaceAll("_pdf_up", "_pdf_down");
        // outfile_down->mkdir(makedirname);
        makedirname.ReplaceAll("_pdf", "_pdf_up");
        outfile->mkdir(makedirname);
        makedirname.ReplaceAll("_pdf_up", "_pdf_down");
        outfile->mkdir(makedirname);
        // cout << "made dir: " << makedirname << endl;
      }
    }
  }


  for(size_t i=0; i<foldernames.size(); i++){
    TString foldername = foldernames.at(i);

    // create list of histogram names (without the _xx tag for the number of the PDF variation)
    infile->cd(foldername);
    dir = gDirectory;
    iter = dir->GetListOfKeys();

    vector<TString> histnames;
    // Loop over all objects in this dir
    while ((key = (TKey*)iter())) {
      TString myclass = "TH1F";

      // Consider only TH1F objects
      if(!(key->GetClassName() == myclass)) continue;
      TString histname = key->ReadObj()->GetName();
      if(histname.Contains("_99")){
        TString histname_base = histname;
        histname_base.ReplaceAll("_99", "");
        histnames.emplace_back(histname_base);
      }
    }

    // go through all PDF variations, there have to be ==100 of them per histogram
    vector<vector<TH1F*>> histograms;
    vector<TH1F*> histograms_nom;
    for(size_t j=0; j<histnames.size(); j++){

      TString histname = histnames[j];


      // systematic histograms (100)
      vector<TH1F*> hists_thisvar;
      for(size_t k=1; k<101; k++){
        TString readoutname = foldername + "/" + histname + "_";
        readoutname += k;
        // cout << "readoutname: " << readoutname << endl;
        TH1F* h = (TH1F*)infile->Get(readoutname);
        hists_thisvar.emplace_back(h);
      }
      histograms.emplace_back(hists_thisvar);


      // nominal histograms (1)
      // std::cout<<"foldername  "<<foldername<<std::endl;
      TString foldername_nom = foldername;
      foldername_nom.ReplaceAll("_pdf", "_nominal");
      // std::cout<<"histname  "<<histname<<std::endl;
      TString histname_nom = histname;
      histname_nom.ReplaceAll("_PDF", "");
      TString readoutname = foldername_nom + "/" + histname_nom;
      // cout << "readoutname: " << readoutname << endl;
      TH1F* h = (TH1F*)infile->Get(readoutname);
      histograms_nom.emplace_back(h);
    }


    // Now calculate RMS in current folder, in each variable, in each bin
    // ===============================================================

    vector<TH1F*> hists_up, hists_down;

    // loop through variables
    for(size_t j=0; j<histograms_nom.size(); j++){
      TH1F* h_up = (TH1F*)histograms_nom[j]->Clone();
      TH1F* h_down = (TH1F*)histograms_nom[j]->Clone();

      // loop through bins
      for(int k=1; k<histograms_nom[j]->GetNbinsX()+1; k++){

        float nom = histograms_nom[j]->GetBinContent(k);

        // loop through 100 PDF histograms
        float rms = 0.;
        for(size_t l=0; l<histograms[j].size(); l++){
          // if (sample == "VLQ_LH_700_2016v3" || sample == "VLQ_RH_650_2016v2") cout << "l: " << l << ", nom: " << nom << ", var: " << histograms[j][l]->GetBinContent(k) << endl;
	  if (sample.Contains("VLQ") && k==1) histograms[j][l]->Scale(histograms_nom[j]->Integral()/histograms[j][l]->Integral());
          rms += pow(histograms[j][l]->GetBinContent(k) - nom, 2);
        }
        rms /= histograms[j].size()-1.;
        rms = sqrt(rms);

        h_up->SetBinContent(k, nom + rms);
        h_down->SetBinContent(k, max((float)0., nom - rms));
        // cout << "value in bin " << k << ": " << nom << " +- " << rms << endl;
      }

      hists_up.emplace_back(h_up);
      hists_down.emplace_back(h_down);
    }


    // Write histograms into file, chosing the correct folder
    TString writefoldername = foldername;
    writefoldername.ReplaceAll("_pdf", "_pdf_up");
    // outfile_up->cd(writefoldername);
    outfile->cd(writefoldername);
    for(size_t j=0; j<hists_up.size(); j++){
      hists_up[j]->Write();
    }
    writefoldername.ReplaceAll("_pdf_up", "_pdf_down");
    // outfile_down->cd(writefoldername);
    outfile->cd(writefoldername);
    for(size_t j=0; j<hists_down.size(); j++){
      hists_down[j]->Write();
    }


  }
  // outfile_up->Close();
  // outfile_down->Close();
  outfile->Close();



  // delete outfile_up;
  // delete outfile_down;
  delete outfile;
  delete infile;

}
