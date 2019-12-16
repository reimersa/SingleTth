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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue
from collections import OrderedDict


berror = False


infile = TFile("fitDiagnostics.root","r")
infile_gen = TFile("higgsCombineTest.GenerateOnly.mH125.123456.root","r")


## Plot toys
x = RooRealVar("x", "m_{T} [GeV]", 400, 2000);
plotter=x.frame();
ds =  infile_gen.Get("toys/toy_1")
hist = ds.binnedClone();
print "Integral of Toys  " + str(hist.sumEntries())
hist.plotOn(plotter)
plotter.Draw()


## Plot prefit s+b and b
sb_prefit = infile.Get("shapes_prefit/much/total")
sb_prefit.SetLineColor(kRed)
sb_prefit.SetLineWidth(2)
sb_prefit.Scale(20)
print "Integral of sb_prefit  "+str(sb_prefit.Integral())
sb_prefit.Draw("HIST same")

b_prefit = infile.Get("shapes_prefit/much/total_background")
b_prefit.SetLineColor(kBlue)
b_prefit.SetLineWidth(2)
b_prefit.Scale(20)
print "Integral of b_prefit  "+ str(b_prefit.Integral())
b_prefit.Draw("HIST same")

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
leg.Draw()

## Plots postfit

raw_input()
