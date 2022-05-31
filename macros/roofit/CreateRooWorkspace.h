#ifndef CreateRooWorkspace_h
#define CreateRooWorkspace_h

#include <TH1F.h>
#include <TString.h>
#include <RooWorkspace.h>
#include <fstream>
#include <iostream>

namespace defs
{
  enum Eregion {eCR, eSR};
  enum Echannel {eMuon, eEle, eComb};
}

class CreateRooWorkspace {

private:
	RooWorkspace* fWS; 	
	TFile* debug_histos;
	std::ofstream infotofile;
	CreateRooWorkspace(const CreateRooWorkspace&);
	CreateRooWorkspace& operator=(const CreateRooWorkspace&);
	float xmin, xmax;

public: 
	CreateRooWorkspace(TString year = "2016v3", TString cat= "chi2h_2");

	void StoreWorkspace(TString year = "2016v3", TString cat = "chi2h_2", TString MA = "99999");

	TH1F* GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds, TString year = "2016v3", TString cat = "chi2h_2");

	void SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds, TString year = "2016v3", TString cat = "chi2h_2");

	void SaveSignals(defs::Echannel ch, TString year = "2016v3", TString cat = "chi2h_2", TString MA="99999");	

	void PrintWorkspace();
	std::vector<std::string> split (std::string s, std::string delimiter);
};


#endif
