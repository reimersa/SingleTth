#! /usr/bin/env python

import os, sys, math, time
import subprocess
from collections import OrderedDict
import os.path
from os import path
from create_datacards_eachYearIndividual_categories import create_datacards_eachYearIndividual_categories
from parallel import submit_limits, ensureDirectory, derive_from_default_settings, create_submitfile
from LimitPlot import read_limits
from plot_bias import plot_bias
from plot_signalinjection import plot_signIn
import argparse
from plot_individualfit import plot_individual_fits
from categories import get_categories
import plot_GOF as pGOF

def main():

    parser = argparse.ArgumentParser(description='')
    parser.add_argument('--plotonly','-p', action='store_true', default=False, dest='plotonly', help='only make limit plots')
    parser.add_argument('--sigfitsonly','-s', action='store_true', default=False, dest='sigfitsonly', help='only do signal fits')
    parser.add_argument('--wows','-w', action='store_true', default=False, dest='wows', help='wo workshpace')
    parser.add_argument('--gof','-gof', action='store_true', default=False, dest='gof_only', help='Run GOFs only')
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
    limitvariable = "M_Tprime" if "MT" in indict["limitvariable"] else "ST_rebin"
    years = {}

    if "Run2" in indict["year"]:  
        dummy = ["2016","2017","2018"]
    else:
        dummy = [indict["year"]]

        
    for el in dummy:
        if "2016" in el:  years[el] =el+"v3" 
        elif "2017" in el: years[el] =el+"v2" 
        else:         years[el] = el


    categories = get_categories(MA, False)
    channels ={}
    channels["much"]="eMuon"
    channels["ech"]="eEle"
    channels["comb"]="eComb"

    indict["limit_postfix"] = indict["limit_postfix"].replace("[MA]",MA).replace("[YEAR]",indict["year"]).replace("[LIMITVARIABLE]",limitvariable)

    if indict["bonly125"]=="True": 
        indict["bbias"] = "False"
        print " =========== Note: Bias test deactivated"
        indict["bsigInj"] = "False"
        print " =========== Note: signal injection test deactivated"


    if not args.plotonly:
        signalfits(indict, MA, years, channels, categories)    

        if not args.sigfitsonly:
            if not args.wows:
                create_workspace(MA,years, limitvariable)
            if args.gof_only:
                submit_gofs(int(MA),1000)
            else:
                create_datacards_eachYearIndividual_categories(years,MA,indict["anaoutputfolder"], b_only125 = indict["bonly125"]=="True", limitvariable = limitvariable)
                submit_limits(MA, blimit = indict["blimit"]=="True", bbias = indict["bbias"]=="True" ,bsigInj =indict["bsigInj"]=="True", postfix=indict["limit_postfix"], b_only125 = indict["bonly125"]=="True")
    else:
        if args.gof_only:
            MTs =[600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175]
            signalparams = ["float"]

            for MT in MTs:
                if MT in [700,725]: continue
                signalparams = ["float"]
                if MT == 600 or MT==800 or MT==1000: signalparams = ["float","fixed"]
                pGOF.plot(MT, MA, signalparams, indict["year"],"l+jets")
        else:
            plot_individual_fits(MA, indict["bonly125"]=="True", years)
	    limit_postfix = indict["limit_postfix"]
	    if indict["bonly125"]=="True" : limit_postfix += "_only125"
	    read_limits(limit_postfix,indict["bonly125"]=="True")
	    if indict["bbias"]=="True": 
	        if int(MA)>200:
	            plot_bias(MA,indict["limit_postfix"],MT="800")
	        else:
	            plot_bias(MA,indict["limit_postfix"],MT="600")
	        plot_bias(MA,indict["limit_postfix"],MT="1000")
	        
	    if indict["bsigInj"]=="True":
	        plot_signIn(MA,indict["limit_postfix"])
	
	    plot_limit(MA,indict["limit_postfix"], indict)
	    if indict["bonly125"]=="False":
	        print "====== Please produce also bonly125=True"
	
def plot_limit(MA,postfix, indict):
    command = 'cd /nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/; '
    #    print command
    #    os.system(command)

    #TString MA, TString postfix, bool draw_data=false, bool draw_had = false, bool draw_2017_comp = true,   bool draw_FullRun2_wocategories = false
    command += 'root -l -q -b "/nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/PlotLimits.cxx(\\"'+MA+'\\",\\"'+indict["limit_postfix"]+'\\",'+indict["draw_data"]+','+indict["draw_had"]+','+indict["draw_2017_comp"]+','+indict["draw_FullRun2_wocategories"]+')"'
    print command
    os.system(command)
    print "/nfs/dust/cms/user/reimersa/SingleTth/theta_SingleTth/utils2/Limits_MC/output/"
    

def create_workspace(MA,years, limitvariable):
    roofit_folder = '/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit' 
    command = 'cd '+roofit_folder+'; make;'
    print command
    os.system(command)

    command = 'cd '+roofit_folder+'; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit; echo $LD_LIBRARY_PATH; ./create_roo '+MA+' '+limitvariable+'; ./create_roo_2017 '+MA+' '+limitvariable+'; ./create_roo_2018 '+MA+' '+limitvariable+';'
    if "allyears" in years:
        command = 'cd '+roofit_folder+'; export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit; echo $LD_LIBRARY_PATH; ./create_roo_allyears '+MA+' '+limitvariable+';'
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
                    exit_code = os.system(command)
                    if exit_code: raise ValueError("Your signal fit didn't work!")
                else:
                    print("File exists already: "+file_to_check)

def submit_gofs(MA, ntoys):
    print "submitting gofs"
    scriptfolder = "/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit/datacards/"

    workdir = os.path.join(scriptfolder, "workdir_gofs")
    ensureDirectory(workdir) 
    MTs =[600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175]
    bfixedR = "false"

    for MT in MTs:
        bfixedR = "false"
        sub_time = "86400"
        if MT == 600 or MT==800 or MT==1000:  
            bfixedR = "true"
            #   sub_time = "24200"
        submissionsettings = derive_from_default_settings("run_GOF.sh",'"%i %i %s %i"' % (MT, ntoys, bfixedR, MA),"gof", scriptfolder, workdir, MA, MT, sub_time)
        submissionscriptname = os.path.join(workdir, 'submit_GOF_condor_MT%i.sub'%(MT))
        create_submitfile(settings=submissionsettings, outfilename=submissionscriptname)
        command = 'condor_submit %s' % (submissionscriptname)
        os.system(command)

        if bfixedR == "true":
            submissionsettings = derive_from_default_settings("run_GOF_fixedR.sh",'"%i %i %s %i"' % (MT, ntoys, bfixedR, MA),"gof", scriptfolder, workdir, MA, MT, sub_time)
            submissionscriptname = os.path.join(workdir, 'submit_GOF_fixedR_condor_MT%i.sub'%(MT))
            create_submitfile(settings=submissionsettings, outfilename=submissionscriptname)
            command = 'condor_submit %s' % (submissionscriptname)
            os.system(command)
            

    

if __name__ == '__main__':
    main()

