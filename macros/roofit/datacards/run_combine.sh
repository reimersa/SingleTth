#!/bin/bash

#masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
#masses=(600)
masses=(550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass
#    eval "combine -M AsymptoticLimits -d Datacard_M$mass.txt -t -1 -n output_$mass --freezeParameters sg_mean,sg_sigma"
    eval "combine -M AsymptoticLimits -d Datacard_M$mass.txt -t -1 -n output_$mass"
    
done