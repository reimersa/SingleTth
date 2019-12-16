#!/bin/bash

#datacard="DataCard2016_M600_much.txt"
datacard="Datacard_M800.txt"


eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much=2,pdf_index_ech=2 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125 --freezeParameters pdf_index_much,pdf_index_ech"

#eval "combine $datacard -M GenerateOnly  --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125"


#eval "combine $datacard -M FitDiagnostics  --setParameters pdf_index_much=2,pdf_index_ech=2 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -3 --rMax 3 --freezeParameters pdf_index_much,pdf_index_ech"

eval "combine $datacard -M FitDiagnostics  --setParameters pdf_index_much=2,pdf_index_ech=2 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters pdf_index_much,pdf_index_ech --saveShapes -v3"

#eval "combine Modified.txt -M FitDiagnostics  --setParameters pdf_index_much=2 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin -3 --rMax 3 --freezeParameters pdf_index_much --saveShapes --saveNormalizations"

#eval "combine $datacard -M FitDiagnostics   --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin 0 --rMax 3  --saveShapes"

#eval "combine $datacard -M FitDiagnostics  --setParameters pdf_index_much=2 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin -10 --rMax 10 --freezeParameters pdf_index_much "




echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw('(r-1)/rErr>>h(20,-4,4)')"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


