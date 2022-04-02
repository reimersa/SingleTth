
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
b_draw_total = False
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")


channels = ["much","ech"]
years = ["2016v3","2017v2","2018"]
categories = ["chi2h_2","catma90"]




# cat = "chi2h_2"
# year = "2017v2"
# channel = "much"
def plot_fits(cat,year,channel):

    infile = TFile("fitDiagnosticsindividualfit.root","r")
    infile_gen = TFile("higgsCombineindividualfit.FitDiagnostics.mH120.root","r")
    infile_initial = TFile("ws_SingleTth_"+year+"_"+cat+"_125.root","r")
    
    c = TCanvas()
    int_toys = 1
    ## Plot toys
    x = RooRealVar("x"+cat+""+year, "m_{T} [GeV]", 400, 1990);
    plotter=x.frame();
    # ds =  infile_gen.Get("toys/toy_1")
    # hist = ds.binnedClone()
    # int_toys = hist.sumEntries()
    # print "Integral of Toys  " + str(hist.sumEntries())
    # hist.plotOn(plotter)
    #plotter.Draw()
    
    
    w = infile_initial.Get('SingleTth'+cat)
    data = w.data("data_obs_"+channel+"_"+year+"_"+cat)
    
    #w = infile_gen.Get('w')
    #data = w.data("data_obs")
    #w.Print()
    #data.Print()
    int_toys = data.sumEntries("0 < x"+cat+""+year+" && x"+cat+""+year+" < 1990") 
    print int_toys
    print data.sumEntries() 
    data.plotOn(plotter)
    
    ## Plot prefit s+b and b
    sb_prefit = infile.Get("shapes_prefit/"+channel+"_"+year+"_"+cat+"/total")
    sb_prefit.SetLineColor(kMagenta)
    sb_prefit.SetLineWidth(2)
    
    b_prefit = infile.Get("shapes_prefit/"+channel+"_"+year+"_"+cat+"/total_background")
    b_prefit.SetLineColor(kMagenta)
    b_prefit.SetLineWidth(2)
    b_prefit.SetLineStyle(2)
    if bscale: b_prefit.Scale(int_toys/sb_prefit.Integral())
    #b_prefit.GetYaxis().SetRangeUser(14,17)
    #b_prefit.GetXaxis().SetRangeUser(500,800)
    b_prefit.Draw("HIST same")

    s_prefit = infile.Get("shapes_prefit/"+channel+"_"+year+"_"+cat+"/total_signal")
    s_prefit.SetLineColor(kMagenta)
    s_prefit.SetLineWidth(2)
    s_prefit.SetLineStyle(2)
    if bscale: s_prefit.Scale(int_toys/sb_prefit.Integral())
    s_prefit.Draw("HIST same")
    
    if bscale: sb_prefit.Scale(int_toys/sb_prefit.Integral())
    if b_draw_total: sb_prefit.Draw("HIST same")
    
    
    
    
    ## Plot postfit b
    sb_postfit_bonly = infile.Get("shapes_fit_b/"+channel+"_"+year+"_"+cat+"/total")
    sb_postfit_bonly.SetLineColor(kGreen+2)
    sb_postfit_bonly.SetLineWidth(2)
    if bscale: sb_postfit_bonly.Scale(int_toys/sb_postfit_bonly.Integral())
    #if b_draw_total: sb_postfit_bonly.Draw("HIST same")
    
    b_postfit_bonly = infile.Get("shapes_fit_b/"+channel+"_"+year+"_"+cat+"/total_background")
    b_postfit_bonly.SetLineColor(kGreen+2)
    b_postfit_bonly.SetLineWidth(2)
    b_postfit_bonly.SetLineStyle(1)
    if bscale: b_postfit_bonly.Scale(int_toys/b_postfit_bonly.Integral())
    b_postfit_bonly.Draw("HIST same")
    
    
    
    ## Plot postfit s+b
    sb_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total")
    sb_postfit_spb.SetLineColor(kBlue)
    sb_postfit_spb.SetLineWidth(2)
    
    b_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total_background")
    b_postfit_spb.SetLineColor(kBlue)
    b_postfit_spb.SetLineWidth(2)
    b_postfit_spb.SetLineStyle(2)
    if bscale: b_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
    b_postfit_spb.Draw("HIST same")
    
    s_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total_signal")
    s_postfit_spb.SetLineColor(kBlue)
    s_postfit_spb.SetLineWidth(2)
    s_postfit_spb.SetLineStyle(2)
    if bscale: s_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
    s_postfit_spb.Draw("HIST same")
    
    if bscale: sb_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
    if b_draw_total: sb_postfit_spb.Draw("HIST same")
    
    plotter.Draw("same")
    
    
    leg=TLegend(0.5,0.5,0.7,0.7,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);
    leg.SetHeader(channel+", "+cat+", "+year)
    
    #leg.AddEntry(plotter,"Toys","lpe")
    leg.AddEntry(sb_prefit,"Total prefit","l")
    leg.AddEntry(sb_postfit_bonly,"Total b only postfit","l")
    leg.AddEntry(sb_postfit_spb,"Total s+b only postfit","l")
    
    leg.Draw()
    
    #c.SetLogy()
    c.SaveAs("individualfit_"+channel+cat+year+".pdf")
    

