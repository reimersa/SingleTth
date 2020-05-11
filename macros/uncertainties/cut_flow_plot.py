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
from ROOT import TFile,TCanvas,gROOT,gStyle,TLegend,TGraphAsymmErrors,kGreen,kOrange,kSpring,TF1,kAzure, TH2F,TH1F,gPad, TPaveText, TH1,kRed,SetOwnership, TMath
from collections import OrderedDict

### Usage: python cut_flow_plot.py /nfs/dust/cms/user/reimersa/SingleTth/2018/Fullselection/NOMINAL_NoBTagSF

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

gStyle.SetOptStat(0)

if len(sys.argv)<1:
    print "Please give folder"
    exit(1)

folder = str(sys.argv[1])
max = 1
br_ljets =1
if len(sys.argv)>2:
    max = float(sys.argv[2])
if len(sys.argv)>3:
    br_ljets = float(sys.argv[3])

eff_dict = {}
eff_value_dict = {}

mainProcess = subprocess.Popen(['ls',folder], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
communicateRes = mainProcess.communicate() 
stdOutValue, stdErrValue = communicateRes
my_output_list = stdOutValue.split("\n")
i=0


for el in my_output_list:
    if "VLQ_LH_800" in el:
        print el
        masses =  re.findall(r'\d+', el)
        print "masses" + str(masses)
        hist_key  = "VLQ" + masses
        print "key:  " + str(hist_key)

        #Get eff histo before any cut
        input_file = TFile(" /nfs/dust/cms/user/reimersa/SingleTth/2018/Preselection/NOMINAL/"+el);
        TH1.AddDirectory(0)
        input_histo = input_file.Get("input_eff/eff;1")

        input_eff = input_histo.GetBinContent(2) 

        prelista = input_file.GetListOfKeys()

        #Create new histogramm for eff
        cut_flow = TH1F("cut_flow","Cut flow table",7,0,7)
        
        #Get eff histo after each cut
        infile = TFile(folder + "/" +el)
        TH1.AddDirectory(0)

        #creating scale factor
        hist1 = infile.Get("output_preselection_eff/eff").GetBinContent(2)
        hist2 = infile.Get("before_trigger_eff/eff").GetBinContent(2)
        sf = hist2/hist1
        print "scale factor: " + str(sf)
        input_eff *= br_ljets
#        input_eff *= sf 

        lista = infile.GetListOfKeys()
        

        print "Preselection Eff"
        #first go over all preselection steps
        for fol in prelista:
            key_value_eff = hist_key + "_"+fol.GetName()
            if "eff" in fol.GetName() and fol.GetName() not in forbidden:

                histo = input_file.Get(fol.GetName() + "/eff")
                eff = histo.GetBinContent(2)/input_eff
                print fol.GetName() + "  " + str(histo.GetBinContent(2))
                if fol.GetName() in preselection:
#                    eff = (histo.GetBinContent(2)*sf)/input_eff
                    eff = (histo.GetBinContent(2))/input_eff

                #fill histgogramm with name an eff
                if "output_eff" in fol.GetName():
                    cut_flow.Fill("1 AK8 jet, 2 AK4 jets",eff)
                    eff_value_dict[key_value_eff] = eff
                else:
                    cut_flow.Fill(fol.GetName(),eff)
                    eff_value_dict[key_value_eff] = eff

        print "Selection Eff"
        #then go over all selection steps
        for fol in lista:
            key_value_eff = str(hist_key) +"_"+ str(fol.GetName())
            if "eff" in fol.GetName() and fol.GetName() not in forbidden:
#            if 'eff' in fol.GetName():

                histo = infile.Get(fol.GetName() + "/eff")
                eff = histo.GetBinContent(2)/input_eff
                print fol.GetName() + "  " + str(histo.GetBinContent(2))
                if fol.GetName() in preselection:
                    eff = (histo.GetBinContent(2)*sf)/input_eff

                #fill histgogramm with name an eff
                cut_flow.Fill(fol.GetName(),eff)
                eff_value_dict[key_value_eff] = eff


        cut_flow.SetMaximum(max)
        cut_flow.SetLineColor(1+i)
        if not i%2: cut_flow.SetLineStyle(1+i//2)
        eff_dict[hist_key] = cut_flow
        i+=1


#draw all histos in eff_dict
a1= TCanvas("a1","",10,10,700,600);
gPad.SetTickx();
gPad.SetTicky();
a1.SetLogy()
leg1 = TLegend(0.5,0.7,0.8,0.9,"","brNDC")
leg1.SetBorderSize(0);
leg1.SetTextSize(0.035);
leg1.SetFillColor(0);
leg1.SetLineColor(1);
leg1.SetTextFont(42);

for keys in eff_dict:
    eff_dict[keys].Draw("hist same");
    a1.Update()
    leg1.AddEntry(eff_dict[keys], keys, "L")

leg1.Draw()
a1.Update()
a1.Print("test/cut_flow_HT.pdf")

print eff_value_dict

#print latex file with table with vaules
# change filename, add also plots with discription of cuts
outputfile = open("test.tex",'w')
outputfile.write('\\documentclass[12pt,a4paper,twoside, openright]{scrbook}\n')
outputfile.write('\\usepackage[pdftex]{graphicx}\n \\usepackage{caption}\n\\usepackage{subfigure}\n \\usepackage[subfigure]{tocloft}\n')
outputfile.write('\\begin{document}\n')

# include two cutflow plots
outputfile.write('\\begin{figure}\n')
outputfile.write('\\includegraphics[scale=0.6]{cut_flow_HT_scaled0p3_brljets}\n')
outputfile.write('\\caption{Signal efficiency scaled with br of 30\%}\n')
outputfile.write('\\label{fig:sig_eff_scaled}\n')
outputfile.write('\\end{figure}\n')

outputfile.write('\\begin{figure}\n')
outputfile.write('\\includegraphics[scale=0.6]{cut_flow_HT_unscaled}\n')
outputfile.write('\\caption{Signal efficiency unscaled }\n')
outputfile.write('\\label{fig:sig_eff_unscaled}\n')
outputfile.write('\\end{figure}\n')

# explanation of the cuts
outputfile.write('\\begin{table} \n')
outputfile.write('\\centering \n')
outputfile.write('\\begin{tabular}{c|c} \n')
outputfile.write( 'lepton  & at least a lepton  \\\\ \n')
outputfile.write( 'output  & 2 AK8 and 1 AK4 OR 1 AK8 and 2 AK4  \\\\ \n')
outputfile.write( 'lep1  & Trigger and exactly 1 lepton with pT $>$ 50 and $|\\eta|<2.4$  \\\\ \n')
outputfile.write( 'topjet2  & 1 AK8 jet with pT 250   \\\\ \n')
outputfile.write( 'twodcut  & pT,rel $>$ 40 or drmin $>$ 0.4  \\\\ \n')
outputfile.write( 'tagger  & at leat 1 H/W/Z tag  \\\\ \n')
outputfile.write( 'reco  & not yet included \\\\ \n')
outputfile.write( 'chi2cut  & chi2 $<$ 50  \\\\ \n')
outputfile.write('\\end{tabular}\n')
outputfile.write('\\caption{}\n')
outputfile.write('\\label{tab:eff}\n')
outputfile.write('\\end{table}\n')


for hkey in eff_dict:
    outputfile.write('\\begin{table} \n')
    outputfile.write('\\centering \n')
    outputfile.write('\\begin{tabular}{c|c|c} \n')
    outputfile.write(str(hkey)+'& cut  & eff  \\\\ \n')
    outputfile.write('\\hline\n')
    outputfile.write('\\hline\n')
#here loop over all entries in the eff_calue_dict
    for key in OrderedDict(sorted(eff_value_dict.items(), key=lambda x: x[1], reverse=True)):
        if hkey in key:
            for st in key.split("_"):
                if "eff" not in st and hkey not in st:
                    outputfile.write('& '+str(st)+'  & '+str(float(str(eff_value_dict[key])[:5]))+'  \\\\ \n')
    outputfile.write('\\end{tabular}\n')
#    outputfile.write('\\caption{}\n')
#    outputfile.write('\\label{tab:eff}\n')
    outputfile.write('\\end{table}\n')
outputfile.write('\\end{document}\n')


raw_input()


# name asjustment in Print
# for different decay
