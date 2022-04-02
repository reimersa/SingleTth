mass="650"
datacard="Datacard_allyears_M${mass}.txt"

freeze_param=""
set_param=""
funct="1"

for cat in "${categories[@]}" ; do
for year in "${years[@]}"; do
freeze_param="${freeze_param}sg_JERmeandown_"$year"_"$cat",sg_JERmeanup_"$year"_"$cat",sg_JECmeandown_"$year"_"$cat",sg_JECmeanup_"$year"_"$cat",sg_JERsigmadown_"$year"_"$cat",sg_JERsigmaup_"$year"_"$cat",sg_JECsigmadown_"$year"_"$cat",sg_JECsigmaup_"$year"_"$cat",sg_JERmeandown2_"$year"_"$cat",sg_JERmeanup2_"$year"_"$cat",sg_JECmeandown2_"$year"_"$cat",sg_JECmeanup2_"$year"_"$cat",sg_JERsigmadown2_"$year"_"$cat",sg_JERsigmaup2_"$year"_"$cat",sg_JECsigmadown2_"$year"_"$cat",sg_JECsigmaup2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat",pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat",sg_mean_"$year"_"$cat",sg_sigma_"$year"_"$cat",sg_mean2_"$year"_"$cat",sg_sigma2_"$year"_"$cat",pdf_index_MT${mass}_much_"$year"_"$cat",pdf_index_MT${mass}_ech_"$year"_"$cat","

freeze_param="${freeze_param},pdf_index_much_"$year"_"$cat",pdf_index_ech_"$year"_"$cat","

#freeze_param="${freeze_param},bgexp2_p0ech_"$year"_"$cat",bgexp2_p1ech_"$year"_"$cat",bgexp2_p0much_"$year"_"$cat",bgexp2_p1much_"$year"_"$cat","

set_param="${set_param}pdf_index_much_"$year"_"$cat"=${funct},pdf_index_ech_"$year"_"$cat"=${funct},pdf_index_MT${mass}_much_"$year"_"$cat"=0,pdf_index_MT${mass}_ech_"$year"_"$cat"=0,"
done
done



freeze_param="${freeze_param: : -1}"
set_param="${set_param: : -1}"



eval "combine $datacard -M FitDiagnostics  --setParameters ${set_param}  --freezeParameters  ${freeze_param}  -n analyse --saveShapes --robustFit=1 --X-rtd MINIMIZER_no_analytic  -t -1" 
