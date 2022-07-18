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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack,TPad,TLine, kMagenta
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")

prop = {}
prop["others"] = [kRed,"others"]
# prop["MH"] = [kBlack,"M_{H}"]
# prop["MA"] = [kRed,"M_{a}"]
#prop["MH"] = [kBlack,"ohne Kategorien"]
prop["MH"] = [kBlack,"prio125"]
prop["MA"] = [kRed,"prio125_woDR"]

infile_mH = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_2017v2.root")

infile_mA = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/prio125_wodR/NOMINAL/uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_ma125_2017v2.root")

#infile_mH = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/SFbtagmujets/NOMINAL/uhh2.AnalysisModuleRunner.MC.background.root")
# infile_mH = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/prio125/NOMINAL/uhh2.AnalysisModuleRunner.MC.background.root")

# infile_mA = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/prio125_wodR/NOMINAL/uhh2.AnalysisModuleRunner.MC.background.root")


def plot_comparison(histname = "M_Tprime_rebin2",xtitle="M_{T} [GeV]"):
    #    dic_list = {"chi2h_2_much_sr","catma60_much_sr","catma90_much_sr","catma175_much_sr","catma300_much_sr"}
    #    dic_list = {"chi2h_2_much_sr","catma90_much_sr","catma300_much_sr"}
    dic_list = {"chi2h_2_much_sr"}

    for dic in dic_list:
        
        hist_mH = infile_mH.Get(dic +"/"+histname)
        hist_mA = infile_mA.Get(dic +"/"+histname)

        hist_to_draw={}
        hist_to_draw["MH"] = hist_mH
        hist_to_draw["MA"] = hist_mA

        c = TCanvas()
        leg = TLegend(0.67,0.72,0.95,0.91, "","brNDC")
        leg.SetBorderSize(0);	
        leg.SetFillStyle(0);
        leg.SetTextSize(0.034);
        leg.SetTextFont(42);

        ma = re.findall("\d+",dic)[0]
        if "chi2h" in dic:
            ma = "125"
        leg.SetHeader("Cat: M_{a} = "+ma+" GeV")

        for el in sorted(hist_to_draw):
            hist = hist_to_draw[el]
            hist.GetYaxis().SetTitle("a.u.")
            hist.GetXaxis().SetTitle(xtitle)
            hist.GetYaxis().SetRangeUser(0,1)
            hist.Scale(1/hist.Integral())

            hist.SetLineWidth(2)
            if prop.has_key(el):
                hist.SetLineColor(prop[el][0])
                hist.SetMarkerColor(prop[el][0])
            else:
                hist.SetLineColor(prop["others"][0])

            hist.Draw("HIST same")
    
            leg.AddEntry(hist, prop[el][1])


        leg.Draw("same")
        c.SaveAs(histname+"_"+dic+".eps")
        c.SaveAs(histname+"_"+dic+".pdf")



plot_comparison(histname = "M_Tprime_rebin2",xtitle="M_{T} [GeV]")
plot_comparison(histname = "chi2_rebin6",xtitle="#chi^{2}")
