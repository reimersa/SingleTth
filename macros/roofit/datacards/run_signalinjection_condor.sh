#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_STORED:$LD_LIBRARY_PATH
export PATH=$PATH_STORED:$PATH


mass=$1
MA=$3
datacard="Datacard_allyears_M${mass}.txt"

years=("2016v3" "2017v2" "2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
freeze_params=""
set_params=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_params="${set_params}pdf_index_much_"$year"_"$cat"=1,pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_ech_"$year"_"$cat"=1,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"

freeze_params="${freeze_params}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","

done
done

freeze_params="${freeze_params: : -1}"
set_params="${set_params: : -1}"

signal=$2
echo "================================ " + $signal + "     "+ $mass


eval "combine $datacard -M FitDiagnostics  --setParameters ${set_params}   --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=2  --cminFallbackAlgo Minuit2,Simplex,0:0.1 -n initial_signal${signal}_${mass}_signal --saveWorkspace --autoMaxPOIs r --autoBoundsPOIs r --cminPreScan --cminPreFit 1" 

eval "python create_snapshot.py -mass ${mass} -signal ${signal} -name signal"

eval "combine -M GenerateOnly -d initialFitWorkspace_${signal}_${mass}_signal.root --snapshotName initialFit --expectSignal ${signal}  --setParameters ${set_params} --saveToys -m 125  --freezeParameters ${freeze_params} -n ${signal}_${mass}_signal -t 1000"

postfix=$4

eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_params} --toysFile higgsCombine${signal}_${mass}_signal.GenerateOnly.mH125.123456.root  -t 300 --rMin -10 --rMax 10 --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=2  --cminFallbackAlgo Minuit2,Simplex,0:0.1 -n ${postfix}_signal${signal}_${mass} --autoMaxPOIs r --autoBoundsPOIs r --cminPreScan --cminPreFit 1"



