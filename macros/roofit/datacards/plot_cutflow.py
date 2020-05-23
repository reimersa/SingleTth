#!/usr/bin/env python
from __future__ import division
import subprocess
import glob
from os import system
import sys
from os import mkdir
from os.path import exists
from array import array
import math
import re
import time
import datetime
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")



infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_semileptonic_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_semileptonic_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_semileptonic_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_semileptonic_2018.root") 
TH1.AddDirectory(0)    


lista_pre = ["cleaner","lepton","jets","met"]
lista = ["triggeroffline","aftertrigger","triggerSF","trigger","btag_3m","reco_much_sr"]
lista_ele = ["triggeroffline_ele","aftertrigger_ele","triggerSF_ele","trigger_ele","btag_3m_ele","reco_ech_sr"]

histname = "sum_event_weights"

hist_cutflow_2016 = TH1F("cutflow_2016","cutflow_2016",10,0,10)
hist_cutflow_2017 = TH1F("cutflow_2017","cutflow_2017",10,0,10)
hist_cutflow_2018 = TH1F("cutflow_2018","cutflow_2018",10,0,10)

hist_cutflow_ele_2016 = TH1F("cutflow_ele_2016","cutflow_2016",10,0,10)
hist_cutflow_ele_2017 = TH1F("cutflow_ele_2017","cutflow_2017",10,0,10)
hist_cutflow_ele_2018 = TH1F("cutflow_ele_2018","cutflow_2018",10,0,10)


# for el in lista:
#     hist_cutflow.SetBinContent(el,0)

# hist_2016_nom = preinfile_2016.Get("cleaner/"+histname)
# hist_2017_nom = preinfile_2017.Get("cleaner/"+histname)
# hist_2018_nom = preinfile_2018.Get("cleaner/"+histname)

hist_2016_nom = infile_2016.Get("cleaner/"+histname)
hist_2017_nom = infile_2017.Get("cleaner/"+histname)
hist_2018_nom = infile_2018.Get("cleaner/"+histname)


int_2016_nom = hist_2016_nom.Integral()
int_2017_nom = hist_2017_nom.Integral()
int_2018_nom = hist_2018_nom.Integral()

int_2016_nom = (76738314 / 92260.16399) * 35900
int_2017_nom = (12852869549.5 / 35279949.78) * 41500
int_2018_nom = (30495602645.8 / 83703610.93) * 59600

for el in lista_pre:
    print el

    hist_2016 = preinfile_2016.Get(el+"/"+histname)
    hist_2017 = preinfile_2017.Get(el+"/"+histname)
    hist_2018 = preinfile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral() *(41500/35900)
    int_2018 = hist_2018.Integral()

    hist_cutflow_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_2018.Fill(el,int_2018/int_2018_nom)

    hist_cutflow_ele_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_ele_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_ele_2018.Fill(el,int_2018/int_2018_nom)


for el in lista:
    print el

    hist_2016 = infile_2016.Get(el+"/"+histname)
    hist_2017 = infile_2017.Get(el+"/"+histname)
    hist_2018 = infile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_2018.Fill(el,int_2018/int_2018_nom)

    if "reco" in el:
        print "++++++++++ ttbar eff"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)
    if "trigger" in el:
        print "++++++++++ TTbar  eff trigger"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)


for el in lista_ele:
    print el

    hist_2016 = infile_2016.Get(el+"/"+histname)
    hist_2017 = infile_2017.Get(el+"/"+histname)
    hist_2018 = infile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_ele_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_ele_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_ele_2018.Fill(el,int_2018/int_2018_nom)
    if "reco" in el:
        print "++++++++++ TTbar ele eff reco"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)
    if "trigger" in el:
        print "++++++++++ TTbar ele eff trigger"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)


