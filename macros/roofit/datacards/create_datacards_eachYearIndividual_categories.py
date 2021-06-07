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

b_multipdf = True
b_signalrate = True
b_lumiunc = True
b_JEC = False
shift = 0
if not b_JEC: shift = 8
#cat = "chi2h_2"

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

####second gaus
mean2_value = {} # for all years and mass points: key is year_mass
mean2_value_JEC_up = {} # for all years and mass points: key is year_mass
mean2_value_JEC_down = {} # for all years and mass points: key is year_mass
mean2_value_JER_up = {} # for all years and mass points: key is year_mass
mean2_value_JER_down = {} # for all years and mass points: key is year_mass
mean2_error = {} # for all years and mass points: key is year_mass
sigma2_value = {} # for all years and mass points: key is year_mass
sigma2_error = {} # for all years and mass points: key is year_mass
sigma2_value_JEC_up = {} # for all years and mass points: key is year_mass
sigma2_value_JEC_down = {} # for all years and mass points: key is year_mass
sigma2_value_JER_up = {} # for all years and mass points: key is year_mass
sigma2_value_JER_down = {} # for all years and mass points: key is year_mass

fnorm_value = {} # for all years and mass points: key is year_mass
fnorm_error = {} # for all years and mass points: key is year_mass
fnorm_value_JEC_up = {} # for all years and mass points: key is year_mass
fnorm_value_JEC_down = {} # for all years and mass points: key is year_mass
fnorm_value_JER_up = {} # for all years and mass points: key is year_mass
fnorm_value_JER_down = {} # for all years and mass points: key is year_mass



bg3p_v = {} # for all years and parameters: key is year_parameter

bg3p_err = {} # for all years and parameters: key is year_parameter

masses = []


all_years_eaul = False

####### rate unc: both, ech, much wegschreiben

# rate_unc = {}
# rate_unc["2016v3"] = 1.135
# rate_unc["2017v2"] = 1.127
# rate_unc["2018"] = 1.126
# if all_years_eaul:
#     rate_unc["2016v3"] = 1.05
#     rate_unc["2018"] = 1.05
#     rate_unc["2017v2"] = 1.05

# rate_btag_unc = {}
# rate_btag_unc["2016v3"] = 1.10
# rate_btag_unc["2017v2"] = 1.10
# rate_btag_unc["2018"] = 1.10


lumi_unc = {}
lumi_unc["2016v3"] = 0.025
lumi_unc["2017v2"] = 0.023
lumi_unc["2018"] = 0.025

#years = {"2016v3","2017v2","2018"}
#years = {"2018","2016v3"}
years = {"2017v2"}
#ma_mass = "99999"
ma_mass = "75"

#categories = ["chi2h_2", "catma60","catma90","catma175", "catma300"]
#categories = ["chi2h_2", "catma90","catma175", "catma300"] 
categories = ["catma60","catma90", "catma300"] 
#categories = ["chi2h_2"] 

number_of_channels = 2 * len(years) * len(categories)
number_of_backgrounds = "*"

#### read out rate unc for e, mu and both together
folder = "/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/"
tot_rate_unc = {}
ech_rate_unc = {}
much_rate_unc = {}




