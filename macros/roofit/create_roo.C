#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

	bool dodata = false;
	bool all_bkgds = true;

  // create the workspace
  CreateRooWorkspace prod;

  // muon channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eSR, defs::eMuon, dodata, all_bkgds);

  // electron channel: save data hist and bkg fit to the workspace
  prod.SaveDataAndBkgFunc(defs::eSR, defs::eEle, dodata, all_bkgds);

  //  std::cout<<"Hier"<<std::endl;
  // now produce the signals
  prod.SaveSignals(defs::eMuon);

  // now produce the signals
  prod.SaveSignals(defs::eEle);  
  //  std::cout<<"Hier2"<<std::endl;
  prod.PrintWorkspace();

  prod.StoreWorkspace();

	return 0;

}
