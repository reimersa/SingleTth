#!/bin/bash

#masses=(550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200)
#masses=(550 575)
masses=(550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225)
#masses=(950 975 1000 1025 1050 1075)

#year="2016v3"
#year="2017v2"
#year="2018"
year="allyears"

for mass in "${masses[@]}" ; do
    echo "Working on mass " $mass

    # eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much=0,pdf_index_ech=0 --freezeParameters pdf_index_much,pdf_index_ech " # normal version --seed 8192

#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much=2,pdf_index_ech=2 --freezeParameters pdf_index_much,pdf_index_ech " # Exp verison

#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much=1,pdf_index_ech=1 --freezeParameters pdf_index_much,pdf_index_ech " # 4p verison

#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M${mass}_much.txt -t -1 -n output_$mass --setParameters pdf_index_much=0 --freezeParameters pdf_index_much" # muon channel only

#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M${mass}_ech.txt -t -1 -n output_$mass --setParameters pdf_index_ech=0 --freezeParameters pdf_index_ech" # electron channel only

#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_${mass} --setParameters pdf_index_much=0,pdf_index_ech=0,pdf_index_MT${mass}_much=0,pdf_index_MT${mass}_ech=0 --freezeParameters pdf_index_much,pdf_index_ech,pdf_index_MT${mass}_much,pdf_index_MT${mass}_ech" # freeze signal variations

##### all three years
#    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much_2016v3=2,pdf_index_ech_2016v3=2,pdf_index_much_2017v2=2,pdf_index_ech_2017v2=2,pdf_index_much_2018=2,pdf_index_ech_2018=2 --freezeParameters pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018 " # normal version --seed 8192

##### all three years with 3p as uncertainty
    eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much_2016v3=1,pdf_index_ech_2016v3=1,pdf_index_much_2017v2=1,pdf_index_ech_2017v2=1,pdf_index_much_2018=1,pdf_index_ech_2018=1 --freezeParameters pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018 " # normal version --seed 8192



############ freeze all JEC and JER variation each year
#eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much_2016v3=0,pdf_index_ech_2016v3=0 --freezeParameters pdf_index_much_2016v3,pdf_index_ech_2016v3,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3 " # normal version --seed 8192


#eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much_2017v2=0,pdf_index_ech_2017v2=0 --freezeParameters pdf_index_much_2017v2,pdf_index_ech_2017v2,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2 " # normal version --seed 8192

#eval "combine -M AsymptoticLimits -d Datacard_"$year"_M$mass.txt -t -1 -n output_$mass --setParameters pdf_index_much_2018=0,pdf_index_ech_2018=0 --freezeParameters pdf_index_much_2018,pdf_index_ech_2018,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018 " # normal version --seed 8192


    
done