c1 = TCanvas()
leg = TLegend(0.2,0.2,0.55,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
leg.SetHeader("t#bar{t}")

hist_cutflow_2016.SetLineColor(kRed)
hist_cutflow_2017.SetLineColor(kBlack)
hist_cutflow_2018.SetLineColor(kBlue)

hist_cutflow_2016.SetLineStyle(1)
hist_cutflow_2017.SetLineStyle(5)
hist_cutflow_2018.SetLineStyle(2)

hist_cutflow_2016.SetLineWidth(2)
hist_cutflow_2017.SetLineWidth(2)
hist_cutflow_2018.SetLineWidth(2)

hist_cutflow_2016.Draw("HIST same")
hist_cutflow_2017.Draw("HIST same")
hist_cutflow_2018.Draw("HIST same")

leg.AddEntry(hist_cutflow_2016,"2016 (inclusive)","l")
leg.AddEntry(hist_cutflow_2017,"2017 (l+jets)","l")
leg.AddEntry(hist_cutflow_2018,"2018 (l+jets)","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_ttbar_muchannel.eps")
c1.SaveAs("SF/cutflow_ttbar_muchannel.pdf")







###### print background composition
infile_2016_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_DY = infile_2016_DY.Get("reco_much_sr/"+histname).Integral()

infile_2016_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_Wjets = infile_2016_Wjets.Get("reco_much_sr/"+histname).Integral()

infile_2016_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_QCD = infile_2016_QCD.Get("reco_much_sr/"+histname).Integral()

infile_2016_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_TTV = infile_2016_TTV.Get("reco_much_sr/"+histname).Integral()

infile_2016_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_Diboson = infile_2016_Diboson.Get("reco_much_sr/"+histname).Integral()

infile_2016_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_SingleTop = infile_2016_SingleTop.Get("reco_much_sr/"+histname).Integral()

hist_2016_ttbar = infile_2016.Get("reco_much_sr/"+histname).Integral()

total_2016 = hist_2016_ttbar+hist_2016_SingleTop+hist_2016_Diboson+hist_2016_TTV+hist_2016_QCD+hist_2016_Wjets+hist_2016_DY

print "========== 2016"
print "ttbar:  %.2f"%(hist_2016_ttbar/total_2016)
print "DY:  %.2f"%(hist_2016_DY/total_2016)
print "Wjets:  %.2f"%(hist_2016_Wjets/total_2016)
print "QCD:  %.2f"%(hist_2016_QCD/total_2016)
print "TTV:  %.2f"%(hist_2016_TTV/total_2016)
print "Diboson:  %.2f"%(hist_2016_Diboson/total_2016)
print "SingleTop:  %.2f"%(hist_2016_SingleTop/total_2016)


infile_2017_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_DY = infile_2017_DY.Get("reco_much_sr/"+histname).Integral()

infile_2017_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_Wjets = infile_2017_Wjets.Get("reco_much_sr/"+histname).Integral()

infile_2017_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_QCD = infile_2017_QCD.Get("reco_much_sr/"+histname).Integral()

infile_2017_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_TTV = infile_2017_TTV.Get("reco_much_sr/"+histname).Integral()

infile_2017_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_Diboson = infile_2017_Diboson.Get("reco_much_sr/"+histname).Integral()

infile_2017_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_SingleTop = infile_2017_SingleTop.Get("reco_much_sr/"+histname).Integral()

hist_2017_ttbar = infile_2017.Get("reco_much_sr/"+histname).Integral()

total_2017 = hist_2017_ttbar+hist_2017_SingleTop+hist_2017_Diboson+hist_2017_TTV+hist_2017_QCD+hist_2017_Wjets+hist_2017_DY

print "========== 2017"
print "ttbar:  %.2f"%(hist_2017_ttbar/total_2017)
print "DY:  %.2f"%(hist_2017_DY/total_2017)
print "Wjets:  %.2f"%(hist_2017_Wjets/total_2017)
print "QCD:  %.2f"%(hist_2017_QCD/total_2017)
print "TTV:  %.2f"%(hist_2017_TTV/total_2017)
print "Diboson:  %.2f"%(hist_2017_Diboson/total_2017)
print "SingleTop:  %.2f"%(hist_2017_SingleTop/total_2017)

infile_2018_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2018.root") 
TH1.AddDirectory(0)    
hist_2018_DY = infile_2018_DY.Get("reco_much_sr/"+histname).Integral()

infile_2018_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2018.root") 
TH1.AddDirectory(0)    
hist_2018_Wjets = infile_2018_Wjets.Get("reco_much_sr/"+histname).Integral()

infile_2018_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2018.root") 
TH1.AddDirectory(0)    
hist_2018_QCD = infile_2018_QCD.Get("reco_much_sr/"+histname).Integral()

infile_2018_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2018.root") 
TH1.AddDirectory(0)    
hist_2018_TTV = infile_2018_TTV.Get("reco_much_sr/"+histname).Integral()

infile_2018_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2018.root") 
TH1.AddDirectory(0)    
hist_2018_Diboson = infile_2018_Diboson.Get("reco_much_sr/"+histname).Integral()

infile_2018_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2018.root") 
TH1.AddDirectory(0)    
hist_2018_SingleTop = infile_2018_SingleTop.Get("reco_much_sr/"+histname).Integral()

hist_2018_ttbar = infile_2018.Get("reco_much_sr/"+histname).Integral()

total_2018 = hist_2018_ttbar+hist_2018_SingleTop+hist_2018_Diboson+hist_2018_TTV+hist_2018_QCD+hist_2018_Wjets+hist_2018_DY

print "========== 2018"
print "ttbar:  %.2f"%(hist_2018_ttbar/total_2018)
print "DY:  %.2f"%(hist_2018_DY/total_2018)
print "Wjets:  %.2f"%(hist_2018_Wjets/total_2018)
print "QCD:  %.2f"%(hist_2018_QCD/total_2018)
print "TTV:  %.2f"%(hist_2018_TTV/total_2018)
print "Diboson:  %.2f"%(hist_2018_Diboson/total_2018)
print "SingleTop:  %.2f"%(hist_2018_SingleTop/total_2018)



print "============= data Muon"
infile_2016_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_data = infile_2016_data.Get("trigger/"+histname).Integral()
print "2016:  %.2f"%hist_2016_data

infile_2017_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_data = infile_2017_data.Get("trigger/"+histname).Integral()
print "2017:  %.2f"%hist_2017_data


infile_2018_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
TH1.AddDirectory(0)    
hist_2018_data = infile_2018_data.Get("trigger/"+histname).Integral()
print "2018:  %.2f"%hist_2018_data

print "2017 scaled:  %.2f"%(hist_2017_data*(35.9/41.5))
print "2018 scaled:  %.2f"%(hist_2018_data*(35.9/59.7))



print "============= data Electron"
infile_2016_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
TH1.AddDirectory(0)    
hist_2016_data = infile_2016_data.Get("trigger/"+histname).Integral()
print "2016:  %.2f"%hist_2016_data

infile_2017_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
TH1.AddDirectory(0)    
hist_2017_data = infile_2017_data.Get("trigger/"+histname).Integral()
print "2017:  %.2f"%hist_2017_data


infile_2018_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
TH1.AddDirectory(0)    
hist_2018_data = infile_2018_data.Get("trigger/"+histname).Integral()
print "2018:  %.2f"%hist_2018_data

print "2017 scaled:  %.2f"%(hist_2017_data*(35.9/41.5))
print "2018 scaled:  %.2f"%(hist_2018_data*(35.9/59.7))



####### data cut flow
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
TH1.AddDirectory(0)    


hist_cutflow_data_2016 = TH1F("cutflow_data_2016","cutflow_2016",10,0,10)
hist_cutflow_data_2017 = TH1F("cutflow_data_2017","cutflow_2017",10,0,10)
hist_cutflow_data_2018 = TH1F("cutflow_data_2018","cutflow_2018",10,0,10)
# for el in lista:
#     hist_cutflow.SetBinContent(el,0)

hist_2016_nom = preinfile_2016.Get("cleaner/"+histname)
hist_2017_nom = preinfile_2017.Get("cleaner/"+histname)
hist_2018_nom = preinfile_2018.Get("cleaner/"+histname)

# hist_2016_nom = infile_2016.Get("cleaner/"+histname)
# hist_2017_nom = infile_2017.Get("cleaner/"+histname)
# hist_2018_nom = infile_2018.Get("cleaner/"+histname)


int_2016_nom = hist_2016_nom.Integral()
int_2017_nom = hist_2017_nom.Integral()
int_2018_nom = hist_2018_nom.Integral()


for el in lista_pre:
    print el

    hist_2016 = preinfile_2016.Get(el+"/"+histname)
    hist_2017 = preinfile_2017.Get(el+"/"+histname)
    hist_2018 = preinfile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_data_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_data_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_data_2018.Fill(el,int_2018/int_2018_nom)


for el in lista:
    print el

    hist_2016 = infile_2016.Get(el+"/"+histname)
    hist_2017 = infile_2017.Get(el+"/"+histname)
    hist_2018 = infile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_data_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_data_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_data_2018.Fill(el,int_2018/int_2018_nom)
    if "reco" in el:
        print "++++++++++ Data eff reco"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)
    if "trigger" in el:
        print "++++++++++ Data eff trigger"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)


