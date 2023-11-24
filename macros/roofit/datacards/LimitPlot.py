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


def read_limits(postfix, b_only125 = False):
    berror = False
    #year = "2016v3"
    #year = "2017v2"
    #year = "2018"
    year = "allyears"
    #    postfix = "_mavariable_Run2_onlyexp_ma125_allsyst"
    
    postfix_only125 = ""
    if b_only125: postfix_only125 = "_only125"
    
    masses = []
    
    if "allyears" in year:
        inputfile = open("AnalysisOutput_2018_chi2h_2.txt","r")
    else:
        inputfile = open("AnalysisOutput_"+year+"_chi2h_2.txt","r")
    
    listOfLines = inputfile.readlines()
    i=0
    for line in listOfLines:
        print line
        if "Muon" in line and "Signal" in line:
            print "in Muon"
            print line
            for j in range(i+1,len(listOfLines)):
                if "Electron" in listOfLines[j]: break
                mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
                masses.append(mass)
        i+=1
    
    print masses

    outputfile = open("Limits_combine_"+year+postfix+".txt","w")
    outputfile_obs = open("Limits_combine_obs_"+year+postfix+".txt","w")
    outputfile_theta = open("Limits_combine_theta_"+year+postfix+".txt","w")


    for mass in masses:
        if "550" in mass or "575" in mass: continue
        if b_only125 and int(mass) > 700: continue 
        if "ma75" in postfix and int(mass) > 1000: continue 
        if int(mass)>=1200: continue
        print mass
        rootfile = TFile.Open("higgsCombineoutput_"+mass+postfix_only125+".AsymptoticLimits.mH120.root","r")
        tree = rootfile.limit 
        limits = []
        for event in tree:
            limits.append(event.limit)
    
        outputfile.write(mass+"\t"+str(limits[2])+"\t"+str(limits[0])+"\t"+str(limits[4])+"\t"+str(limits[1])+"\t"+str(limits[3])+"\t")
        outputfile_obs.write(mass+"\t"+str(limits[5])+"\t")
        outputfile_theta.write(str(limits[2])+", ")
    
    
    outputfile.close()
    outputfile_obs.close()
    outputfile_theta.close()
    
    
    x = open("Limits_combine_"+year+postfix+".txt").read()
    open("Limits_combine_"+year+postfix+".txt","w").write(x[:-1])
    
    
