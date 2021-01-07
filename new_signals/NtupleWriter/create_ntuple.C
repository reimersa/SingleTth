#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "TString.h"
#include "TFile.h"

#include "TSystem.h"

void LHE_NtupleWriter(TString infilename, TString outfilename);

int main(int argc, char *argv[])
{

  void *dir = gSystem->OpenDirectory("../files");

  for (int i=0; i<1000; ++i){
    const char* filename = gSystem->GetDirEntry(dir);
    if (!filename) break;

    TString in(filename);
    if (in.BeginsWith(".")) continue;
    //if (!in.BeginsWith("ttbar")) continue;
    //if (!in.Contains("flat")) continue;

    // now check if the file should be processed (i.e. is within [first, last])
    /*
    TObjArray* arr = in.Tokenize("_");
    TString fn = ((TObjString*)arr->At(arr->GetLast()))->GetString();
    fn.ReplaceAll(".lhe", "");
    Int_t fnumber = fn.Atoi();

    if ( !(fnumber >= first && fnumber <= last) ) continue;
    */

    TString out(filename);
    out.ReplaceAll(".lhe", ".root");
    in.Prepend("../files/");
    out.Prepend("../ntuples/");
    std::cout << "in file = " << in << std::endl;
    std::cout << "out file = " << out << std::endl;

    LHE_NtupleWriter(in, out);
  }
  
  exit(0);
  
}
