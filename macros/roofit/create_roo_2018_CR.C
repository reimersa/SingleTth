#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;

  // create the workspace
  CreateRooWorkspace prod("2018");

  // muon channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eCR, defs::eMuon, dodata, all_bkgds, "2018");

  // electron channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eCR, defs::eEle, dodata, all_bkgds,"2018");

  //  std::cout<<"Hier"<<std::endl;
  // now produce the signals
  prod.SaveSignals(defs::eMuon,"2018");

  // now produce the signals
  prod.SaveSignals(defs::eEle,"2018");  
  //  std::cout<<"Hier2"<<std::endl;
  prod.PrintWorkspace();

  prod.StoreWorkspace("2018");

	return 0;

}
