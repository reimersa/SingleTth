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


#lista_pre = ["cleaner","lepton","jets","met"]
lista_pre = ["lepton","jets","met"]
lista = ["triggeroffline","aftertrigger","triggerSF","trigger","btag_3m","reco_much_sr"]
lista_ele = ["triggeroffline_ele","aftertrigger_ele","triggerSF_ele","trigger_ele","btag_3m_ele","reco_ech_sr"]

histname = "sum_event_weights"


def create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, process = "ttbar", channel="muon", mode="eff"):
    lista = ["triggeroffline","trigger","btag_3m","reco_much_sr"]
    lista_ele = ["triggeroffline_ele","trigger_ele","btag_3m_ele","reco_ech_sr"]

    if "elec" in channel:
        lista = lista_ele
        print lista

    hist_cutflow_2016 = TH1F("cutflow_2016","cutflow_2016",7,0,7)
    hist_cutflow_2017 = TH1F("cutflow_2017","cutflow_2017",7,0,7)
    hist_cutflow_2018 = TH1F("cutflow_2018","cutflow_2018",7,0,7)

    hist_cutflow_ele_2016 = TH1F("cutflow_ele_2016","cutflow_2016",7,0,7)
    hist_cutflow_ele_2017 = TH1F("cutflow_ele_2017","cutflow_2017",7,0,7)
    hist_cutflow_ele_2018 = TH1F("cutflow_ele_2018","cutflow_2018",7,0,7)


    int_2016_nom = 0
    int_2017_nom = 0
    int_2018_nom = 0


    if "ttbar" in process:
        int_2016_nom = (76738314 / 92260.16399) * 35900
        int_2017_nom = (12852869549.5 / 35279949.78) * 41500
        int_2018_nom = (30495602645.8 / 83703610.93) * 59600

    if "WJets" in process:
        ####  Nevts / Lumi weight
        int_2016_nom = ((10020533/7597.068234)+(39449178/24239.87096)+(19069732/43814.59297)+(7759701/131117.8907)+(18687480/1281676.211)+(7830536/1176425.624)+(6872441/4273666.897)+(2637821/67786609.31)) * 35900
        int_2017_nom = ((9677900/7337.300986)+(32948958/24497.36654)+(18463505/45872.06211)+(14250114/246755.2208)+(21582309/1662735.67)+(11241373/2182790.874)+(39070465/39031433.57)+(20467964/2656452174)) * 41500
        int_2018_nom = ((28084244/21906.58658)+(29497106/21641.31034)+(25445174/61166.28365)+(5932701/100759.1882)+(19771294/1522039.569)+(8234601/1506237.607)+(7610000/6874435.411)+(3202556/411111168.2)) * 59600

    if "DY" in process:
        ####  Nevts / Lumi weight
        int_2016_nom = ((9691660/46376.6826)+(10977326/60547.18646)+(9589193/190194.9712)+(9725661/1392575.108)+(8253178/4908486.33)+(2673066/3447373.715)+(596079/3200905.371)+(399492/91105257.76)) * 35900
        int_2017_nom = ((11180126/61665.7621)+(10675441/211739.9445)+(10174800/1456885.368)+(8691608/5169237.723)+(3089712/3984709.669)+(616923/3312836.292)+(401334/91525330.96)) * 41500
        int_2018_nom = ((11530510/63598.36075)+(11225887/222657.6579)+(9428101/1349968.786)+(8862104/5270638.333)+(3114084/4016141.513)+(536416/2880518.95)+(427051/97390164.08)) * 59600


    if "SingleTop" in process:
        ####  Nevts / Lumi weight
        int_2016_nom = ((64996300/477843.6995)+(38619669/477080.5312)+(30047536.3612/2911582.981)+(33033621.7277/9818810.853)+(6833550/190615.0628)+(6789494/189386.1646)) * 35900
        int_2017_nom = ((706283792.622/5192499.578)+(274098646.803/3386024.049)+(172980138.73/8837195.004)+(196773098.023/10052727.74)+(9883805/2955966.181)) * 41500
        int_2018_nom = ((241533624.285/12339449.8)+(304349712.982/15548592.94)+(326172585.614/4029309.273)+(815051128.772/5992141.808)+(19700000/5891712.126)+(9652000/1383537.452)) * 59600

    if "Diboson" in process:
        ####  Nevts / Lumi weight
        int_2016_nom = ((1999000/164148.4644)+(1998400/38636.58334)+(8654600/173102.3861)+(497702/46470.77498)+(234018996.749/41826451.61)+(9357734.256/3085306.382)+(181249856.86/28696937.44)+(19993200.00/4513494.294)+(57586850.00/102104344.0)+(496436/154172.6708)+(489955/121275.9901)+(103121112/82102796.18)+(340455185.425/42050868.54)) * 35900
        int_2017_nom = ((398732037.701/7975119.261)+(269158746.179/48107014.51)+(22048867.9872/1810549.186)+(28758493.7045/9481864.063)+(4548138.32956/1026748.915)+(9176336.78475/7306000.625)+(156973439.711/48749515.44)+(5254422.09293/9316351.229)) * 41500
        int_2018_nom = ((7850000/103452.8202)+(3885000/141067.5381)+(1979000/162746.7105)) * 59600

    if "TTV" in process:
        ####  Nevts / Lumi weight
        int_2016_nom = ((1473733.62503/5827337.386)+(502198.857161/1985760.606)+(1786575.571/8744863.294)+(569424.146326/1401831.97)) * 35900
        int_2017_nom = ((7269266.098/1838397.39627)+(377559.451904/929491.5113)+(1687666.07841/8260724.809)) * 41500
        int_2018_nom = ((4911941/23115016.47)+(835296/1917135.644)+(750000/1469435.737)+(3220911.09063/12735907.83)) * 59600



    if "VLQ" in process:
        ####  Nevts / Lumi weight
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

        if "eff" in mode:
            hist_cutflow_2016.Fill(el,int_2016/int_2016_nom)
            hist_cutflow_2017.Fill(el,int_2017/int_2017_nom)
            hist_cutflow_2018.Fill(el,int_2018/int_2018_nom)
            
            hist_cutflow_ele_2016.Fill(el,int_2016/int_2016_nom)
            hist_cutflow_ele_2017.Fill(el,int_2017/int_2017_nom)
            hist_cutflow_ele_2018.Fill(el,int_2018/int_2018_nom)
            
        elif "raw" in  mode or "cut" in mode:
            hist_cutflow_2016.Fill(el,int_2016)
            hist_cutflow_2017.Fill(el,int_2017)
            hist_cutflow_2018.Fill(el,int_2018)
            
            hist_cutflow_ele_2016.Fill(el,int_2016)
            hist_cutflow_ele_2017.Fill(el,int_2017)
            hist_cutflow_ele_2018.Fill(el,int_2018)



    for el in lista:
        print el

        name=""
        if "triggeroffline" in el:
            name = "lepton $p_{T}$"
        else:
            name = el

        hist_2016 = infile_2016.Get(el+"/"+histname)
        hist_2017 = infile_2017.Get(el+"/"+histname)
        hist_2018 = infile_2018.Get(el+"/"+histname)

        int_2016 = hist_2016.Integral()
        int_2017 = hist_2017.Integral()
        int_2018 = hist_2018.Integral()

        if "eff" in mode:
            hist_cutflow_2016.Fill(name,int_2016/int_2016_nom)
            hist_cutflow_2017.Fill(name,int_2017/int_2017_nom)
            hist_cutflow_2018.Fill(name,int_2018/int_2018_nom)

        elif "raw" in  mode or "cut" in mode:
            hist_cutflow_2016.Fill(name,int_2016)
            hist_cutflow_2017.Fill(name,int_2017)
            hist_cutflow_2018.Fill(name,int_2018)


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
    c1.SaveAs("SF/cutflow_"+process+"_muchannel.eps")
    c1.SaveAs("SF/cutflow_"+process+"_muchannel.pdf")

    ######### print latex table
    f = open("cutflow_"+process+"_"+channel+"_"+mode+".tex", "w")
    f.write(" \\begin{table} \n \\centering \n\\begin{tabular}{c c c c } \n cut & 2016 & 2017 &  2018\\\\ \n \\hline \n")
    if "raw" in mode:
        f.write("total number & %.3f" %(int_2016_nom)+" & %.3f" %(int_2017_nom)+" & %.3f" %(int_2018_nom) + "\\\\ \n")
        
    if "cut" in mode:
        f.write(hist_cutflow_2016.GetXaxis().GetBinLabel(1).replace("_ele","").replace("_","\\_") +" & %.3f" %(hist_cutflow_2016.GetBinContent(1)/int_2016_nom)+" & %.3f" %(hist_cutflow_2017.GetBinContent(1)/int_2017_nom)+" & %.3f" %(hist_cutflow_2018.GetBinContent(1)/int_2018_nom) + "\\\\ \n")
        for bin in range(2,hist_cutflow_2016.GetNbinsX()+1):
            if "lepton" not in hist_cutflow_2016.GetXaxis().GetBinLabel(bin):
                f.write(hist_cutflow_2016.GetXaxis().GetBinLabel(bin).replace("_ele","").replace("_","\\_") +" & %.3f" %(hist_cutflow_2016.GetBinContent(bin)/hist_cutflow_2016.GetBinContent(bin-1))+" & %.3f" %(hist_cutflow_2017.GetBinContent(bin)/hist_cutflow_2017.GetBinContent(bin-1))+" & %.3f" %(hist_cutflow_2018.GetBinContent(bin)/hist_cutflow_2018.GetBinContent(bin-1)) + "\\\\ \n")
            else:
                f.write(hist_cutflow_2016.GetXaxis().GetBinLabel(bin) +" & %.3f" %(hist_cutflow_2016.GetBinContent(bin)/hist_cutflow_2016.GetBinContent(bin-1))+" & %.3f" %(hist_cutflow_2017.GetBinContent(bin)/hist_cutflow_2017.GetBinContent(bin-1))+" & %.3f" %(hist_cutflow_2018.GetBinContent(bin)/hist_cutflow_2018.GetBinContent(bin-1)) + "\\\\ \n")

            
    else:
        for bin in range(1,hist_cutflow_2016.GetNbinsX()+1):
            #    print hist_cutflow_2016.GetXaxis().GetBinLabel(bin)
            if "lepton" not in hist_cutflow_2016.GetXaxis().GetBinLabel(bin):
                f.write(hist_cutflow_2016.GetXaxis().GetBinLabel(bin).replace("_ele","").replace("_","\\_") +" & %.3f" %(hist_cutflow_2016.GetBinContent(bin))+" & %.3f" %(hist_cutflow_2017.GetBinContent(bin))+" & %.3f" %(hist_cutflow_2018.GetBinContent(bin)) + "\\\\ \n")
            else:
                f.write(hist_cutflow_2016.GetXaxis().GetBinLabel(bin) +" & %.3f" %(hist_cutflow_2016.GetBinContent(bin))+" & %.3f" %(hist_cutflow_2017.GetBinContent(bin))+" & %.3f" %(hist_cutflow_2018.GetBinContent(bin)) + "\\\\ \n")

    f.write(" \\end{tabular}\n \\caption{Cutflow for "+process+" in "+channel+" channel}\n \\label{tab:cutflow_"+process+channel+"}\n \\end{table}")
    f.close()



