#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_STORED:$LD_LIBRARY_PATH
export PATH=$PATH_STORED:$PATH


mass=$1
datacard="Datacard_allyears_M${mass}.txt"

name="dijet3p"
if [ $3 -eq 1 ]
then
name="exp2p"
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

postfix="_mavariable_cb_condor_MT${mass}GeV_MA${MA}"


years=("2016v3" "2017v2" "2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","

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


eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n initial_signal${signal}_${mass}_${name} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal ${signal} -name ${name}"


eval "combine -M GenerateOnly  -d initialFitWorkspace_${signal}_${mass}_${name}.root --snapshotName initialFit  --setParameters  ${set_param}  -t 1000 --expectSignal ${signal} --saveToys -m 125  --freezeParameters ${freeze_param} -n ${signal}_${mass}_${name}"


eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombine${signal}_${mass}_${name}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n ${name}${postfix}"



