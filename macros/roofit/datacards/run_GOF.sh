#!/bin/bash

#year="2016v3"
#year="2017v2"
#year="2018"
year="allyears"

mass=600

    eval "combine -M GoodnessOfFit -d Datacard_"$year"_M${mass}_much.txt -t -1 -n output_$mass --setParameters pdf_index_much_2016v3=0,pdf_index_ech_2016v3=0,pdf_index_much_2017v2=0,pdf_index_ech_2017v2=0,pdf_index_much_2018=1,pdf_index_ech_2018=0 --freezeParameters pdf_index_much_2016v3,pdf_index_ech_2016v3,pdf_index_much_2017v2,pdf_index_ech_2017v2,pdf_index_much_2018,pdf_index_ech_2018,sg_JERmeandown_2016v3,sg_JERmeanup_2016v3,sg_JECmeandown_2016v3,sg_JECmeanup_2016v3,sg_JERsigmadown_2016v3,sg_JERsigmaup_2016v3,sg_JECsigmadown_2016v3,sg_JECsigmaup_2016v3,,sg_JERmeandown_2017v2,sg_JERmeanup_2017v2,sg_JECmeandown_2017v2,sg_JECmeanup_2017v2,sg_JERsigmadown_2017v2,sg_JERsigmaup_2017v2,sg_JECsigmadown_2017v2,sg_JECsigmaup_2017v2,sg_JERmeandown_2018,sg_JERmeanup_2018,sg_JECmeandown_2018,sg_JECmeanup_2018,sg_JERsigmadown_2018,sg_JERsigmaup_2018,sg_JECsigmadown_2018,sg_JECsigmaup_2018 --algo=saturated " # normal version --seed 8192
