#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "TSystem.h"

void make_all(int first=0, int last=100000)
{
  
  gSystem->Load("LHE_NtupleWriter_C.so");

  void *dir = gSystem->OpenDirectory("../files");

  for (int i=0; i<1000; ++i){
    const char* filename = gSystem->GetDirEntry(dir);
    if (!filename) break;

    TString in(filename);
    if (in.BeginsWith(".")) continue;
    if (!in.BeginsWith("ttbar")) continue;
    if (!in.Contains("flat")) continue;

    // now check if the file should be processed (i.e. is within [first, last])
    TObjArray* arr = in.Tokenize("_");
    TString fn = ((TObjString*)arr->At(arr->GetLast()))->GetString();
    fn.ReplaceAll(".lhe", "");
    Int_t fnumber = fn.Atoi();

    if ( !(fnumber >= first && fnumber <= last) ) continue;

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
