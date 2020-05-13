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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, TArrow
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(1)
gStyle.SetPaintTextFormat("2.3f")



parser = argparse.ArgumentParser(description='Plot the signal strength.')
parser.add_argument('-integer', metavar='N', type=float,
                   help='Singal stregth to plot')
parser.add_argument('--name', dest='outname',type=str)


args = parser.parse_args()
signalstrength = args.integer


c1 = TCanvas()
infile = TFile("higgsCombineTest.GoodnessOfFit.mH120.123456.root","r")
tree_fit_sb = infile.Get("limit")
tree_fit_sb.Draw("limit>>h(40,50,150)")

histo = gROOT.FindObject("h")
histo.GetXaxis().SetTitle("Saturated test statistic")
histo.GetYaxis().SetTitle("Toys")
gPad.Update()



ar4 = TArrow(signalstrength,0,signalstrength,10,0.05,"<|");
ar4.SetLineWidth(2);
ar4.SetFillColor(2);
ar4.SetLineColor(2);
ar4.Draw();

c1.Print(args.outname+".eps")
