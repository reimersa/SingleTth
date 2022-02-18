#!/bin/bash

mass="800"
datacard="Datacard_allyears_M${mass}.txt"
#datacard="Datacard_allyears_M${mass}_much.txt"
#datacard="Datacard_allyears_M${mass}_ech.txt"

signal=0.0
function_param=0
#name="exp2p_r1"
name="dijet3p_r0"
postfix="_mavariable_cb_MT${mass}GeV_MA125"


years=("2016v3" "2017v2" "2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
#categories=("catma90" "catma60" "catma300")
#categories=("chi2h_2")
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","

#set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
#set_param="${set_param: : -1}"


#####  all trhee years r=1
funct="1"
set_param=""
for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done
set_param="${set_param: : -1}"


eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n initial_signal1.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 1.0"


eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit  --setParameters  ${set_param}  -t 1000 --expectSignal 1.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n exp2p_r1${postfix}"
#eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10  --cminDefaultMinimizerStrategy=0 -n exp2p_r1${postfix}"


# ####### dijet 3p
funct="0"
set_param=""
for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done
set_param="${set_param: : -1}"

eval "combine $datacard -M FitDiagnostics  --setParameters ${set_param}  --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n initial_signal1.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 1.0"


 eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit  --setParameters ${set_param}  -t 1000 --expectSignal 1.0 --saveToys -m 125  --freezeParameters ${freeze_param}"



eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param} --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n dijet3p_r1${postfix}"


# # ##### r=0
funct="1"
set_param=""
for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done
set_param="${set_param: : -1}"

eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param}  --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n initial_signal0.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 0.0"


 eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit   --setParameters  ${set_param}  -t 1000 --expectSignal 0.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


  
 
eval "combine $datacard -M FitDiagnostics  --setParameters   ${set_param}  --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n exp2p_r0${postfix}"


# ########dijet 3p
funct=0
set_param=""
for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done
set_param="${set_param: : -1}"

eval "combine $datacard -M FitDiagnostics  --setParameters ${set_param}  --freezeParameters  ${freeze_param} --cminDefaultMinimizerStrategy=0 -n initial_signal0.0_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal 0.0"


 eval "combine -M GenerateOnly  -d initialFitWorkspace.root --snapshotName initialFit  --setParameters ${set_param}  -t 1000 --expectSignal 0.0 --saveToys -m 125  --freezeParameters ${freeze_param}"


eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_param} --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters ${freeze_param}  --cminDefaultMinimizerStrategy=0 -n dijet3p_r0${postfix}"






echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw(\"(r-1)/rErr>>h(20,-4,4)\")"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