create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018)
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018,channel="electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018,mode="raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018,channel="electron", mode ="raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018,mode="cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018,channel="electron", mode ="cut")


###### Wjets
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets", channel = "electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets", mode = "raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets", channel = "electron", mode = "raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets", mode = "cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "WJets", channel = "electron", mode = "cut")

###### DY
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY",channel = "electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY",mode ="raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY",channel = "electron",mode="raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY",mode ="cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "DY",channel = "electron",mode="cut")

###### SingleTop
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop",channel="electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop",mode = "raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop",channel="electron",mode="raw")


create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop",mode = "cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "SingleTop",channel="electron",mode="cut")


###### Diboson
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson", channel="electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson",mode="raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson", channel="electron",mode="raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson",mode="cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "Diboson", channel="electron",mode="cut")


###### TTV
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV",channel="electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV",mode="raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV",channel="electron",mode="raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV",mode="cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "TTV",channel="electron",mode="cut")


###### VLQ
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_B_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_B_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1000")

###### VLQ
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_B_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/SFbtagcomb/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_B_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700",channel="electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700",mode="raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700",channel="electron",mode="raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700",mode="cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ700",channel="electron",mode="cut")


###### VLQ
infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_B_2016v3.root") 
TH1.AddDirectory(0)    

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/SFbtagcomb/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_2018.root") 
TH1.AddDirectory(0)    

preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_B_2016v3.root") 
TH1.AddDirectory(0)    

preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_2017v2.root") 
TH1.AddDirectory(0)    

preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_2018.root") 
TH1.AddDirectory(0)    

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200",channel = "electron")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200",mode = "raw")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200",channel = "electron",mode = "raw")

create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200",mode = "cut")
create_cutflow( preinfile_2016, preinfile_2017,preinfile_2018,infile_2016,infile_2017,infile_2018, "VLQ1200",channel = "electron",mode = "cut")




# ###### print background composition
# infile_2016_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_DY = infile_2016_DY.Get("reco_much_sr/"+histname).Integral()

# infile_2016_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_Wjets = infile_2016_Wjets.Get("reco_much_sr/"+histname).Integral()

# infile_2016_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_QCD = infile_2016_QCD.Get("reco_much_sr/"+histname).Integral()

# infile_2016_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_TTV = infile_2016_TTV.Get("reco_much_sr/"+histname).Integral()

# infile_2016_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_Diboson = infile_2016_Diboson.Get("reco_much_sr/"+histname).Integral()

# infile_2016_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_SingleTop = infile_2016_SingleTop.Get("reco_much_sr/"+histname).Integral()

# hist_2016_ttbar = infile_2016.Get("reco_much_sr/"+histname).Integral()

# total_2016 = hist_2016_ttbar+hist_2016_SingleTop+hist_2016_Diboson+hist_2016_TTV+hist_2016_QCD+hist_2016_Wjets+hist_2016_DY

# print "========== 2016"
# print "ttbar:  %.2f"%(hist_2016_ttbar/total_2016)
# print "DY:  %.2f"%(hist_2016_DY/total_2016)
# print "Wjets:  %.2f"%(hist_2016_Wjets/total_2016)
# print "QCD:  %.2f"%(hist_2016_QCD/total_2016)
# print "TTV:  %.2f"%(hist_2016_TTV/total_2016)
# print "Diboson:  %.2f"%(hist_2016_Diboson/total_2016)
# print "SingleTop:  %.2f"%(hist_2016_SingleTop/total_2016)


# infile_2017_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_DY = infile_2017_DY.Get("reco_much_sr/"+histname).Integral()

# infile_2017_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_Wjets = infile_2017_Wjets.Get("reco_much_sr/"+histname).Integral()

# infile_2017_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_QCD = infile_2017_QCD.Get("reco_much_sr/"+histname).Integral()

