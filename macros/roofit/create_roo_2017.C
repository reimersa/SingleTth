#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;

  // create the workspace
  CreateRooWorkspace prod("2017v2");

  // muon channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eSR, defs::eMuon, dodata, all_bkgds, "2017v2");

  // electron channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eSR, defs::eEle, dodata, all_bkgds,"2017v2");

  //  std::cout<<"Hier"<<std::endl;
  // now produce the signals
  prod.SaveSignals(defs::eMuon,"2017v2");

  // now produce the signals
  prod.SaveSignals(defs::eEle,"2017v2");  
  //  std::cout<<"Hier2"<<std::endl;
  prod.PrintWorkspace();

  prod.StoreWorkspace("2017v2");

	return 0;

}