print "Start to collect normalisations"
for icat in categories:
    tot_rate_unc[icat] = {}
    ech_rate_unc[icat] = {}
    much_rate_unc[icat] = {}

    for year in years:
        if "2016" in year and "chi2h" not in icat: continue
        f_tot = open(folder + "SignalFitOutput_"+year+"_"+icat+"_"+ma_mass+".txt", "r")
        f_much = open(folder + "SignalFitOutput_"+year+"_"+icat+"_"+ma_mass+"_much.txt", "r")
        f_ech = open(folder + "SignalFitOutput_"+year+"_"+icat+"_"+ma_mass+"_ech.txt", "r")
    
        # tot unc for both e and mu
        tot_rate_unc[icat][year] = {}
    
        for line in f_tot:
            if "total rate unc" not in line: continue
            mass = re.findall(r'\d+\.*\d+', line)[0]
            unc = float(re.findall(r'\d+\.*\d+', line)[1])
            if len(years) > 2:
                unc = math.sqrt(float(unc)*float(unc) + 0.018 * 0.018 )
            else:
                unc = math.sqrt(float(unc)*float(unc) + lumi_unc[year] * lumi_unc[year] )
            unc+=1
            tot_rate_unc[icat][year][mass] = unc
    
        print sorted(tot_rate_unc)
    
    
        # e ch unc 
        ech_rate_unc[icat][year] = {}
    
        for line in f_ech:
            if "total rate unc" not in line: continue
            mass = re.findall(r'\d+\.*\d+', line)[0]
            unc = float(re.findall(r'\d+\.*\d+', line)[1])
            unc+=1
            ech_rate_unc[icat][year][mass] = unc
    
        print sorted(ech_rate_unc)
    
        # mu ch unc 
        much_rate_unc[icat][year] = {}
    
        for line in f_much:
            if "total rate unc" not in line: continue
            mass = re.findall(r'\d+\.*\d+', line)[0]
            unc = float(re.findall(r'\d+\.*\d+', line)[1])
            unc+=1
            much_rate_unc[icat][year][mass] = unc
    
        print sorted(much_rate_unc)
    

###

