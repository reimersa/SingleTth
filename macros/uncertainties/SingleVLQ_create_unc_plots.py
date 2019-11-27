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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlack, TPad, kTRUE
from collections import OrderedDict

### USAGE: 



class bcolors:
    HEADER = '\033[95m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW  = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    BR = '\033[94m' #BLue
    PROCESS = '\033[93m' #Yellow
    CHANNEL = '\033[95m' #Magenta
    SYSTEMATIC = '\033[96m' #Cyan

gStyle.SetOptStat(0)
gROOT.SetBatch(kTRUE)

berror = False

yplot = 0.65;
yratio = 0.34;

y3 = 0.99
y2 = y3-yplot
y1 = y2-yratio+0.00001
x1 = 0.01
x2 = 0.99


#read in postfit file
inputfile = TFile("/nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/input/theta_histograms_fullsyst.root","READ")
TH1.AddDirectory(0)

outputfolder = "/nfs/dust/cms/user/reimersa/SingleTth/uncertaintyPlots/"


#list of uncertainties in CR and SR
systname_SR_muon={"muid","pu","eleid","elereco","eletrigger","muiso","mutrigger","btag_bc","btag_udsg","JEC","JER","pdf"}

#list of histograms in SR
hists_SR = {"MTprime_much_sr"}


# loop over channels and process
channel = {"muon"}
process = {"TTbar_2016v3","VLQ_RH_600_2016v2"}


i=0
for ch in channel:
    for pro in process:

#go over all histograms
        for hist in hists_SR:
#read in nominal histo of SR
            nominal_name = hist+"__"+pro
            nominal_hist_SR = inputfile.Get(nominal_name)
            print "got nominal histo "+nominal_name

#go over all uncertainties and create a plot per histogram in SR
            if "muon" in ch :
                unc_names = systname_SR_muon
            else:
                unc_names = systname_SR_elec
            for unc in unc_names:
                print "------------------" + unc
# create name of the histogram with up and down variation
                histname_up = nominal_name+"__"+unc+"__plus"
                histname_down = nominal_name+"__"+unc+"__minus"

# read in histogram and plot it
                hist_up = inputfile.Get(histname_up)
                hist_down = inputfile.Get(histname_down)
                
                i+=1
                c1 = TCanvas("c"+str(i),"",10,10,600,600)
                m_rp1_top =  TPad("pad1", "Control Plots 2", x1, y2, x2, y3);
                m_rp1 =  TPad("rp1", "Ratio2", x1, y1, x2, y2);
                m_pad1 = TPad("pad1", "Control Plots 2", x1, y1, x2, y3);

                m_rp1_top.SetTopMargin(0.065); 
                m_rp1_top.SetBottomMargin(0.0);  
                m_rp1_top.SetLeftMargin(0.19); 
                m_rp1_top.SetRightMargin(0.05);
                m_rp1.SetTopMargin(0.0);    
                m_rp1.SetBottomMargin(0.35);  
                m_rp1.SetLeftMargin(0.19);  
                m_rp1.SetRightMargin(0.05);

                m_pad1.Draw()
                m_rp1.Draw();
                m_rp1_top.Draw(); 
                c1.Update();
                m_rp1_top.cd(); 
                gPad.SetTickx();
                gPad.SetTicky();
                nominal_hist_SR.SetTitle("")
                nominal_hist_SR.Draw()
                hist_up.SetTitle("")
                hist_up.SetLineColor(kRed)
                hist_up.Draw("HIST same")
                hist_down.SetTitle("")
                hist_down.SetLineColor(kBlack)
                hist_down.Draw("HIST same")

                pt = TPaveText(.7,.7,.9,.9,"nbNDC")
                pt.AddText("SR "+ pro + " "+ch + " "+unc)
                pt.Draw()


#plot also ratio
                m_rp1.cd()
                

                ratio_up = hist_up.Clone()
                ratio_up.Add(nominal_hist_SR,-1)
                ratio_up.Divide(nominal_hist_SR)

                ratio_up.GetYaxis().SetTitle("#frac{sys-nom}{nom}");
                ratio_up.GetYaxis().SetNdivisions(505);




                ratio_down = hist_down.Clone()
                ratio_down.Add(nominal_hist_SR,-1)
                ratio_down.Divide(nominal_hist_SR)


                ratio_up.GetYaxis().SetRangeUser(ratio_down.GetMinimum()-0.01,ratio_up.GetMaximum()+0.01)
                ratio_up.Draw("HIST")
                ratio_down.Draw("HIST same")


                
                c1.Print(outputfolder+nominal_name+"__"+unc+".pdf")

