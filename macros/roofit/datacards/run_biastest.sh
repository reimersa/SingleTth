#!/bin/bash

mass="700"
datacard="Datacard_allyears_M${mass}.txt"
#datacard="Datacard_allyears_M${mass}_much.txt"
#datacard="Datacard_allyears_M${mass}_ech.txt"

year="2016v3"
signal=0.0
function_param=0
#name="exp2p_r1"
name="dijet3p_r0"
postfix="_fitmin450GeV_MT${mass}GeV"
# ####### one year specific

#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=${function_param},pdf_index_MT${mass}_much_${year}=0,pdf_index_ech_${year}=${function_param},pdf_index_MT${mass}_ech_${year}=0  --freezeParameters  pdf_index_much_${year},pdf_index_MT${mass}_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 -n initial_signal${signal}_${mass}${postfix} --saveWorkspace --saveShapes" 

# eval "python create_snapshot.py -mass ${mass} -signal ${signal}"


#  eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit --toysFrequentist --bypassFrequentistFit --setParameters pdf_index_much_${year}=${function_param},pdf_index_ech_${year}=${function_param},pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0  -t 1000 --expectSignal ${signal} --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=${function_param},pdf_index_ech_${year}=${function_param},pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 -n ${name}"


#####  alll trhee years r=1
set_param="pdf_index_much_2016v3=1,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=1,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=1,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=1,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=1,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=1,pdf_index_MT${mass}_ech_2018=0"

freeze_param="pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018"

eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n initial_signal1.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 1.0"


 eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit --toysFrequentist --bypassFrequentistFit --setParameters  ${set_param}  -t 1000 --expectSignal 1.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n exp2p_r1${postfix}"


####### dijet 3p

set_param="pdf_index_much_2016v3=0,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=0,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=0,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=0,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=0,pdf_index_MT${mass}_ech_2018=0"

eval "combine $datacard -M FitDiagnostics  --setParameters ${set_param}  --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n initial_signal1.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 1.0"


 eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit --toysFrequentist --bypassFrequentistFit --setParameters ${set_param}  -t 1000 --expectSignal 1.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


##set_param="pdf_index_much_2016v3=1,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=1,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=1,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=1,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=1,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=1,pdf_index_MT${mass}_ech_2018=0"


eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param} --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n dijet3p_r1${postfix}"


# ##### r=0
# set_param="pdf_index_much_2016v3=1,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=1,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=1,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=1,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=1,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=1,pdf_index_MT${mass}_ech_2018=0"

# freeze_param="pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018"

# eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n initial_signal0.0_${mass} --saveWorkspace" 

# eval "python create_snapshot.py -mass ${mass} -signal 0.0"


#  eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit --toysFrequentist --bypassFrequentistFit --setParameters  ${set_param}  -t 1000 --expectSignal 0.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


  
 
# eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n exp2p_r0${postfix}"


# ########dijet 3p

# set_param="pdf_index_much_2016v3=0,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=0,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=0,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=0,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=0,pdf_index_MT${mass}_ech_2018=0"

# eval "combine $datacard -M FitDiagnostics  --setParameters ${set_param}  --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n initial_signal0.0_${mass} --saveWorkspace" 

# eval "python create_snapshot.py -mass ${mass} -signal 0.0"


#  eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit --toysFrequentist --bypassFrequentistFit --setParameters ${set_param}  -t 1000 --expectSignal 0.0 --saveToys -m 125  --freezeParameters ${freeze_param}"

# ## eval "combine $datacard -M GenerateOnly  --toysFrequentist --bypassFrequentistFit --setParameters ${set_param}  -t 100 --expectSignal 0.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


# ##set_param="pdf_index_much_2016v3=1,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=1,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=1,pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=1,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=1,pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=1,pdf_index_MT${mass}_ech_2018=0"


# eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param} --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n dijet3p_r0${postfix}"




####



#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_ech_${year}=0,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes


###### Muon Channel Only

# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_${year}=2,pdf_index_MT${mass}_much_${year}=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=2,pdf_index_MT${mass}_much_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 500 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0"


# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_${year}=0,pdf_index_MT${mass}_much_${year}=0 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year},bg3p_p2much_${year},bg3p_p0much_${year},bg3p_p1much_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_MT${mass}_much_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year},bg3p_p2much_${year},bg3p_p0much_${year},bg3p_p1much_${year} --cminDefaultMinimizerStrategy=0"



#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_ech_${year}=0,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes


############  nur exp ohne andere
# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -3 --rMax 3 --freezeParameters  pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0"






echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw(\"(r-1)/rErr>>h(20,-4,4)\")"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


