
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


signalstrength = {0.0,0.25,0.5,0.75,1,1.5,2,2.5,3}
masses = {600,800,1000}

color = [kBlue,kRed,kBlack]
style = [0,5,2]
marker = [20,21,22]

gStyle.SetOptStat(0)

ymax =3.1

hist_to_draw = {}

#hist = TH1F("hist","hist",50,0,3)

i=0
for mass in masses:
    print "============"
    print mass
    hist = TH1F("hist"+str(mass),"hist"+str(mass),50,0,3.5)
    hist_doublegaussian = TH1F("hist_doublegaussian"+str(mass),"hist_doublegaussian"+str(mass),50,0,3.5)
    if mass > 600: ymax = 1
    f= TF1("f","[0]+[1]*x",0,ymax)

    for sig in signalstrength:
        if mass > 600 and sig > 1: continue
        print sig
        infile = TFile("fitDiagnosticssignal"+str(sig)+"_"+str(mass)+".root","r")
        TH1.AddDirectory(0)
        tree_fit_sb = infile.Get("tree_fit_sb")
        tree_fit_sb.Draw("r>>h(100,-2,5)")
        histo = gROOT.FindObject("h")
        mean = histo.GetMean()
        std = histo.GetStdDev()
    
        hist.SetBinContent(hist.FindBin(sig),mean)
        hist.SetBinError(hist.FindBin(sig),std)
    
        # if mass==600 or mass==1000:
        #     infile = TFile("fitDiagnosticssignal"+str(sig)+"_"+str(mass)+"_doublegaussian.root","r")
        #     TH1.AddDirectory(0)
        #     tree_fit_sb = infile.Get("tree_fit_sb")
        #     tree_fit_sb.Draw("r>>h(100,-2,5)")
        #     histo = gROOT.FindObject("h")
        #     mean = histo.GetMean()
        #     std = histo.GetStdDev()
    
        #     hist_doublegaussian.SetBinContent(hist_doublegaussian.FindBin(sig),mean)
        #     hist_doublegaussian.SetBinError(hist_doublegaussian.FindBin(sig),std)
    
    
    hist.GetXaxis().SetTitle("Injected signal cross section [pb]")
    hist.GetYaxis().SetTitle("Mean(Fitted signal cross section) [pb]")
    f.SetLineColor(color[i])
    f.SetLineWidth(2)
    f.SetLineStyle(style[i])
    hist.SetLineWidth(2)
    hist.SetMarkerStyle(marker[i])
    hist.SetMarkerColor(color[i])
    hist.SetMarkerSize(1.5)
    hist.GetYaxis().SetRangeUser(0,3.5)
    hist.Fit(f,"R")
    hist_to_draw[str(i)+"M_{T} = "+str(mass)]  = hist.Clone()
    # if mass==600or mass==1000:
    #     hist_doublegaussian.SetLineWidth(2)
    #     hist_doublegaussian.SetMarkerStyle(marker[i])
    #     hist_doublegaussian.SetMarkerColor(color[i])
    #     hist_doublegaussian.SetMarkerSize(1.5)
    #     hist_doublegaussian.GetYaxis().SetRangeUser(0,3.5)
    #     hist_doublegaussian.Fit(f,"R")
    #     hist_to_draw[str(i)+"M_{T} = "+str(mass)+" with low mass tail"]  = hist.Clone()


    i+=1


c1 = TCanvas()   
gStyle.SetOptFit(0) 
leg = TLegend(0.2,0.6,0.55,0.9, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
leg.SetHeader("Signal mass")
fl= TF1("fl","[0]+[1]*x",0,3.5)
fl.SetParameter(0,0)
fl.SetParameter(1,1)

i=0
for key in sorted(hist_to_draw):
    print key
    hist = hist_to_draw[key]

    hist.Draw("P same")
    hist.SetLineStyle(style[i])
    hist.SetLineColor(color[i])
    leg.AddEntry(hist,key[1:],"lp")    
    i+=1

fl.SetLineWidth(1)
fl.SetLineStyle(kDashed)
fl.Draw("same")
leg.AddEntry(fl,"X=Y","l")
leg.Draw()
lumi = CMSPlotStyle.draw_lumi(True)
lumi.Draw("same")


c1.Print("signalinjection.eps")
c1.Print("signalinjection.pdf")
