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






def create_plot(infilename_toys, infilename_data, outfilename, year, ch, MT, MA,signalparam):
    print "Toys: " + infilename_toys
    print "Data: " + infilename_data
    c1 = TCanvas()
    infile = TFile(infilename_toys+".root","r")
    tree_fit_sb = infile.Get("limit")
    tree_fit_sb.Draw("limit>>h(50)")
    
    
    histo = gROOT.FindObject("h")
    histo.GetXaxis().SetTitle("Saturated test statistic")
    histo.GetYaxis().SetTitle("Toys")
    gPad.Update()
    
    
    infile_data = TFile(infilename_data+".root","r")
    tree_data = infile_data.Get("limit")
    tree_data.Draw("limit>>h_data(50)")
    histo_data = gROOT.FindObject("h_data")
    
    tree_fit_sb.Draw("limit>>h(50)")

    ar4 = TArrow(histo_data.GetMean(),0,histo_data.GetMean(),10,0.05,"<|");
    ar4.SetLineWidth(2);
    ar4.SetFillColor(2);
    ar4.SetLineColor(2);
    ar4.Draw();
    
    infotext = year + ", "+ch+ ", "+str(MT)+ ", "+ str(MA)+ ", "+str(signalparam)
    info = CMSPlotStyle.draw_info(infotext, 0.6, 0.95, textalign = 31, factor = 1)

    c1.Print(outfilename+".pdf")



#uebergeben von run_analysis genauso wie MT
#signalparams = ["float", "fixed"]

def plot(MT, MA, signalparams, year, ch):


    for signalparam in signalparams:
        infilename_base = "higgsCombineGOF"+signalparam+"R_XXX_M"+str(MT)+"_MA"+str(MA)+".GoodnessOfFit.mH120"
        #infilename_base = "higgsCombineGOF"+signalparam+"R_XXX_M"+str(MT)+".GoodnessOfFit.mH120.123456"
        infilename_toys = infilename_base.replace("XXX","toys")+".123456"
        infilename_data = infilename_base.replace("XXX","data")
        outfilename = "GOF_"+signalparam+"_MT"+str(MT) + "_MA"+str(MA)+"_"+year
        create_plot(infilename_toys,infilename_data, outfilename, year, ch, MT, MA,signalparam)
