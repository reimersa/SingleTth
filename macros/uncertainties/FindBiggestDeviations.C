#include <TString.h>
#include <TVirtualFitter.h>
#include <iostream>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <TFitResult.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
#include <sstream>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <string>
#include <TROOT.h>
#include <TRint.h>
#include <TClass.h>
#include <TKey.h>


using namespace std;

void FindBiggestDeviations(){

  //Files & histograms for other processes
  TString path = "/nfs/dust/cms/user/reimersa/SingleVLQStephanie/Fullselection/emuChannels/";
  TString doforsyst = "TT";
  TString process = "TTbar";
  unique_ptr<TFile> f_uu, f_un, f_nu, f_nd, f_dn, f_dd, f_nom;
  f_uu.reset (new TFile(path + "SCALE" + doforsyst + "_upup/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_un.reset (new TFile(path + "SCALE" + doforsyst + "_upnone/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_nu.reset (new TFile(path + "SCALE" + doforsyst + "_noneup/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_nd.reset (new TFile(path + "SCALE" + doforsyst + "_nonedown/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_dn.reset (new TFile(path + "SCALE" + doforsyst + "_downnone/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_dd.reset (new TFile(path + "SCALE" + doforsyst + "_downdown/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));
  f_nom.reset(new TFile(path + "NOMINAL/uhh2.AnalysisModuleRunner.MC." + process  + ".root","READ"));


  vector<TString> histfolders = {"chi2h_2", "dRbb_10", "dRbw_15"};
  vector<TString> channel_tags = {"ech", "much"};
  vector<TString> region_tags = {"sr", "cr"};


  unique_ptr<TFile> f_out_up, f_out_dn;
  f_out_up.reset(new TFile(path + "SCALE" + doforsyst + "_up/" + process  + ".root","RECREATE"));
  f_out_dn.reset(new TFile(path + "SCALE" + doforsyst + "_down/" + process  + ".root","RECREATE"));

  for(unsigned int i=0; i<channel_tags.size(); i++){
    for(unsigned int j=0; j<region_tags.size(); j++){
      for(unsigned int k=0; k<histfolders.size(); k++){

        TString histfolder = histfolders[k] + "_" + channel_tags[i] + "_" + region_tags[j];

        f_out_up->mkdir(histfolder);
        f_out_dn->mkdir(histfolder);

        unique_ptr<TH1D> h_uu, h_un, h_nu, h_nd, h_dn, h_dd, h_nom;
        TString histname = "";
        f_nom->cd(histfolder);
        TDirectory* dir = gDirectory;
        TIter iter(dir->GetListOfKeys());
        TKey *key;
        TH1D* h;
        while ((key = (TKey*)iter())) {
          TClass *cl = gROOT->GetClass(key->GetClassName());
          if (!cl->InheritsFrom("TH1")) continue;
          h = (TH1D*)key->ReadObj();
          histname = h->GetName();

          h_uu.reset((TH1D*)f_uu->Get(histfolder + "/" + histname));
          h_un.reset((TH1D*)f_un->Get(histfolder + "/" + histname));
          h_nu.reset((TH1D*)f_nu->Get(histfolder + "/" + histname));
          h_nd.reset((TH1D*)f_nd->Get(histfolder + "/" + histname));
          h_dn.reset((TH1D*)f_dn->Get(histfolder + "/" + histname));
          h_dd.reset((TH1D*)f_dd->Get(histfolder + "/" + histname));
          h_nom.reset((TH1D*)f_nom->Get(histfolder + "/" + histname));


          const int nbins = h_nom->GetNbinsX();
          vector<double> min_bins, max_bins, max_err, min_err;

          for(int i=1; i<h_nom->GetNbinsX()+1; i++){
            double entries[7] = {h_uu->GetBinContent(i),h_un->GetBinContent(i),h_nu->GetBinContent(i),h_nd->GetBinContent(i),h_dn->GetBinContent(i),h_dd->GetBinContent(i),h_nom->GetBinContent(i)};
            double errors[7] = {h_uu->GetBinError(i),h_un->GetBinError(i),h_nu->GetBinError(i),h_nd->GetBinError(i),h_dn->GetBinError(i),h_dd->GetBinError(i),h_nom->GetBinError(i)};
            double min = 9999999;
            double max_error = 0, min_error = 0;
            double max = 0;
            for(int j=0; j<7; j++){
              if(entries[j] > max) {max = entries[j]; max_error = errors[j];}
              if(entries[j] < min) {min = entries[j]; min_error = errors[j];}
            }

            min_bins.push_back(min);
            max_bins.push_back(max);
            min_err.push_back(min_error);
            max_err.push_back(max_error);
          }

          vector<double> bins_low;
          for(int i=1; i<h_nom->GetNbinsX()+2; i++){
            bins_low.push_back(h_nom->GetBinLowEdge(i));
          }

          unique_ptr<TH1D> hist_out_up, hist_out_dn;
          hist_out_up.reset(new TH1D(histname,"",  nbins, &bins_low[0]));
          for(int i=1; i<h_nom->GetNbinsX()+1; i++){
            hist_out_up->SetBinContent(i,max_bins[i-1]);
            hist_out_up->SetBinError(i, max_err[i-1]);
          }


          hist_out_dn.reset(new TH1D(histname,"",  nbins, &bins_low[0]));
          for(int i=1; i<h_nom->GetNbinsX()+1; i++){
            hist_out_dn->SetBinContent(i,min_bins[i-1]);
            hist_out_dn->SetBinError(i, min_err[i-1]);
          }


          cout << "histname: " << histname << endl;
          //Paths for other processes
          f_out_up->cd(histfolder);
          hist_out_up->Write();
          f_out_dn->cd(histfolder);
          hist_out_dn->Write();
        }
      }
    }
  }

  f_out_up->Close();
  f_out_dn->Close();

}
