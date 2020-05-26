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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath,TTree, RooRealVar,RooPlot,RooDataHist,RooDataSet, kBlue, kDashed, TTree, kTRUE, kBlack,TPad,TLine
from collections import OrderedDict
import argparse
import CMSPlotStyle

berror = False
bscale = False
beps = True
gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetOptFit()
gStyle.SetPaintTextFormat("2.3f")

colors = [kRed,kBlue,kRed,kBlue]

infile_MC_2017 = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.MC.background.root")
TH1.AddDirectory(0)    

infile_data_2017_much = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Muon_2017v2.root")
TH1.AddDirectory(0)    

infile_data_2017_ech = TFile("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mediumWP/NOMINAL/uhh2.AnalysisModuleRunner.DATA.DATA_Electron_2017v2.root")
TH1.AddDirectory(0)    


def plot_comparison(infile_MC, infile_data_much, infile_data_ech, histfolder, histname, filename,year):

    hist_MC_much = infile_MC.Get(histfolder + "/" + histname)
    hist_MC_ech = infile_MC.Get(histfolder + "_ele/" + histname)

    hist_Data_much = infile_data_much.Get(histfolder + "/" + histname)
    hist_Data_ech = infile_data_ech.Get(histfolder + "_ele/" + histname)

    hist_MC_much.Scale(1/hist_MC_much.Integral())
    hist_MC_ech.Scale(1/hist_MC_ech.Integral())

    hist_Data_much.Scale(1/hist_Data_much.Integral())
    hist_Data_ech.Scale(1/hist_Data_ech.Integral())
    
    # c1 = TCanvas()
    # leg = TLegend(0.5,0.5,0.95,0.9, "","brNDC")
    # leg.SetBorderSize(0);	
    # leg.SetFillStyle(0);
    # leg.SetTextSize(0.035);
    # leg.SetTextFont(42);
    # leg.SetHeader(year)

    hist_MC_much.SetLineColor(kRed)
    hist_MC_ech.SetLineColor(kBlue)

    hist_Data_much.SetMarkerColor(kRed)
    hist_Data_ech.SetMarkerColor(kBlue)

    hist_Data_much.SetMarkerStyle(20)
    hist_Data_ech.SetMarkerStyle(21)

    # hist_Data_much.GetXaxis().SetTitle(histname)
    # hist_Data_much.GetYaxis().SetTitle("a.u.")

    # hist_Data_much.Draw("P")
    # hist_Data_ech.Draw("P same")
    # hist_MC_much.Draw("HIST same")
    # hist_MC_ech.Draw("HIST same")


    # leg.AddEntry(hist_Data_much, "Data #mu","P")
    # leg.AddEntry(hist_Data_ech, "Data e","P")

    # leg.AddEntry( hist_MC_much,"MC #mu","l")
    # leg.AddEntry( hist_MC_ech,"MC e","l")

    # leg.Draw()

    # c1.SetLogy()
    # c1.SaveAs(filename+".eps")
    # c1.SaveAs(filename+".pdf")

    hist_to_draw={}
    hist_to_draw["MC #mu"] = hist_MC_much
    hist_to_draw["MC e"] = hist_MC_ech
    hist_to_draw["Data e"] = hist_Data_ech
    hist_to_draw["Data #mu"] = hist_Data_much

    ratio_to_draw={}
    ratio = hist_MC_much.Clone()
    ratio.Divide(hist_MC_ech)
    ratio_to_draw["MC"] = ratio

    ratio = hist_Data_much.Clone()
    ratio.Divide(hist_Data_ech)
    ratio_to_draw["Data"] = ratio


    plot_ratio(hist_to_draw,ratio_to_draw,"",filename,histname,"a.u.",0.001,0.4,0.5,0.5,0.9,0.9,True, "",leg_opt = "lp", extratext = "", ratiotitle = "#mu / e", xmin=0, xmax=1, ratioymin=0.5, ratioymax=1.5,extrax=0.9, extray=0.82, extratext2="", extrax2=0.9, extray2=0.82, markers = [20,21,20,21,1,1,1], bratioleg = True, lumitext = "",legheader = year)



def plot_ratio(hist_to_draw,ratio_to_draw,folder,filename,xtitle,ytitle,ymin,ymax,legxmin,legymin,legxmax,legymax,blogy, draw_opt,leg_opt = "pe", extratext = "", ratiotitle = "Data / MC", xmin=0, xmax=200, ratioymin=0.5, ratioymax=1.5,extrax=0.9, extray=0.82, extratext2="", extrax2=0.9, extray2=0.82, markers = [1,1,1,1,1,1,1], bratioleg = False, lumitext = "35.9",legheader = ""):

    c = TCanvas()
    gStyle.SetErrorX(0)

    yplot = 0.69
    yratio = 0.25                                 #  y6 +-------------+  
    y6 = 0.97                                     #     |     pad1    |
    y5 = y6-yplot                                 #  y5 |-------------|
    y4 = y5-yratio                                #     |     rp1     |
    y4 = 0.01
    x1 = 0.01                                     #  y4 +-------------+
    x2 = 0.95
    leftmargin = 0.15
    rightmargin = 0.03

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

    leg=TLegend(legxmin,legymin,legxmax,legymax,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.06);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);
    leg.SetHeader(legheader)
    c.GetFrame().SetFillColor(21);
    c.GetFrame().SetBorderSize(12);

    i=0
    for key in sorted(hist_to_draw.iterkeys()):
        hist_to_draw[key].SetLineColor(colors[i])