c1 = TCanvas()
leg = TLegend(0.6,0.6,0.95,0.9, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
leg.SetHeader("Data")

hist_cutflow_data_2016.SetLineColor(kRed)
hist_cutflow_data_2017.SetLineColor(kBlack)
hist_cutflow_data_2018.SetLineColor(kBlue)

hist_cutflow_data_2016.SetLineStyle(1)
hist_cutflow_data_2017.SetLineStyle(5)
hist_cutflow_data_2018.SetLineStyle(2)

hist_cutflow_data_2016.SetLineWidth(2)
hist_cutflow_data_2017.SetLineWidth(2)
hist_cutflow_data_2018.SetLineWidth(2)

hist_cutflow_data_2016.Draw("HIST same")
hist_cutflow_data_2017.Draw("HIST same")
hist_cutflow_data_2018.Draw("HIST same")

leg.AddEntry(hist_cutflow_data_2016,"2016","l")
leg.AddEntry(hist_cutflow_data_2017,"2017","l")
leg.AddEntry(hist_cutflow_data_2018,"2018","l")

leg.Draw()
c1.SetLogy()
c1.SaveAs("SF/cutflow_data_muchannel.eps")
c1.SaveAs("SF/cutflow_data_muchannel.pdf")

############# data electron channel
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
TH1.AddDirectory(0)    

hist_cutflow_ele_data_2016 = TH1F("cutflow_ele_data_2016","cutflow_2016",10,0,10)
hist_cutflow_ele_data_2017 = TH1F("cutflow_ele_data_2017","cutflow_2017",10,0,10)
hist_cutflow_ele_data_2018 = TH1F("cutflow_ele_data_2018","cutflow_2018",10,0,10)

hist_2016_nom = preinfile_2016.Get("cleaner/"+histname)
hist_2017_nom = preinfile_2017.Get("cleaner/"+histname)
hist_2018_nom = preinfile_2018.Get("cleaner/"+histname)

int_2016_nom = hist_2016_nom.Integral()
int_2017_nom = hist_2017_nom.Integral()
int_2018_nom = hist_2018_nom.Integral()


for el in lista_pre:
    print el

    hist_2016 = preinfile_2016.Get(el+"/"+histname)
    hist_2017 = preinfile_2017.Get(el+"/"+histname)
    hist_2018 = preinfile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_ele_data_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_ele_data_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_ele_data_2018.Fill(el,int_2018/int_2018_nom)


for el in lista_ele:
    print el

    hist_2016 = infile_2016.Get(el+"/"+histname)
    hist_2017 = infile_2017.Get(el+"/"+histname)
    hist_2018 = infile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_ele_data_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_ele_data_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_ele_data_2018.Fill(el,int_2018/int_2018_nom)
    if "reco" in el:
        print "++++++++++ Data ele eff reco"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)
    if "trigger" in el:
        print "++++++++++ Data ele eff trigger"
        print "2016:  %.4f"%(int_2016/int_2016_nom)
        print "2017:  %.4f"%(int_2017/int_2017_nom)
        print "2018:  %.4f"%(int_2018/int_2018_nom)


