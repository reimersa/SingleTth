#!/bin/bash



# for mass in {600,800,1000}
# for mass in {1000,800}
# do
mass=1000
#datacard="Datacard_allyears_M${mass}_much.txt"
datacard="Datacard_allyears_M${mass}.txt"

# signal_list=0 0.5 1 1.5 2 2.5 3
# if [ ${mass} -ne "600" ]
# then
#     signal_list= 0 0.25 0.5 0.75 1 
# fi

#for signal in 0.0 0.25 0.5 0.75 1.0 1.5 2.0 2.5 3.0 
# for signal in 0.0 0.25 0.5 0.75 1.0 
# do
signal=1
echo ""
echo ""
echo "================================ " + $signal + "     "+ $mass
echo ""
echo ""


eval "combine generate.txt -M GenerateOnly --setParameters pdf_index_much_2016v3=0,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal ${signal} --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass} --seed 123123"


eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=0,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123123.root  -t 1 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal${signal}_${mass} --saveShapes"




# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 1 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass} --seed 123123"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123123.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal1_${mass}"



# eval "python plot_signalstrength.py 1 --name \"Much_signal1_M${mass}_2016\" --mass ${mass}"

# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1 --rMin -10 --rMax 10#  --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n TEST_signal1_${mass}  --saveShapes"




# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 3 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass} --seed 911222"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.911222.root  -t 507 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal3_${mass}"


# eval "python plot_signalstrength.py 3 --name \"Much_signal3_M${mass}_2016\" --mass ${mass}"

# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 0 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal0_${mass}"


# eval "python plot_signalstrength.py 0 --name \"Much_signal0_M${mass}_2016\" --mass ${mass}"

# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n TEST_signal0_${mass} --saveShapes"


# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 0.25 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal0p25_${mass}"


# eval "python plot_signalstrength.py 0p25 --name \"Much_signal0_M${mass}_2016\" --mass ${mass}"


# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 0.5 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal0_${mass}"


# eval "python plot_signalstrength.py 0 --name \"Much_signal0_M${mass}_2016\" --mass ${mass}"

# eval "combine $datacard -M GenerateOnly --setParameters pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFrequentist -t 1000 --expectSignal 0.75 --saveToys -m 125  --freezeParameters pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 -n ${mass}"


# eval "combine $datacard -M FitDiagnostics  --setParameters  pdf_index_much_2016v3=2,pdf_index_MT${mass}_much_2016v3=0,pdf_index_ech_2016v3=2,pdf_index_MT${mass}_ech_2016v3=0 --toysFile higgsCombine${mass}.GenerateOnly.mH125.123456.root  -t 1000 --rMin -10 --rMax 10 --freezeParameters  pdf_index_much_2016v3,pdf_index_MT${mass}_much_2016v3,pdf_index_ech_2016v3,pdf_index_MT${mass}_ech_2016v3,sg_mean_2016v3,sg_sigma_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 --cminDefaultMinimizerStrategy=0 -n signal0_${mass}"


# eval "python plot_signalstrength.py 0 --name \"Much_signal0_M${mass}_2016\" --mass ${mass}"

# done
# echo "python plot_signalstrength.py 0 0.25 0.5 0.75 1 1.5 2 2.5 3 --name \"MuEch_signal_M${mass}_2016\" --mass ${mass}"
# eval "python plot_signalstrength.py 0 0.25 0.5 0.75 1 1.5 2 2.5 3 --name \"MuEch_signal_M${mass}_2016\" --mass ${mass}"
#done
