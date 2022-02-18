#!/bin/bash



#for mass in {600,1000}
#for mass in {1000,800}
for mass in {800,600,1000}
do
#mass=1000
MA="125"
#datacard="Datacard_allyears_M${mass}_much.txt"
datacard="Datacard_allyears_M${mass}.txt"

years=("2016v3" "2017v2" "2018")
categories=("catma60" "chi2h_2" "catma90" "catma175" "catma300")
#categories=("catma90" "catma60" "catma300")
#categories=("chi2h_2")
freeze_params=""
set_params=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
set_params="${set_params}pdf_index_much_"$year"_"$cat"=1,pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_ech_"$year"_"$cat"=1,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"

freeze_params="${freeze_params}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat","

#freeze_params="pdf_index_much_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018"

done
done

freeze_params="${freeze_params: : -1}"
set_params="${set_params: : -1}"

for signal in 0.0 0.1 0.25 0.35 0.5 0.75 1.0 
#for signal in  1.0 
do
#signal=1.0
echo ""
echo ""
echo "================================ " + $signal + "     "+ $mass
echo ""
echo ""




########
##
## NOMINAL
##
#######

eval "combine $datacard -M FitDiagnostics  --setParameters ${set_params}   --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 -n initial_signal${signal}_${mass} --saveWorkspace" 

eval "python create_snapshot.py -mass ${mass} -signal ${signal}"

eval "combine -M GenerateOnly -d initialFitWorkspace.root --snapshotName initialFit --expectSignal ${signal}  --setParameters ${set_params} --saveToys -m 125  --freezeParameters ${freeze_params} -n ${mass} -t 1000"
#eval "combine -M GenerateOnly -d initialFitWorkspace.root --snapshotName initialFit --expectSignal ${signal} --toysFrequentist --bypassFrequentistFit --setParameters ${set_params} --saveToys -m 125  --freezeParameters ${freeze_params} -n ${mass} -t 1000"

#eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=0,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal ${signal} --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass} --seed 123123"--> old version


eval "combine $datacard -M FitDiagnostics  --setParameters  ${set_params} --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 300 --rMin -10 --rMax 10 --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 -n signal${signal}_${mass}_${MA}"


done
# echo "python plot_signalstrength.py 0 0.25 0.5 0.75 1 1.5 2 2.5 3 --name \"MuEch_signal_M${mass}_2016\" --mass ${mass}"
# eval "python plot_signalstrength.py 0 0.25 0.5 0.75 1 1.5 2 2.5 3 --name \"MuEch_signal_M${mass}_2016\" --mass ${mass}"
done
