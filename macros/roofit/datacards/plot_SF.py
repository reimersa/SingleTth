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

infile_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_2017v2.root") 
TH1.AddDirectory(0)    

infile_2018 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.TTbar_2018.root") 
TH1.AddDirectory(0)    

lista = ['eventweight_final','eventweight_lumi','weight_btag','weight_pu','weight_sfelec_id','weight_sfelec_reco','weight_sfelec_trigger','weight_sfmu_id','weight_sfmu_iso','weight_sfmu_trigger']

tree_2016 = infile_2016.Get("AnalysisTree")
tree_2017 = infile_2017.Get("AnalysisTree")
tree_2018 = infile_2018.Get("AnalysisTree")

for el in lista:
    print el
    tree_2016.Draw(el+">>h2016(100,0,1)")
    histo_2016 = gROOT.FindObject("h2016")
    tree_2017.Draw(el+">>h2017(100,0,1)")
    histo_2017 = gROOT.FindObject("h2017")
    tree_2018.Draw(el+">>h2018(100,0,1)")
    histo_2018 = gROOT.FindObject("h2018")

    histo_2016.SetLineColor(kRed)
    histo_2017.SetLineColor(kBlack)
    histo_2018.SetLineColor(kBlue)

    histo_2016.SetLineWidth(2)
    histo_2017.SetLineWidth(2)
    histo_2018.SetLineWidth(2)

    histo_2016.SetLineStyle(1)
    histo_2017.SetLineStyle(5)
    histo_2018.SetLineStyle(2)

    histo_2016.GetXaxis().SetTitle(el)
    histo_2017.GetXaxis().SetTitle(el)
    histo_2018.GetXaxis().SetTitle(el)

    histo_2016.GetYaxis().SetTitle("a.u.")
    histo_2017.GetYaxis().SetTitle("a.u.")
    histo_2018.GetYaxis().SetTitle("a.u.")

    histo_2016.Scale(1/histo_2016.Integral())
    histo_2017.Scale(1/histo_2017.Integral())
    histo_2018.Scale(1/histo_2018.Integral())


    c1 = TCanvas()   
    leg = TLegend(0.2,0.6,0.55,0.9, "","brNDC")
    leg.SetBorderSize(0);	
    leg.SetFillStyle(0);
    leg.SetTextSize(0.035);
    leg.SetTextFont(42);

    histo_2016.Draw("HIST same")
    histo_2017.Draw("HIST same")
    histo_2018.Draw("HIST same")
    
    leg.AddEntry(histo_2016,"2016","l")
    leg.AddEntry(histo_2017,"2017","l")
    leg.AddEntry(histo_2018,"2018","l")

    leg.Draw()

    c1.SaveAs("SF/"+el+".eps")
    c1.SaveAs("SF/"+el+".pdf")
