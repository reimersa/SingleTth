from __future__ import division
import subprocess
import glob
import os
import sys
from array import array
import math
import re


preselection = False
year = "2016"
year_tag =year
if "16" in year: year_tag+="v3"
if "17" in year: year_tag+="v2"

jsonfile = "2016/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.root"
if "17" in year: jsonfile = "2017/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.root"
if "18" in year: jsonfile = "2018/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.root"

pileupfileMC = "2016/MyMCPileupHistogram.root"
if "17" in year: pileupfileMC = "2017/MyMCPileupHistogram.root"
if "18" in year: pileupfileMC = "2018/MyMCPileupHistogram.root"

pileupfileData = "2016/MyDataPileupHistogram.root"
if "17" in year: pileupfileData = "2017/MyDataPileupHistogram.root"
if "18" in year: pileupfileData = "2018/MyDataPileupHistogram.root"

btagcalibfile = "2016/DeepJet_2016LegacySF_WP_V1.csv"
if "17" in year: btagcalibfile = ""
if "18" in year: btagcalibfile = ""

lumi = 35920
if "17" in year: lumi = "41530"
if "16" in year: lumi = "59740"

files = os.listdir(".")

postfix = "_newsamples"
if preselection: postfix = "Preselection_newsamples"
postfix += year


out = open("SingleTth"+postfix+".xml", "w")

out.write('<?xml version="1.0" encoding="UTF-8"?> \n')
out.write('<!DOCTYPE JobConfiguration PUBLIC "" "JobConfig.dtd"[\n')
out.write('<!ENTITY TargetLumi      "'+lumi+'"> \n')
out.write('<!ENTITY PRESELdir       "/nfs/dust/cms/user/reimersa/SingleTth/'+year+'/Preselection/NOMINAL/">\n')
if not preselection: 
    out.write('<!ENTITY FULLSELdir      "/nfs/dust/cms/user/reimersa/SingleTth/'+year+'/Fullselection/mavariable/NOMINAL">\n')

out.write('<!ENTITY b_Cacheable     "False"> \n')
out.write('<!ENTITY NEVT            "-1"> \n')
out.write('<!ENTITY PROOFdir        "/nfs/dust/cms/user/reimersa/.proof2">\n')
out.write('\n')

for f in files:
    if "TprimeBToTH_TLep" not in f: continue
    print(f)

    MA=re.findall("MH\d*",f)[0].replace("MH","")
    MT=re.findall("MT\d*",f)[0].replace("MT","")
    name = "VLQ_LH_"+MT+"_ma"+MA
    print name
    path = os.getcwd()
    path += "/"

    if preselection:
        out.write('<!ENTITY '+name+' SYSTEM "'+path+f+'">\n')
    else:
        out.write('<!ENTITY '+name+'  "&PRESELdir;/uhh2.AnalysisModuleRunner.MC.VLQ_LH_'+MT+'_ma'+MA+'_'+year_tag+'.root">\n')


out.write(']>\n')

out.write('\n')

out.write('<!-- \n')
out.write('<ConfigParse NEventsBreak="0" FileSplit="25" AutoResubmit="5"/>\n') 
if preselection:
    out.write('<ConfigSGE RAM="4" DISK="3" Mail="anna.benecke@desy.de" Notification="as" Workdir="workdir_SingleTth_Pre'+year+'"/>\n') 
else:
    out.write('<ConfigSGE RAM="4" DISK="3" Mail="anna.benecke@desy.de" Notification="as" Workdir="workdir_SingleTth'+year+'"/>\n') 
out.write('-->\n') 

out.write('\n')

out.write('<JobConfiguration JobName="ExampleCycleJob" OutputLevel="INFO">\n')
out.write('	<Library Name="libSUHH2SingleTth"/>\n') 
out.write('	<Package Name="SUHH2SingleTth.par" />\n') 

if preselection:
    out.write('	<Cycle Name="uhh2::AnalysisModuleRunner" OutputDirectory="&PRESELdir;/" PostFix="" TargetLumi="&TargetLumi;" >\n') 
else:
    out.write('	<Cycle Name="uhh2::AnalysisModuleRunner" OutputDirectory="&FULLSELdir;/" PostFix="" TargetLumi="&TargetLumi;" >\n') 

out.write('\n')

