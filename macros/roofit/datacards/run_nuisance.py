#!/bin/bash


mass=$1

years=("2017v2" "2016v3" "2018")
categories=("chi2h_2" "catma90" "catma175" "catma300" "catma60")

funct="1"

freeze_param=""
set_param=""

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",sg_mean2_"$year"_"$cat","


#freeze_param="${freeze_param},sg_sigma2_"$year"_"$cat","
#freeze_param="${freeze_param},sg_sigma_"$year"_"$cat",,sg_mean_"$year"_"$cat","
#freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","
freeze_param="${freeze_param},pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","
# freeze_param="${freeze_param},bgexp2_p1much_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p0ech_"$year"_"$cat","
#freeze_param="${freeze_param},bg3p_p1much_"$year"_"$cat",bg3p_p1ech_"$year"_"$cat",bg3p_p0much_"$year"_"$cat",bg3p_p0ech_"$year"_"$cat",bg3p_p2much_"$year"_"$cat",bg3p_p2ech_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
#set_param="${set_param}bgexp2_p0much_2018_catma90=5.76024,bg3p_p1ech_2017v2_catma90=-4.50492,bgexp2_p0much_2017v2_catma90=6.37625,bgexp2_p0ech_2016v3_catma90=4.33042,bg3p_p0much_2016v3_catma90=11.9721,bg3p_p2ech_2017v2_catma90=-2.31808,bg3p_p1much_2016v3_catma90=3.27259,bg3p_p2much_2016v3_catma90=1.09485,bgexp2_p1much_2017v2_catma90=3.98319,bgexp2_p1ech_2018_catma90=3.9209,bg3p_p0ech_2017v2_catma90=45.16,bg3p_p2ech_2016v3_catma90=3.61891,bg3p_p1ech_2018_catma90=6.61369,bg3p_p0much_2017v2_catma90=-9.46827,bgexp2_p1ech_2017v2_catma90=4.04613,bg3p_p0ech_2018_catma90=-0.638188,bg3p_p2ech_2018_catma90=2.47795,bg3p_p2much_2018_catma90=3.18862,bgexp2_p1much_2018_catma90=3.95635,bg3p_p1much_2018_catma90=8.65065,bg3p_p1ech_2016v3_catma90=9.07433,bg3p_p2much_2017v2_catma90=3.01117,bg3p_p0ech_2016v3_catma90=-9.78905,bgexp2_p0ech_2017v2_catma90=5.77725,bgexp2_p1much_2016v3_catma90=3.75012,bgexp2_p1ech_2016v3_catma90=3.93908,bg3p_p1much_2017v2_catma90=8.50185,bgexp2_p0much_2016v3_catma90=5.03689,bg3p_p0much_2018_catma90=-9.53489,bgexp2_p0ech_2018_catma90=5.3916,bg3p_p2ech_2017v2_chi2h_2=0.108108,bg3p_p2much_2018_chi2h_2=0.548779,bgexp2_p0ech_2018_chi2h_2=6.74893,bgexp2_p0much_2016v3_chi2h_2=6.86793,bg3p_p1ech_2017v2_chi2h_2=2.20064,bg3p_p1much_2016v3_chi2h_2=4.46473,bg3p_p1ech_2016v3_chi2h_2=3.52094,bg3p_p0ech_2017v2_chi2h_2=13.1748,bg3p_p1much_2018_chi2h_2=3.50244,bgexp2_p1much_2017v2_chi2h_2=3.9321,bg3p_p2ech_2018_chi2h_2=-0.104869,bg3p_p2much_2017v2_chi2h_2=-2.50525,bgexp2_p1ech_2017v2_chi2h_2=3.76423,bgexp2_p1ech_2016v3_chi2h_2=3.78777,bgexp2_p0much_2018_chi2h_2=7.81103,bg3p_p0much_2016v3_chi2h_2=5.57228,bg3p_p2ech_2016v3_chi2h_2=0.847332,bgexp2_p0ech_2016v3_chi2h_2=6.50014,bg3p_p0much_2018_chi2h_2=8.54708,bgexp2_p1much_2018_chi2h_2=3.95835,bg3p_p0ech_2018_chi2h_2=17.038,bgexp2_p1ech_2018_chi2h_2=3.80695,bg3p_p1much_2017v2_chi2h_2=-3.67884,bg3p_p1ech_2018_chi2h_2=1.47381,bgexp2_p0much_2017v2_chi2h_2=7.55685,bgexp2_p1much_2016v3_chi2h_2=3.89071,bg3p_p0much_2017v2_chi2h_2=37.6431,bg3p_p0ech_2016v3_chi2h_2=9.04538,bg3p_p2much_2016v3_chi2h_2=1.18868,bgexp2_p0ech_2017v2_chi2h_2=7.07914"
done
done

freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"

year="allyears"

echo "Working on mass " $mass


eval "text2workspace.py Datacard_"$year"_M$mass.txt -m 125" 
#     eval "combine Datacard_"$year"_M$mass.txt -M GenerateOnly --setParameters  ${set_params}  --freezeParameters ${freeze_params}  --toysFrequentist -t 1 --expectSignal 0 --saveToys -m 125 --saveWorkspace"


eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root --doInitialFit -m 125  --freezeParameters ${freeze_param} --setParameters ${set_param} --rMin -3 --rMax 3 --seed 123456  --setRobustFitTolerance 1  --cminPreScan --cminPreFit 1 --autoMaxPOIs r --autoBoundsPOIs r --cminDefaultMinimizerStrategy 0" # normal version --seed 8192

eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root -m 125  --doFits --rMin -3 --rMax 3 --seed 123456  --setRobustFitTolerance 1  --cminPreScan --cminPreFit 1 --autoMaxPOIs r --autoBoundsPOIs r --cminDefaultMinimizerStrategy 0"

eval "combineTool.py -M Impacts -d Datacard_"$year"_M$mass.root  -m 125 -o impacts_MT${mass}.json"


 

#    eval "source remove_parameters.sh"

eval "plotImpacts.py -i impacts_MT${mass}.json -o impacts_MT${mass}"




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


