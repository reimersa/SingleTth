double DoFTest(double c1, double c2, double p1, double p2, double n);

void ftest()
{

	int usep2 = 0;
	int useexpp2 = 0;
	int usep3 = 0;
	int usep4 = 0;
	int usep5 = 0;

    // ------------ CR MC ---------------

	cout << endl << "CR MC e+jets, 2p vs 3p" << endl;
	double c1 = 102.37;
	double c2 = 77.68;
	double p1 = 2;
	double p2 = 3;
	double n = 72;
    double exlCL = exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2++;
    //    else usep3++;

	cout << endl << "CR MC e+jets, Exp vs 3p" << endl;
	c1 = 81.37;
	c2 = 77.68;
	p1 = 2;
	p2 = 3;
	n = 72;
    exlCL = exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep3++;


	cout << endl << "CR MC e+jets, 3p vs 4p" << endl;
	c1 = 77.68;
	c2 = 66.26;
	p1 = 3;
	p2 = 4;
	n = 72;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3++;
    //    else usep4++;

	cout << endl << "CR MC e+jets, Exp vs 4p" << endl;
	c1 = 81.37;
	c2 = 66.26;
	p1 = 2;
	p2 = 4;
	n = 72;
    exlCL = exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep4++;

    // cout << endl << "CR MC e+jets, 4p vs 5p" << endl;
    // 	c1 = 84.90;
    // 	c2 = 84.35;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 75;
    // exlCL = DoFTest(c1, c2, p1, p2, n);    
    // if (exlCL>0.05) usep4++;
    // else usep5++;

	cout << endl << "CR MC mu+jets, 2p vs 3p" << endl;
	c1 = 121.47;
	c2 = 94.27;
	p1 = 2;
	p2 = 3;
	n = 72;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2++;
    //    else usep3++;

	cout << endl << "CR MC mu+jets, Exp vs 3p" << endl;
	c1 = 97.54;
	c2 = 94.27;
	p1 = 2;
	p2 = 3;
	n = 72;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep3++;


	cout << endl << "CR MC mu+jets, 3p vs 4p" << endl;
	c1 = 94.27;
	c2 = 84.28;
	p1 = 3;
	p2 = 4;
	n = 72;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3++;
    //    else usep4++;

	cout << endl << "CR MC mu+jets, Exp vs 4p" << endl;
	c1 = 97.54;
	c2 = 84.28;
	p1 = 2;
	p2 = 4;
	n = 72;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep4++;


    // 	cout << endl << "CR MC mu+jets, 4p vs 5p" << endl;
    // 	c1 = 78.80;
    // 	c2 = 78.26;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 74;
    // exlCL = DoFTest(c1, c2, p1, p2, n);
    // if (exlCL>0.05) usep4++;
    // else usep5++;

    // ------------ CR Data ---------------

	int usep2d = 0;
	int useexpp2d = 0;
	int usep3d = 0;
	int usep4d = 0;
	int usep5d = 0;

	cout << endl << "CR Data e+jets, 2p vs 3p" << endl;
	c1 = 92.59;
	c2 = 78.55;
	p1 = 2;
	p2 = 3;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2d++;
    //    else usep3d++;

	cout << endl << "CR Data e+jets, Exp2p vs 3p" << endl;
	c1 = 82.08;
	c2 = 78.55;
	p1 = 2;
	p2 = 3;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2d++;
    else usep3d++;


	cout << endl << "CR Data e+jets, 3p vs 4p" << endl;
	c1 = 78.55;
	c2 = 75;
	p1 = 3;
	p2 = 4;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3d++;
    //    else usep4d++;

	cout << endl << "CR Data e+jets, Exp2p vs 4p" << endl;
	c1 = 82.08;
	c2 = 75;
	p1 = 2;
	p2 = 4;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2d++;
    else usep4d++;


    // 	cout << endl << "CR Data e+jets, 4p vs 5p" << endl;
    // 	c1 = 83.41;
    // 	c2 = 82.70;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 72;
    // 	exlCL = DoFTest(c1, c2, p1, p2, n);
    // if (exlCL>0.05) usep4d++;
    // else usep5d++;

	cout << endl << "CR Data mu+jets, 2p vs 3p" << endl;
	c1 = 59.52;
	c2 = 48.95;
	p1 = 2;
	p2 = 3;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2d++;
    //    else usep3d++;

	cout << endl << "CR Data mu+jets, Exp2p vs 3p" << endl;
	c1 = 49.75;
	c2 = 48.95;
	p1 = 2;
	p2 = 3;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2d++;
    else usep3d++;


	cout << endl << "CR Data mu+jets, 3p vs 4p" << endl;
	c1 = 48.95;
	c2 = 48.71;
	p1 = 3;
	p2 = 4;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3d++;
    //    else usep4d++;

	cout << endl << "CR Data mu+jets, Exp2p vs 4p" << endl;
	c1 = 49.75;
	c2 = 48.71;
	p1 = 2;
	p2 = 4;
	n = 72;
	exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2d++;
    else usep4d++;


    // 	cout << endl << "CR Data mu+jets, 4p vs 5p" << endl;
    // 	c1 = 72.93;
    // 	c2 = 72.82;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 73;
    // 	exlCL = DoFTest(c1, c2, p1, p2, n);
    // if (exlCL>0.05) usep4d++;
    // else usep5d++;

	// ------------ SR MC ---------------

	cout << endl << "SR MC e+jets, 2p vs 3p" << endl;
	c1 = 55.01;
	c2 = 54.99;
	p1 = 2;
	p2 = 3;
	n = 74;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2++;
    //    else usep3++;

	cout << endl << "SR MC e+jets, Exp2p vs 3p" << endl;
	c1 = 89.72;
	c2 = 54.99;
	p1 = 2;
	p2 = 3;
	n = 74;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep3++;


	cout << endl << "SR MC e+jets, 3p vs 4p" << endl;
	c1 = 54.99;
	c2 = 54.74;
	p1 = 3;
	p2 = 4;
	n = 74;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3++;
    //    else usep4++;

	cout << endl << "SR MC e+jets, Exp2p vs 4p" << endl;
	c1 = 89.72;
	c2 = 54.74;
	p1 = 2;
	p2 = 4;
	n = 74;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep4++;


    // cout << endl << "SR MC e+jets, 4p vs 5p" << endl;
    // 	c1 = 57.21;
    // 	c2 = 51.60;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 75;
    // exlCL = DoFTest(c1, c2, p1, p2, n);    
    // if (exlCL>0.05) usep4++;
    // else usep5++;

	cout << endl << "SR MC mu+jets, 2p vs 3p" << endl;
	c1 = 61.07;
	c2 = 57.86;
	p1 = 2;
	p2 = 3;
	n = 73;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep2++;
    //    else usep3++;

	cout << endl << "SR MC mu+jets, Exp2 vs 3p" << endl;
	c1 = 119.9;
	c2 = 57.86;
	p1 = 2;
	p2 = 3;
	n = 73;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep3++;

	cout << endl << "SR MC mu+jets, 3p vs 4p" << endl;
	c1 = 57.86;
	c2 = 57.8;
	p1 = 3;
	p2 = 4;
	n = 75;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) usep3++;
    //    else usep4++;

	cout << endl << "SR MC mu+jets, Exp2 vs 4p" << endl;
	c1 = 119.9;
	c2 = 57.8;
	p1 = 2;
	p2 = 4;
	n = 73;
    exlCL = DoFTest(c1, c2, p1, p2, n);
    if (exlCL>0.05) useexpp2++;
    else usep4++;


    // cout << endl << "SR MC mu+jets, 4p vs 5p" << endl;
    // 	c1 = 44.89;
    // 	c2 = 37.65;
    // 	p1 = 4;
    // 	p2 = 5;
    // 	n = 75;
    // exlCL = DoFTest(c1, c2, p1, p2, n);    
    // if (exlCL>0.05) usep4++;
    // else usep5++;


    cout << "\n----------- SUMMARY -----------" << endl;
    cout << "Use 2-par fit: " << usep2+usep2d << endl;
    cout << "Use EXP2-par fit: " << useexpp2+useexpp2d << endl;
    cout << "Use 3-par fit: " << usep3+usep3d << endl;
    cout << "Use 4-par fit: " << usep4+usep4d << endl;
    cout << "Use 5-par fit: " << usep5+usep5d << endl;
    cout << "Tests on Data in CRs only:" << endl;
    cout << "Use 2-par fit: " << usep2d << endl;
    cout << "Use EXP2-par fit: " << useexpp2d << endl;
    cout << "Use 3-par fit: " << usep3d << endl;
    cout << "Use 4-par fit: " << usep4d << endl;
    cout << "Use 5-par fit: " << usep5d << endl;


}

double DoFTest(double c1, double c2, double p1, double p2, double n)
{

	double F = ( (c1-c2)/(p2-p1) ) / (c2/(n-p2-1)) ;
	cout << "F = " << F << endl;
	double cF = TMath::FDistI(F, p2-p1, n-p2);
	cout << "cumulative F-distribution: CL in new model with one more parameter: " << cF << endl;
	cout << "CL to exclude model with more parameters = " << 1.-cF << endl;
	return 1.-cF; 

}
