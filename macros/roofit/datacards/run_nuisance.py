#!/bin/bash

years="2016v3"
masses=(600)
mass=600
set_params="pdf_index_much_2016v3=1,pdf_index_ech_2016v3=1,pdf_index_much_2017v2=1,pdf_index_ech_2017v2=1,pdf_index_much_2018=1,pdf_index_ech_2018=1,pdf_index_MT${mass}_much_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0,pdf_index_MT${mass}_much_2017v2=0,pdf_index_MT${mass}_ech_2017v2=0,pdf_index_MT${mass}_much_2018=0,pdf_index_MT${mass}_ech_2018=0"

#set_params="pdf_index_much_${years}=2,pdf_index_ech_${years}=2,pdf_index_MT${mass}_much_${years}=0,pdf_index_MT${mass}_ech_${years}=0"
#set_params="pdf_index_much_${years}=1,pdf_index_ech_${years}=1,pdf_index_MT${mass}_much_${years}=0,pdf_index_MT${mass}_ech_${years}=0"
set_params_sig="pdf_index_MT${mass}_much_${years}=0,pdf_index_MT${mass}_ech_${years}=0"

freeze_params="pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018,pdf_index_MT${mass}_much_2016v3,pdf_index_MT${mass}_ech_2016v3,pdf_index_MT${mass}_much_2017v2,pdf_index_MT${mass}_ech_2017v2,pdf_index_MT${mass}_much_2018,pdf_index_MT${mass}_ech_2018"

#freeze_params="pdf_index_much_${years},pdf_index_ech_${years},sg_JERmeandown_${years},sg_JERmeanup_${years},sg_JECmeandown_${years},sg_JECmeanup_${years},sg_JERsigmadown_${years},sg_JERsigmaup_${years},sg_JECsigmadown_${years},sg_JECsigmaup_${years},pdf_index_MT${mass}_much_${years},pdf_index_MT${mass}_ech_${years}"

#freeze_params="sg_JERmeandown_${years},sg_JERmeanup_${years},sg_JECmeandown_${years},sg_JECmeanup_${years},sg_JERsigmadown_${years},sg_JERsigmaup_${years},sg_JECsigmadown_${years},sg_JECsigmaup_${years}"

freeze_params_sig="pdf_index_MT${mass}_much_${years},pdf_index_MT${mass}_ech_${years}"



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
