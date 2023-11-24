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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack, kMagenta, TPad, TLine, RooFit
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
bprefit = True
bpostfitb = True
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")

MT = 600

infile = TFile("fitDiagnosticsinitial_signal1.00_"+str(MT)+"_signal.root","r")
#infile_gen = TFile("higgsCombine1.00_"+str(MT)+"_signal.GenerateOnly.mH125.123456.root","r")
infile_gen = TFile("TEST.root","r")
#infile_post = TFile("fitDiagnosticstestToy__mavariable_wodR_all_ST_rebin_2018_ma125_allsyst_2pexp3dijet_signal1.00_"+str(MT)+".root")
infile_post = TFile("fitDiagnosticsoutToy__signal1.00_"+str(MT)+".root")

c = TCanvas()

yplot = 0.69
yratio = 0.25                                 #  y6 +-------------+  
y6 = 0.97                                     #     |     pad1    |
y5 = y6-yplot                                 #  y5 |-------------|
y4 = y5-yratio                                #     |     rp1     |
y4 = 0.01
x1 = 0.01                                     #  y4 +-------------+
x2 = 0.95
leftmargin = 0.15
rightmargin = 0.045

pad_top = TPad("pad", "Control Plots", x1, y5, x2, y6)
pad_top.SetTopMargin(0.05);
pad_top.SetBottomMargin(0.025);
pad_top.SetLeftMargin(leftmargin);
pad_top.SetRightMargin(rightmargin);
pad_top.Draw();
pad_ratio = TPad("rp", "Ratio", x1, y4, x2, y5);
pad_ratio.SetTopMargin(0);
pad_ratio.SetBottomMargin(0.47);
pad_ratio.SetLeftMargin(leftmargin);
pad_ratio.SetRightMargin(rightmargin);
pad_ratio.Draw();
pad_top.cd()

## Plot prefit s+b and b
sb_prefit = infile.Get("shapes_prefit/much_2018_chi2h_2/total")
sb_prefit.SetLineColor(kMagenta)
sb_prefit.SetLineWidth(2)
#scalefactor = int_toys/sb_prefit.Integral()
int_sbprefit = sb_prefit.Integral()
#if bscale: sb_prefit.Scale(scalefactor)
if bprefit: sb_prefit.Draw("HIST same")


## Plot toys
x = RooRealVar("xchi2h_22018", "m_{T} [GeV]", 400, 2000);
plotter=x.frame();
ds =  infile_gen.Get("toys/toy_1")
hist = ds.binnedClone()
int_toys = hist.sumEntries()
#hist.Rescale(sb_prefit.Integral()/int_toys)
print "Integral of Toys  " + str(hist.sumEntries())
hist.plotOn(plotter,RooFit.Rescale(sb_prefit.Integral()/int_toys))
c.SetLogy()
pad_top.SetLogy()
plotter.GetYaxis().SetRangeUser(0.001,10e2)
plotter.Draw()


# ## Plot prefit s+b and b
# sb_prefit = infile.Get("shapes_prefit/much_2018_chi2h_2/total")
# sb_prefit.SetLineColor(kMagenta)
# sb_prefit.SetLineWidth(2)
# scalefactor = int_toys/sb_prefit.Integral()
# int_sbprefit = sb_prefit.Integral()
# if bscale: sb_prefit.Scale(scalefactor)
# if bprefit: sb_prefit.Draw("HIST same")


b_prefit = infile.Get("shapes_prefit/much_2018_chi2h_2/total_background")
b_prefit.SetLineColor(kMagenta)
b_prefit.SetLineWidth(2)
b_prefit.SetLineStyle(2)
if bscale: b_prefit.Scale(scalefactor * (b_prefit.Integral() / int_sbprefit))
if bprefit: b_prefit.Draw("HIST same")

s_prefit = infile.Get("shapes_prefit/much_2018_chi2h_2/total_signal")
s_prefit.SetLineColor(kMagenta)
s_prefit.SetLineWidth(2)
s_prefit.SetLineStyle(2)
print("prefit",s_prefit.Integral())
if bscale: s_prefit.Scale(scalefactor * (s_prefit.Integral()/int_sbprefit))
if bprefit: s_prefit.Draw("HIST same")





