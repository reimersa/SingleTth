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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, kOrange
from collections import OrderedDict


infile = TFile("fitDiagnostics.root","r")
tree = infile.Get("tree_fit_sb")

list_param = ["bgexp2_p0much_2016v3","bgexp2_p1much_2016v3","bgexp2_p0ech_2016v3","bgexp2_p1ech_2016v3"]



for el in list_param:
    c = TCanvas()

    tree.SetLineColor(kOrange)
    tree.Draw(el,"r>1.2")
    tree.SetLineColor(kBlue)
    tree.Draw(el,"r<0.5","same")
    tree.SetLineColor(kRed)
    tree.Draw(el,"r<0", "same")

    print el
    raw_input()
    c.SaveAs(el+".eps")


