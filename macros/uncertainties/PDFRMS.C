#include "../include/cosmetics.h"
#include "../include/Tools.h"
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

void FindRMS(TString puppidir, map<TString, TString> samplemap, TString sample);

void AnalysisTool::PDFRMS(){

  map<TString, TString> samplemap;
  samplemap["TTbar"] = "uhh2.AnalysisModuleRunner.MC.TTbar.root";
  samplemap["WJets"] = "uhh2.AnalysisModuleRunner.MC.WJets.root";
  samplemap["DYJets"] = "uhh2.AnalysisModuleRunner.MC.DYJets.root";
  samplemap["SingleTop"] = "uhh2.AnalysisModuleRunner.MC.ST.root";
  samplemap["QCD"] = "uhh2.AnalysisModuleRunner.MC.QCD_Mu.root";
  samplemap["RSGluon_M500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M500.root";
  samplemap["RSGluon_M750"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M750.root";
  samplemap["RSGluon_M1000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M1000.root";
  samplemap["RSGluon_M1250"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M1250.root";
  samplemap["RSGluon_M1500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M1500.root";
  samplemap["RSGluon_M2000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M2000.root";
  samplemap["RSGluon_M2500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M2500.root";
  samplemap["RSGluon_M3000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M3000.root";
  samplemap["RSGluon_M3500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M3500.root";
  samplemap["RSGluon_M4000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M4000.root";
  samplemap["RSGluon_M4500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M4500.root";
  samplemap["RSGluon_M5000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M5000.root";
  samplemap["RSGluon_M5500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M5500.root";
  samplemap["RSGluon_M6000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M6000.root";
  samplemap["RSGluon_M6500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M6500.root";
  samplemap["RSGluon_M7000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M7000.root";
  samplemap["RSGluon_M7500"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M7500.root";
  samplemap["RSGluon_M8000"] = "uhh2.AnalysisModuleRunner.MC.RSGluon_M8000.root";

  TString puppidir = AnalysisTool::dnn_path_puppi;

  FindRMS(puppidir, samplemap, "TTbar");
  FindRMS(puppidir, samplemap, "WJets");
  FindRMS(puppidir, samplemap, "DYJets");
  FindRMS(puppidir, samplemap, "SingleTop");
  FindRMS(puppidir, samplemap, "QCD");
  FindRMS(puppidir, samplemap, "RSGluon_M500");
  FindRMS(puppidir, samplemap, "RSGluon_M750");
  FindRMS(puppidir, samplemap, "RSGluon_M1000");
  FindRMS(puppidir, samplemap, "RSGluon_M1250");
  FindRMS(puppidir, samplemap, "RSGluon_M1500");
  FindRMS(puppidir, samplemap, "RSGluon_M2000");
  FindRMS(puppidir, samplemap, "RSGluon_M2500");
  FindRMS(puppidir, samplemap, "RSGluon_M3000");
  FindRMS(puppidir, samplemap, "RSGluon_M3500");
  FindRMS(puppidir, samplemap, "RSGluon_M4000");
  FindRMS(puppidir, samplemap, "RSGluon_M4500");
  FindRMS(puppidir, samplemap, "RSGluon_M5000");
  FindRMS(puppidir, samplemap, "RSGluon_M5500");
  FindRMS(puppidir, samplemap, "RSGluon_M6000");
  FindRMS(puppidir, samplemap, "RSGluon_M6500");
  FindRMS(puppidir, samplemap, "RSGluon_M7000");
  FindRMS(puppidir, samplemap, "RSGluon_M7500");
  FindRMS(puppidir, samplemap, "RSGluon_M8000");

}


void FindRMS(TString puppidir, map<TString, TString> samplemap, TString sample){
  gStyle->SetOptStat(0);
  // gErrorIgnoreLevel = 2002;

  // Open File
  // ==========

  TString infilename = puppidir + "/PDF/" + samplemap[sample];
  cout << "infilename: " << infilename << endl;
  TFile* infile = new TFile(infilename, "READ");

  // outfiles
  TString outfilename_up = puppidir + "/PDF_up/" + samplemap[sample];
  TString outfilename_down = puppidir + "/PDF_down/" + samplemap[sample];
  cout << "outfilename_up: " << outfilename_up << endl;
  cout << "outfilename_down: " << outfilename_down << endl;
  TFile* outfile_up = new TFile(outfilename_up, "RECREATE");
  TFile* outfile_down = new TFile(outfilename_down, "RECREATE");




  // Loop through the entire file and subtract minor backgrounds from data in every single histogram in the "General"-folder (!)
  // ===========================================================================================================================

  // get list of foldernames: The PDF folders have "_PDF" in their name
  infile->cd();
  vector<TString> foldernames;
  TDirectory* dir = gDirectory;
  TIter iter(dir->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)iter())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if(cl->InheritsFrom("TDirectoryFile")){
      TString name = key->ReadObj()->GetName();
      if(name != "SFrame" && name.Contains("_PDF")){
        foldernames.emplace_back(name);
        // make dirs without "_PDF"
        TString makedirname = name;
        makedirname.ReplaceAll("_PDF", "_General");
        outfile_up->mkdir(makedirname);
        outfile_down->mkdir(makedirname);
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
      for(size_t k=0; k<100; k++){
        TString readoutname = foldername + "/" + histname + "_";
        readoutname += k;
        // cout << "readoutname: " << readoutname << endl;
        TH1F* h = (TH1F*)infile->Get(readoutname);
        hists_thisvar.emplace_back(h);
      }
      histograms.emplace_back(hists_thisvar);


      // nominal histograms (1)
      TString foldername_nom = foldername;
      foldername_nom.ReplaceAll("_PDF", "_General");
      TString readoutname = foldername_nom + "/" + histname;
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
    writefoldername.ReplaceAll("_PDF", "_General");
    outfile_up->cd(writefoldername);
    for(size_t j=0; j<hists_up.size(); j++){
      hists_up[j]->Write();
    }
    outfile_down->cd(writefoldername);
    for(size_t j=0; j<hists_down.size(); j++){
      hists_down[j]->Write();
    }


  }
  outfile_up->Close();
  outfile_down->Close();



  delete outfile_up;
  delete outfile_down;
  delete infile;

}
