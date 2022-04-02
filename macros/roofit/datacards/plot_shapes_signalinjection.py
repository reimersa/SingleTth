
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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack, kMagenta
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = True
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")


infile = TFile("fitDiagnosticssignal1.0_600_200.root","r")
infile_gen = TFile("higgsCombine1.0_600_signal.GenerateOnly.mH125.123456.root","r")

c = TCanvas()
## Plot toys
x = RooRealVar("xchi2h_22017v2", "m_{T} [GeV]", 400, 2000);
plotter=x.frame();
ds =  infile_gen.Get("toys/toy_1")
hist = ds.binnedClone()
int_toys = hist.sumEntries()
print "Integral of Toys  " + str(hist.sumEntries())
hist.plotOn(plotter)
plotter.Draw()


## Plot prefit s+b and b
b_prefit = infile.Get("shapes_prefit/much_2017v2_chi2h_2/total_background")
b_prefit.SetLineColor(kMagenta)
b_prefit.SetLineWidth(2)
b_prefit.SetLineStyle(2)
if bscale: b_prefit.Scale(int_toys/b_prefit.Integral())
b_prefit.Draw("HIST same")

s_prefit = infile.Get("shapes_prefit/much_2017v2_chi2h_2/total_signal")
s_prefit.SetLineColor(kMagenta)
s_prefit.SetLineWidth(2)
s_prefit.SetLineStyle(2)
if bscale: s_prefit.Scale(int_toys/b_prefit.Integral())
s_prefit.Draw("HIST same")

sb_prefit = infile.Get("shapes_prefit/much_2017v2_chi2h_2/total")
sb_prefit.SetLineColor(kMagenta)
sb_prefit.SetLineWidth(2)
if bscale: sb_prefit.Scale(int_toys/b_prefit.Integral())
sb_prefit.Draw("HIST same")




## Plot postfit b
sb_postfit_bonly = infile.Get("shapes_fit_b/much_2017v2_chi2h_2/total")
sb_postfit_bonly.SetLineColor(kGreen+2)
sb_postfit_bonly.SetLineWidth(2)
if bscale: sb_postfit_bonly.Scale(int_toys/sb_postfit_bonly.Integral())
sb_postfit_bonly.Draw("HIST same")

b_postfit_bonly = infile.Get("shapes_fit_b/much_2017v2_chi2h_2/total_background")
b_postfit_bonly.SetLineColor(kGreen+2)
b_postfit_bonly.SetLineWidth(2)
b_postfit_bonly.SetLineStyle(2)
if bscale: b_postfit_bonly.Scale(1/b_postfit_bonly.Integral())
#b_postfit_bonly.Draw("HIST same")



## Plot postfit s+b
b_postfit_spb = infile.Get("shapes_fit_s/much_2017v2_chi2h_2/total_background")
b_postfit_spb.SetLineColor(kBlue)
b_postfit_spb.SetLineWidth(2)
b_postfit_spb.SetLineStyle(2)
if bscale: b_postfit_spb.Scale(int_toys/b_postfit_spb.Integral())
b_postfit_spb.Draw("HIST same")

s_postfit_spb = infile.Get("shapes_fit_s/much_2017v2_chi2h_2/total_signal")
s_postfit_spb.SetLineColor(kBlue)
s_postfit_spb.SetLineWidth(2)
s_postfit_spb.SetLineStyle(2)
if bscale: s_postfit_spb.Scale(int_toys/b_postfit_spb.Integral())
s_postfit_spb.Draw("HIST same")

sb_postfit_spb = infile.Get("shapes_fit_s/much_2017v2_chi2h_2/total")
sb_postfit_spb.SetLineColor(kBlue)
sb_postfit_spb.SetLineWidth(2)
if bscale: sb_postfit_spb.Scale(int_toys/b_postfit_spb.Integral())
sb_postfit_spb.Draw("HIST same")



leg=TLegend(0.5,0.5,0.7,0.7,"","brNDC")
leg.SetBorderSize(0);
leg.SetTextSize(0.035);
leg.SetFillColor(0);
leg.SetFillStyle(0);
leg.SetLineColor(1);
leg.SetTextFont(42);

leg.AddEntry(plotter,"Toys","lpe")
leg.AddEntry(sb_prefit,"Total prefit","l")
leg.AddEntry(sb_postfit_bonly,"Total b only postfit","l")
leg.AddEntry(sb_postfit_spb,"Total s+b only postfit","l")

leg.Draw()

c.SetLogy()
c.SaveAs("shapes_signalinjection.pdf")
