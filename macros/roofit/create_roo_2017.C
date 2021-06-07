#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;
  TString MA = "75";

  std::vector<TString> categories = {"chi2h_2", "catma90","catma175","catma300"};
  if(MA=="75") categories = {"catma60", "catma90","catma300"};
  //  std::vector<TString> categories = {"chi2h_2","catma90"};
  for(unsigned int icat = 0; icat < categories.size(); icat++){
    TString cat = categories[icat];

    // create the workspace
    CreateRooWorkspace prod("2017v2",cat);

    // muon channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eMuon, dodata, all_bkgds, "2017v2",cat);

    // electron channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eEle, dodata, all_bkgds,"2017v2",cat);

    // now produce the signals
    prod.SaveSignals(defs::eMuon,"2017v2",cat,MA);

    // now produce the signals
    prod.SaveSignals(defs::eEle,"2017v2",cat,MA);  
  

    prod.PrintWorkspace();

    prod.StoreWorkspace("2017v2",cat,MA);
  }
	return 0;

}
