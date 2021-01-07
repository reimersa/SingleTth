#!/bin/bash

years="2016v3"
masses=(1000)
mass=1000

functionused=1

set_params="pdf_index_much_2016v3=${functionused},pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=${functionused},pdf_index_MT${mass}_ech_2016v3=0,pdf_index_much_2017v2=${functionused},pdf_index_MT${mass}_much_2017v2=0,pdf_index_ech_2017v2=${functionused},pdf_index_MT${mass}_ech_2017v2=0,pdf_index_much_2018=${functionused},pdf_index_MT${mass}_much_2018=0,pdf_index_ech_2018=${functionused},pdf_index_MT${mass}_ech_2018=0"

freeze_params="pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_much_2017v2,pdf_index_MT${mass}_much_2017v2,pdf_index_ech_2017v2,pdf_index_MT${mass}_ech_2017v2,sg_mean_2017v2,sg_sigma_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,pdf_index_much_2018,pdf_index_MT${mass}_much_2018,pdf_index_ech_2018,pdf_index_MT${mass}_ech_2018,sg_mean_2018,sg_sigma_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018,sg_JERmeandown2_2018,sg_JERmeanup2_2018,sg_JECmeandown2_2018,sg_JECmeanup2_2018,sg_JERsigmadown2_2018,sg_JERsigmaup2_2018,sg_JECsigmadown2_2018,sg_JECsigmaup2_2018,sg_JERmeandown2_2017v2,sg_JERmeanup2_2017v2,sg_JECmeandown2_2017v2,sg_JECmeanup2_2017v2,sg_JERsigmadown2_2017v2,sg_JERsigmaup2_2017v2,sg_JECsigmadown2_2017v2,sg_JECsigmaup2_2017v2,sg_JERmeandown2_2016v3,sg_JERmeanup2_2016v3,sg_JECmeandown2_2016v3,sg_JECmeanup2_2016v3,sg_JERsigmadown2_2016v3,sg_JERsigmaup2_2016v3,sg_JECsigmadown2_2016v3,sg_JECsigmaup2_2016v3,sg_mean2_2016v3,sg_sigma2_2016v3,sg_mean2_2017v2,sg_sigma2_2017v2,sg_mean2_2018,sg_sigma2_2018,sg_fnorm_2018,sg_fnorm_2017v2,sg_fnorm_2016v3"



year="allyears"

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass


    eval "text2workspace.py Datacard_"$year"_M$mass.txt -m 125" 
#     eval "combine Datacard_"$year"_M$mass.txt -M GenerateOnly --setParameters  ${set_params}  --freezeParameters ${freeze_params}  --toysFrequentist -t 1 --expectSignal 0 --saveToys -m 125 --saveWorkspace"




##### all three years
   eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root --doInitialFit --robustFit 1 -m 125  --freezeParameters ${freeze_params} --setParameters ${set_params} --rMin -3 --rMax 3 --cminDefaultMinimizerStrategy=0" # normal version --seed 8192

    eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root -m 125 --robustFit 1 --doFits --rMin -3 --rMax 3 --cminDefaultMinimizerStrategy=0 "

    eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root  -m 125 -o impacts.json"


 

#    eval "source remove_parameters.sh"

    eval "plotImpacts.py -i impacts.json -o impacts"




##############  Nuisance pre and post fit
# eval "combine Datacard_"$year"_M$mass.txt -M GenerateOnly --setParameters  pdf_index_much_2016v3=2,pdf_index_ech_2016v3=2,pdf_index_much_2017v2=2,pdf_index_ech_2017v2=2,pdf_index_much_2018=2,pdf_index_ech_2018=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_MT${mass}_much_2017v2=0,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_MT${mass}_much_2018=0,pdf_index_MT${mass}_ech_2018=0  --freezeParameters  pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_MT${mass}_ech_2016v3,pdf_index_MT${mass}_much_2017v2,pdf_index_MT${mass}_ech_2017v2,pdf_index_MT${mass}_much_2018,pdf_index_MT${mass}_ech_2018 --toysFrequentist -t 1 --expectSignal 0 --saveToys -m 125"

# eval "combine Datacard_"$year"_M$mass.txt -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_ech_2016v3=2,pdf_index_much_2017v2=2,pdf_index_ech_2017v2=2,pdf_index_much_2018=2,pdf_index_ech_2018=2  --freezeParameters  pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018 --cminDefaultMinimizerStrategy=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root -t 1"

# eval "python /nfs/dust/cms/user/abenecke/CMSSW_10_2_10/CMSSW_10_2_10/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py fitDiagnostics.root -g outputfile.root --all --pullDef relDiffAsymErrs"
    
############ each nuissane by itself
# eval "combine -M MultiDimFit -n _initialFit_Test --algo singles --redefineSignalPOIs r --robustFit 1 --freezeParameters pdf_index_much_2016v3,pdf_index_ech_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,pdf_index_MT600_much_2016v3,pdf_index_MT600_ech_2016v3 --expectSignal 0 -m 125 -d Datacard_allyears_M600.root --setParameters pdf_index_much_2016v3=2,pdf_index_ech_2016v3=2,pdf_index_MT600_much_2016v3=0,pdf_index_MT600_ech_2016v3=0"

# eval "combine Datacard_"$year"_M$mass.txt -M MultiDimFit  --setParameters  ${set_params}  --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 --algo impact -P bgexp2_p0ech_2016v3"

# eval "combine Datacard_"$year"_M$mass.txt -M MultiDimFit  --setParameters  ${set_params}  --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 --algo impact -P bgexp2_p1ech_2016v3"

# eval "combine Datacard_"$year"_M$mass.txt -M MultiDimFit  --setParameters  ${set_params}  --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 --algo impact -P bgexp2_p0much_2016v3"

# eval "combine Datacard_"$year"_M$mass.txt -M MultiDimFit  --setParameters  ${set_params}  --freezeParameters  ${freeze_params} --cminDefaultMinimizerStrategy=0 --algo impact -P bgexp2_p1much_2016v3"


#     eval "plotImpacts.py -i impacts.json -o impacts"

done