##################### signal cutflow
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_2018.root") 
TH1.AddDirectory(0)    


lista_pre = ["cleaner","lepton","jets","met"]
lista = ["trigger","btag_3m","reco_much_sr"]

histname = "sum_event_weights"

hist_cutflow_signal_2016 = TH1F("cutflow_signal_2016","cutflow_2016",10,0,10)
hist_cutflow_signal_2017 = TH1F("cutflow_signal_2017","cutflow_2017",10,0,10)
hist_cutflow_signal_2018 = TH1F("cutflow_signal_2018","cutflow_2018",10,0,10)

int_2016_nom = 35900
int_2017_nom = 41500
int_2018_nom = 59600

for el in lista_pre:
    print el

    hist_2016 = preinfile_2016.Get(el+"/"+histname)
    hist_2017 = preinfile_2017.Get(el+"/"+histname)
    hist_2018 = preinfile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral() *(41500/35900)
    int_2018 = hist_2018.Integral()

    hist_cutflow_signal_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_signal_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_signal_2018.Fill(el,int_2018/int_2018_nom)


for el in lista:
    print el

    hist_2016 = infile_2016.Get(el+"/"+histname)
    hist_2017 = infile_2017.Get(el+"/"+histname)
    hist_2018 = infile_2018.Get(el+"/"+histname)

    int_2016 = hist_2016.Integral()
    int_2017 = hist_2017.Integral()
    int_2018 = hist_2018.Integral()

    hist_cutflow_signal_2016.Fill(el,int_2016/int_2016_nom)
    hist_cutflow_signal_2017.Fill(el,int_2017/int_2017_nom)
    hist_cutflow_signal_2018.Fill(el,int_2018/int_2018_nom)


