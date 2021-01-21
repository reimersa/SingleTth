#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "TString.h"
#include "TFile.h"
#include <sys/stat.h>

#include "TSystem.h"

void LHE_NtupleWriter(TString infilename, TString outfilename);

int main(int argc, char *argv[])
{
  struct stat buffer;
  //  TString folder = "tH_b_topWdecay_LH_narrow_MT700_MH75/";
  std::vector<TString> folders {"VLQ_v4_5FNS_UFO_MT1100_MH300/","VLQ_v4_5FNS_UFO_MT1000_MH300/","VLQ_v4_5FNS_UFO_MT900_MH300/","VLQ_v4_5FNS_UFO_MT1200_MH300/","VLQ_v4_5FNS_UFO_MT800_MH300/","VLQ_v4_5FNS_UFO_MT700_MH300/","VLQ_v4_5FNS_UFO_MT600_MH300/","VLQ_v4_5FNS_UFO_MT700_MH25/","VLQ_v4_5FNS_UFO_MT700_MH250/","VLQ_v4_5FNS_UFO_MT700_MH75/","VLQ_v4_5FNS_UFO_MT700_MH50/","VLQ_v4_5FNS_UFO_MT700_MH500/","VLQ_v4_5FNS_UFO_MT700_MH100/","VLQ_v4_5FNS_UFO_MT700_MH125/","VLQ_v4_5FNS_UFO_MT700_MH150/","VLQ_v4_5FNS_UFO_MT700_MH200/","VLQ_v4_5FNS_UFO_MT700_MH300/","VLQ_v4_5FNS_UFO_MT700_MH400/"};

  for(int i=0;i<folders.size();i++){
    TString folder = folders.at(i);
    void *dir = gSystem->OpenDirectory("/nfs/dust/cms/user/reimersa/SingleTth/new_signals/lhe/LHEevents_VLQ_v4_5FNS_UFO/"+folder);

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
      in.Prepend("/nfs/dust/cms/user/reimersa/SingleTth/new_signals/lhe/LHEevents_VLQ_v4_5FNS_UFO/"+folder);
      if(stat("/nfs/dust/cms/user/reimersa/SingleTth/new_signals/ntuples/LHEevents_VLQ_v4_5FNS_UFO/"+folder,&buffer) !=0){
	mkdir("/nfs/dust/cms/user/reimersa/SingleTth/new_signals/ntuples/LHEevents_VLQ_v4_5FNS_UFO/"+folder,0777);
      }
      out.Prepend("/nfs/dust/cms/user/reimersa/SingleTth/new_signals/ntuples/LHEevents_VLQ_v4_5FNS_UFO/"+folder);
      std::cout << "in file = " << in << std::endl;
      std::cout << "out file = " << out << std::endl;
      
      LHE_NtupleWriter(in, out);
    }
  }
  
  exit(0);
  
}
