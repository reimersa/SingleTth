#!/bin/bash

mass="600"
#datacard="Datacard_allyears_M${mass}.txt"
datacard="Datacard_allyears_M${mass}_much.txt"
#datacard="Datacard_allyears_M${mass}_ech.txt"

year="2016v3"

 eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_${year}=3,pdf_index_ech_${year}=3,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"




eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=2,pdf_index_ech_${year}=2,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 -n dijet2p"

#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_ech_${year}=0,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes


###### Muon Channel Only

# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_${year}=2,pdf_index_MT${mass}_much_${year}=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=2,pdf_index_MT${mass}_much_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 500 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0"


# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_${year}=0,pdf_index_MT${mass}_much_${year}=0 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year},bg3p_p2much_${year},bg3p_p0much_${year},bg3p_p1much_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_MT${mass}_much_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_MT${mass}_much_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year},bg3p_p2much_${year},bg3p_p0much_${year},bg3p_p1much_${year} --cminDefaultMinimizerStrategy=0"



#eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_${year}=0,pdf_index_ech_${year}=0,pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters  pdf_index_much_${year},pdf_index_ech_${year},pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0 --saveShapes --saveNormalizations" ## with shapes


############  nur exp ohne andere
# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_MT${mass}_much_${year}=0,pdf_index_MT${mass}_ech_${year}=0 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1000 --rMin -3 --rMax 3 --freezeParameters  pdf_index_MT${mass}_much_${year},pdf_index_MT${mass}_ech_${year},sg_mean_${year},sg_sigma_${year},sg_JERmeandown_${year},sg_JERmeanup_${year},sg_JECmeandown_${year},sg_JECmeanup_${year},sg_JERsigmadown_${year},sg_JERsigmaup_${year},sg_JECsigmadown_${year},sg_JECsigmaup_${year} --cminDefaultMinimizerStrategy=0"






echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw(\"(r-1)/rErr>>h(20,-4,4)\")"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


