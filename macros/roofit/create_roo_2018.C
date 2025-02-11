#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;
  //  TString MA = "99999";
  TString MA = "450";

  std::vector<TString> categories = {"chi2h_2", "catma90","catma175","catma300"};
  if(MA=="75") categories = {"catma60", "catma90","catma300"};
  if(MA=="175") categories = {"catma175","catma300","chi2h_2"};
  if(MA=="450") categories = {"catma175","catma300"};

  for(unsigned int icat = 0; icat < categories.size(); icat++){
    TString cat = categories[icat];

    // create the workspace
    CreateRooWorkspace prod("2018",cat);


    // muon channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eMuon, dodata, all_bkgds, "2018",cat);
    
    // electron channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eEle, dodata, all_bkgds,"2018",cat);

    // now produce the signals
    prod.SaveSignals(defs::eMuon,"2018",cat,MA);
    
    // now produce the signals
    prod.SaveSignals(defs::eEle,"2018",cat,MA);  
  

    prod.PrintWorkspace();
    
    prod.StoreWorkspace("2018",cat,MA);
  }
	return 0;

}
