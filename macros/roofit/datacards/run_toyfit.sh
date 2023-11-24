#!/bin/bash


mass=$1
MA=$3
datacard="Datacard_allyears_M${mass}.txt"

years=("2016v3" "2017v2" "2018")
years=("2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
categories=("chi2h_2")
freeze_params=""
set_params=""

func=1

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_params="${set_params}pdf_index_much_"$year"_"$cat"=${func},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_ech_"$year"_"$cat"=${func},pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"

#freeze_params="${freeze_params}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat
freeze_params="${freeze_params},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat
#freeze_params="${freeze_params},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat
freeze_params="${freeze_params},sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat
#freeze_params="${freeze_params},sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","
freeze_params="${freeze_params},sg_alphaR_"$year"_"$cat",sg_alphaL_"$year"_"$cat",sg_nR_"$year"_"$cat",sg_nL_"$year"_"$cat","
#freeze_params="${freeze_params},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
#freeze_params="${freeze_params},bg4pexp_p1much_"$year"_"$cat",bg4pexp_p1ech_"$year"_"$cat",bg4pexp_p0much_"$year"_"$cat",bg4pexp_p0ech_"$year"_"$cat",bg4pexp_p2much_"$year"_"$cat",bg4pexp_p2ech_"$year"_"$cat","
#freeze_params="${freeze_params},bg4pexp_p0much_"$year"_"$cat",bg4pexp_p0ech_"$year"_"$cat","
#freeze_params="${freeze_params},bg4pexp_p3much_"$year"_"$cat",bg4pexp_p3ech_"$year"_"$cat","
#freeze_params="${freeze_params},bg4pexp_p1much_"$year"_"$cat",bg4pexp_p1ech_"$year"_"$cat","
#freeze_params="${freeze_params},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat","
done
done

freeze_params="${freeze_params: : -1}"
set_params="${set_params: : -1}"

signal=$2
echo "================================ " + $signal + "     "+ $mass



eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_params} --toysFile TEST.root  -t 1 --rMin -10 --rMax 10 --freezeParameters  ${freeze_params} --setRobustFitTolerance 1 --cminPreScan --cminPreFit 1 --cminDefaultMinimizerStrategy 0 --autoMaxPOIs r --autoBoundsPOIs r -n outToy_${postfix}_signal${signal}_${mass} --saveShapes"
