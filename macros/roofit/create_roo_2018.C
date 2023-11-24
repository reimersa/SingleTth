#include "CreateRooWorkspace.h"

int main(int argc, char *argv[])
{
  using namespace std;

  bool dodata = true;
  bool all_bkgds = true;
  //  TString MA = "99999";
  //  TString MA = "125";
  TString MA = argv[1];

  TString limitvariable = argv[2];
  std::cout<<"limitvariable "<<limitvariable<<std::endl;


  std::vector<TString> categories = {"chi2h_2", "catma90","catma175","catma300"};
  if(MA=="75") categories = {"catma60", "catma90","catma300"};
  if(MA=="100") categories = {"catma90","catma300","chi2h_2"};
  if(MA=="125") categories = {"catma90","catma300","chi2h_2","catma175"};
  if(MA=="175") categories = {"chi2h_2","catma175","catma300"};
  if(MA=="200") categories = {"catma175","catma300","chi2h_2"};
  if(MA=="250") categories = {"catma175","catma300"};
  if(MA=="350") categories = {"catma175","catma300"};
  if(MA=="450") categories = {"catma175","catma300"};
  if(MA=="500") categories = {"catma175","catma300"};

  for(unsigned int icat = 0; icat < categories.size(); icat++){
    TString cat = categories[icat];
    std::cout<<"cat "<<cat<<std::endl;
    
    // create the workspace
    CreateRooWorkspace prod("2018",cat);
    std::cout<<"produced workshpace"<<std::endl;

    // muon channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eMuon, dodata, all_bkgds, "2018",cat,limitvariable);
    std::cout<<"save background Muon"<<std::endl;

    // electron channel: save data hist and bkg fit to the workspace
    prod.SaveDataAndBkgFunc(defs::eSR, defs::eEle, dodata, all_bkgds,"2018",cat,limitvariable);
    std::cout<<"save background Ele"<<std::endl;

    // now produce the signals
    prod.SaveSignals(defs::eMuon,"2018",cat,MA,limitvariable);
    std::cout<<"Save Signal Muon"<<std::endl;
    
    // now produce the signals
    prod.SaveSignals(defs::eEle,"2018",cat,MA,limitvariable);  
    std::cout<<"Save Signal Ele"<<std::endl;
  

    prod.PrintWorkspace();
    
    prod.StoreWorkspace("2018",cat,MA);
  }
	return 0;

}