c1 = TCanvas()
leg = TLegend(0.2,0.2,0.55,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
leg.SetHeader("T #rightarrow Ht, M=900 GeV")

hist_cutflow_signal_2016.SetLineColor(kRed)
hist_cutflow_signal_2017.SetLineColor(kBlack)
hist_cutflow_signal_2018.SetLineColor(kBlue)

hist_cutflow_signal_2016.SetLineStyle(1)
hist_cutflow_signal_2017.SetLineStyle(5)
hist_cutflow_signal_2018.SetLineStyle(2)

hist_cutflow_signal_2016.SetLineWidth(2)
hist_cutflow_signal_2017.SetLineWidth(2)
hist_cutflow_signal_2018.SetLineWidth(2)

hist_cutflow_signal_2016.Draw("HIST same")
hist_cutflow_signal_2017.Draw("HIST same")
hist_cutflow_signal_2018.Draw("HIST same")

leg.AddEntry(hist_cutflow_signal_2016,"2016","l")
leg.AddEntry(hist_cutflow_signal_2017,"2017","l")
leg.AddEntry(hist_cutflow_signal_2018,"2018","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_signal_muchannel.eps")
c1.SaveAs("SF/cutflow_signal_muchannel.pdf")


c1 = TCanvas()
leg = TLegend(0.4,0.6,0.95,0.9, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);

hist_cutflow_signal_2016.SetLineColor(kBlack)

hist_cutflow_signal_2016.SetLineStyle(kDashed)

hist_cutflow_signal_2016.SetLineWidth(2)

hist_cutflow_signal_2016.Draw("HIST same")
hist_cutflow_2016.Draw("HIST same")

leg.AddEntry(hist_cutflow_signal_2016,"2016 (T #rightarrow Ht, M=900 GeV)","l")
leg.AddEntry(hist_cutflow_2016,"2016 (t#bar{t})","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_mixed_muchannel.eps")
c1.SaveAs("SF/cutflow_mixed_muchannel.pdf")


################# electron ttbar and data comparison for 2017 and 2018
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2017.SetLineColor(kRed)
hist_cutflow_2017.SetLineColor(kBlue)
hist_cutflow_ele_data_2017.SetLineColor(kOrange+2)
hist_cutflow_data_2017.GetYaxis().SetRangeUser(0.00001,1.5)

hist_cutflow_data_2017.Draw("HIST same")
hist_cutflow_2017.Draw("HIST same")
hist_cutflow_ele_data_2017.Draw("HIST same")
hist_cutflow_ele_2017.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2017,"2017 (data #mu)","l")
leg.AddEntry(hist_cutflow_2017,"2017 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2017,"2017 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2017,"2017 (t#bar{t}, e)","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu.eps")
c1.SaveAs("SF/cutflow_ele_mu.pdf")


