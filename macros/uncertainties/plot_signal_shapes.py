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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlack, TPad, kTRUE, kBlue, kMagenta, kGray, kSienna
from collections import OrderedDict
import CMSPlotStyle

gROOT.SetBatch(kTRUE)


inputfile = open("/nfs/dust/cms/user/abenecke/CMSSW_10_2_10/CMSSW_10_2_10/src/UHH2/SingleTth/macros/sigfits/SignalFitOutput.txt","r")


CMSPlotStyle.right_margin = 0.14
style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptFit(000000000)

c = TCanvas()
leg=TLegend(0.5,0.5,0.7,0.7,"","brNDC")
leg.SetBorderSize(0);
leg.SetTextSize(0.035);
leg.SetFillColor(0);
leg.SetFillStyle(0);
leg.SetLineColor(1);
leg.SetTextFont(42);


gaussfunction = TF1("gauss","exp(-0.5*((x-[0])/[1])^2)",380,1250)
gaussfunction.SetParameter(0,584)
gaussfunction.SetParameter(1,60)

gaussfunction.DrawCopy()

content = inputfile.readlines()
content = [x.strip() for x in content] 
ii=0
for line in content:

    if "meanfit" in line and "param0" in line:
        print re.findall(r'\d+\.*\d+', line)[0]
        print re.findall(r'\d+\.*\d+',content[ii+1])[0]
        param0 = float(re.findall(r'\d+\.*\d+', line)[0])
        param1 = float(re.findall(r'\d+\.*\d+', content[ii+1])[0])
        print param1
        gaussfunction.SetParameter(0,param0)
        gaussfunction.SetParameter(1,param1)
        gaussfunction.DrawCopy("same")
#        leg.AddEntry(gaussfunction, "Nominal", "l")

        # gaussfunction.SetParameter(0,re.findall(r'\d+\.*\d+', line)[0] + re.findall(r'\d+\.*\d+', line)[1])
        # gaussfunction.SetParameter(1,re.findall(r'\d+\.*\d+', line)[0]+ 0.5 * re.findall(r'\d+\.*\d+', line)[1])
        # gaussfunction.DrawCopy("same")
        # leg.AddEntry("+1#sigma")

        
        
    
    ii+=1
        


c.Print("SignalShapes.pdf")
c.Print("SignalShapes.eps")



