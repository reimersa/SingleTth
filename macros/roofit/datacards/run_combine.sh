#!/bin/bash

#masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
#masses=(700)
#masses=(550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)
masses=(925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)

#year="2016v3"
#year="2017v2"
#year="2018"
year_nt="allyears"
cat="chi2h_2"

years=("2017v2" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"
#echo ${freeze_param}

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass


funct="1"

##### all three years with 3p as uncertainty
eval "combine -M AsymptoticLimits -d Datacard_"$year_nt"_M${mass}.txt  -n output_$mass --setParameters ${set_param} --freezeParameters ${freeze_param}" # normal version --seed 8192




    
done
