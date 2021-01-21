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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, TH2D, TH1D, kMagenta, kCyan, kPink, TPad, TLine, kDashed, TObject,kTeal,kSolid, kDotted,kGray,kViolet
from collections import OrderedDict
import CMSPlotStyle

class bcolors:
    HEADER = '\033[95m'
    RED = '\033[91m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW  = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    BR = '\033[94m' #BLue
    PROCESS = '\033[93m' #Yellow
    CHANNEL = '\033[95m' #Magenta
    SYSTEMATIC = '\033[96m' #Cyan

gROOT.SetBatch(kTRUE)

style = CMSPlotStyle.getStyle()
style.cd()
gROOT.SetStyle("CMS_Style")
gStyle.SetOptStat(0)
gStyle.SetPaintTextFormat("2.3f")


cm_dict = {}
cm_dict["VLQ_M700_MH25"] = [kBlack,25,kSolid]
cm_dict["VLQ_M700_MH50"] = [kBlue+2,21,kDotted]
cm_dict["VLQ_M700_MH75"] = [kMagenta+3,21,kDashed]
cm_dict["VLQ_M700_MH100"] = [kPink,21,kDashed]
cm_dict["VLQ_M700_MH125"] = [kOrange,21,kDashed]
cm_dict["VLQ_M700_MH150"] = [kGreen+2,21,kDashed]
cm_dict["VLQ_M700_MH200"] = [kBlue,21,kDashed]
cm_dict["VLQ_M700_MH300"] = [kRed,21,kDashed]
cm_dict["VLQ_M700_MH400"] = [kTeal+3,21,kDashed]
cm_dict["VLQ_M700_MH500"] = [kGray,21,kDashed]
cm_dict["VLQ_M600_MH300"] = [kMagenta-2,21,kDashed]
cm_dict["VLQ_M800_MH300"] = [kViolet,21,kDashed]
cm_dict["VLQ_M900_MH300"] = [kViolet+3,21,kDashed]
cm_dict["VLQ_M1000_MH300"] = [kRed+3,21,kDashed]
cm_dict["VLQ_M1100_MH300"] = [kGreen+3,21,kDashed]
cm_dict["VLQ_M1200_MH300"] = [kMagenta+7,21,kDashed]
cm_dict["others"] = [kRed,20,kSolid]

leg_dict = {}
# leg_dict[""] ={}
# leg_dict[""][""] =[0.6,0.6,0.9,0.9]

xtitle_dict = {}
xtitle_dict["Tprime_mass"] = "M_{T} [GeV]"
xtitle_dict["Higgs_mass"] = "M_{a} [GeV]"
xtitle_dict["Higgs_pt"] = "p_{T,a} [GeV]"
xtitle_dict["Top_pt"] = "p_{T,t} [GeV]"
xtitle_dict["Top_mass"] = "M_{t} [GeV]"
xtitle_dict["Tprime_PDGID_daughters"] = "PDG Id of T daughters"
xtitle_dict["forwardjet_pt"] =  "forward quark p_{T} [GeV]"
xtitle_dict["bquarkjet_pt"] =  "b quark p_{T} [GeV]"
xtitle_dict["forwardjet_eta"] =  "forward quark #eta"
xtitle_dict["bquarkjet_eta"] =  "b quark #eta"
xtitle_dict["forwardjet_eta_ptg30"] =  "forward quark #eta for p_{T} > 30 GeV"
xtitle_dict["bquarkjet_eta_ptg30"] =  "b quark #eta for p_{T} > 30 GeV"
xtitle_dict["Higgs_bquark_pt"] =  "b quark p_{T} [GeV]"
xtitle_dict["Higgs_antibquark_pt"] =  "#bar{b} quark p_{T} [GeV]"
xtitle_dict["DeltaR_bantib_Higgs"] = "#Delta R(b,#bar{b})"


ymax_dict = {}
ymax_dict["Tprime_PDGID_daughters"] = 1.1
ymax_dict["Higgs_PDGID_daughters"] = 1.3
ymax_dict["Higgs_mass"] = 2.2
ymax_dict["Top_mass"] = 2.2
ymax_dict["Higgs_pt"] = 0.2
ymax_dict["Top_pt"] = 0.3
ymax_dict["forwardjet_pt"] = 0.3
ymax_dict["forwardjet_eta"] = 0.05
ymax_dict["forwardjet_eta_ptg30"] = 0.05
ymax_dict["bquarkjet_eta"] = 0.04
ymax_dict["bquarkjet_eta_ptg30"] = 0.06
ymax_dict["baurkjet_pt"] = 0.2
ymax_dict["Higgs_bquark_pt"] = 0.18
ymax_dict["Higgs_antibquark_pt"] = 0.18
ymax_dict["DeltaR_bantib_Higgs"] = 0.7



berror = False
beps = True

infiles = {}

infolder = "/nfs/dust/cms/user/reimersa/SingleTth/GenInfo/"

folder = infolder + "plots/"

infile_simulation_VLQ_LH_1000_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_1000_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M1000_MH300"] = infile_simulation_VLQ_LH_1000_MH300

infile_simulation_VLQ_LH_1100_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_1100_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M1100_MH300"] = infile_simulation_VLQ_LH_1100_MH300

infile_simulation_VLQ_LH_1200_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_1200_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M1200_MH300"] = infile_simulation_VLQ_LH_1200_MH300


infile_simulation_VLQ_LH_900_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_900_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M900_MH300"] = infile_simulation_VLQ_LH_900_MH300

infile_simulation_VLQ_LH_800_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_800_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M800_MH300"] = infile_simulation_VLQ_LH_800_MH300

infile_simulation_VLQ_LH_700_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH300"] = infile_simulation_VLQ_LH_700_MH300

infile_simulation_VLQ_LH_600_MH300 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_600_MH300.root")
TH1.AddDirectory(0)
infiles["VLQ_M600_MH300"] = infile_simulation_VLQ_LH_600_MH300

infile_simulation_VLQ_LH_700_MH25 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH25.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH25"] = infile_simulation_VLQ_LH_700_MH25

infile_simulation_VLQ_LH_700_MH50 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH50.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH50"] = infile_simulation_VLQ_LH_700_MH50

infile_simulation_VLQ_LH_700_MH75 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH75.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH75"] = infile_simulation_VLQ_LH_700_MH75

infile_simulation_VLQ_LH_700_MH100 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH100.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH100"] = infile_simulation_VLQ_LH_700_MH100

infile_simulation_VLQ_LH_700_MH125 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH125.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH125"] = infile_simulation_VLQ_LH_700_MH125

infile_simulation_VLQ_LH_700_MH150 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH150.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH150"] = infile_simulation_VLQ_LH_700_MH150

infile_simulation_VLQ_LH_700_MH200 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH200.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH200"] = infile_simulation_VLQ_LH_700_MH200

infile_simulation_VLQ_LH_700_MH250 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH250.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH250"] = infile_simulation_VLQ_LH_700_MH250

infile_simulation_VLQ_LH_700_MH400 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH400.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH400"] = infile_simulation_VLQ_LH_700_MH400

infile_simulation_VLQ_LH_700_MH500 = TFile(infolder +"uhh2.AnalysisModuleRunner.MC.VLQ_LH_700_MH500.root")
TH1.AddDirectory(0)
infiles["VLQ_M700_MH500"] = infile_simulation_VLQ_LH_700_MH500




def plot_distribution(infiles,folder,histname,filename,xtitle,ytitle,legxmin,legxmax,legymin,legymax,blogy=False, ymax = 0.1,btline = False,legheader = "", folder_ext = "",yoffset = 1.8, filename_ext = ""):

    print histname
    hist_to_draw={}

    for key in infiles:
        print key
        hist = infiles[key].Get(histname)

        if leg_dict.has_key(filename) and leg_dict[filename].has_key(folder_ext):
            legxmin = leg_dict[filename][folder_ext][0]
            legxmax = leg_dict[filename][folder_ext][2]
            legymin = leg_dict[filename][folder_ext][1]
            legymax = leg_dict[filename][folder_ext][3]

        if "Tprime_mass" in filename:
            hist.GetXaxis().SetRangeUser(0,1300)

        if "Higgs_pt" in filename:
            hist.GetXaxis().SetRangeUser(0,700)

        if "Top_pt" in filename:
            hist.GetXaxis().SetRangeUser(0,700)

        if "Higgs_bquark_pt" in filename or "Higgs_antibquark_pt" in filename:
            hist.GetXaxis().SetRangeUser(0,700)

        if "forwardjet_pt" in filename:
            hist.GetXaxis().SetRangeUser(0,400)

        if "bquarkjet_pt" in filename:
            hist.GetXaxis().SetRangeUser(0,300)

        hist.Scale(1./hist.Integral())

        hist_to_draw[key] = hist

        if xtitle_dict.has_key(filename):
            xtitle = xtitle_dict[filename]

    plot(hist_to_draw,folder,filename,xtitle,ytitle,10e-5,ymax,legxmin,legymin,legxmax,legymax,blogy,"Hist ",1,btline = btline,legheader = legheader,folder_ext=folder_ext,yoffset = yoffset, filename_ext = filename_ext,signal_hist_name = histname)

def plot_distributions(infiles,folder,histfolder,btline = False,legheader = "", folder_ext = "",blogy = True):

    for key in infiles:
        lista = infiles[key].Get(histfolder).GetListOfKeys()
        break

    for el in lista:
        ymax = 0.8

        if ymax_dict.has_key(el.GetName()):
            ymax = ymax_dict[el.GetName()]


        plot_distribution(infiles,folder,histfolder+"/"+el.GetName(),el.GetName(),el.GetName(),"A.u",0.2,0.9,0.5,0.95,btline = btline,ymax = ymax, blogy = blogy,legheader = legheader, folder_ext = folder_ext)       




def plot(hist_to_draw,folder,filename,xtitle,ytitle,ymin,ymax,legxmin,legymin,legxmax,legymax,blogy, draw_opt, markerstyle, legheader = "", legncolumn=1, abovecanvas = "",extrx = 0.5, extry = 0.5, mass  = "", btline = False, folder_ext="",yoffset = 1.2, filename_ext = "",signal_hist_name = "jetid_dijet/eta"):

    CMSPlotStyle.left_margin = 0.15
    CMSPlotStyle.right_margin = 0.04
    style = CMSPlotStyle.getStyle()
    style.cd()
    gROOT.SetStyle("CMS_Style")
    gStyle.SetOptStat(0)
    gStyle.SetPaintTextFormat("2.3f")
    dummy = TObject()

    c = TCanvas()
    leg=TLegend(legxmin,legymin,legxmax,legymax,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);
    leg.SetFillStyle(0)
    leg.SetHeader(legheader);
    leg.SetNColumns(2);

    i=0
    for key in sorted(hist_to_draw.iterkeys()):

        hist_to_draw[key].SetLineWidth(2)
        if(cm_dict.has_key(key)):
            hist_to_draw[key].SetLineColor(cm_dict[key][0])
            hist_to_draw[key].SetLineStyle(cm_dict[key][2])
            hist_to_draw[key].SetMarkerColor(cm_dict[key][0])
            hist_to_draw[key].SetMarkerStyle(cm_dict[key][1])
        else:
            hist_to_draw[key].SetLineColor(cm_dict["others"][0])
            hist_to_draw[key].SetLineStyle(cm_dict["others"][2])
            hist_to_draw[key].SetMarkerColor(cm_dict["others"][0])
            hist_to_draw[key].SetMarkerStyle(cm_dict["others"][1])

        hist_to_draw[key].SetMarkerSize(2)
        hist_to_draw[key].GetXaxis().SetTitle(xtitle)
        hist_to_draw[key].GetYaxis().SetTitle(ytitle)
        hist_to_draw[key].GetYaxis().SetTitleOffset(yoffset)
        hist_to_draw[key].GetYaxis().SetRangeUser(ymin,ymax)



        hist_to_draw[key].Draw(draw_opt+"same")
        i+=1

        title = "M_{T}="+key[5:]
        title = title.replace("_MH"," M_{a}=")

        leg.AddEntry(hist_to_draw[key], title,"l")


    text_dummy = CMSPlotStyle.draw_info(abovecanvas,0.37,0.94, textsizefactor = 0.7)
    text_dummy.Draw("same")

    leg.Draw()

    if blogy: c.SetLogy()
    if beps: c.Print(folder+"/"+folder_ext+"/"+filename+filename_ext+".eps")
    c.Print(folder+"/"+folder_ext+"/"+filename+filename_ext+".pdf")



plot_distributions(infiles,folder,"GenInfo",legheader = "", folder_ext = "",blogy = False)
