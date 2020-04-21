#!/bin/bash

#datacard="DataCard2016_M600_much.txt"
datacard="Datacard_allyears_M600_much.txt"


#eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=1,pdf_index_ech_2016v3=1,pdf_index_MT600_much_2016v3=0,pdf_index_MT600_ech_2016v3=0 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT600_much_2016v3,pdf_index_MT600_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3"


#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT600_much_2016v3=0,pdf_index_MT600_ech_2016v3=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT600_much_2016v3,pdf_index_MT600_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0"

#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT600_much_2016v3=0,pdf_index_MT600_ech_2016v3=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT600_much_2016v3,pdf_index_MT600_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes


###### Muon Channel Only

eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=3,pdf_index_MT600_much_2016v3=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT600_much_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3"


eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT600_much_2016v3=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT600_much_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0"


# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=0,pdf_index_MT600_much_2016v3=0 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT600_much_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,bg3p_p2much_2016v3,bg3p_p0much_2016v3,bg3p_p1much_2016v3"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=0,pdf_index_MT600_much_2016v3=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT600_much_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,bg3p_p2much_2016v3,bg3p_p0much_2016v3,bg3p_p1much_2016v3 --cminDefaultMinimizerStrategy=0"



#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT600_much_2016v3=0,pdf_index_MT600_ech_2016v3=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT600_much_2016v3,pdf_index_MT600_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes







echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw(\"(r-1)/rErr>>h(20,-4,4)\")"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


