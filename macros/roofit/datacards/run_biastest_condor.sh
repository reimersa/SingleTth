#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_STORED:$LD_LIBRARY_PATH
export PATH=$PATH_STORED:$PATH


mass=$1
datacard="Datacard_allyears_M${mass}.txt"

name="exp2p"
if [ $3 -eq 1 ]
then
name="expX"
fi

signal=$2
if [ 1 -eq "$(echo "$signal == 0.0" | bc)" ]
then
name="${name}_r0"
else
name="${name}_r1"
fi

echo $name

MA=$4

postfix=$5"_MT${mass}GeV"


years=("2016v3" "2017v2" "2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
#freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","
freeze_param="${freeze_param},sg_alphaR_"$year"_"$cat",sg_alphaL_"$year"_"$cat",sg_nR_"$year"_"$cat",sg_nL_"$year"_"$cat","
#freeze_param="${freeze_param},sg_nR_"$year"_"$cat",sg_nL_"$year"_"$cat","

# freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat","
freeze_param="${freeze_param},sg_sigma_"$year"_"$cat",sg_mean_"$year"_"$cat","
freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
#freeze_param="${freeze_param},bg3p_p1much_"$year"_"$cat",bg3p_p1ech_"$year"_"$cat",bg3p_p0much_"$year"_"$cat",bg3p_p0ech_"$year"_"$cat",bg3p_p2much_"$year"_"$cat",bg3p_p2ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexpX_p1much_"$year"_"$cat",bgexpX_p1ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexpX_p0much_"$year"_"$cat",bgexpX_p0ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexpX_p2much_"$year"_"$cat",bgexpX_p2ech_"$year"_"$cat","


done
done

freeze_param="${freeze_param: : -1}"


funct=$3
set_param=""
for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done
set_param="${set_param: : -1}"


#eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param} --cminDefaultMinimizerStrategy 2  -n initial_signal${signal}_${mass}_${name} --saveWorkspace --cminPreScan --cminPreFit 3 --robustHesse 1 --cminDefaultMinimizerTolerance 1 --rMin 0 --rMax 5" 

## default way
# echo "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  -n initial_signal${signal}_${mass}_${name} --saveWorkspace --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 --rMin -5 --rMax 5 --bypassFrequentistFit"

# eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  -n initial_signal${signal}_${mass}_${name} --saveWorkspace --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 --rMin -5 --rMax 5 --bypassFrequentistFit --text2workspace X-assign-flatParam-prior"

#generator asimov 
eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  -n initial_signal${signal}_${mass}_${name} --saveWorkspace --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 --rMin -5 --rMax 5 -t -1 --expectSignal ${signal}"




#--cminPreScan --cminPreFit 3 --robustHesse 1 --cminDefaultMinimizerTolerance 1 --rMin 0 --rMax 5" 

# --cminDefaultMinimizerStrategy=2  --cminFallbackAlgo Minuit2,Simplex,0:0.1

# default mway
#eval "python create_snapshot.py -mass ${mass} -signal ${signal} -name ${name}"


#eval "combine -M GenerateOnly  -d initialFitWorkspace_${signal}_${mass}_${name}.root --snapshotName initialFit  --setParameters  ${set_param}  -t 1000 --expectSignal ${signal} --saveToys -m 125  --freezeParameters ${freeze_param} -n ${signal}_${mass}_${name} "
#Asimov
#eval "combine -M GenerateOnly  -d initialFitWorkspace_${signal}_${mass}_${name}.root --snapshotName initialFit  --setParameters  ${set_param}  -t -1 --expectSignal ${signal} --saveToys -m 125  --freezeParameters ${freeze_param} -n ${signal}_${mass}_${name} "


#eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombine${signal}_${mass}_${name}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -5 --rMax 5 --freezeParameters  ${freeze_param}  -n ${name}${postfix} --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 --noErrors --minos none "
#asimov
eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombine${signal}_${mass}_${name}.GenerateOnly.mH125.123456.root  -t -1 --rMin -5 --rMax 5 --freezeParameters  ${freeze_param}  -n ${name}${postfix} --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 --noErrors --minos none "

eval "combine  $datacard  -M MultiDimFit --algo grid --rMin -0.2 --rMax 0.2 --points 100 -t -1   --toysFile higgsCombine${signal}_${mass}_${name}.GenerateOnly.mH125.123456.root --setParameters   ${set_param} --freezeParameters  ${freeze_param}  -n likelihood${name}${postfix}  --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 "

eval "combine  $datacard  -M MultiDimFit --algo grid --rMin 0 --rMax 2 --points 1000 -t -1   --toysFile higgsCombine${signal}_${mass}_${name}.GenerateOnly.mH125.123456.root --setParameters   ${set_param} --freezeParameters  ${freeze_param}  -n likelihood_r1${name}${postfix}  --cminDefaultMinimizerStrategy 0  --cminPreScan --cminPreFit 1 --cminDefaultMinimizerTolerance 1 "

#--cminDefaultMinimizerStrategy=2 -n ${name}${postfix}  --cminPreScan --cminPreFit 1" # --robustHesse 1"