for f in files:
    if "TprimeBToTH_TLep" not in f: continue

    MA=re.findall("MH\d*",f)[0].replace("MH","")
    MT=re.findall("MT\d*",f)[0].replace("MT","")
    name = "VLQ_LH_"+MT+"_ma"+MA

    lumi = -1
    
    fi = open(f,"r")
    lastline = fi.readlines()[-1]
    lumi =  re.findall("\d+.\d+",lastline)[0]

    if lumi == -1: raise Exception("no lumi found!")

    if preselection:
        out.write('	<InputData Lumi="'+lumi+'" NEventsMax="&NEVT;" Type="MC" Version="VLQ_LH_'+MT+'_ma'+MA+'_'+year_tag+'" Cacheable="&b_Cacheable;">\n')
        out.write('			&'+name+';\n')
        out.write('			<InputTree Name="AnalysisTree" />\n')
        out.write('			<OutputTree Name="AnalysisTree" />\n')
        out.write('		</InputData>\n')
    else:
        out.write('	  <InputData Lumi="'+lumi+'" NEventsMax="&NEVT;" Type="MC" Version="VLQ_LH_'+MT+'_ma'+MA+'_'+year_tag+'" Cacheable="&b_Cacheable;"> \n')
        out.write('       <In FileName="&VLQ_LH_'+MT+'_ma'+MA+';" Lumi="0.0"/> \n')
        out.write('	  		<InputTree Name="AnalysisTree" /> \n')  
        out.write('	  		<OutputTree Name="AnalysisTree" /> \n')  
        out.write('	  	</InputData>')
    


out.write('		<UserConfig> \n') 
out.write(' 			<Item Name="PrimaryVertexCollection" Value="offlineSlimmedPrimaryVertices" /> \n') 
out.write(' 			<Item Name="ElectronCollection" Value="slimmedElectronsUSER" /> \n') 
out.write(' 			<Item Name="MuonCollection" Value="slimmedMuonsUSER" /> \n') 
out.write(' 			<Item Name="JetCollection" Value="jetsAk4CHS" /> \n') 
out.write(' 			<Item Name="METName" Value="slimmedMETs" /> \n') 
out.write(' 			<Item Name="GenJetCollection" Value="slimmedGenJets" /> \n') 
out.write(' 			<Item Name="GenParticleCollection" Value="GenParticles" /> \n') 
out.write('			<Item Name="GenInfoName" Value="genInfo" /> \n') 

out.write(' 			<Item Name="lumi_file" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+jsonfile+'" /> \n') 
out.write(' 			<Item Name="lumihists_lumi_per_bin" Value="1000."/> \n') 



if preselection:
    out.write(' 		<Item Name="AnalysisModule" Value="SingleTthPreselectionModule" /> \n') 
else:
    out.write(' 		<Item Name="AnalysisModule" Value="SingleTthAnalysisModule" /> \n') 


out.write(' 		<Item Name="use_sframe_weight" Value="false" /> \n') 

out.write(' 		<Item Name="pileup_directory" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+pileupfileMC+'" /> \n') 

out.write(' 		<Item Name="pileup_directory_data" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+pileupfileData+'" /> \n') 
out.write(' 		<Item Name="pileup_directory_data_up" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+year+'/MyDataPileupHistogram_72383.root" /> \n') 
out.write(' 		<Item Name="pileup_directory_data_down" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+year+'/MyDataPileupHistogram_66017.root" /> \n') 
if not preselection:
    out.write('		<Item Name="MCBtagEfficiencies" Value="/nfs/dust/cms/user/abenecke/CMSSW_10_2_10/CMSSW_10_2_10/src/UHH2/SingleTth/data/'+year+'/BTagMCEfficiencyHists_mediumWP.root" /> \n')
    out.write('		<Item Name="BTagCalibration" Value="/nfs/dust/cms/user/reimersa/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/'+btagcalibfile+'" /> \n')



out.write(' 		<Item Name="jecsmear_direction" Value="nominal" /> \n') 
out.write(' 		<Item Name="jersmear_direction" Value="nominal" /> \n') 
out.write(' 		<Item Name="ScaleVariationMuR"       Value="none" /> \n') 
out.write(' 		<Item Name="ScaleVariationMuF"       Value="none" /> \n') 
out.write('		<Item Name="sys_muonid"      Value="nominal" /> \n') 
out.write('		<Item Name="sys_muoniso"     Value="nominal" /> \n') 
out.write('		<Item Name="sys_muontrigger" Value="nominal" /> \n') 
out.write('		<Item Name="sys_eleid"       Value="nominal" /> \n') 
out.write('		<Item Name="sys_elereco"     Value="nominal" /> \n') 
out.write('		<Item Name="sys_eletrigger"  Value="nominal" /> \n') 
out.write('		<Item Name="sys_btag"        Value="central" /> \n') 
out.write('		<Item Name="sys_pu"          Value="nominal" /> \n') 


out.write(' 		<Item Name="TestKey" Value="TestKeyValue" /> \n') 
out.write(' 	</UserConfig> \n') 
out.write(' </Cycle> \n') 
out.write(' </JobConfiguration> \n')




out.close()
