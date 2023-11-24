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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack, kMagenta, TPad, TLine, RooFit
from collections import OrderedDict
import argparse
import CMSPlotStyle

gROOT.SetBatch(kTRUE)

MT = 600

#### which toy
infile = TFile("fitDiagnostics_mavariable_wodR_all_ST_rebin_2018_ma125_allsyst_2pexp3dijet_signal1.00_"+str(MT)+".root","r")
tree = infile.Get("tree_fit_sb")


for i,e in enumerate(tree):
    print("r",e.r)
    if e.r >0.95 and e.r < 1.05:
        i_Toy = i 
        break
        

infile.Close()

print("i_Toy",i)

#### get toy
infile_gen = TFile("higgsCombine1.00_"+str(MT)+"_signal.GenerateOnly.mH125.123456.root","r")
ds =  infile_gen.Get("toys/toy_"+str(i_Toy+1))
ds.SetName("toy_1")
outfile = TFile("TEST.root","recreate")
outfile.mkdir("toys")
outfile.cd("toys")

ds.Write()

outfile.Close()
