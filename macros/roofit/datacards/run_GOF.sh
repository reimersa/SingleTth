#!/bin/bash

#year="2016v3"
#year="2017v2"
#year="2018"
year="allyears"
channel="ech"
name="GOF_SR_data_2017v2_${channel}"
mass=600

funct=0

set_param="pdf_index_much_2016v3=${funct},pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=${funct},pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=${funct},pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=${funct},pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=${funct},pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=${funct},pdf_index_MT${mass}_ech_2018=0"

freeze_param="pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018,sg_JERmeandown2_2018,sg_JERmeanup2_2018,sg_JECmeandown2_2018,sg_JECmeanup2_2018,sg_JERsigmadown2_2018,sg_JERsigmaup2_2018,sg_JECsigmadown2_2018,sg_JECsigmaup2_2018,sg_JERmeandown2_2017v2,sg_JERmeanup2_2017v2,sg_JECmeandown2_2017v2,sg_JECmeanup2_2017v2,sg_JERsigmadown2_2017v2,sg_JERsigmaup2_2017v2,sg_JECsigmadown2_2017v2,sg_JECsigmaup2_2017v2,sg_JERmeandown2_2016v3,sg_JERmeanup2_2016v3,sg_JECmeandown2_2016v3,sg_JECmeanup2_2016v3,sg_JERsigmadown2_2016v3,sg_JERsigmaup2_2016v3,sg_JECsigmadown2_2016v3,sg_JECsigmaup2_2016v3,sg_mean2_2016v3,sg_sigma2_2016v3,sg_mean2_2017v2,sg_sigma2_2017v2,sg_mean2_2018,sg_sigma2_2018,sg_fnorm_2018,sg_fnorm_2017v2,sg_fnorm_2016v3"



    eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}_${channel}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated -t 1000 -s 123456" # normal version --seed 8192


echo "============================================"


    eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}_${channel}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated --saveWorkspace --fixedSignalStrength=0" # normal version --seed 8192





echo "  "
echo "  "
echo "python plot_GOF.py --name ${name} -integer  "
echo "  "
echo "  "