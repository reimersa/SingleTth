#!/bin/bash

datacard="DataCard2016_M600_much.txt"


eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much=1 --toysFrequentist -t 100 --expectSignal 1 --saveToys -m 125 --freezeParameters pdf_index_much"

#eval "combine $datacard -M FitDiagnostics  --setParameters pdf_index_much=1 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 100 --rMin 0 --rMax 3 --freezeParameters pdf_index_much"

eval "combine $datacard -M FitDiagnostics  --setParameters pdf_index_much=1 --toysFile higgsCombineTest.GenerateOnly.mH125.123456.root  -t 1 --rMin 0 --rMax 3 --freezeParameters pdf_index_much --saveShapes"


echo ""
echo "Please do ... next"
echo "root -l fitDiagnostics.root"
echo "tree_fit_sb->Draw('(r-1)/rErr>>h(20,-4,4)')"
echo ""
echo ""
echo "Or do"
echo "python analyse.py"


