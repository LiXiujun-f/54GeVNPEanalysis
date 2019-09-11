void readkaonv2()
{
  //read for 62GeV
  TFile* f = new TFile("Kaonv2.root","recreate");
  // ---------------- Particle species: K0S ----------------
  Double_t pt_bin_center1[12] = {0.3055,0.5055,0.7055,0.9055,1.1055,1.2955,1.5055,1.7055,1.9045,2.1055,2.3765,2.8505};
Double_t v2_values1[12] = {0.00198652,0.0112981,0.0238828,0.0339505,0.0424336,0.0478754,0.0581319,0.0648812,0.0700103,0.0724468,0.0723535,0.0751846};
Double_t v2_stat_error1[12] = {0.00227729,0.00120877,0.00095651,0.000971685,0.00113409,0.00141872,0.00185778,0.00247909,0.00332167,0.00442997,0.00471773,0.00699403};
Double_t v2_syst_low_error1[12] = {0.000101985,7.73174e-05,1.82751e-05,4.72853e-05,0.000101226,0.000709156,7.83354e-05,0.000100027,0.00045105,0.000284388,0.000832823,0.00111581};
Double_t v2_syst_high_error1[12] = {9.64639e-05,7.96868e-05,1.79283e-05,4.85112e-05,9.81695e-05,0.000683506,7.5013e-05,9.52614e-05,0.000476797,0.000302354,0.000894356,0.00122727};
  Double_t totalerr1[12];
  CombineStatAndSys(12, v2_syst_high_error1, v2_syst_low_error1, v2_stat_error1, totalerr1);
  TGraphErrors* g1 = new TGraphErrors(12,pt_bin_center1,v2_values1,0,v2_syst_low_error1);
  g1->SetName("K0s_0_10_low");
  g1->Write();
  ifstream in; 
  in.open("kaonv2.txt");
  int const Nlines2=19;
  string lines;
  getline( in, lines);
  cout<<lines<<endl;
  // getline(in, lines);
  // cout<<lines<<endl;
  double x2[Nlines2],y2[Nlines2],stat2[Nlines2],sys2[Nlines2],err2[Nlines2];
  for (int i=0;i<Nlines2;i++)
  {
     in>>x2[i]>>y2[i]>>stat2[i]>>sys2[i]; 
     cout<<x2[i]<<" "<<y2[i]<<" "<<stat2[i]<<" "<<sys2[i]<<endl;
     err2[i] = sqrt(stat2[i]*stat2[i]+sys2[i]*sys2[i]);
  }
  TGraphErrors* g2 = new TGraphErrors(Nlines2, x2, y2, 0, err2);
  g2->SetName("K0s_0_10_high");
  g2->Write();

  getline(in, lines);
  getline(in, lines);
  cout<< lines <<endl;
  int const Nlines3 = 20; 
  double x3[Nlines3],y3[Nlines3],stat3[Nlines3],sys3[Nlines3],err3[Nlines3];
  for (int i=0;i<Nlines3;i++)
  {
     in>>x3[i]>>y3[i]>>stat3[i]>>sys3[i]; 
     cout<<x3[i]<<" "<<y3[i]<<" "<<stat3[i]<<" " <<sys3[i] <<endl;
     err3[i] = sqrt(stat3[i]*stat3[i]+sys3[i]*sys3[i]);
  }
  TGraphErrors* g3 = new TGraphErrors(Nlines3, x3, y3, 0, err3);
  g3->SetName("K0s_10_40_high");
  g3->Write();

  
  getline(in, lines);
  getline(in, lines);
  cout<< lines <<endl;
  int const Nlines4 = 19; 
  double x4[Nlines4],y4[Nlines4],stat4[Nlines4],sys4[Nlines4],err4[Nlines4];
  for (int i=0;i<Nlines4;i++)
  {
     in>>x4[i]>>y4[i]>>stat4[i]>>sys4[i]; 
     cout<<x4[i]<<" "<<y4[i]<<" "<<stat4[i]<<" " <<sys4[i] <<endl;
     err4[i] = sqrt(stat4[i]*stat4[i]+sys4[i]*sys4[i]);
  }
  TGraphErrors* g4 = new TGraphErrors(Nlines4, x4, y4, 0, err4);
  g4->SetName("K0s_40_80_high");
  g4->Write();

  // ---------------- Particle species: K0S ----------------
  Double_t pt_bin_center5[16] = {0.3055,0.5055,0.7055,0.9055,1.1055,1.2955,1.5045,1.7045,1.9055,2.1045,2.3055,2.5055,2.7055,3.0055,3.5055,4.3835};
Double_t v2_values5[16] = {0.0149509,0.0325485,0.0544036,0.0746852,0.0935453,0.10966,0.122711,0.13235,0.142751,0.146556,0.15011,0.152067,0.152662,0.147626,0.143745,0.125034};
  Double_t v2_stat_error5[16] = {0.000905161,0.000529081,0.000440907,0.00045888,0.000539548,0.000676342,0.000879975,0.00116744,0.00155384,0.0020684,0.00273658,0.00362899,0.00477974,0.00495135,0.00716939,0.0187828};
  Double_t v2_syst_low_error5[16] = {2.97767e-05,3.98877e-05,0.000102103,0.000259179,0.000123756,0.000231234,3.54388e-05,9.03881e-05,0.000399084,0.000119542,0.000267363,0.000910811,0.00147867,0.00220537,0.00509472,0.00515544};
  Double_t v2_syst_high_error5[16] = {3.00072e-05,4.03235e-05,0.000100825,0.000254444,0.000120684,0.000223905,3.68321e-05,8.9138e-05,0.000379419,0.000115383,0.000250776,0.000847693,0.00135969,0.00200814,0.00454054,0.00441772};
  Double_t totalerr5[16];
  CombineStatAndSys(16, v2_syst_high_error5, v2_syst_low_error5, v2_stat_error5, totalerr5);
  // TGraphErrors* g5 = new TGraphErrors(16,pt_bin_center5, v2_values5,0,v2_syst_low_error5);
  TGraphErrors* g5 = new TGraphErrors(14,pt_bin_center5, v2_values5,0,v2_syst_low_error5);
  g5->SetName("K0s_10_40_low");
  g5->Write();
  
  // ---------------- Particle species: K0S ----------------
  Double_t pt_bin_center6[14] = {0.3055,0.5055,0.7055,0.9055,1.1055,1.2955,1.5055,1.7055,1.9055,2.1055,2.3045,2.5055,2.8005,3.5785};
  Double_t v2_values6[14] = {0.0200815,0.0437734,0.0712467,0.0989919,0.119656,0.141234,0.154692,0.163372,0.178179,0.198169,0.190484,0.219581,0.206113,0.236938};
  Double_t v2_stat_error6[14] = {0.0024479,0.0015891,0.00141937,0.00153701,0.00185049,0.00235031,0.00307841,0.0041083,0.00549761,0.00728901,0.00963311,0.0127796,0.0117945,0.0225808};
  Double_t v2_syst_low_error6[14] = {0.000609582,0.000237911,0.000179072,6.86525e-05,0.000103215,0.000748517,0.00075348,0.00239006,0.000236286,0.000779958,0.00377497,0.00164039,0.00264608,0.00428611};
  Double_t v2_syst_high_error6[14] = {0.000599282,0.00023411,0.000175437,7.05932e-05,0.00010696,0.000781309,0.000791294,0.00226732,0.000223154,0.000734593,0.00403722,0.00176313,0.00241655,0.00369561};
  Double_t totalerr6[14];
  CombineStatAndSys(14, v2_syst_high_error6, v2_syst_low_error6, v2_stat_error6, totalerr6);
  // TGraphErrors* g6 = new TGraphErrors(14,pt_bin_center6,v2_values6,0,v2_syst_high_erro6);
  TGraphErrors* g6 = new TGraphErrors(14,pt_bin_center6,v2_values6,0, totalerr6);
  g6->SetName("K0s_40_80_low");
  g6->Write();
}

void CombineStatAndSys(int nbins, double* syshigh, double* syslow, double* stat ,double* total)
{
  for (int ibin=0;ibin<nbins;ibin++)
  {
    double tmp=syslow[ibin];
    if (syshigh[ibin]>syslow[ibin]) tmp=syshigh[ibin];
    total[ibin] =  sqrt(stat[ibin]*stat[ibin]+tmp*tmp);  
  } 
}