################# electron ttbar and data comparison for 2017 and 2018
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2018.SetLineColor(kRed)
hist_cutflow_2018.SetLineColor(kBlue)
hist_cutflow_ele_data_2018.SetLineColor(kOrange+2)
hist_cutflow_data_2018.GetYaxis().SetRangeUser(0.00001,1.5)

hist_cutflow_data_2018.Draw("HIST same")
hist_cutflow_2018.Draw("HIST same")
hist_cutflow_ele_data_2018.Draw("HIST same")
hist_cutflow_ele_2018.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2018,"2018 (data #mu)","l")
leg.AddEntry(hist_cutflow_2018,"2018 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2018,"2018 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2018,"2018 (t#bar{t}, e)","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu_2018.eps")
c1.SaveAs("SF/cutflow_ele_mu_2018.pdf")


################# electron ttbar and data comparison for 2017 and 2016
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2016.SetLineColor(kRed)
hist_cutflow_2016.SetLineColor(kBlue)
hist_cutflow_ele_data_2016.SetLineColor(kOrange+2)
hist_cutflow_data_2016.GetYaxis().SetRangeUser(0.00001,1.5)

hist_cutflow_data_2016.SetLineStyle(2)
hist_cutflow_2016.SetLineStyle(2)


hist_cutflow_data_2016.Draw("HIST same")
hist_cutflow_2016.Draw("HIST same")
hist_cutflow_ele_data_2016.Draw("HIST same")
hist_cutflow_ele_2016.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2016,"2016 (data #mu)","l")
leg.AddEntry(hist_cutflow_2016,"2016 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2016,"2016 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2016,"2016 (t#bar{t}, e)","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu_2016.eps")
c1.SaveAs("SF/cutflow_ele_mu_2016.pdf")

################# electron/mu ttbar and data comparison for 2017 respectively
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2017_resp = hist_cutflow_data_2017.Clone()
hist_cutflow_2017_resp = hist_cutflow_2017.Clone()
hist_cutflow_ele_data_2017_resp = hist_cutflow_ele_data_2017.Clone()
hist_cutflow_ele_2017_resp  = hist_cutflow_ele_2017.Clone()

for bin in range(2,hist_cutflow_data_2017.GetNbinsX()):
    hist_cutflow_data_2017_resp.SetBinContent(bin,hist_cutflow_data_2017.GetBinContent(bin)/hist_cutflow_data_2017.GetBinContent(bin-1))
    hist_cutflow_2017_resp.SetBinContent(bin,hist_cutflow_2017.GetBinContent(bin)/hist_cutflow_2017.GetBinContent(bin-1))
    hist_cutflow_ele_data_2017_resp.SetBinContent(bin,hist_cutflow_ele_data_2017.GetBinContent(bin)/hist_cutflow_ele_data_2017.GetBinContent(bin-1))
    hist_cutflow_ele_2017_resp.SetBinContent(bin,hist_cutflow_ele_2017.GetBinContent(bin)/hist_cutflow_ele_2017.GetBinContent(bin-1))


hist_cutflow_data_2017_resp.GetYaxis().SetRangeUser(0.001,1.1)

hist_cutflow_data_2017_resp.Draw("HIST same")
hist_cutflow_2017_resp.Draw("HIST same")
hist_cutflow_ele_data_2017_resp.Draw("HIST same")
hist_cutflow_ele_2017_resp.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2017_resp,"2017 (data #mu)","l")
leg.AddEntry(hist_cutflow_2017_resp,"2017 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2017_resp,"2017 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2017_resp,"2017 (t#bar{t}, e)","l")

leg.Draw()

c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu_2017_resp.eps")
c1.SaveAs("SF/cutflow_ele_mu_2017_resp.pdf")


