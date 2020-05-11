#!/usr/bin/env python
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath, kBlue, kBlack, kFALSE, kTRUE,kSienna,TLatex, Double, TMultiGraph, kMagenta, TPad, TLine, kDashed, kGray, TGaxis
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


gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetPaintTextFormat("2.3f")

yOffset = 0.08
xOffset = 0.08
ySize = 0.09
xSize =0.09

colors = [kBlue,kBlack, kRed]
beps = True

def plot(hist_to_draw,folder,filename,xtitle,ytitle,ymin,ymax,legxmin,legymin,legxmax,legymax,blogy, draw_opt, markerstyle, extratext = "", b_draw_leg = True, xmin = 0, xmax =100):
    c = TCanvas()
    leg=TLegend(legxmin,legymin,legxmax,legymax,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    i=0
    for key in sorted(hist_to_draw.iterkeys()):
        hist_to_draw[key].SetLineColor(colors[i])
        hist_to_draw[key].SetMarkerColor(colors[i])
        hist_to_draw[key].SetMarkerStyle(markerstyle[i])
        hist_to_draw[key].SetMarkerSize(2)
        hist_to_draw[key].GetXaxis().SetTitle(xtitle)
        hist_to_draw[key].GetXaxis().SetTitleSize(xSize)
        hist_to_draw[key].GetXaxis().SetTitleOffset(xOffset)
        hist_to_draw[key].GetYaxis().SetTitle(ytitle)
        hist_to_draw[key].GetYaxis().SetTitleSize(ySize)
        hist_to_draw[key].GetYaxis().SetTitleOffset(yOffset)
        hist_to_draw[key].GetYaxis().SetRangeUser(ymin,ymax)
        hist_to_draw[key].GetXaxis().SetRangeUser(xmin,xmax)

        hist_to_draw[key].Draw(draw_opt+"same")
        i+=1
        leg.AddEntry(hist_to_draw[key], key,"lpe")
    

    if b_draw_leg: leg.Draw()

    extra_text = TLatex(3.5, 24, extratext);
    extra_text.SetNDC()
    extra_text.SetX(0.5)
    extra_text.SetY(0.5)
    extra_text.SetTextFont(52)
    extra_text.SetTextSize(0.035)
    extra_text.Draw()

    CMSPlotStyle.extratext = "Simulation"
    text = CMSPlotStyle.draw_cmstext("left", True)
    text[0].Draw()
    text[1].Draw()
    lumi = CMSPlotStyle.draw_lumi(True)
    lumi.Draw()
        


    if blogy: c.SetLogy()
    if beps: c.Print(folder+filename+".eps")
    c.Print(folder+filename+".pdf")


def plot_fit(hist,fitfunction,folder,filename,xtitle,ytitle,ymin,ymax,legxmin,legymin,legxmax,legymax,blogy, draw_opt, markerstyle, extratext = "", extrax=0.5,extray=0.5,b_draw_leg = True, plot_stats=False, xmin = 0 ,xmax=100, mcsample = "Z+jets"):

    if plot_stats:
        gStyle.SetOptFit(1111);
        gStyle.SetStatX(0.9);
        gStyle.SetStatY(0.9); 

    c = TCanvas()
    leg=TLegend(legxmin,legymin,legxmax,legymax,"","brNDC")
    leg.SetBorderSize(0);
    leg.SetTextSize(0.035);
    leg.SetFillColor(0);
    leg.SetLineColor(1);
    leg.SetTextFont(42);

    i=0
    hist.SetLineColor(colors[i])
    hist.SetMarkerColor(colors[i])
    hist.SetMarkerStyle(markerstyle)
    hist.GetXaxis().SetTitle(xtitle)
    hist.GetXaxis().SetTitleSize(xSize)
    hist.GetXaxis().SetTitleOffset(xOffset)
    hist.GetYaxis().SetTitle(ytitle)
    hist.GetYaxis().SetTitleSize(ySize)
    hist.GetYaxis().SetTitleOffset(yOffset)
    hist.GetYaxis().SetRangeUser(ymin,ymax)
    hist.GetXaxis().SetRangeUser(xmin,xmax)

    hist.Fit(fitfunction,"R")
    leg.AddEntry(hist,"MC, "+mcsample,"lpe")
    leg.AddEntry(fitfunction,"N_{vertices} = %.3f #mu + %.3f"%(fitfunction.GetParameter(1),fitfunction.GetParameter(0)),"l")

    hist.Draw(draw_opt+"same")

    extrainfo = CMSPlotStyle.draw_info(extratext,extrax,extray)
    extrainfo.Draw()

#    CMSPlotStyle.extratext = "#splitline{Simulation}{Supplementary}"
    CMSPlotStyle.extratext = "Simulation"
    text = CMSPlotStyle.draw_cmstext("left", True,1.2)
    text[0].Draw()
    text[1].Draw()
    lumi = CMSPlotStyle.draw_lumi(True)
    lumi.Draw()

    info = CMSPlotStyle.draw_info("#chi^{2}/ndf = %.2f"%(fitfunction.GetChisquare()/fitfunction.GetNDF()), legxmin+0.27,legymin-0.05)
    info.Draw()

    if b_draw_leg: leg.Draw()
    if blogy: c.SetLogy()
    if beps: c.Print(folder+filename+".eps")
    c.Print(folder+filename+".pdf")
