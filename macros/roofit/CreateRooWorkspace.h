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

public: 
	CreateRooWorkspace();

	void StoreWorkspace();

	TH1F* GetAnalysisOutput(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds);

	void SaveDataAndBkgFunc(defs::Eregion region, defs::Echannel ch, bool dodata, bool all_bkgds);

	void SaveSignals(defs::Echannel ch);	

	void PrintWorkspace();

};


#endif
