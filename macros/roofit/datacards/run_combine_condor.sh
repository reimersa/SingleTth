#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_STORED:$LD_LIBRARY_PATH
export PATH=$PATH_STORED:$PATH


year_nt="allyears"

years=("2017v2" "2016v3" "2018" "allyears")
categories=("chi2h_2" "catma90" "catma175" "catma300" "catma60")
funct="1"
freeze_param=""
set_param=""
mass=$1
postfix_only125=$2


for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","


# freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat","
# freeze_param="${freeze_param},sg_sigma_"$year"_"$cat",sg_mean_"$year"_"$cat","
freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
#freeze_param="${freeze_param},bg3p_p1much_"$year"_"$cat",bg3p_p1ech_"$year"_"$cat",bg3p_p0much_"$year"_"$cat",bg3p_p0ech_"$year"_"$cat",bg3p_p2much_"$year"_"$cat",bg3p_p2ech_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"
#echo ${freeze_param}


echo "Working on mass " $mass



#eval "combine -M AsymptoticLimits -d Datacard_"$year_nt"_M${mass}.txt  -n output_$mass$postfix_only125 --setParameters ${set_param} --freezeParameters ${freeze_param} --cminDefaultMinimizerStrategy=2  --cminFallbackAlgo Minuit2,Simplex,0:0.1" 

#this one
eval "combine -M AsymptoticLimits -d Datacard_"$year_nt"_M${mass}.txt  -n output_$mass$postfix_only125 --setParameters ${set_param} --freezeParameters ${freeze_param}  --seed 123456 --cminPreScan --cminPreFit 1 --cminDefaultMinimizerStrategy 0 " 

# --cminFallbackAlgo Minuit2,Simplex,0:0.1
#--X-rtd ADDNLL_CBNLL=0 --cminPreScan 


eval "combine Datacard_"$year_nt"_M${mass}.txt -M FitDiagnostics  --setParameters ${set_param}   --freezeParameters  ${freeze_param}  -n individualfit_"$year_nt"_M${mass} --saveWorkspace --saveShapes   --seed 123456 --plots --setRobustFitTolerance 1 --cminPreScan --cminPreFit 1 --cminDefaultMinimizerStrategy 0 --autoMaxPOIs r --autoBoundsPOIs r"

#echo "combine Datacard_"$year_nt"_M${mass}.txt -M FitDiagnostics  --setParameters ${set_param}   --freezeParameters  ${freeze_param}  -n individualfit_"$year_nt"_M${mass} --saveWorkspace --saveShapes   --seed 123456 --plots --setRobustFitTolerance 1 --cminPreScan --cminPreFit 1 -v5"

#--robustFit=1 --cminDefaultMinimizerStrategy=0 --verbose 3