## Plot postfit b
sb_postfit_bonly = infile_post.Get("shapes_fit_b/much_2018_chi2h_2/total")
sb_postfit_bonly.SetLineColor(kGreen+2)
sb_postfit_bonly.SetLineWidth(2)
if bscale: sb_postfit_bonly.Scale(int_toys/sb_postfit_bonly.Integral())
if bpostfitb: sb_postfit_bonly.Draw("HIST same")

b_postfit_bonly = infile_post.Get("shapes_fit_b/much_2018_chi2h_2/total_background")
b_postfit_bonly.SetLineColor(kGreen+2)
b_postfit_bonly.SetLineWidth(2)
b_postfit_bonly.SetLineStyle(2)
if bscale: b_postfit_bonly.Scale(1/b_postfit_bonly.Integral())
#if bpostfitb: b_postfit_bonly.Draw("HIST same")



## Plot postfit s+b
b_postfit_spb = infile_post.Get("shapes_fit_s/much_2018_chi2h_2/total_background")
b_postfit_spb.SetLineColor(kBlue)
b_postfit_spb.SetLineWidth(2)
b_postfit_spb.SetLineStyle(2)
if bscale: b_postfit_spb.Scale(int_toys/b_postfit_spb.Integral())
b_postfit_spb.Draw("HIST same")

s_postfit_spb = infile_post.Get("shapes_fit_s/much_2018_chi2h_2/total_signal")
s_postfit_spb.SetLineColor(kBlue)
s_postfit_spb.SetLineWidth(2)
s_postfit_spb.SetLineStyle(2)
print("postfit",s_postfit_spb.Integral())
#if bscale: s_postfit_spb.Scale(int_toys/s_postfit_spb.Integral())
s_postfit_spb.Draw("HIST same")

sb_postfit_spb = infile_post.Get("shapes_fit_s/much_2018_chi2h_2/total")
sb_postfit_spb.SetLineColor(kBlue)
sb_postfit_spb.SetLineWidth(2)
if bscale: sb_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
sb_postfit_spb.Draw("HIST same")


pad_ratio.cd()

ratio_prefit_postfitb = b_postfit_bonly.Clone()
ratio_prefit_postfitb.Divide( b_prefit)

ratio_prefit_postfitb.GetXaxis().SetRangeUser(400,2000)
ratio_prefit_postfitb.GetXaxis().SetTitleSize(0.25)
ratio_prefit_postfitb.GetXaxis().SetLabelSize(0.15)
ratio_prefit_postfitb.GetXaxis().SetTitleOffset(0.7)

ratio_prefit_postfitb.GetYaxis().SetRangeUser(0.8,1.2)
ratio_prefit_postfitb.GetYaxis().SetTitleSize(0.25)
ratio_prefit_postfitb.GetYaxis().SetLabelSize(0.1)
ratio_prefit_postfitb.GetYaxis().SetTitleOffset(0.7)

ratio_prefit_postfitb.GetYaxis().SetNdivisions(505)
ratio_prefit_postfitb.GetXaxis().SetNdivisions(510)

ratio_prefit_postfitspb_bonly = b_postfit_spb.Clone()
ratio_prefit_postfitspb_bonly.Divide( b_prefit)

ratio_prefit_prefitspb = sb_prefit.Clone()
ratio_prefit_prefitspb.Divide( b_prefit)

ratio_prefit_postfitspb = sb_postfit_spb.Clone()
ratio_prefit_postfitspb.Divide( b_prefit)

# ratio_prefit_toys = hist.Clone()
# ratio_prefit_toys.Divide( b_prefit)


ratio_prefit_postfitb.Draw("LP")
ratio_prefit_postfitspb_bonly.Draw("same")
ratio_prefit_prefitspb.Draw("same")
ratio_prefit_postfitspb.Draw("same")
#ratio_prefit_toys.Draw("same")

line = TLine (400,1,2000,1)
line.SetLineColor(kRed)
line.SetLineStyle(kDashed)
line.Draw("same")


gPad.Update();
gPad.RedrawAxis();


pad_top.cd()

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

#c.SetLogy()
c.SaveAs("shapes_signalinjection.pdf")
