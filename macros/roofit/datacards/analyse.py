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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed
from collections import OrderedDict


berror = False
bscale = False

infile = TFile("fitDiagnosticssignal1_1000.root","r")
infile_gen = TFile("higgsCombine1000.GenerateOnly.mH125.123123.root","r")
infile_gen2 = TFile("higgsCombine600.GenerateOnly.mH125.123123_signal0.root","r")


## Plot toys
x = RooRealVar("x", "m_{T} [GeV]", 400, 2000);
plotter=x.frame();
ds =  infile_gen.Get("toys/toy_1")
hist = ds.binnedClone();
print "Integral of Toys  " + str(hist.sumEntries())
if bscale: hist.Scale(1/hist.sumEntries())
hist.plotOn(plotter)
plotter.Draw()

##### second toys
ds2 =  infile_gen2.Get("toys/toy_1")
hist2 = ds2.binnedClone();
print "Integral of Toys  " + str(hist2.sumEntries())
if bscale: hist2.Scale(1/hist2.sumEntries())
hist2.plotOn(plotter)
#plotter.Draw("same")


## Plot prefit s+b and b
sb_prefit = infile.Get("shapes_prefit/much_2016v3/total")
sb_prefit.SetLineColor(kRed)
sb_prefit.SetLineWidth(2)
if bscale: sb_prefit.Scale(1/sb_prefit.Integral())
print "Integral of sb_prefit  "+str(sb_prefit.Integral())
sb_prefit.Draw("HIST same")

b_prefit = infile.Get("shapes_prefit/much_2016v3/total_background")
b_prefit.SetLineColor(kBlue)
b_prefit.SetLineWidth(2)
if bscale: b_prefit.Scale(1/b_prefit.Integral())
print "Integral of b_prefit  "+ str(b_prefit.Integral())
b_prefit.Draw("HIST same")


## Plot postfit s+b and b
sb_postfit = infile.Get("shapes_fit_s/much_2016v3/total")
sb_postfit.SetLineColor(kRed)
sb_postfit.SetLineStyle(kDashed)
sb_postfit.SetLineWidth(2)
if bscale: sb_postfit.Scale(1/sb_postfit.Integral())
print "Integral of sb_postfit  "+str(sb_postfit.Integral())
sb_postfit.Draw("HIST same")

b_postfit = infile.Get("shapes_fit_b/much_2016v3/total")
b_postfit.SetLineColor(kBlue)
b_postfit.SetLineStyle(kDashed)
b_postfit.SetLineWidth(2)
if bscale: b_postfit.Scale(1/b_postfit.Integral())
print "Integral of b_postfit  "+ str(b_postfit.Integral())
b_postfit.Draw("HIST same")


leg=TLegend(0.5,0.5,0.7,0.7,"","brNDC")
leg.SetBorderSize(0);
leg.SetTextSize(0.035);
leg.SetFillColor(0);
leg.SetFillStyle(0);
leg.SetLineColor(1);
leg.SetTextFont(42);

leg.AddEntry(plotter,"Toys","lpe")
leg.AddEntry(sb_prefit,"s+b prefit","l")
leg.AddEntry(b_prefit,"b prefit","l")
leg.AddEntry(sb_postfit,"s+b postfit","l")
leg.AddEntry(b_postfit,"b postfit","l")

leg.Draw()

## Plots postfit

raw_input()
