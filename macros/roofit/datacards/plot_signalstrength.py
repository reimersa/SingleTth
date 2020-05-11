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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE
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



parser = argparse.ArgumentParser(description='Plot the signal strength.')
parser.add_argument('integer', metavar='N', type=float,nargs='+',
                   help='Singal stregth to plot')
parser.add_argument('--name', dest='outname',type=str)
parser.add_argument('--mass', dest='mass',type=int)
parser.add_argument('-xmax', dest='xmax',type=float, default = 3.5)


args = parser.parse_args()
signalstrength = []

for sig in args.integer:
    signalstrength.append(sig)


c2 = TCanvas()

hist = TH1F("hist","hist",50,0,args.xmax)

for sig in signalstrength:
    
    infile = TFile("fitDiagnosticssignal"+str(sig)+"_"+str(args.mass)+".root","r")
    tree_fit_sb = infile.Get("tree_fit_sb")
    tree_fit_sb.Draw("r>>h(100,-2,5)")
    histo = gROOT.FindObject("h")
    mean = histo.GetMean()
    std = histo.GetStdDev()
    
    hist.SetBinContent(hist.FindBin(sig),mean)
    hist.SetBinError(hist.FindBin(sig),std)

    gStyle.SetOptStat(1)
    name_sig = str(sig)
    c2.SaveAs("r_"+name_sig.replace(".","p")+".eps")
    
gStyle.SetOptStat(0)
c1 = TCanvas()    
f= TF1("f","[0]+[1]*x",0,args.xmax)
hist.Fit(f)
hist.GetXaxis().SetTitle("Injected signal cross section [pb]")
hist.GetYaxis().SetTitle("Mean(Fitted signal cross section) [pb]")
hist.Draw()

#tree_fit_sb.Draw("(r-"+str(signalstrength)+")/rErr>>h(20,-4,4)")
#tree_fit_sb.Draw("r>>h(50,-4,4)")
#tree_fit_sb.Draw("rErr>>h(40,-4,4)")


#histo = gROOT.FindObject("h")
#histo.GetXaxis().SetTitle("(r-"+str(signalstrength)+")/rErr")
#histo.GetXaxis().SetTitle("r")
#histo.GetYaxis().SetTitle("Events")
#gPad.Update()



filename = args.outname
c1.Print(filename+".eps")
c1.Print(filename+".pdf")