# infile_2017_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_TTV = infile_2017_TTV.Get("reco_much_sr/"+histname).Integral()

# infile_2017_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_Diboson = infile_2017_Diboson.Get("reco_much_sr/"+histname).Integral()

# infile_2017_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_SingleTop = infile_2017_SingleTop.Get("reco_much_sr/"+histname).Integral()

# hist_2017_ttbar = infile_2017.Get("reco_much_sr/"+histname).Integral()

# total_2017 = hist_2017_ttbar+hist_2017_SingleTop+hist_2017_Diboson+hist_2017_TTV+hist_2017_QCD+hist_2017_Wjets+hist_2017_DY

# print "========== 2017"
# print "ttbar:  %.2f"%(hist_2017_ttbar/total_2017)
# print "DY:  %.2f"%(hist_2017_DY/total_2017)
# print "Wjets:  %.2f"%(hist_2017_Wjets/total_2017)
# print "QCD:  %.2f"%(hist_2017_QCD/total_2017)
# print "TTV:  %.2f"%(hist_2017_TTV/total_2017)
# print "Diboson:  %.2f"%(hist_2017_Diboson/total_2017)
# print "SingleTop:  %.2f"%(hist_2017_SingleTop/total_2017)

# infile_2018_DY= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.DYJets_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_DY = infile_2018_DY.Get("reco_much_sr/"+histname).Integral()

# infile_2018_Wjets= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.WJets_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_Wjets = infile_2018_Wjets.Get("reco_much_sr/"+histname).Integral()

# infile_2018_QCD= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.QCDMu_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_QCD = infile_2018_QCD.Get("reco_much_sr/"+histname).Integral()

# infile_2018_TTV= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTV_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_TTV = infile_2018_TTV.Get("reco_much_sr/"+histname).Integral()

# infile_2018_Diboson= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.Diboson_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_Diboson = infile_2018_Diboson.Get("reco_much_sr/"+histname).Integral()

# infile_2018_SingleTop= TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.SingleTop_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_SingleTop = infile_2018_SingleTop.Get("reco_much_sr/"+histname).Integral()

# hist_2018_ttbar = infile_2018.Get("reco_much_sr/"+histname).Integral()

# total_2018 = hist_2018_ttbar+hist_2018_SingleTop+hist_2018_Diboson+hist_2018_TTV+hist_2018_QCD+hist_2018_Wjets+hist_2018_DY

# print "========== 2018"
# print "ttbar:  %.2f"%(hist_2018_ttbar/total_2018)
# print "DY:  %.2f"%(hist_2018_DY/total_2018)
# print "Wjets:  %.2f"%(hist_2018_Wjets/total_2018)
# print "QCD:  %.2f"%(hist_2018_QCD/total_2018)
# print "TTV:  %.2f"%(hist_2018_TTV/total_2018)
# print "Diboson:  %.2f"%(hist_2018_Diboson/total_2018)
# print "SingleTop:  %.2f"%(hist_2018_SingleTop/total_2018)



# print "============= data Muon"
# infile_2016_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_data = infile_2016_data.Get("trigger/"+histname).Integral()
# print "2016:  %.2f"%hist_2016_data

# infile_2017_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_data = infile_2017_data.Get("trigger/"+histname).Integral()
# print "2017:  %.2f"%hist_2017_data


# infile_2018_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_data = infile_2018_data.Get("trigger/"+histname).Integral()
# print "2018:  %.2f"%hist_2018_data

# print "2017 scaled:  %.2f"%(hist_2017_data*(35.9/41.5))
# print "2018 scaled:  %.2f"%(hist_2018_data*(35.9/59.7))



# print "============= data Electron"
# infile_2016_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
# TH1.AddDirectory(0)    
# hist_2016_data = infile_2016_data.Get("trigger/"+histname).Integral()
# print "2016:  %.2f"%hist_2016_data

# infile_2017_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
# TH1.AddDirectory(0)    
# hist_2017_data = infile_2017_data.Get("trigger/"+histname).Integral()
# print "2017:  %.2f"%hist_2017_data


# infile_2018_data = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
# TH1.AddDirectory(0)    
# hist_2018_data = infile_2018_data.Get("trigger/"+histname).Integral()
# print "2018:  %.2f"%hist_2018_data

# print "2017 scaled:  %.2f"%(hist_2017_data*(35.9/41.5))
# print "2018 scaled:  %.2f"%(hist_2018_data*(35.9/59.7))



# ####### data cut flow
# infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
# TH1.AddDirectory(0)    

# infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
# TH1.AddDirectory(0)    

# infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
# TH1.AddDirectory(0)    

# preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2016v3.root") 
# TH1.AddDirectory(0)    

# preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root") 
# TH1.AddDirectory(0)    

# preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2018.root") 
# TH1.AddDirectory(0)    


# hist_cutflow_data_2016 = TH1F("cutflow_data_2016","cutflow_2016",10,0,10)
# hist_cutflow_data_2017 = TH1F("cutflow_data_2017","cutflow_2017",10,0,10)
# hist_cutflow_data_2018 = TH1F("cutflow_data_2018","cutflow_2018",10,0,10)
# # for el in lista:
# #     hist_cutflow.SetBinContent(el,0)

