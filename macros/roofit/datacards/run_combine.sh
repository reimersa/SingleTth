#!/bin/bash

masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
#masses=(600)

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass
    eval "combine -M AsymptoticLimits -d Datacard_M$mass.txt -t -1 -n output_$mass --freezeParameters sg_mean,sg_sigma"
    
done