################# electron/mu ttbar and data comparison for 2018 respectively
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2018_resp = hist_cutflow_data_2018.Clone()
hist_cutflow_2018_resp = hist_cutflow_2018.Clone()
hist_cutflow_ele_data_2018_resp = hist_cutflow_ele_data_2018.Clone()
hist_cutflow_ele_2018_resp  = hist_cutflow_ele_2018.Clone()

for bin in range(2,hist_cutflow_data_2018.GetNbinsX()):
    hist_cutflow_data_2018_resp.SetBinContent(bin,hist_cutflow_data_2018.GetBinContent(bin)/hist_cutflow_data_2018.GetBinContent(bin-1))
    hist_cutflow_2018_resp.SetBinContent(bin,hist_cutflow_2018.GetBinContent(bin)/hist_cutflow_2018.GetBinContent(bin-1))
    hist_cutflow_ele_data_2018_resp.SetBinContent(bin,hist_cutflow_ele_data_2018.GetBinContent(bin)/hist_cutflow_ele_data_2018.GetBinContent(bin-1))
    hist_cutflow_ele_2018_resp.SetBinContent(bin,hist_cutflow_ele_2018.GetBinContent(bin)/hist_cutflow_ele_2018.GetBinContent(bin-1))


hist_cutflow_data_2018_resp.GetYaxis().SetRangeUser(0.,1.1)

hist_cutflow_data_2018_resp.Draw("HIST same")
hist_cutflow_2018_resp.Draw("HIST same")
hist_cutflow_ele_data_2018_resp.Draw("HIST same")
hist_cutflow_ele_2018_resp.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2018_resp,"2018 (data #mu)","l")
leg.AddEntry(hist_cutflow_2018_resp,"2018 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2018_resp,"2018 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2018_resp,"2018 (t#bar{t}, e)","l")

leg.Draw()

#c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu_2018_resp.eps")
c1.SaveAs("SF/cutflow_ele_mu_2018_resp.pdf")


################# electron/mu ttbar and data comparison for 2016 respectively
c1 = TCanvas()
leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


hist_cutflow_data_2016_resp = hist_cutflow_data_2016.Clone()
hist_cutflow_2016_resp = hist_cutflow_2016.Clone()
hist_cutflow_ele_data_2016_resp = hist_cutflow_ele_data_2016.Clone()
hist_cutflow_ele_2016_resp  = hist_cutflow_ele_2016.Clone()

for bin in range(2,hist_cutflow_data_2016.GetNbinsX()):
    hist_cutflow_data_2016_resp.SetBinContent(bin,hist_cutflow_data_2016.GetBinContent(bin)/hist_cutflow_data_2016.GetBinContent(bin-1))
    hist_cutflow_2016_resp.SetBinContent(bin,hist_cutflow_2016.GetBinContent(bin)/hist_cutflow_2016.GetBinContent(bin-1))
    hist_cutflow_ele_data_2016_resp.SetBinContent(bin,hist_cutflow_ele_data_2016.GetBinContent(bin)/hist_cutflow_ele_data_2016.GetBinContent(bin-1))
    hist_cutflow_ele_2016_resp.SetBinContent(bin,hist_cutflow_ele_2016.GetBinContent(bin)/hist_cutflow_ele_2016.GetBinContent(bin-1))


hist_cutflow_data_2016_resp.GetYaxis().SetRangeUser(0.,1.1)

hist_cutflow_data_2016_resp.Draw("HIST same")
hist_cutflow_2016_resp.Draw("HIST same")
hist_cutflow_ele_data_2016_resp.Draw("HIST same")
hist_cutflow_ele_2016_resp.Draw("HIST same")


leg.AddEntry(hist_cutflow_data_2016_resp,"2016 (data #mu)","l")
leg.AddEntry(hist_cutflow_2016_resp,"2016 (t#bar{t}, #mu)","l")

leg.AddEntry(hist_cutflow_ele_data_2016_resp,"2016 (data, e)","l")
leg.AddEntry(hist_cutflow_ele_2016_resp,"2016 (t#bar{t}, e)","l")

leg.Draw()

#c1.SetLogy()
c1.SaveAs("SF/cutflow_ele_mu_2016_resp.eps")
c1.SaveAs("SF/cutflow_ele_mu_2016_resp.pdf")