# hist_2016_nom = preinfile_2016.Get("cleaner/"+histname)
# hist_2017_nom = preinfile_2017.Get("cleaner/"+histname)
# hist_2018_nom = preinfile_2018.Get("cleaner/"+histname)

# # hist_2016_nom = infile_2016.Get("cleaner/"+histname)
# # hist_2017_nom = infile_2017.Get("cleaner/"+histname)
# # hist_2018_nom = infile_2018.Get("cleaner/"+histname)


# int_2016_nom = hist_2016_nom.Integral()
# int_2017_nom = hist_2017_nom.Integral()
# int_2018_nom = hist_2018_nom.Integral()


# for el in lista_pre:
#     print el

#     hist_2016 = preinfile_2016.Get(el+"/"+histname)
#     hist_2017 = preinfile_2017.Get(el+"/"+histname)
#     hist_2018 = preinfile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral()
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_data_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_data_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_data_2018.Fill(el,int_2018/int_2018_nom)


# for el in lista:
#     print el

#     hist_2016 = infile_2016.Get(el+"/"+histname)
#     hist_2017 = infile_2017.Get(el+"/"+histname)
#     hist_2018 = infile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral()
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_data_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_data_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_data_2018.Fill(el,int_2018/int_2018_nom)
#     if "reco" in el:
#         print "++++++++++ Data eff reco"
#         print "2016:  %.4f"%(int_2016/int_2016_nom)
#         print "2017:  %.4f"%(int_2017/int_2017_nom)
#         print "2018:  %.4f"%(int_2018/int_2018_nom)
#     if "trigger" in el:
#         print "++++++++++ Data eff trigger"
#         print "2016:  %.4f"%(int_2016/int_2016_nom)
#         print "2017:  %.4f"%(int_2017/int_2017_nom)
#         print "2018:  %.4f"%(int_2018/int_2018_nom)


# c1 = TCanvas()
# leg = TLegend(0.6,0.6,0.95,0.9, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);
# leg.SetHeader("Data")

# hist_cutflow_data_2016.SetLineColor(kRed)
# hist_cutflow_data_2017.SetLineColor(kBlack)
# hist_cutflow_data_2018.SetLineColor(kBlue)

# hist_cutflow_data_2016.SetLineStyle(1)
# hist_cutflow_data_2017.SetLineStyle(5)
# hist_cutflow_data_2018.SetLineStyle(2)

# hist_cutflow_data_2016.SetLineWidth(2)
# hist_cutflow_data_2017.SetLineWidth(2)
# hist_cutflow_data_2018.SetLineWidth(2)

# hist_cutflow_data_2016.Draw("HIST same")
# hist_cutflow_data_2017.Draw("HIST same")
# hist_cutflow_data_2018.Draw("HIST same")

# leg.AddEntry(hist_cutflow_data_2016,"2016","l")
# leg.AddEntry(hist_cutflow_data_2017,"2017","l")
# leg.AddEntry(hist_cutflow_data_2018,"2018","l")

# leg.Draw()
# c1.SetLogy()
# c1.SaveAs("SF/cutflow_data_muchannel.eps")
# c1.SaveAs("SF/cutflow_data_muchannel.pdf")

# ############# data electron channel
# infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
# TH1.AddDirectory(0)    

# infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
# TH1.AddDirectory(0)    

# infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
# TH1.AddDirectory(0)    

# preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2016v3.root") 
# TH1.AddDirectory(0)    

# preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root") 
# TH1.AddDirectory(0)    

# preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2018.root") 
# TH1.AddDirectory(0)    

# hist_cutflow_ele_data_2016 = TH1F("cutflow_ele_data_2016","cutflow_2016",10,0,10)
# hist_cutflow_ele_data_2017 = TH1F("cutflow_ele_data_2017","cutflow_2017",10,0,10)
# hist_cutflow_ele_data_2018 = TH1F("cutflow_ele_data_2018","cutflow_2018",10,0,10)

# hist_2016_nom = preinfile_2016.Get("cleaner/"+histname)
# hist_2017_nom = preinfile_2017.Get("cleaner/"+histname)
# hist_2018_nom = preinfile_2018.Get("cleaner/"+histname)

# int_2016_nom = hist_2016_nom.Integral()
# int_2017_nom = hist_2017_nom.Integral()
# int_2018_nom = hist_2018_nom.Integral()


# for el in lista_pre:
#     print el

#     hist_2016 = preinfile_2016.Get(el+"/"+histname)
#     hist_2017 = preinfile_2017.Get(el+"/"+histname)
#     hist_2018 = preinfile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral()
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_ele_data_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_ele_data_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_ele_data_2018.Fill(el,int_2018/int_2018_nom)


# for el in lista_ele:
#     print el

