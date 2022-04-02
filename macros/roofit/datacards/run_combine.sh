#!/bin/bash

#masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
masses=(650)
#masses=(550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)
#masses=(875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)

#year="2016v3"
#year="2017v2"
#year="2018"
year_nt="allyears"



years=("2017v2" "2016v3" "2018" "allyears")
categories=("chi2h_2" "catma90" "catma175" "catma300" "catma60")
funct="0"

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass

freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","

#freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
#freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
#freeze_param="${freeze_param},bg3p_p1much_"$year"_"$cat",bg3p_p1ech_"$year"_"$cat",bg3p_p0much_"$year"_"$cat",bg3p_p0ech_"$year"_"$cat",bg3p_p2much_"$year"_"$cat",bg3p_p2ech_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"
#echo ${freeze_param}





##### all three years with 3p as uncertainty
eval "combine -M AsymptoticLimits -d Datacard_"$year_nt"_M${mass}.txt  -n output_$mass --setParameters ${set_param} --freezeParameters ${freeze_param} " # normal version --seed 8192

    
done
