#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_STORED:$LD_LIBRARY_PATH
export PATH=$PATH_STORED:$PATH

name="GOF_SR_allyears"
mass=$1
ntoys=$2
bfixedR=$3
MA=$4
funct=1

years=("2016v3" "2017v2" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300")
freeze_param=""
set_param=""


for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","


#freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat","
#freeze_param="${freeze_param},sg_sigma_"$year"_"$cat",sg_mean_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
#freeze_param="${freeze_param},bg3p_p1much_"$year"_"$cat",bg3p_p1ech_"$year"_"$cat",bg3p_p0much_"$year"_"$cat",bg3p_p0ech_"$year"_"$cat",bg3p_p2much_"$year"_"$cat",bg3p_p2ech_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"

year="allyears"



echo "floating R"

eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated -t ${ntoys}   -n GOFfloatR_toys_M${mass}_MA${MA}" # normal version --seed 8192

echo "============================================"


eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}.txt --setParameters ${set_param} --freezeParameters ${freeze_param} --algo=saturated --saveWorkspace  -n GOFfloatR_data_M${mass}_MA${MA}" # normal version --seed 8192



echo "  "
echo "  "
echo "python plot_GOF.py --name ${name} -integer  "
echo "  "
echo "  "