#     hist_2016 = infile_2016.Get(el+"/"+histname)
#     hist_2017 = infile_2017.Get(el+"/"+histname)
#     hist_2018 = infile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral()
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_ele_data_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_ele_data_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_ele_data_2018.Fill(el,int_2018/int_2018_nom)
#     if "reco" in el:
#         print "++++++++++ Data ele eff reco"
#         print "2016:  %.4f"%(int_2016/int_2016_nom)
#         print "2017:  %.4f"%(int_2017/int_2017_nom)
#         print "2018:  %.4f"%(int_2018/int_2018_nom)
#     if "trigger" in el:
#         print "++++++++++ Data ele eff trigger"
#         print "2016:  %.4f"%(int_2016/int_2016_nom)
#         print "2017:  %.4f"%(int_2017/int_2017_nom)
#         print "2018:  %.4f"%(int_2018/int_2018_nom)


# ##################### signal cutflow
# infile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_RH_1000_2016v3.root") 
# TH1.AddDirectory(0)    

# infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2017v2.root") 
# TH1.AddDirectory(0)    

# infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2018.root") 
# TH1.AddDirectory(0)    

# preinfile_2016 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2016/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_RH_1000_2016v3.root") 
# TH1.AddDirectory(0)    

# preinfile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2017v2.root") 
# TH1.AddDirectory(0)    

# preinfile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_2018.root") 
# TH1.AddDirectory(0)    


# #lista_pre = ["cleaner","lepton","jets","met"]
# #lista = ["trigger","btag_3m","reco_much_sr"]

# histname = "sum_event_weights"

# hist_cutflow_signal_2016 = TH1F("cutflow_signal_2016","cutflow_2016",10,0,10)
# hist_cutflow_signal_2017 = TH1F("cutflow_signal_2017","cutflow_2017",10,0,10)
# hist_cutflow_signal_2018 = TH1F("cutflow_signal_2018","cutflow_2018",10,0,10)

# int_2016_nom = 35900
# int_2017_nom = 41500
# int_2018_nom = 59600

# for el in lista_pre:
#     print el

#     hist_2016 = preinfile_2016.Get(el+"/"+histname)
#     hist_2017 = preinfile_2017.Get(el+"/"+histname)
#     hist_2018 = preinfile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral() *(41500/35900)
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_signal_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_signal_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_signal_2018.Fill(el,int_2018/int_2018_nom)


# for el in lista:
#     print el

#     hist_2016 = infile_2016.Get(el+"/"+histname)
#     hist_2017 = infile_2017.Get(el+"/"+histname)
#     hist_2018 = infile_2018.Get(el+"/"+histname)

#     int_2016 = hist_2016.Integral()
#     int_2017 = hist_2017.Integral()
#     int_2018 = hist_2018.Integral()

#     hist_cutflow_signal_2016.Fill(el,int_2016/int_2016_nom)
#     hist_cutflow_signal_2017.Fill(el,int_2017/int_2017_nom)
#     hist_cutflow_signal_2018.Fill(el,int_2018/int_2018_nom)


# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.55,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);
# leg.SetHeader("T #rightarrow Ht, M=900 GeV")

# hist_cutflow_signal_2016.SetLineColor(kRed)
# hist_cutflow_signal_2017.SetLineColor(kBlack)
# hist_cutflow_signal_2018.SetLineColor(kBlue)

# hist_cutflow_signal_2016.SetLineStyle(1)
# hist_cutflow_signal_2017.SetLineStyle(5)
# hist_cutflow_signal_2018.SetLineStyle(2)

# hist_cutflow_signal_2016.SetLineWidth(2)
# hist_cutflow_signal_2017.SetLineWidth(2)
# hist_cutflow_signal_2018.SetLineWidth(2)

# hist_cutflow_signal_2016.Draw("HIST same")
# hist_cutflow_signal_2017.Draw("HIST same")
# hist_cutflow_signal_2018.Draw("HIST same")

# leg.AddEntry(hist_cutflow_signal_2016,"2016","l")
# leg.AddEntry(hist_cutflow_signal_2017,"2017","l")
# leg.AddEntry(hist_cutflow_signal_2018,"2018","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_signal_muchannel.eps")
# c1.SaveAs("SF/cutflow_signal_muchannel.pdf")


# c1 = TCanvas()
# leg = TLegend(0.4,0.6,0.95,0.9, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);

# hist_cutflow_signal_2016.SetLineColor(kBlack)

# hist_cutflow_signal_2016.SetLineStyle(kDashed)

# hist_cutflow_signal_2016.SetLineWidth(2)

# hist_cutflow_signal_2016.Draw("HIST same")
# hist_cutflow_2016.Draw("HIST same")

# leg.AddEntry(hist_cutflow_signal_2016,"2016 (T #rightarrow Ht, M=900 GeV)","l")
# leg.AddEntry(hist_cutflow_2016,"2016 (t#bar{t})","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_mixed_muchannel.eps")
# c1.SaveAs("SF/cutflow_mixed_muchannel.pdf")


# ################# electron ttbar and data comparison for 2017 and 2018
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2017.SetLineColor(kRed)
# hist_cutflow_2017.SetLineColor(kBlue)
# hist_cutflow_ele_data_2017.SetLineColor(kOrange+2)
# hist_cutflow_data_2017.GetYaxis().SetRangeUser(0.00001,1.5)

