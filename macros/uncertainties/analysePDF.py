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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, Double, TMultiGraph
from collections import OrderedDict
import CMSPlotStyle


gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat("2.3f")

year = "2017"
MT = "800"
MA = "450"

histfolder="catma300_much_sr_pdf/"
histname="M_Tprime_PDF"

yeartag = ""
if "2016" in year: yeartag = "v3"
if "2017" in year: yeartag = "v2"

infolder = "/nfs/dust/cms/user/reimersa/SingleTth/"+year+"/Finalselection/mavariable/NOMINAL/"


infile = TFile(infolder+"uhh2.AnalysisModuleRunner.MC.VLQ_LH_"+MT+"_ma"+MA+"_"+year+yeartag+".root")

c = TCanvas()

#for k in range(1,101):
for k in range(1,2):
    print k
    hist = infile.Get(histfolder+histname+"_"+str(k))
    hist.SetLineColor(k)
    hist.GetXaxis().SetRangeUser(200,1000)
    hist.Draw("HIST same")

hist_nom = infile.Get(histfolder.replace("_pdf","_nominal")+histname.replace("_PDF",""))
hist_nom.SetLineColor(kRed)
hist_nom.SetLineWidth(2)
hist_nom.Draw("HIST same")


c.Print("PDF_variations_MT"+MT+"_MA"+MA+".pdf")
