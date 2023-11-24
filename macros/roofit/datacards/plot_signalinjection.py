
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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack
from collections import OrderedDict
import argparse
import CMSPlotStyle


def plot_signIn(MA,postfix):
    berror = False
    bscale = False
    gROOT.SetBatch(kTRUE)
    
    style = CMSPlotStyle.getStyle()
    style.cd()
    gROOT.SetStyle("CMS_Style")
    gStyle.SetOptStat(0)
    gStyle.SetOptFit()
    gStyle.SetPaintTextFormat("2.3f")
    
    
    #signalstrength = {0.0,0.25,0.5,0.75,1.0,1.5,2.0,2.5,3.0}
    signalstrength = {'0.00','0.10','0.25','0.35','0.50','0.75','1.00'}
    masses = {600,800,1000}

    if int(MA)>200:
        masses = {800,1000}
    
    color = [kBlue,kBlack,kOrange]
    color_lm = [kBlue,kBlack,kOrange]
    style = [0,5,2]
    style_lm = [3,4]
    marker = [20,21,22]
    marker_lm = [24,26]
    
    gStyle.SetOptStat(0)
    
    ymax =1.3
    xmax = 1.3
    
    hist_to_draw = {}
    
    #hist = TH1F("hist","hist",50,0,3)
    
    i=0
    ii=0
    for mass in masses:
        print "============"
        print mass
        hist = TH1F("hist"+str(mass),"hist"+str(mass),50,0,xmax)
        hist_doublegaussian = TH1F("hist_doublegaussian"+str(mass),"hist_doublegaussian"+str(mass),50,0,xmax)
        if mass > 600: xmax = 1.1
        f= TF1("f","[0]+[1]*x",0,xmax)
    
        for sig in signalstrength:
            if mass > 600 and float(sig) > 1: continue
            print sig
            infile = TFile("fitDiagnostics"+postfix+"_signal"+str(sig)+"_"+str(mass)+".root","r")
            TH1.AddDirectory(0)
            tree_fit_sb = infile.Get("tree_fit_sb")
            #            tree_fit_sb.Draw("(r-%f)/rErr>>h2(50,-1,1)"%float(sig))
            tree_fit_sb.Draw("rErr>>h2(50,-1,1)")

            histo2 = gROOT.FindObject("h2")
            c3 = TCanvas()
            gStyle.SetOptFit(0) 
            leg2 = TLegend(0.2,0.6,0.55,0.9, "","brNDC")
            leg2.SetBorderSize(0);	
            leg2.SetFillStyle(0);
            leg2.SetTextSize(0.035);
            leg2.SetTextFont(42);
            leg2.SetHeader("M_{a} = "+MA)
            
            histo2.Draw()
            c3.Print("normgauss_"+postfix+"_"+str(mass)+"_"+str(sig)+".pdf")


            tree_fit_sb.Draw("r>>h(50,-1,2)")
            histo = gROOT.FindObject("h")
            mean = histo.GetMean()
    #        std = histo.GetStdDev()
            std = histo.GetMeanError()


            gaus = TF1("gaus","gaus", mean - 2*histo.GetRMS(), mean + 2*histo.GetRMS())
            histo.Fit(gaus, "R")
            mean = gaus.GetParameter(1)
            std = gaus.GetParameter(2)
    
            
            c2 = TCanvas()
            gStyle.SetOptFit(0) 
            leg = TLegend(0.2,0.6,0.55,0.9, "","brNDC")
            leg.SetBorderSize(0);	
            leg.SetFillStyle(0);
            leg.SetTextSize(0.035);
            leg.SetTextFont(42);
            leg.SetHeader("M_{a} = "+MA)
            
            histo.Draw()
            c2.Print("gauss_"+postfix+"_"+str(mass)+"_"+str(sig)+".pdf")

    
            hist.SetBinContent(hist.FindBin(float(sig)),mean)
            hist.SetBinError(hist.FindBin(float(sig)),std)
        
    #         if mass==600 or mass==1000:
    #             infile = TFile("fitDiagnosticssignal"+str(sig)+"_"+str(mass)+"_doublegaussian.root","r")
    #             TH1.AddDirectory(0)
    #             tree_fit_sb = infile.Get("tree_fit_sb")
    #             tree_fit_sb.Draw("r>>h(100,-2,5)")
    #             histo = gROOT.FindObject("h")
    #             mean = histo.GetMean()
    # #            std = histo.GetStdDev()
    #             std = histo.GetMeanError()
    #             hist_doublegaussian.SetBinContent(hist_doublegaussian.FindBin(sig),mean)
    #             hist_doublegaussian.SetBinError(hist_doublegaussian.FindBin(sig),std)
        
        
        hist.GetXaxis().SetTitle("Injected signal cross section [pb]")
        hist.GetYaxis().SetTitle("Mean(Fitted signal cross section) [pb]")
        f.SetLineColor(color[i])
        f.SetLineWidth(2)
        f.SetLineStyle(style[i])
        
        hist.SetLineWidth(2)
        hist.SetMarkerStyle(marker[i])
        hist.SetMarkerColor(color[i])
        hist.SetMarkerSize(1.5)
        hist.GetYaxis().SetRangeUser(0,ymax)
        hist.Fit(f,"R")
        hist_to_draw[str(i)+"M_{T} = "+str(mass)]  = hist.Clone()
    #     if mass==600 or mass==1000:
    #         hist_doublegaussian.SetLineWidth(2)
    #         hist_doublegaussian.SetMarkerStyle(marker_lm[ii])
    #         hist_doublegaussian.SetMarkerColor(color_lm[ii])
    #         hist_doublegaussian.SetMarkerSize(1.5)
    #         hist_doublegaussian.GetYaxis().SetRangeUser(0,3.5)
    #         f.SetLineStyle(style_lm[ii])
    #         hist_doublegaussian.Fit(f,"R")
    # #        hist_to_draw[str(i)+"M_{T} = "+str(mass)+" (low mass tail)"]  = hist_doublegaussian.Clone()
    #         ii+=1
        
        i+=1
    
    
    c1 = TCanvas()   
    gStyle.SetOptFit(0) 
    leg = TLegend(0.2,0.6,0.55,0.9, "","brNDC")
    leg.SetBorderSize(0);	
    leg.SetFillStyle(0);
    leg.SetTextSize(0.035);
    leg.SetTextFont(42);
    leg.SetHeader("M_{a} = "+MA)
    fl= TF1("fl","[0]+[1]*x",0,3.5)
    fl.SetParameter(0,0)
    fl.SetParameter(1,1)
    
    i=0
    ii=0
    for key in sorted(hist_to_draw):
        print key
        print i
        hist = hist_to_draw[key]
    
        hist.Draw("P same")
        if "tail" in key:
            hist.SetLineStyle(style_lm[ii])
            hist.SetLineColor(color_lm[ii])
        else:
            hist.SetLineStyle(style[i])
            hist.SetLineColor(color[i])
    
        leg.AddEntry(hist,key[1:],"lp")
        if "tail" in key:
            ii+=1
        else:
            i+=1
    
    fl.SetLineWidth(1)
    fl.SetLineStyle(kDashed)
    fl.SetLineColor(kRed)
    fl.Draw("same")
    leg.AddEntry(fl,"X=Y","l")
    leg.Draw()
    lumi = CMSPlotStyle.draw_lumi(True)
    lumi.Draw("same")
    
    
    c1.Print("signalinjection_"+str(mass)+".eps")
    c1.Print("signalinjection"+postfix+"_"+str(mass)+".pdf")



