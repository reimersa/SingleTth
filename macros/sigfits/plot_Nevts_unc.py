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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack,TPad,TLine, kTeal, kViolet,kMagenta
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
beps = True
gROOT.SetBatch(kTRUE)


CMSPlotStyle.left_margin=0.16
CMSPlotStyle.bottom_margin=0.14

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")

colors = [kRed,kBlue,kRed,kBlack,kOrange,kTeal,kGreen+2,kViolet,kMagenta,kBlue+3,kRed+3,kOrange+3,kTeal+3,kGreen-3,kViolet+3]
markers = [20,21,22,23,24,25,26,27,28,29,20,21,22,23,24,25,26,27,28,29]

#list of unc considered
unc_dict = {}


# read in txt file
f = open("Nevents_test.txt", "r")

# loop over line and fill hist
for line in f:
    line_list = line.split("  ")
    print line_list
    if "down" in line_list[0]: continue
    if not unc_dict.has_key(line_list[0]):
        unc_dict[line_list[0]] = {}
    unc_dict[line_list[0]][line_list[2]] = line_list[4]

#close txt file
f.close()


#create Cavans to plot all hist

c1 = TCanvas()
leg = TLegend(0.5,0.5,0.95,0.9, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);


print unc_dict
print len(unc_dict)
hist_to_draw = {}

ii=0
# loop over uncertainties and create a hist for each
for unc in sorted(unc_dict):
    #one hist per unc 
    hist = TH1F("hist","difference in tail for uncertainty as function of mass",8,0,8)
    hist.Fill("600",0)
    hist.Fill("650",0)
    hist.Fill("700",0)
    hist.Fill("800",0)
    hist.Fill("900",0)
    hist.Fill("1000",0)
    hist.Fill("1100",0)
    for mass in sorted(unc_dict[unc]):
        hist.Fill(mass,float(unc_dict[unc][mass])- float(unc_dict["nominal"][mass]))
    

    hist_to_draw[unc] = hist.Clone()


# hist for quad. add. error
hist_add = TH1F("hist_add","added",8,0,8)
for bin in range(1,hist_add.GetNbinsX()+1):
    hist_add.SetBinContent(bin,0)


for unc in hist_to_draw:
    hist = hist_to_draw[unc]

    for bin in range(1,hist.GetNbinsX()+1):
        hist_add.SetBinContent(bin,hist_add.GetBinContent(bin)+pow(hist.GetBinContent(bin),2))

    print hist_add.GetBinContent(1)

    hist.SetLineColor(colors[ii])
    hist.SetMarkerStyle(markers[ii])
    hist.SetMarkerSize(1.5)
    hist.SetMarkerColor(colors[ii])
    hist.GetYaxis().SetRangeUser(0,0.10)
    hist.GetXaxis().SetTitle("T mass [GeV]")
    hist.GetYaxis().SetTitle("% missed (Nominal - Uncertainty)")
    hist.GetYaxis().SetTitleOffset(1.3)

    hist.Draw("Hist P same")
    leg.AddEntry(hist,str(unc),"lpe")
    ii+=1

for bin in range(1,hist_add.GetNbinsX()+1):   
    hist_add.SetBinContent(bin,math.sqrt(hist_add.GetBinContent(bin)))

hist_add.SetLineColor(kBlack)
# hist_add.Draw("HIST same")
# leg.AddEntry(hist_add,"Tot. syst.","l")

leg.Draw()
c1.Print("Nevts_unc.eps")
c1.Print("Nevts_unc.pdf")

