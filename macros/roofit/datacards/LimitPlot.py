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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree
from collections import OrderedDict


berror = False


masses = []
inputfile = open("AnalysisOutput2016.txt","r")
listOfLines = inputfile.readlines()
i=0
for line in listOfLines:
    if "Muon" in line and "Signal" in line:
        for j in range(i+1,len(listOfLines)):
            if "Electron" in listOfLines[j]: break
            mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
            masses.append(mass)
    i+=1


#masses = ['600']
limits = []
outputfile = open("Limits_combine.txt","w")
for mass in masses:
    rootfile = TFile.Open("higgsCombineoutput_"+mass+".AsymptoticLimits.mH120.root","r")
    tree = rootfile.limit 
    for event in tree:
        limits.append(event.limit)

    outputfile.write(mass+"\t"+str(limits[2])+"\t"+str(limits[0])+"\t"+str(limits[4])+"\t"+str(limits[1])+"\t"+str(limits[3])+"\t")

outputfile.close()
