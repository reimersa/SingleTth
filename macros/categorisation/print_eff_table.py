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

prop["chi2h_2_much_sr"] = [kBlue,"M_{a} = 125 GeV"]

prop["catma60_much_sr"] = [kOrange,"M_{a} = 60 GeV"]

prop["catma90_much_sr"] = [kGreen+2,"M_{a} = 90 GeV"]

prop["catma175_much_sr"] = [kMagenta,"M_{a} = 175 GeV"]

prop["catma300_much_sr"] = [kBlack,"M_{a} = 300 GeV"]


def list_of_signal(folder):
    #find all signal files in folder

    mainProcess = subprocess.Popen(['ls',folder], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    communicateRes = mainProcess.communicate() 
    stdOutValue, stdErrValue = communicateRes

    #put them in a list
    my_output_list = stdOutValue.split("\n")

    #print and return
    VLQ_samples = []
    print "Found the following files"
    for el in my_output_list:
        if "VLQ" in el:
            print el
            VLQ_samples.append(el)
    return VLQ_samples


def calculate_percentage_per_category(folder, sample, muchannel = True):
    #open sample and get chi2 historgams
    print
    print sample
    infile = TFile(folder + sample)
    lista = infile.GetListOfKeys()
    perc_dict = {}
    total=0
    for el in lista:
        if not ("chi2h_2" in el.GetName() or "cat" in el.GetName()): continue
        if "Jets" in el.GetName() or "Mu" in el.GetName() or "Lumi" in el.GetName() or "Ele" in el.GetName() or "Event" in el.GetName() or "cr" in el.GetName(): continue
        if muchannel and "ech" in el.GetName(): continue
        if not muchannel and "much" in el.GetName(): continue

        #add all together and save individuals
        hist = infile.Get(el.GetName() + "/chi2")
        total = total+hist.Integral()
        perc_dict[el.GetName()] = hist.Integral() 


    #after for loop calculate percentage
    print 
    print "cat \t\t | eff"
    print "------------------------------"
    for el in sorted(perc_dict):
        perc_dict[el] = perc_dict[el] / total
        
        #print table and return
        print str(el) +"\t %.2f"%perc_dict[el]

    return perc_dict


def create_eff_table(folder,signal_list = []):
    #when signal list is empty look for all samples
    if not len(signal_list):
        signal_list = list_of_signal(folder)

    #loop over all samples and calculate percentage
    for el in signal_list:
        calculate_percentage_per_category(folder, el)
        calculate_percentage_per_category(folder, el, False)

def creat_eff_plot_per_cat(folder, signal_list = []):
    #when signal list is empty look for all samples
    if not len(signal_list):
        signal_list = list_of_signal(folder)

    #loop over all samples and calculate percentage
    sample_dict = {}
    for el in signal_list:
        mass = int(re.findall(r'\d+', el)[1])
        sample_dict[mass] =  calculate_percentage_per_category(folder, el)
        calculate_percentage_per_category(folder, el, False)

    print sample_dict
    hist_ma125 = TH1F("hist_ma125","eff in cat ma 125",len(signal_list),0,len(signal_list))
    bin = 1
    for el in sorted(sample_dict):
        print el
        hist_ma125.GetXaxis().SetBinLabel(bin,str(el))
        bin+=1
    
    for el in sorted(sample_dict):
        hist_ma125.Fill(str(el), sample_dict[el]["chi2h_2_much_sr"])


    c = TCanvas()
    hist_ma125.GetYaxis().SetTitle("[%]")
    hist_ma125.GetXaxis().SetTitle("M_{T} [GeV]")
    hist_ma125.Draw("HIST")
    
    leg = TLegend(0.55,0.55,0.85,0.79, "","brNDC")
    leg.SetBorderSize(0);	
    leg.SetFillStyle(0);
    leg.SetTextSize(0.035);
    leg.SetTextFont(42);
    leg.SetHeader("Signal")
    leg.AddEntry(hist_ma125, "M_{a} = 125 GeV")

    text = CMSPlotStyle.draw_info("Cat: M_{a} = 125 GeV, #mu+jets",0.15,0.945,11)
    text.Draw("same")

    c.SaveAs("catma125.eps")
    c.SaveAs("catma125.pdf")

def creat_eff_plot_per_sig(folder, signal_list = []):
    #when signal list is empty look for all samples
    if not len(signal_list):
        signal_list = list_of_signal(folder)

    #loop over all samples and calculate percentage
    sample_dict = {}
    for el in signal_list:
        mass = int(re.findall(r'\d+', el)[1])
        sample_dict[mass] =  calculate_percentage_per_category(folder, el)
        calculate_percentage_per_category(folder, el, False)

    print sample_dict
    
    hist_to_draw = {}
    dic_list = {"chi2h_2_much_sr","catma60_much_sr","catma90_much_sr","catma175_much_sr","catma300_much_sr",}
    for dic in dic_list:
        hist = TH1F("hist","eff in cat ma 125",len(signal_list),0,len(signal_list))
        bin = 1
        for el in sorted(sample_dict):
            print el
            hist.GetXaxis().SetBinLabel(bin,str(el))
            bin+=1

        for el in sorted(sample_dict):
            hist.Fill(str(el), sample_dict[el][dic])
            
        hist_to_draw[dic] = hist.Clone()
        hist.Delete()


    c = TCanvas()
    leg = TLegend(0.17,0.72,0.9,0.91, "","brNDC")
    leg.SetBorderSize(0);	
    leg.SetFillStyle(0);
    leg.SetTextSize(0.034);
    leg.SetTextFont(42);
    leg.SetNColumns(3)
    leg.SetHeader("Category:")
    
    for el in sorted(hist_to_draw):
        hist = hist_to_draw[el]
        hist.GetYaxis().SetTitle("[%]")
        hist.GetXaxis().SetTitle("M_{T} [GeV]")
        hist.GetYaxis().SetRangeUser(0,0.6)

        hist.SetLineWidth(2)
        if prop.has_key(dic):
            hist.SetLineColor(prop[el][0])
            hist.SetMarkerColor(prop[el][0])
        else:
            hist.SetLineColor(prop["others"][0])
        hist.Draw("HIST same")
    
        leg.AddEntry(hist, prop[el][1])

    leg.Draw()
    text = CMSPlotStyle.draw_info("Signal: M_{a} = 125 GeV, #mu+jets",0.15,0.945,11)
    text.Draw("same")

    c.SaveAs("sigma125.eps")
    c.SaveAs("sigma125.pdf")


create_eff_table("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mavariable/NOMINAL/")
#creat_eff_plot_per_cat("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mavariable/NOMINAL/")
creat_eff_plot_per_sig("/nfs/dust/cms/user/reimersa/SingleTth/2017/Fullselection/mavariable/NOMINAL/")
