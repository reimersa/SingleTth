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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE,kBlack, TObject
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")

# parser = argparse.ArgumentParser(description='Plot the bias.')
# parser.add_argument('integer', metavar='N', type=float,nargs='+',
#                    help='Singal stregth to plot')
# parser.add_argument('--name', dest='outname',type=str)
# parser.add_argument('-xmax', dest='xmax',type=float, default = 3.5)

# args = parser.parse_args()

c2 = TCanvas()

# exp vs exp
infile = TFile("fitDiagnosticsexp.root","r")
TH1.AddDirectory(0)
tree_fit_sb_exp = infile.Get("tree_fit_sb")
tree_fit_sb_exp.Draw("(r-1)/rErr>>h(50,-4,6.5)")
histo_exp = gROOT.FindObject("h")
histo_exp.SetLineColor(kRed)
histo_exp.SetLineWidth(2)
mean_exp = histo_exp.GetMean()

# 2p vs exp
infile = TFile("fitDiagnosticsdijet2p.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet2p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet2p.Draw("(r-1)/rErr>>h2p(50,-4,6.5)")
histo_dijet2p = gROOT.FindObject("h2p")
histo_dijet2p.SetLineColor(kBlue)
histo_dijet2p.SetLineWidth(2)
histo_dijet2p.SetLineStyle(5)
mean_dijet2p = histo_dijet2p.GetMean()


# 3p vs exp
infile = TFile("fitDiagnosticsdijet3p.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet3p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet3p.Draw("(r-1)/rErr>>h3p(50,-4,6.5)")
histo_dijet3p = gROOT.FindObject("h3p")
histo_dijet3p.SetLineColor(kBlack)
histo_dijet3p.SetLineWidth(2)
histo_dijet3p.SetLineStyle(2)
mean_dijet3p = histo_dijet3p.GetMean()

histo_exp.GetXaxis().SetTitle("(r-1)/rErr")
histo_exp.GetYaxis().SetTitle("Toys")
histo_exp.SetMaximum(120)
histo_exp.Draw("HIST")
histo_dijet2p.Draw("same")
histo_dijet3p.Draw("same")

text = CMSPlotStyle.draw_info("Bias test: r=1",0.55,0.85,11)
text.Draw("same")

text2 = CMSPlotStyle.draw_info("Fit function: exp.",0.55,0.8,11)
text2.Draw("same")

leg = TLegend(0.55,0.55,0.85,0.79, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
#leg.SetHeader("#splitline{Bias test: r=1}{#splitline{Fit function: exp.}{Generation with}}")
leg.SetHeader("Generation with")
leg.AddEntry(histo_exp,"exp. (<r/#sigma_{r}> = %.2f)"%(mean_exp),"l")
leg.AddEntry(histo_dijet2p,"dijet 2p (<r/#sigma_{r}> = %.2f)"%(mean_dijet2p),"l")
leg.AddEntry(histo_dijet3p,"dijet 3p (<r/#sigma_{r}> = %.2f)"%(mean_dijet3p),"l")
leg.Draw()

lumi = CMSPlotStyle.draw_lumi(True)
lumi.Draw("same")

c2.SaveAs("bias_r1.eps")
c2.SaveAs("bias_r1.pdf")

c3 = TCanvas()

# exp vs exp
infile = TFile("fitDiagnosticsexp_r0.root","r")
TH1.AddDirectory(0)
tree_fit_sb_exp = infile.Get("tree_fit_sb")
tree_fit_sb_exp.Draw("(r)/rErr>>h(50,-4,7.5)")
histo_exp = gROOT.FindObject("h")
histo_exp.SetLineColor(kRed)
histo_exp.SetLineWidth(2)
mean_exp = histo_exp.GetMean()

# 2p vs exp
infile = TFile("fitDiagnosticsdijet2p_r0.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet2p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet2p.Draw("(r)/rErr>>h2p(50,-4,7.5)")
histo_dijet2p = gROOT.FindObject("h2p")
histo_dijet2p.SetLineColor(kBlue)
histo_dijet2p.SetLineWidth(2)
histo_dijet2p.SetLineStyle(5)
mean_dijet2p = histo_dijet2p.GetMean()


# 3p vs exp
infile = TFile("fitDiagnosticsdijet3p_r0.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet3p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet3p.Draw("(r)/rErr>>h3p(50,-4,7.5)")
histo_dijet3p = gROOT.FindObject("h3p")
histo_dijet3p.SetLineColor(kBlack)
histo_dijet3p.SetLineWidth(2)
histo_dijet3p.SetLineStyle(2)
mean_dijet3p = histo_dijet3p.GetMean()

histo_exp.GetXaxis().SetTitle("r/#sigma_{r}")
histo_exp.GetYaxis().SetTitle("Toys")
histo_exp.SetMaximum(120)
histo_exp.Draw("HIST")
histo_dijet2p.Draw("same")
histo_dijet3p.Draw("same")

text = CMSPlotStyle.draw_info("Bias test: r=0",0.55,0.85,11)
text.Draw("same")

text2 = CMSPlotStyle.draw_info("Fit function: exp.",0.55,0.8,11)
text2.Draw("same")

leg = TLegend(0.55,0.55,0.85,0.79, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
#leg.SetHeader("#splitline{Bias test: r=1}{#splitline{Fit function: exp.}{Generation with}}")
leg.SetHeader("Generation with")
leg.AddEntry(histo_exp,"exp. (<r/#sigma_{r}> = %.2f)"%(mean_exp),"l")
leg.AddEntry(histo_dijet2p,"dijet 2p (<r/#sigma_{r}> = %.2f)"%(mean_dijet2p),"l")
leg.AddEntry(histo_dijet3p,"dijet 3p (<r/#sigma_{r}> = %.2f)"%(mean_dijet3p),"l")
leg.Draw()

lumi = CMSPlotStyle.draw_lumi(True)
lumi.Draw("same")


c3.SaveAs("bias_r0.eps")
c3.SaveAs("bias_r0.pdf")

c4 = TCanvas()

# exp vs exp
infile = TFile("fitDiagnosticsexp.root","r")
TH1.AddDirectory(0)
tree_fit_sb_exp = infile.Get("tree_fit_sb")
tree_fit_sb_exp.Draw("(r-1)/rErr>>h(50,-4,6.5)")
histo_exp = gROOT.FindObject("h")
histo_exp.SetLineColor(kRed)
histo_exp.SetLineWidth(2)
mean_exp = histo_exp.GetMean()

# exp vs exp wo rate
infile = TFile("fitDiagnosticsexp_worate.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet2p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet2p.Draw("(r-1)/rErr>>h2p(50,-4,6.5)")
histo_dijet2p = gROOT.FindObject("h2p")
histo_dijet2p.SetLineColor(kBlue)
histo_dijet2p.SetLineWidth(2)
histo_dijet2p.SetLineStyle(5)
mean_dijet2p = histo_dijet2p.GetMean()


# exp vs exp wo unc
infile = TFile("fitDiagnosticsexp_wounc.root","r")
TH1.AddDirectory(0)
tree_fit_sb_dijet3p = infile.Get("tree_fit_sb")
tree_fit_sb_dijet3p.Draw("(r-1)/rErr>>h3p(50,-4,6.5)")
histo_dijet3p = gROOT.FindObject("h3p")
histo_dijet3p.SetLineColor(kBlack)
histo_dijet3p.SetLineWidth(2)
histo_dijet3p.SetLineStyle(2)
mean_dijet3p = histo_dijet3p.GetMean()

histo_exp.GetXaxis().SetTitle("(r-1)/rErr")
histo_exp.GetYaxis().SetTitle("Toys")
histo_exp.SetMaximum(120)
histo_exp.Draw("HIST")
histo_dijet2p.Draw("same")
histo_dijet3p.Draw("same")

text = CMSPlotStyle.draw_info("Bias test: r=1",0.55,0.85,11)
text.Draw("same")

text2 = CMSPlotStyle.draw_info("Fit function: exp.",0.55,0.8,11)
text2.Draw("same")

leg = TLegend(0.55,0.5,0.85,0.79, "","brNDC")
leg.SetBorderSize(0);	
leg.SetFillStyle(0);
leg.SetTextSize(0.035);
leg.SetTextFont(42);
#leg.SetHeader("#splitline{Bias test: r=1}{#splitline{Fit function: exp.}{Generation with}}")
leg.SetHeader("Generation with exp.")
leg.AddEntry(histo_exp,"with unc. (<#mu> = %.2f)"%(mean_exp),"l")
leg.AddEntry(histo_dijet2p,"#splitline{wo signal yield}{(<#mu> = %.2f)}"%(mean_dijet2p),"l")
leg.AddEntry(histo_dijet3p,"wo unc. (<#mu> = %.2f)"%(mean_dijet3p),"l")
leg.Draw()

lumi = CMSPlotStyle.draw_lumi(True)
lumi.Draw("same")


c4.SaveAs("bias_wounc.eps")
c4.SaveAs("bias_wounc.pdf")