#        hist_to_draw[key].SetFillColor(colors[i])
        hist_to_draw[key].SetLineWidth(2)
        hist_to_draw[key].SetMarkerColor(colors[i])
                        
        hist_to_draw[key].SetMarkerStyle(markers[i])
        hist_to_draw[key].SetMarkerSize(1.2)
        hist_to_draw[key].GetXaxis().SetLabelSize(0)
        hist_to_draw[key].GetYaxis().SetLabelSize(0.05)
        hist_to_draw[key].GetXaxis().SetTitle(xtitle)
        hist_to_draw[key].GetYaxis().SetTitle(ytitle)
        hist_to_draw[key].GetYaxis().SetTitleSize(0.09)
        hist_to_draw[key].GetYaxis().SetTitleOffset(0.8)
        hist_to_draw[key].GetYaxis().SetRangeUser(ymin,ymax)
        hist_to_draw[key].GetXaxis().SetRangeUser(xmin,xmax)




        pad_top.cd()
        if "ata" in key:
            hist_to_draw[key].Draw("E same")
        else:
            hist_to_draw[key].Draw("HIST same")
        title = key
        leg.AddEntry(hist_to_draw[key], title,leg_opt)

        i+=1
    

    i=0
    pad_ratio.cd()
    ratio_leg=TLegend(0.17,0.81,0.37,0.95,"","brNDC")
    ratio_leg.SetBorderSize(0);
    ratio_leg.SetTextSize(0.12);
    ratio_leg.SetFillColor(0);
    ratio_leg.SetLineColor(1);
    ratio_leg.SetTextFont(42);

    ratio2_leg=TLegend(0.45,0.81,0.7,0.95,"","brNDC")
    ratio2_leg.SetBorderSize(0);
    ratio2_leg.SetTextSize(0.12);
    ratio2_leg.SetFillColor(0);
    ratio2_leg.SetLineColor(1);
    ratio2_leg.SetTextFont(42);


    for key in sorted(ratio_to_draw.iterkeys()):
        
        ratio_to_draw[key].SetLineColor(colors[i])
        ratio_to_draw[key].SetMarkerColor(colors[i])
        ratio_to_draw[key].SetMarkerStyle(20)
        

        ratio_to_draw[key].GetXaxis().SetTitle(xtitle)
        ratio_to_draw[key].GetXaxis().SetLabelSize(0.15)
        ratio_to_draw[key].GetXaxis().SetTitleSize(0.15)
        ratio_to_draw[key].GetXaxis().SetTitleOffset(1.4)
        ratio_to_draw[key].GetXaxis().SetRangeUser(xmin,xmax)
        ratio_to_draw[key].GetYaxis().SetTitle(ratiotitle)
        ratio_to_draw[key].GetYaxis().SetLabelSize(0.15)
        ratio_to_draw[key].GetYaxis().SetTitleSize(0.15)
        ratio_to_draw[key].GetYaxis().SetTitleOffset(0.45)
        ratio_to_draw[key].GetYaxis().SetNdivisions(503) 
        ratio_to_draw[key].GetYaxis().SetRangeUser(ratioymin,ratioymax)
#        ratio_to_draw[key].GetYaxis().SetNdivisions(510)
        ratio_leg.AddEntry(ratio_to_draw[key],key,"l")

        ratio_to_draw[key].Draw(draw_opt+" same")
        i+=1

        
    if bratioleg: ratio_leg.Draw()
    if bratioleg: ratio2_leg.Draw()

    line = TLine (xmin,1,xmax,1)
    line.SetLineColor(kRed)
    line.SetLineStyle(kDashed)
    line.Draw("same")

    gPad.Update();
    gPad.RedrawAxis();

    pad_top.cd()

    leg.Draw()
    gPad.Update();
    gPad.RedrawAxis();

    if extratext not in "":
        text = CMSPlotStyle.draw_info(extratext, extrax,extray, factor = 1.5)

    if extratext2 not in "":
        text2 = CMSPlotStyle.draw_info(extratext2, extrax2,extray2, factor = 1.5)

#    cmslogo = CMSPlotStyle.draw_cmstext("left", False,2)


#    lumi = CMSPlotStyle.draw_lumi(False,0.4,lumitext)
#    lumi.Draw()

#    anti = CMSPlotStyle.draw_info("Anti-k_{T}, R = 0.4",0.9,0.87,factor=1.5)
#    anti.Draw()

    if blogy: pad_top.SetLogy()
    if beps: c.Print(folder+filename+".eps")
    c.Print(folder+filename+".pdf")

    # CMSPlotStyle.extratext = "Preliminary"
    # cmslogo = CMSPlotStyle.draw_cmstext("left", True,2)
    # c.Print(folder+filename+"_pre.pdf")





#############  main


plot_comparison(infile_MC_2017, infile_data_2017_much,infile_data_2017_ech, "btagsf", "DeepJet_discriminant_jet1", "Comparison/comp_2017_DeepJet_discriminant_jet1","2017")


plot_comparison(infile_MC_2017, infile_data_2017_much,infile_data_2017_ech, "btagsf", "DeepJet_discriminant_jet2", "Comparison/comp_2017_DeepJet_discriminant_jet2","2017")


plot_comparison(infile_MC_2017, infile_data_2017_much,infile_data_2017_ech, "btagsf", "DeepJet_discriminant_jet3", "Comparison/comp_2017_DeepJet_discriminant_jet3","2017")
