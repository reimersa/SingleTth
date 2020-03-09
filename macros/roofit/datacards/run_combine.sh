#!/bin/bash

#masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
#masses=(550 575)
masses=(550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)
#masses=(950 975 1000 1025 1050 1075)

#year="2016v3"
year="2017v2"
#year="2018"

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass
#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --freezeParameters sg_mean,sg_sigma"
    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much=0,pdf_index_ech=0 --freezeParameters pdf_index_much,pdf_index_ech"
#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much=2,pdf_index_ech=2 --freezeParameters pdf_index_much,pdf_index_ech"
    
done