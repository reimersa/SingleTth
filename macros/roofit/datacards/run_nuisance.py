#!/bin/bash


masses=(600)
mass=600

functionused=1

years=("2017v2" "2016v3" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300" "catma60")
funct="1"
freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","

#freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
#freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"

year="allyears"

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass


    eval "text2workspace.py Datacard_"$year"_M$mass.txt -m 125" 
#     eval "combine Datacard_"$year"_M$mass.txt -M GenerateOnly --setParameters  ${set_params}  --freezeParameters ${freeze_params}  --toysFrequentist -t 1 --expectSignal 0 --saveToys -m 125 --saveWorkspace"




##### all three years
   eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root --doInitialFit --robustFit 1 -m 125  --freezeParameters ${freeze_param} --setParameters ${set_param} --rMin -3 --rMax 3 --cminDefaultMinimizerStrategy=0" # normal version --seed 8192

    eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root -m 125 --robustFit 1 --doFits --rMin -3 --rMax 3 --cminDefaultMinimizerStrategy=0"

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