# hist_cutflow_data_2017.Draw("HIST same")
# hist_cutflow_2017.Draw("HIST same")
# hist_cutflow_ele_data_2017.Draw("HIST same")
# hist_cutflow_ele_2017.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2017,"2017 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2017,"2017 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2017,"2017 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2017,"2017 (t#bar{t}, e)","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu.eps")
# c1.SaveAs("SF/cutflow_ele_mu.pdf")


# ################# electron ttbar and data comparison for 2017 and 2018
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2018.SetLineColor(kRed)
# hist_cutflow_2018.SetLineColor(kBlue)
# hist_cutflow_ele_data_2018.SetLineColor(kOrange+2)
# hist_cutflow_data_2018.GetYaxis().SetRangeUser(0.00001,1.5)

# hist_cutflow_data_2018.Draw("HIST same")
# hist_cutflow_2018.Draw("HIST same")
# hist_cutflow_ele_data_2018.Draw("HIST same")
# hist_cutflow_ele_2018.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2018,"2018 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2018,"2018 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2018,"2018 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2018,"2018 (t#bar{t}, e)","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu_2018.eps")
# c1.SaveAs("SF/cutflow_ele_mu_2018.pdf")


# ################# electron ttbar and data comparison for 2017 and 2016
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2016.SetLineColor(kRed)
# hist_cutflow_2016.SetLineColor(kBlue)
# hist_cutflow_ele_data_2016.SetLineColor(kOrange+2)
# hist_cutflow_data_2016.GetYaxis().SetRangeUser(0.00001,1.5)

# hist_cutflow_data_2016.SetLineStyle(2)
# hist_cutflow_2016.SetLineStyle(2)


# hist_cutflow_data_2016.Draw("HIST same")
# hist_cutflow_2016.Draw("HIST same")
# hist_cutflow_ele_data_2016.Draw("HIST same")
# hist_cutflow_ele_2016.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2016,"2016 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2016,"2016 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2016,"2016 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2016,"2016 (t#bar{t}, e)","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu_2016.eps")
# c1.SaveAs("SF/cutflow_ele_mu_2016.pdf")

# ################# electron/mu ttbar and data comparison for 2017 respectively
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2017_resp = hist_cutflow_data_2017.Clone()
# hist_cutflow_2017_resp = hist_cutflow_2017.Clone()
# hist_cutflow_ele_data_2017_resp = hist_cutflow_ele_data_2017.Clone()
# hist_cutflow_ele_2017_resp  = hist_cutflow_ele_2017.Clone()

# for bin in range(2,hist_cutflow_data_2017.GetNbinsX()):
#     hist_cutflow_data_2017_resp.SetBinContent(bin,hist_cutflow_data_2017.GetBinContent(bin)/hist_cutflow_data_2017.GetBinContent(bin-1))
#     hist_cutflow_2017_resp.SetBinContent(bin,hist_cutflow_2017.GetBinContent(bin)/hist_cutflow_2017.GetBinContent(bin-1))
#     hist_cutflow_ele_data_2017_resp.SetBinContent(bin,hist_cutflow_ele_data_2017.GetBinContent(bin)/hist_cutflow_ele_data_2017.GetBinContent(bin-1))
#     hist_cutflow_ele_2017_resp.SetBinContent(bin,hist_cutflow_ele_2017.GetBinContent(bin)/hist_cutflow_ele_2017.GetBinContent(bin-1))


# hist_cutflow_data_2017_resp.GetYaxis().SetRangeUser(0.001,1.1)

# hist_cutflow_data_2017_resp.Draw("HIST same")
# hist_cutflow_2017_resp.Draw("HIST same")
# hist_cutflow_ele_data_2017_resp.Draw("HIST same")
# hist_cutflow_ele_2017_resp.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2017_resp,"2017 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2017_resp,"2017 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2017_resp,"2017 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2017_resp,"2017 (t#bar{t}, e)","l")

# leg.Draw()

# c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu_2017_resp.eps")
# c1.SaveAs("SF/cutflow_ele_mu_2017_resp.pdf")


# ################# electron/mu ttbar and data comparison for 2018 respectively
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2018_resp = hist_cutflow_data_2018.Clone()
# hist_cutflow_2018_resp = hist_cutflow_2018.Clone()
# hist_cutflow_ele_data_2018_resp = hist_cutflow_ele_data_2018.Clone()
# hist_cutflow_ele_2018_resp  = hist_cutflow_ele_2018.Clone()

# for bin in range(2,hist_cutflow_data_2018.GetNbinsX()):
#     hist_cutflow_data_2018_resp.SetBinContent(bin,hist_cutflow_data_2018.GetBinContent(bin)/hist_cutflow_data_2018.GetBinContent(bin-1))
#     hist_cutflow_2018_resp.SetBinContent(bin,hist_cutflow_2018.GetBinContent(bin)/hist_cutflow_2018.GetBinContent(bin-1))
#     hist_cutflow_ele_data_2018_resp.SetBinContent(bin,hist_cutflow_ele_data_2018.GetBinContent(bin)/hist_cutflow_ele_data_2018.GetBinContent(bin-1))
#     hist_cutflow_ele_2018_resp.SetBinContent(bin,hist_cutflow_ele_2018.GetBinContent(bin)/hist_cutflow_ele_2018.GetBinContent(bin-1))


