#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/SingleTth/include/SingleTthSelections.h"
#include "UHH2/SingleTth/include/SingleTthPreselectionHists.h"
#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include "UHH2/SingleTth/include/SingleTthModules.h"
#include <UHH2/SingleTth/include/ModuleBASE.h>
#include "UHH2/SingleTth/include/SingleTthHists.h"
#include "UHH2/SingleTth/include/SingleTthGenInfoHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class SingleTthGenInfoModule: public ModuleBASE {
  public:

    explicit SingleTthGenInfoModule(Context & ctx);
    virtual bool process(Event & event) override;
    void book_histograms(uhh2::Context&, vector<string>);
    void fill_histograms(uhh2::Event&, string);

  private:


    // store the Hists collection as member variables.
    unique_ptr<Hists> h_GenInfo;
    std::unique_ptr<AnalysisModule> printer;
  };



  SingleTthGenInfoModule::SingleTthGenInfoModule(Context & ctx){

    cout << "Hello from SingleTthGenInfoModule!" << endl;

    // Set up Printer:
  printer.reset(new GenParticlesPrinter(ctx));

    // Set up Hists classes:
  h_GenInfo.reset(new SingleTthGenInfoHists(ctx, "GenInfo"));
  }


  bool SingleTthGenInfoModule::process(Event & event) {

  cout << "Hello from SingleTthGenInfoModule process!" << endl;

  printer->process(event);

  h_GenInfo->fill(event);

    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(SingleTthGenInfoModule)

}
