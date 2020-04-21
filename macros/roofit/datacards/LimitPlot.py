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
#year = "2016v3"
#year = "2017v2"
#year = "2018"
year = "allyears"

postfix = "_mediumWP_allyearsunc_exp"

masses = []

if "allyears" in year:
    inputfile = open("AnalysisOutput_2016v3.txt","r")
else:
    inputfile = open("AnalysisOutput_"+year+".txt","r")

listOfLines = inputfile.readlines()
i=0
for line in listOfLines:
    if "Muon" in line and "Signal" in line:
        for j in range(i+1,len(listOfLines)):
            if "Electron" in listOfLines[j]: break
            mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
            masses.append(mass)
    i+=1

print masses
#masses = ['550', '575', '600', '625', '650', '675', '700', '725', '750', '775', '800', '825', '875', '925', '950', '1000','1025', '1050', '1075', '1100', '1125', '1150', '1175', '1200', '1225']

outputfile = open("Limits_combine_"+year+postfix+".txt","w")
for mass in masses:
    print mass
    rootfile = TFile.Open("higgsCombineoutput_"+mass+".AsymptoticLimits.mH120.root","r")
    tree = rootfile.limit 
    limits = []
    for event in tree:
        limits.append(event.limit)

    outputfile.write(mass+"\t"+str(limits[2])+"\t"+str(limits[0])+"\t"+str(limits[4])+"\t"+str(limits[1])+"\t"+str(limits[3])+"\t")


outputfile.close()


x = open("Limits_combine_"+year+postfix+".txt").read()
open("Limits_combine_"+year+postfix+".txt","w").write(x[:-1])
