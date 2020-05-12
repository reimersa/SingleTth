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


berror = False

b_multipdf = False
b_signalrate = False
b_lumiunc = True

bkg_much_norm = {} # for all years: key is year
bkg_ech_norm = {} # for all years: key is year

signal_much_norm = {} # for all years and mass points: key is year_mass
signal_ech_norm = {} # for all years and mass points: key is year_mass
mean_value = {} # for all years and mass points: key is year_mass
mean_value_JEC_up = {} # for all years and mass points: key is year_mass
mean_value_JEC_down = {} # for all years and mass points: key is year_mass
mean_value_JER_up = {} # for all years and mass points: key is year_mass
mean_value_JER_down = {} # for all years and mass points: key is year_mass
mean_error = {} # for all years and mass points: key is year_mass
sigma_value = {} # for all years and mass points: key is year_mass
sigma_error = {} # for all years and mass points: key is year_mass
sigma_value_JEC_up = {} # for all years and mass points: key is year_mass
sigma_value_JEC_down = {} # for all years and mass points: key is year_mass
sigma_value_JER_up = {} # for all years and mass points: key is year_mass
sigma_value_JER_down = {} # for all years and mass points: key is year_mass



bg3p_v = {} # for all years and parameters: key is year_parameter

bg3p_err = {} # for all years and parameters: key is year_parameter

masses = []


all_years_eaul = True

rate_unc = {}
rate_unc["2016v3"] = 1.05
rate_unc["2017v2"] = 1.15
rate_unc["2018"] = 1.12
if all_years_eaul:
    rate_unc["2016v3"] = 1.15
    rate_unc["2018"] = 1.15

lumi_unc = {}
lumi_unc["2016v3"] = 1.025
lumi_unc["2017v2"] = 1.023
lumi_unc["2018"] = 1.025

#years = {"2016v3","2017v2","2018"}
years = {"2016v3"}

number_of_channels = 2 * len(years)
number_of_backgrounds = "*"


