#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;


  std::vector<TString> categories = {"chi2h_2"};
  for(unsigned int icat = 0; icat < categories.size(); icat++){
    TString cat = categories[icat];

    // create the workspace
    CreateRooWorkspace prod("2018",cat);

    // muon channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eCR, defs::eMuon, dodata, all_bkgds, "2018",cat);

    // electron channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eCR, defs::eEle, dodata, all_bkgds,"2018",cat);

    // now produce the signals
    prod.SaveSignals(defs::eMuon,"2018",cat);

    // now produce the signals
    prod.SaveSignals(defs::eEle,"2018",cat);  
  

    prod.PrintWorkspace();

    prod.StoreWorkspace("2018",cat);
  }
	return 0;

}
