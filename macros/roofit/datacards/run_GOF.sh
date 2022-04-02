#!/bin/bash

#year="2016v3"
#year="2017v2"
#year="2018"
#year="allyears"
#channel="ech"
#name="GOF_SR_sim_2016v3_2017v2_2018_${channel}_catma60"
name="GOF_SR_allyears"
mass=850

funct=0

years=("2016v3" "2017v2" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","


set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"

year="allyears"




eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated -t 1000 -s 123456" # normal version --seed 8192

echo "============================================"


eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated --saveWorkspace --fixedSignalStrength=0" # normal version --seed 8192



echo "  "
echo "  "
echo "python plot_GOF.py --name ${name} -integer  "
echo "  "
echo "  "