def plot_comp(cat, year, channel):

    infile = TFile("fitDiagnosticsindividualfit.root","r")
    infile_gen = TFile("higgsCombineindividualfit.FitDiagnostics.mH120.root","r")
    infile_initial = TFile("ws_SingleTth_"+year+"_"+cat+"_125.root","r")

    bak_infile = TFile("bak.fitDiagnosticsindividualfit.root","r")
    bak_infile_gen = TFile("bak.higgsCombineindividualfit.FitDiagnostics.mH120.root","r")

    c = TCanvas()
    int_toys = 1
    ## Plot toys
    x = RooRealVar("x"+cat+""+year, "m_{T} [GeV]", 400, 1990);
    plotter=x.frame();
    
    
    w = infile_initial.Get('SingleTth'+cat)
    data = w.data("data_obs_"+channel+"_"+year+"_"+cat)
    int_toys = data.sumEntries("0 < x"+cat+""+year+" && x"+cat+""+year+" < 1990") 
    print int_toys
    print data.sumEntries() 
    data.plotOn(plotter)


    ## Plot postfit b
    b_postfit_bonly = infile.Get("shapes_fit_b/"+channel+"_"+year+"_"+cat+"/total_background")
    b_postfit_bonly.SetLineColor(kGreen+2)
    b_postfit_bonly.SetLineWidth(2)
    b_postfit_bonly.SetLineStyle(1)
    if bscale: b_postfit_bonly.Scale(int_toys/b_postfit_bonly.Integral())
    b_postfit_bonly.Draw("HIST")

    b_bak_postfit_bonly = bak_infile.Get("shapes_fit_b/"+channel+"_"+year+"_"+cat+"/total_background")
    b_bak_postfit_bonly.SetLineColor(kGreen-2)
    b_bak_postfit_bonly.SetLineWidth(2)
    b_bak_postfit_bonly.SetLineStyle(2)
    if bscale: b_bak_postfit_bonly.Scale(int_toys/b_bak_postfit_bonly.Integral())
    b_bak_postfit_bonly.Draw("HIST same")
    
    
    
    ## Plot postfit s+b
    sb_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total")
    sb_postfit_spb.SetLineColor(kBlue)
    sb_postfit_spb.SetLineWidth(2)

    sb_bak_postfit_spb = bak_infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total")
    sb_bak_postfit_spb.SetLineColor(kBlue-3)
    sb_bak_postfit_spb.SetLineWidth(2)
    sb_bak_postfit_spb.SetLineStyle(2)

    
    b_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total_background")
    b_postfit_spb.SetLineColor(kBlue)
    b_postfit_spb.SetLineWidth(2)
    b_postfit_spb.SetLineStyle(2)
    if bscale: b_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
#    b_postfit_spb.Draw("HIST same")
    
    s_postfit_spb = infile.Get("shapes_fit_s/"+channel+"_"+year+"_"+cat+"/total_signal")
    s_postfit_spb.SetLineColor(kBlue)
    s_postfit_spb.SetLineWidth(2)
    s_postfit_spb.SetLineStyle(2)
    if bscale: s_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
#    s_postfit_spb.Draw("HIST same")
    
    if bscale: sb_postfit_spb.Scale(int_toys/sb_postfit_spb.Integral())
    sb_postfit_spb.Draw("HIST same")
    if bscale: sb_bak_postfit_spb.Scale(int_toys/sb_bak_postfit_spb.Integral())
    sb_bak_postfit_spb.Draw("HIST same")
    
    plotter.Draw("same")
    
    
    leg=TLegend(0.5,0.5,0.7,0.7,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);
    leg.SetHeader(channel+", "+cat+", "+year)
    
    leg.AddEntry(b_postfit_bonly,"Total b only postfit","l")
    leg.AddEntry(b_bak_postfit_bonly,"Full Run2: Total b only postfit","l")
    leg.AddEntry(sb_postfit_spb,"Total s+b only postfit","l")
    leg.AddEntry(sb_bak_postfit_spb,"Full Run2: Total s+b only postfit","l")
    
    leg.Draw()
    
    #c.SetLogy()
    c.SaveAs("individualfit_comp_"+channel+cat+year+".pdf")



####################
#
#  MAIN
#
###################3
# for channel in channels:
#     for cat in categories:
#         for year in years:

#             plot_fits(cat,year,channel)
            

plot_comp("chi2h_2","2017v2","much")
plot_comp("chi2h_2","2017v2","ech")
