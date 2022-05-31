#! /usr/bin/env python

import os, sys, math, time
import subprocess
from collections import OrderedDict
import os.path
from os import path
from create_datacards_eachYearIndividual_categories import create_datacards_eachYearIndividual_categories
from parallel import submit_limits
from LimitPlot import read_limits
import argparse

def main():

    parser = argparse.ArgumentParser(description='')
    parser.add_argument('--plotonly','-p', action='store_true', default=False, dest='plotonly', help='only make limit plots')
    args = parser.parse_args()

    # check if all signal output is available for the MA mass specified in the configuration file
    infile = open("/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit/datacards/configuration.txt","r")
    listOfLines = infile.readlines()
    indict = {}
    for line in listOfLines:
        line = line.strip()
        line = line.split("#")[0].strip(" ")
        if len(line.split("="))>1:
            indict[line.split("=")[0]] = line.split("=")[1]
            
    print indict

    #get MA from configuration file
    MA = indict["MA"]
    years = {}

    if "Run2" in indict["year"]:  
        dummy = ["2016","2017","2018"]
    else:
        dummy = [indict["year"]]

        
    for el in dummy:
        if "2016" in el:  years[el] =el+"v3" 
        elif "2017" in el: years[el] =el+"v2" 
        else:         years[el] = el


    categories = get_cat(MA)
    channels ={}
    channels["much"]="eMuon"
    channels["ech"]="eEle"
    channels["comb"]="eComb"

    indict["limit_postfix"] = indict["limit_postfix"].replace("[MA]",MA).replace("[YEAR]",indict["year"])


    if not args.plotonly:
        signalfits(indict, MA, years, channels, categories)    

        create_workspace(MA)
        create_datacards_eachYearIndividual_categories(years,MA,indict["anaoutputfolder"])
        submit_limits(MA, indict["blimit"], indict["bbias"] ,indict["bsigInj"])
    else:
        read_limits(indict["limit_postfix"])
        plot_limit(MA,indict["limit_postfix"], indict)

def plot_limit(MA,postfix, indict):
    command = 'cd /nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/; '
    #    print command
    #    os.system(command)

    #TString MA, TString postfix, bool draw_data=false, bool draw_had = false, bool draw_2017_comp = true,   bool draw_FullRun2_wocategories = false
    command += 'root -l -q -b "/nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/PlotLimits.cxx(\\"'+MA+'\\",\\"'+indict["limit_postfix"]+'\\",'+indict["draw_data"]+','+indict["draw_had"]+','+indict["draw_2017_comp"]+','+indict["draw_FullRun2_wocategories"]+')"'
    print command
    os.system(command)
    print "/nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/"
    

def create_workspace(MA):
    roofit_folder = '/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit' 
    command = 'cd '+roofit_folder+'; make;'
    print command
    os.system(command)

    command = 'cd '+roofit_folder+'; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/nfs/dust/cms/user/abenecke/CMSSW_10_2_10/CMSSW_10_2_10/src/UHH2/SingleTth/macros/roofit; echo $LD_LIBRARY_PATH; ./create_roo '+MA+'; ./create_roo_2017 '+MA+'; ./create_roo_2018 '+MA+';'
    print command
    os.system(command)


    

def signalfits(indict, MA, years, channels, categories):
    for year in years:
        folder = indict["anaoutputfolder"]
        folder = folder.replace("[YEAR]",year).replace("v2","").replace("v3","")
        for ch in channels:
            for cat in categories:
                file_to_check = folder + "SignalFitOutput_"+years[year]+"_"+cat+"_"+MA+"_"+ch+".txt"
                file_to_check = file_to_check.replace("_comb","")
                print file_to_check
                if not path.exists(file_to_check):
                    command = 'root -l -q "/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/sigfits/sig_fit.C(\\"'+MA+'\\",\\"'+years[year]+'\\",'+channels[ch]+')"'
                    print command
                    os.system(command)
                else:
                    print("File exists already: "+file_to_check)



    

def get_cat(MA):
    # get correct categories
    categories = ["catma60","catma90","catma175", "catma300","chi2h_2"] 
    if MA == "75":
        categories = ["catma60","catma90", "catma300"] 
    if MA == "125":
        categories = ["catma90","chi2h_2","catma300"] 
    if MA == "450":
        categories = ["catma175", "catma300"] 
    if MA == "100":
        categories = ["catma90","chi2h_2","catma300"]
    if MA == "175":
        categories = ["chi2h_2","catma175","catma300"]
    if MA == "200":
        categories = ["catma175","catma300","chi2h_2"]
    if MA ==  "250":
        categories = ["catma175","catma300"]
    if MA ==  "350":
        categories = ["catma175","catma300"]
    if MA ==  "500":
        categories = ["catma175","catma300"]

    return categories



if __name__ == '__main__':
    main()