for year in years:
    # Find all possible MT where we have the normalisation
    inputfile = open("AnalysisOutput_"+year+".txt","r")
    listOfLines = inputfile.readlines()
    i=0
    for line in listOfLines:
        if "Muon" in line and "Signal" not in line:
            muon_norm_line =  listOfLines[i+1].strip()
            bkg_much_norm[year] = muon_norm_line.split("N = ")[1]

        if "Electron" in line and "Signal" not in line:
            elec_norm_line =  listOfLines[i+1].strip()
            bkg_ech_norm[year] = elec_norm_line.split("N = ")[1]

        if "Muon" in line and "Signal" in line:
            for j in range(i+1,len(listOfLines)):
                if "Electron" in listOfLines[j]: break
                mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
                masses.append(mass)

                norm = re.findall(r'\d+.\d+',listOfLines[j])[1]
                signal_much_norm[year+"_"+mass] = norm
                mean_value[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[2]
                mean_error[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[3]

                sigma_value[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[4]
                sigma_error[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[5]

                mean_value_JER_up[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[6]
                mean_value_JER_down[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[8]
                mean_value_JEC_up[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[10]
                mean_value_JEC_down[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[12]

                sigma_value_JER_up[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[7]
                sigma_value_JER_down[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[9]
                sigma_value_JEC_up[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[11]
                sigma_value_JEC_down[year+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[13]



        if "Electron" in line and "Signal" in line:
            for j in range(i+1,len(listOfLines)):
                mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
                norm = re.findall(r'\d+.\d+',listOfLines[j])[1]
                signal_ech_norm[year+"_"+mass] = norm

        if "Bg" in line:
            for j in range(i+1,len(listOfLines)):
                if "bg" in listOfLines[j]:
                    value = re.findall(r'[-+]?\d+.\d+',listOfLines[j])[1]
                    error = re.findall(r'[-+]?\d+.\d+',listOfLines[j])[2]
                    key = listOfLines[j].split("  ")[0]

                    bg3p_v[key] = value
                    bg3p_err[key] = error



        i+=1 

# open file to write
for mass in masses:
    outputfile = open("Datacard_allyears_M"+str(mass)+".txt",'w')
    outputfile.write("# Version of the 139/pb SingleTth Analysis \n\n")
    outputfile.write("------------------------------ \n")
    outputfile.write("imax  "+str(number_of_channels)+" number of channels \n")
    outputfile.write("jmax  "+str(number_of_backgrounds)+" number of backgrounds \n")
    outputfile.write("kmax  * number of nuisance parameters (sources of systematical uncertainties) \n \n")
    outputfile.write("------------------------------ \n")


    bins = "bin           "
    double_bins = "bin           "
    obs = "observation    "
    process_names = "process        "
    process_bins =  "process        "
    rates = "rate           "
    lumi = "lumi     lnN   "
    rate_sig = "rate_signal     lnN    "

    for year in sorted(years):
        outputfile.write("shapes * ech_"+year+" ws_SingleTth_"+str(year)+".root SingleTth:$PROCESS_$CHANNEL \n \n")
        outputfile.write("shapes * much_"+year+" ws_SingleTth_"+str(year)+".root SingleTth:$PROCESS_$CHANNEL \n \n")




        bins +=  "ech_"+year+"   much_"+year+"   "
        double_bins +=  "ech_"+year+"    ech_"+year+"   much_"+year+"   "+"   much_"+year+"   "
        obs +=    " -1             -1 "
        if b_multipdf:
            process_names += " roomultipdf_MT"+str(mass)+"   roomultipdf  roomultipdf_MT"+str(mass)+"   roomultipdf"
        else:
            process_names += " roomultipdf_MT"+str(mass)+"   Bkgfunc_Exp2p  roomultipdf_MT"+str(mass)+"   Bkgfunc_Exp2p"
        
        process_bins += "0   1    0    1   "

        rates += str(signal_ech_norm[year+"_"+mass]) + "   "+ str(bkg_ech_norm[year])+"          "+str(signal_much_norm[year+"_"+mass])+"   "+str(bkg_much_norm[year]) + "   "

        lumi += str(lumi_unc[year]) + "       -    " + str(lumi_unc[year]) + "       -    "
        rate_sig += str(rate_unc[year]) + "       -    " + str(rate_unc[year]) + "       -    "

    process_names += " \n"
    process_bins += " \n"
    rates += "\n \n"
    lumi += " \n"
    rate_sig += " \n"
    
    outputfile.write("------------------------------ \n")
    outputfile.write("# name of channels, and number of observed events (total number of event in Data) \n \n")

    bins +=" \n"
    double_bins +=" \n"
    obs += " \n \n"
    outputfile.write(bins)
    outputfile.write(obs)

    outputfile.write("------------------------------ \n")
    outputfile.write("# name of the channel you are considering, name of the process (signal,bkg,...),\n # process unique ID (positive number for backgrounds, and zero or negative for signal)\n # expected events for each process (total number of events in MC)\n \n")


    outputfile.write(double_bins)
    outputfile.write(process_names)
    outputfile.write(process_bins)
    outputfile.write(rates)

    outputfile.write("------------------------------ \n")
    outputfile.write("# list of independent sources of uncertainties, and give their effect (syst. error) \n \n")
    
    if b_lumiunc:
        outputfile.write(lumi)
    if b_signalrate:
        outputfile.write(rate_sig)

    for year in years:
        if b_multipdf:
            outputfile.write("pdf_index_much_"+year+" discrete \n ")
            outputfile.write("pdf_index_ech_"+year+" discrete \n ")


        outputfile.write("pdf_index_MT"+str(mass)+"_much_"+year+" discrete \n ")
        outputfile.write("pdf_index_MT"+str(mass)+"_ech_"+year+" discrete \n ")

        outputfile.write("sg_mean_"+year+" param "+str(mean_value[year+"_"+mass])+"  "+str(mean_error[year+"_"+mass])+" \n ")
        outputfile.write("sg_sigma_"+year+" param "+str(sigma_value[year+"_"+mass])+"  "+str(sigma_error[year+"_"+mass])+" \n ")

        ##### JER and JEC variations
        outputfile.write("sg_JERmeandown_"+year+" param "+str(mean_value_JER_down[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JERmeanup_"+year+" param "+str(mean_value_JER_up[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JECmeandown_"+year+" param "+str(mean_value_JEC_down[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JECmeanup_"+year+" param "+str(mean_value_JEC_up[year+"_"+mass])+"  0.001 \n ")


        outputfile.write("sg_JERsigmadown_"+year+" param "+str(sigma_value_JER_down[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JERsigmaup_"+year+" param "+str(sigma_value_JER_up[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JECsigmadown_"+year+" param "+str(sigma_value_JEC_down[year+"_"+mass])+"  0.001 \n ")
        outputfile.write("sg_JECsigmaup_"+year+" param "+str(sigma_value_JEC_up[year+"_"+mass])+"  0.001 \n ")

    
    for key in bg3p_v:
        outputfile.write(str(key) +" param "+str(bg3p_v[key])+"  "+str(bg3p_err[key])+" \n ")
    outputfile.write("------------------------------ \n")



    outputfile.close()


##### PDF signal individual for all years
