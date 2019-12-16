#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/SingleTth/include/SingleTthSelections.h"
#include "UHH2/SingleTth/include/SingleTthPreselectionHists.h"
#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/AdditionalSelections.h"
#include "UHH2/SingleTth/include/SingleTthModules.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {

  class SingleTthPreselectionModule: public AnalysisModule {
  public:

    explicit SingleTthPreselectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    unique_ptr<CommonModules> common;

    // declare the Selections to use.
    unique_ptr<Selection> jet_sel, muon_sel_much, ele_sel_much, muon_sel_ech, ele_sel_ech, lumi_sel;

    // store the Hists collection as member variables.
    unique_ptr<Hists> h_nocuts, h_jets_nocuts, h_ele_nocuts, h_mu_nocuts, h_event_nocuts, h_topjets_nocuts, h_lumi_nocuts;
    unique_ptr<Hists> h_cleaner, h_jets_cleaner, h_ele_cleaner, h_mu_cleaner, h_event_cleaner, h_topjets_cleaner, h_lumi_cleaner;
    unique_ptr<Hists> h_lepton, h_jets_lepton, h_ele_lepton, h_mu_lepton, h_event_lepton, h_topjets_lepton, h_lumi_lepton;
    unique_ptr<Hists> h_electron, h_jets_electron, h_ele_electron, h_mu_electron, h_event_electron, h_topjets_electron, h_lumi_electron;
    unique_ptr<Hists> h_jets, h_jets_jets, h_ele_jets, h_mu_jets, h_event_jets, h_topjets_jets, h_lumi_jets;
    unique_ptr<Hists> h_met, h_jets_met, h_ele_met, h_mu_met, h_event_met, h_topjets_met, h_lumi_met;

    MuonId MuId;
    ElectronId EleId;
    JetId Jet_ID;

    bool is_mc;

  };


  SingleTthPreselectionModule::SingleTthPreselectionModule(Context & ctx){

    cout << "Hello from SingleTthPreselectionModule!" << endl;

    for(auto & kv : ctx.get_all()) cout << " " << kv.first << " = " << kv.second << endl;

    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    EleId = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(30.0, 2.4));
    Year year = extract_year(ctx);
    if (year == Year::is2016v2) MuId = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
    else                        MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonID(Muon::PFIsoTight));
    Jet_ID = AndId<Jet>(jet_pfid, PtEtaCut(30.0, 2.4));

    is_mc = ctx.get("dataset_type") == "MC";

    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true);
    common->set_electron_id(EleId);
    common->set_muon_id(MuId);
    common->set_jet_id(Jet_ID);
    common->switch_jetPtSorter();
    common->init(ctx);


    // 2. set up selections

    //Preselection
    muon_sel_much.reset(new NMuonSelection(1, 1));
    ele_sel_much.reset(new NElectronSelection(0, 0));
    muon_sel_ech.reset(new NMuonSelection(0, 0));
    ele_sel_ech.reset(new NElectronSelection(1, 1));
    jet_sel.reset(new NJetSelection(3, -1));
    lumi_sel.reset(new LumiSelection(ctx));

    // 3. Set up Hists classes:
    h_nocuts.reset(new SingleTthPreselectionHists(ctx, "nocuts"));
    h_jets_nocuts.reset(new JetHists(ctx, "Jets_nocuts"));
    h_ele_nocuts.reset(new ElectronHists(ctx, "Ele_nocuts"));
    h_mu_nocuts.reset(new MuonHists(ctx, "Mu_nocuts"));
    h_event_nocuts.reset(new EventHists(ctx, "Event_nocuts"));
    h_topjets_nocuts.reset(new TopJetHists(ctx, "Topjets_nocuts"));
    h_lumi_nocuts.reset(new LuminosityHists(ctx, "Lumi_nocuts"));

    h_cleaner.reset(new SingleTthPreselectionHists(ctx, "cleaner"));
    h_jets_cleaner.reset(new JetHists(ctx, "Jets_cleaner"));
    h_ele_cleaner.reset(new ElectronHists(ctx, "Ele_cleaner"));
    h_mu_cleaner.reset(new MuonHists(ctx, "Mu_cleaner"));
    h_event_cleaner.reset(new EventHists(ctx, "Event_cleaner"));
    h_topjets_cleaner.reset(new TopJetHists(ctx, "Topjets_cleaner"));
    h_lumi_cleaner.reset(new LuminosityHists(ctx, "Lumi_cleaner"));

    h_lepton.reset(new SingleTthPreselectionHists(ctx, "lepton"));
    h_jets_lepton.reset(new JetHists(ctx, "Jets_lepton"));
    h_ele_lepton.reset(new ElectronHists(ctx, "Ele_lepton"));
    h_mu_lepton.reset(new MuonHists(ctx, "Mu_lepton"));
    h_event_lepton.reset(new EventHists(ctx, "Event_lepton"));
    h_topjets_lepton.reset(new TopJetHists(ctx, "Topjets_lepton"));
    h_lumi_lepton.reset(new LuminosityHists(ctx, "Lumi_lepton"));

    h_jets.reset(new SingleTthPreselectionHists(ctx, "jets"));
    h_jets_jets.reset(new JetHists(ctx, "Jets_jets"));
    h_ele_jets.reset(new ElectronHists(ctx, "Ele_jets"));
    h_mu_jets.reset(new MuonHists(ctx, "Mu_jets"));
    h_event_jets.reset(new EventHists(ctx, "Event_jets"));
    h_topjets_jets.reset(new TopJetHists(ctx, "Topjets_jets"));
    h_lumi_jets.reset(new LuminosityHists(ctx, "Lumi_jets"));

    h_met.reset(new SingleTthPreselectionHists(ctx, "met"));
    h_jets_met.reset(new JetHists(ctx, "Jets_met"));
    h_ele_met.reset(new ElectronHists(ctx, "Ele_met"));
    h_mu_met.reset(new MuonHists(ctx, "Mu_met"));
    h_event_met.reset(new EventHists(ctx, "Event_met"));
    h_topjets_met.reset(new TopJetHists(ctx, "Topjets_met"));
    h_lumi_met.reset(new LuminosityHists(ctx, "Lumi_met"));



  }


  bool SingleTthPreselectionModule::process(Event & event) {

    h_nocuts->fill(event);
    h_jets_nocuts->fill(event);
    h_ele_nocuts->fill(event);
    h_mu_nocuts->fill(event);
    h_event_nocuts->fill(event);
    h_lumi_nocuts->fill(event);

    if(!lumi_sel->passes(event)) return false;
    bool pass_common = common->process(event);
    if(!pass_common) return false;

    h_cleaner->fill(event);
    h_jets_cleaner->fill(event);
    h_ele_cleaner->fill(event);
    h_mu_cleaner->fill(event);
    h_event_cleaner->fill(event);
    h_lumi_cleaner->fill(event);
    // lepton selection, consider both: e and mu channel
    if(!((muon_sel_much->passes(event) && ele_sel_much->passes(event)) || (muon_sel_ech->passes(event) && ele_sel_ech->passes(event)))) return false;
    h_lepton->fill(event);
    h_jets_lepton->fill(event);
    h_ele_lepton->fill(event);
    h_mu_lepton->fill(event);
    h_event_lepton->fill(event);
    h_lumi_lepton->fill(event);

    if(!jet_sel->passes(event)) return false;
    h_jets->fill(event);
    h_jets_jets->fill(event);
    h_ele_jets->fill(event);
    h_mu_jets->fill(event);
    h_event_jets->fill(event);
    h_lumi_jets->fill(event);


    if(event.met->pt() < 30) return false;
    h_met->fill(event);
    h_jets_met->fill(event);
    h_ele_met->fill(event);
    h_mu_met->fill(event);
    h_event_met->fill(event);
    h_lumi_met->fill(event);

    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(SingleTthPreselectionModule)

}
