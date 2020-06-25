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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, kOrange, kTRUE
from collections import OrderedDict
import CMSPlotStyle

#infile = TFile("fitDiagnostics.root","r")
infile = TFile("fitDiagnosticsexp2p_r0_fitmin500GeV_MT600GeV.root","r")
tree = infile.Get("tree_fit_sb")
tree_b = infile.Get("tree_fit_b")

list_param = ["bgexp2_p0much_2016v3","bgexp2_p1much_2016v3","bgexp2_p0ech_2016v3","bgexp2_p1ech_2016v3","bgexp2_p0much_2017v2","bgexp2_p1much_2017v2","bgexp2_p0ech_2017v2","bgexp2_p1ech_2017v2","bgexp2_p0much_2018","bgexp2_p1much_2018","bgexp2_p0ech_2018","bgexp2_p1ech_2018"]

gROOT.SetBatch(kTRUE)

for el in list_param:
    #### for differen r
    # c = TCanvas()

    # tree.SetLineColor(kOrange)
    # tree.Draw(el,"r>1.2")
    # tree.SetLineColor(kBlue)
    # tree.Draw(el,"r<0.5","same")
    # tree.SetLineColor(kRed)
    # tree.Draw(el,"r<0", "same")

    # print el
    # raw_input()
    # c.SaveAs(el+".eps")

    # compare signal and bkg test
    c = TCanvas()

    tree.SetLineColor(kOrange)
    tree.Draw(el+">>htemp","","")

    stats =gROOT.FindObject("htemp")

    text = CMSPlotStyle.draw_info("sb <#mu> = %.4f"%stats.GetMean(),0.15,0.85,11)
    text.Draw("same")




    tree_b.SetLineColor(kBlue)
    tree_b.Draw(el+">>htemp2","","same")

    stats2 =gROOT.FindObject("htemp2");

    text2 = CMSPlotStyle.draw_info("b <#mu> = %.4f"%stats2.GetMean(),0.15,0.8,11)
    text2.Draw("same")



    print el
#    raw_input()
    c.SaveAs("params/"+el+".eps")