# hist_cutflow_data_2018_resp.GetYaxis().SetRangeUser(0.,1.1)

# hist_cutflow_data_2018_resp.Draw("HIST same")
# hist_cutflow_2018_resp.Draw("HIST same")
# hist_cutflow_ele_data_2018_resp.Draw("HIST same")
# hist_cutflow_ele_2018_resp.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2018_resp,"2018 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2018_resp,"2018 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2018_resp,"2018 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2018_resp,"2018 (t#bar{t}, e)","l")

# leg.Draw()

# #c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu_2018_resp.eps")
# c1.SaveAs("SF/cutflow_ele_mu_2018_resp.pdf")


# ################# electron/mu ttbar and data comparison for 2016 respectively
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_data_2016_resp = hist_cutflow_data_2016.Clone()
# hist_cutflow_2016_resp = hist_cutflow_2016.Clone()
# hist_cutflow_ele_data_2016_resp = hist_cutflow_ele_data_2016.Clone()
# hist_cutflow_ele_2016_resp  = hist_cutflow_ele_2016.Clone()

# for bin in range(2,hist_cutflow_data_2016.GetNbinsX()):
#     hist_cutflow_data_2016_resp.SetBinContent(bin,hist_cutflow_data_2016.GetBinContent(bin)/hist_cutflow_data_2016.GetBinContent(bin-1))
#     hist_cutflow_2016_resp.SetBinContent(bin,hist_cutflow_2016.GetBinContent(bin)/hist_cutflow_2016.GetBinContent(bin-1))
#     hist_cutflow_ele_data_2016_resp.SetBinContent(bin,hist_cutflow_ele_data_2016.GetBinContent(bin)/hist_cutflow_ele_data_2016.GetBinContent(bin-1))
#     hist_cutflow_ele_2016_resp.SetBinContent(bin,hist_cutflow_ele_2016.GetBinContent(bin)/hist_cutflow_ele_2016.GetBinContent(bin-1))


# hist_cutflow_data_2016_resp.GetYaxis().SetRangeUser(0.,1.1)

# hist_cutflow_data_2016_resp.Draw("HIST same")
# hist_cutflow_2016_resp.Draw("HIST same")
# hist_cutflow_ele_data_2016_resp.Draw("HIST same")
# hist_cutflow_ele_2016_resp.Draw("HIST same")


# leg.AddEntry(hist_cutflow_data_2016_resp,"2016 (data #mu)","l")
# leg.AddEntry(hist_cutflow_2016_resp,"2016 (t#bar{t}, #mu)","l")

# leg.AddEntry(hist_cutflow_ele_data_2016_resp,"2016 (data, e)","l")
# leg.AddEntry(hist_cutflow_ele_2016_resp,"2016 (t#bar{t}, e)","l")

# leg.Draw()

# #c1.SetLogy()
# c1.SaveAs("SF/cutflow_ele_mu_2016_resp.eps")
# c1.SaveAs("SF/cutflow_ele_mu_2016_resp.pdf")



# ################# electron/mu signal comparison for 2016/17/18 respectively
# c1 = TCanvas()
# leg = TLegend(0.2,0.2,0.35,0.5, "","brNDC")
# leg.SetBorderSize(0);	
# leg.SetFillStyle(0);
# leg.SetTextSize(0.035);
# leg.SetTextFont(42);


# hist_cutflow_signal_2016_resp = hist_cutflow_signal_2016.Clone()
# hist_cutflow_signal_2017_resp = hist_cutflow_signal_2017.Clone()
# hist_cutflow_signal_2018_resp = hist_cutflow_signal_2018.Clone()


# for bin in range(2,hist_cutflow_signal_2016.GetNbinsX()):
#     hist_cutflow_signal_2016_resp.SetBinContent(bin,hist_cutflow_signal_2016.GetBinContent(bin)/hist_cutflow_signal_2016.GetBinContent(bin-1))
#     hist_cutflow_signal_2017_resp.SetBinContent(bin,hist_cutflow_signal_2017.GetBinContent(bin)/hist_cutflow_signal_2017.GetBinContent(bin-1))
#     hist_cutflow_signal_2018_resp.SetBinContent(bin,hist_cutflow_signal_2018.GetBinContent(bin)/hist_cutflow_signal_2018.GetBinContent(bin-1))





# hist_cutflow_signal_2016_resp.GetYaxis().SetRangeUser(0.,1.1)

# hist_cutflow_signal_2016_resp.Draw("HIST same")
# hist_cutflow_signal_2017_resp.Draw("HIST same")
# hist_cutflow_signal_2018_resp.Draw("HIST same")



# leg.AddEntry(hist_cutflow_signal_2016_resp,"2016","l")
# leg.AddEntry(hist_cutflow_signal_2017_resp,"2017","l")
# leg.AddEntry(hist_cutflow_signal_2018_resp,"2018","l")

# leg.Draw()

# #c1.SetLogy()
# c1.SaveAs("SF/cutflow_signal_resp.eps")
# c1.SaveAs("SF/cutflow_signal_resp.pdf")