print "Start to collect uncertainties"
for icat in categories:
    bkg_much_norm[icat] = {}
    bkg_ech_norm[icat] = {}


    for year in years:
        if "2016" in year and "chi2h" not in icat: continue
        # Find all possible MT where we have the normalisation
        inputfile = open("AnalysisOutput_"+year+"_"+icat+".txt","r")
        listOfLines = inputfile.readlines()
        i=0
        for line in listOfLines:
            if "Muon" in line and "Signal" not in line:
                muon_norm_line =  listOfLines[i+1].strip()
                bkg_much_norm[icat][year] = muon_norm_line.split("N = ")[1]
    
            if "Electron" in line and "Signal" not in line:
                elec_norm_line =  listOfLines[i+1].strip()
                bkg_ech_norm[icat][year] = elec_norm_line.split("N = ")[1]
    
            if "Muon" in line and "Signal" in line:
                for j in range(i+1,len(listOfLines)):
                    if "Electron" in listOfLines[j]: break
                    mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
                    if mass not in masses and int(mass)>590:
                        masses.append(mass)
     
                    norm = re.findall(r'\d+.\d+',listOfLines[j])[1]
                    signal_much_norm[year+"_"+icat+"_"+mass] = norm
                    mean_value[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[2]
                    mean_error[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[3]
    
                    sigma_value[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[4]
                    sigma_error[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[5]

                    if b_JEC:
                        mean_value_JER_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[6]
                        mean_value_JER_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[8]
                        mean_value_JEC_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[10]
                        mean_value_JEC_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[12]
                        
                        sigma_value_JER_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[7]
                        sigma_value_JER_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[9]
                        sigma_value_JEC_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[11]
                        sigma_value_JEC_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[13]
    
                    mean2_value[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[14-shift]
                    mean2_error[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[15-shift]
    
                    sigma2_value[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[16-shift]
                    sigma2_error[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[17-shift]
                    if b_JEC:
                    
                        mean2_value_JER_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[18]
                        mean2_value_JER_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[19]
                        mean2_value_JEC_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[20]
                        mean2_value_JEC_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[21]
    
                        sigma2_value_JER_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[22]
                        sigma2_value_JER_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[23]
                        sigma2_value_JEC_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[24]
                        sigma2_value_JEC_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[25]
    
                    # fnorm_value[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[26]
                    # fnorm_error[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[27]
                    # fnorm_value_JER_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[28]
                    # fnorm_value_JER_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[29]
                    # fnorm_value_JEC_up[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[30]
                    # fnorm_value_JEC_down[year+"_"+icat+"_"+mass] = re.findall(r'\d+.\d+',listOfLines[j])[31]
    
    
    
            if "Electron" in line and "Signal" in line:
                for j in range(i+1,len(listOfLines)):
                    mass = re.findall(r'\d+.\d+',listOfLines[j])[0]
                    norm = re.findall(r'\d+.\d+',listOfLines[j])[1]
                    signal_ech_norm[year+"_"+icat+"_"+mass] = norm

            if "Bg" in line:
                for j in range(i+1,len(listOfLines)):
                    if "bg" in listOfLines[j]:
                        if len(re.findall(r'[-+]?\d+.\d+',listOfLines[j]))>2:
                            value = re.findall(r'[-+]?\d+.\d+',listOfLines[j])[len(re.findall(r'[-+]?\d+.\d+',listOfLines[j]))-2]
                            error = re.findall(r'[-+]?\d+.\d+',listOfLines[j])[len(re.findall(r'[-+]?\d+.\d+',listOfLines[j]))-1]
                        else:
                            value = re.findall(r'[-+]?\d+.\d*',listOfLines[j])[3]
                            error = re.findall(r'[-+]?\d+.\d*',listOfLines[j])[4]
                            
                        key = listOfLines[j].split("  ")[0]
    
                        bg3p_v[key] = value
                        bg3p_err[key] = error
    
    
    
            i+=1 
    
# open file to write
for mass in masses:
    outputfile = open("Datacard_allyears_M"+str(mass)+".txt",'w')
    outputfile.write("# Version of the 139/pb SingleTth Analysis \n\n")
    outputfile.write("------------------------------ \n")

    if "2016v3" in years:
        if int(mass)<700 and "catma175" in categories and "catma300" in categories:
            outputfile.write("imax  "+str(2 * (len(years)-1) * (len(categories)-2)+2)+" number of channels \n")
        elif int(mass)<700 and ("catma175" in categories or "catma300" in categories):
            outputfile.write("imax  "+str(2 * (len(years)-1) * (len(categories)-1)+2)+" number of channels \n")
        elif int(mass)<800  and "catma175" in categories and "catma300" in categories and "2018" in years:
            outputfile.write("imax  "+str(2 * (len(years)-1) * len(categories)-2+2)+" number of channels \n")
        elif int(mass)<800  and ("catma175" in categories or "catma300" in categories) and "2018" in years:
            outputfile.write("imax  "+str(2 * (len(years)-1) * len(categories)-1+2)+" number of channels \n")
        else: 
            outputfile.write("imax  "+str(number_of_channels-(2*(len(categories)-1)))+" number of channels \n")

    else:
        if int(mass)<700 and "catma175" in categories and "catma300" in categories:
            outputfile.write("imax  "+str(2 * len(years) * (len(categories)-2))+" number of channels \n")
        elif int(mass)<700 and ("catma175" in categories or "catma300" in categories):
            outputfile.write("imax  "+str(2 * len(years) * (len(categories)-1))+" number of channels \n")
        elif int(mass)<800  and "catma175" in categories and "catma300" in categories and "2018" in years:
            outputfile.write("imax  "+str(2 * len(years) * len(categories)-2)+" number of channels \n")
        elif int(mass)<800  and ("catma175" in categories or "catma300" in categories) and "2018" in years:
            outputfile.write("imax  "+str(2 * len(years) * len(categories)-1)+" number of channels \n")
        else: 
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
#    lumi = "lumi     lnN   "
    rate_sig = "rate_signal     lnN    "
    rate_sig_mu_2016 = "rate_signal_mu_2016v3     lnN    "
    rate_sig_ech_2016 = "rate_signal_ech_2016v3     lnN    "
    rate_sig_mu_2017 = "rate_signal_mu_2017v2     lnN    "
    rate_sig_ech_2017 = "rate_signal_ech_2017v2     lnN    "
    rate_sig_mu_2018 = "rate_signal_mu_2018     lnN    "
    rate_sig_ech_2018 = "rate_signal_ech_2018     lnN    "


    
    for cat in categories:
        if "175" in cat and int(mass) < 700: continue
        if "300" in cat and int(mass) < 700: continue

        for year in sorted(years):
            if "300" in cat and int(mass) < 800 and "2018" in year: continue
            if "2016" in year and "chi2h" not in cat: continue
            outputfile.write("shapes * ech_"+year+"_"+cat+" ws_SingleTth_"+str(year)+"_"+cat+"_"+ma_mass+".root SingleTth"+cat+":$PROCESS_$CHANNEL \n \n")
            outputfile.write("shapes * much_"+year+"_"+cat+" ws_SingleTth_"+str(year)+"_"+cat+"_"+ma_mass+".root SingleTth"+cat+":$PROCESS_$CHANNEL \n \n")

            bins +=  "ech_"+year+"_"+cat+"   much_"+year+"_"+cat+"   "
            double_bins +=  "ech_"+year+"_"+cat+"    ech_"+year+"_"+cat+"   much_"+year+"_"+cat+"   "+"   much_"+year+"_"+cat+"   "
            obs +=    " -1             -1 "
            if b_multipdf:
                process_names += " roomultipdf_MT"+str(mass)+"   roomultipdf  roomultipdf_MT"+str(mass)+"   roomultipdf"
            else:
                process_names += " roomultipdf_MT"+str(mass)+"   Bkgfunc_Exp2p  roomultipdf_MT"+str(mass)+"   Bkgfunc_Exp2p"
                
            process_bins += "0   1    0    1   "

            rates += str(signal_ech_norm[year+"_"+cat+"_"+mass]) + "   "+ str(bkg_ech_norm[cat][year])+"          "+str(signal_much_norm[year+"_"+cat+"_"+mass])+"   "+str(bkg_much_norm[cat][year]) + "   "

            #        lumi += str(lumi_unc[year]) + "       -    " + str(lumi_unc[year]) + "       -    "
            rate_sig += str(tot_rate_unc[cat][year][mass]) + "       -   "+str(tot_rate_unc[cat][year][mass]) +"       -    "
            if("2016" in year):
                rate_sig_ech_2016 += str(ech_rate_unc[cat][year][mass]) + "       -    -       -    "
                rate_sig_mu_2016 += "-       -    " + str(much_rate_unc[cat][year][mass]) + "       -    "
                rate_sig_ech_2017 +=  "-       -    -       -    "
                rate_sig_mu_2017 += "-       -    -       -    "
                rate_sig_ech_2018 +=  "-       -    -       -    "
                rate_sig_mu_2018 += "-       -    -       -    "
            if("2017" in year):
                rate_sig_ech_2017 += str(ech_rate_unc[cat][year][mass]) + "       -    -       -    "
                rate_sig_mu_2017 += "-       -    " + str(much_rate_unc[cat][year][mass]) + "       -    "
                rate_sig_ech_2016 +=  "-       -    -       -    "
                rate_sig_mu_2016 += "-       -    -       -    "
                rate_sig_ech_2018 +=  "-       -    -       -    "
                rate_sig_mu_2018 += "-       -    -       -    "
            if("2018" in year):
                rate_sig_ech_2018 += str(ech_rate_unc[cat][year][mass]) + "       -    -       -    "
                rate_sig_mu_2018 += "-       -    " + str(much_rate_unc[cat][year][mass]) + "       -    "
                rate_sig_ech_2016 +=  "-       -    -       -    "
                rate_sig_mu_2016 += "-       -    -       -    "
                rate_sig_ech_2017 +=  "-       -    -       -    "
                rate_sig_mu_2017 += "-       -    -       -    "
	
	

    process_names += " \n"
    process_bins += " \n"
    rates += "\n \n"
#    lumi += " \n"
    rate_sig_mu_2016 += " \n"
    rate_sig_mu_2017 += " \n"
    rate_sig_mu_2018 += " \n"
    rate_sig += " \n"
    rate_sig_ech_2016 += " \n"
    rate_sig_ech_2017 += " \n"
    rate_sig_ech_2018 += " \n"
    
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
    
#    if b_lumiunc:
#        outputfile.write(lumi)
    if b_signalrate:
        outputfile.write(rate_sig)

    for cat in categories:
        if "175" in cat and int(mass) < 700: continue
        if "300" in cat and int(mass) < 700: continue


        for year in years:
            if "300" in cat and int(mass) < 800 and "2018" in year: continue
            if "2016" in year and "chi2h" not in cat: continue
            if b_multipdf:
                outputfile.write("pdf_index_much_"+year+"_"+cat+" discrete \n ")
                outputfile.write("pdf_index_ech_"+year+"_"+cat+" discrete \n ")
            if b_signalrate:
                if "2016" in year:
                    outputfile.write(rate_sig_mu_2016)
                    outputfile.write(rate_sig_ech_2016)
                if "2017" in year:
                    outputfile.write(rate_sig_mu_2017)
                    outputfile.write(rate_sig_ech_2017)
                if "2018" in year:
                    outputfile.write(rate_sig_mu_2018)
                    outputfile.write(rate_sig_ech_2018)
	
	
            outputfile.write("pdf_index_MT"+str(mass)+"_much_"+year+"_"+cat+" discrete \n ")
            outputfile.write("pdf_index_MT"+str(mass)+"_ech_"+year+"_"+cat+" discrete \n ")
	
            outputfile.write("sg_mean_"+year+"_"+cat+" param "+str(mean_value[year+"_"+cat+"_"+mass])+"  "+str(mean_error[year+"_"+cat+"_"+mass])+" \n ")
            outputfile.write("sg_sigma_"+year+"_"+cat+" param "+str(sigma_value[year+"_"+cat+"_"+mass])+"  "+str(sigma_error[year+"_"+cat+"_"+mass])+" \n ")
	
	        ##### JER and JEC variations
            if b_JEC:
                outputfile.write("sg_JERmeandown_"+year+"_"+cat+" param "+str(mean_value_JER_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JERmeanup_"+year+"_"+cat+" param "+str(mean_value_JER_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECmeandown_"+year+"_"+cat+" param "+str(mean_value_JEC_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECmeanup_"+year+"_"+cat+" param "+str(mean_value_JEC_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
	
	
                outputfile.write("sg_JERsigmadown_"+year+"_"+cat+" param "+str(sigma_value_JER_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JERsigmaup_"+year+"_"+cat+" param "+str(sigma_value_JER_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECsigmadown_"+year+"_"+cat+" param "+str(sigma_value_JEC_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECsigmaup_"+year+"_"+cat+" param "+str(sigma_value_JEC_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
	
            outputfile.write("sg_mean2_"+year+"_"+cat+" param "+str(mean2_value[year+"_"+cat+"_"+mass])+"  "+str(mean2_error[year+"_"+cat+"_"+mass])+" \n ")
            outputfile.write("sg_sigma2_"+year+"_"+cat+" param "+str(sigma2_value[year+"_"+cat+"_"+mass])+"  "+str(sigma2_error[year+"_"+cat+"_"+mass])+" \n ")
	
	        ##### JER and JEC variations
            if b_JEC:
                outputfile.write("sg_JERmeandown2_"+year+"_"+cat+" param "+str(mean2_value_JER_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JERmeanup2_"+year+"_"+cat+" param "+str(mean2_value_JER_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECmeandown2_"+year+"_"+cat+" param "+str(mean2_value_JEC_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECmeanup2_"+year+"_"+cat+" param "+str(mean2_value_JEC_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
	
	
                outputfile.write("sg_JERsigmadown2_"+year+"_"+cat+" param "+str(sigma2_value_JER_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JERsigmaup2_"+year+"_"+cat+" param "+str(sigma2_value_JER_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECsigmadown2_"+year+"_"+cat+" param "+str(sigma2_value_JEC_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
                outputfile.write("sg_JECsigmaup2_"+year+"_"+cat+" param "+str(sigma2_value_JEC_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
	
            # ############ fnorm
            # outputfile.write("sg_fnorm_"+year+"_"+cat+" param "+str(fnorm_value[year+"_"+cat+"_"+mass])+"  "+str(fnorm_error[year+"_"+cat+"_"+mass])+" \n ")
            # outputfile.write("sg_JERfnormdown_"+year+"_"+cat+" param "+str(fnorm_value_JER_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
            # outputfile.write("sg_JERfnormup_"+year+"_"+cat+" param "+str(fnorm_value_JER_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
            # outputfile.write("sg_JECfnromdown_"+year+"_"+cat+" param "+str(fnorm_value_JEC_down[year+"_"+cat+"_"+mass])+"  0.001 \n ")
            # outputfile.write("sg_JECfnormup_"+year+"_"+cat+" param "+str(fnorm_value_JEC_up[year+"_"+cat+"_"+mass])+"  0.001 \n ")
	    
        for key in bg3p_v:
            if cat not in key: continue
            outputfile.write(str(key) +" param "+str(bg3p_v[key])+"  "+str(bg3p_err[key])+" \n ")
        outputfile.write("------------------------------ \n")



    outputfile.close()


##### PDF signal individual for all years
