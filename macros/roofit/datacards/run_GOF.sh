#!/bin/bash

#year="2016v3"
#year="2017v2"
#year="2018"
#year="allyears"
channel="ech"
name="GOF_SR_sim_2016v3_2917v2_2018_${channel}_catma60"
mass=800

funct=0

years=("2016v3" "2017v2" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"

year="allyears"

#set_param="pdf_index_much_2016v3=${funct},pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=${funct},pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=${funct},pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=${funct},pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=${funct},pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=${funct},pdf_index_MT${mass}_ech_2018=0"

#freeze_param="pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018,sg_JERmeandown2_2018,sg_JERmeanup2_2018,sg_JECmeandown2_2018,sg_JECmeanup2_2018,sg_JERsigmadown2_2018,sg_JERsigmaup2_2018,sg_JECsigmadown2_2018,sg_JECsigmaup2_2018,sg_JERmeandown2_2017v2,sg_JERmeanup2_2017v2,sg_JECmeandown2_2017v2,sg_JECmeanup2_2017v2,sg_JERsigmadown2_2017v2,sg_JERsigmaup2_2017v2,sg_JECsigmadown2_2017v2,sg_JECsigmaup2_2017v2,sg_JERmeandown2_2016v3,sg_JERmeanup2_2016v3,sg_JECmeandown2_2016v3,sg_JECmeanup2_2016v3,sg_JERsigmadown2_2016v3,sg_JERsigmaup2_2016v3,sg_JECsigmadown2_2016v3,sg_JECsigmaup2_2016v3,sg_mean2_2016v3,sg_sigma2_2016v3,sg_mean2_2017v2,sg_sigma2_2017v2,sg_mean2_2018,sg_sigma2_2018,sg_fnorm_2018,sg_fnorm_2017v2,sg_fnorm_2016v3"



eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}_${channel}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated -t 1000 -s 123456" # normal version --seed 8192
#eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated -t 500 -s 123456" # normal version --seed 8192


echo "============================================"


eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}_${channel}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated --saveWorkspace --fixedSignalStrength=0" # normal version --seed 8192
#eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated --saveWorkspace --fixedSignalStrength=0" # normal version --seed 8192





echo "  "
echo "  "
echo "python plot_GOF.py --name ${name} -integer  "
echo "  "
echo "  "
