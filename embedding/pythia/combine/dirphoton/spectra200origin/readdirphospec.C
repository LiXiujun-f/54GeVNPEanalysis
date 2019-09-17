void readdirphospec()
{
  TFile* file = new TFile("dirphospectra.root","recreate");
  ifstream read;
  read.open("dirpho_spectra.txt");
   
  string input;
  getline(read,input); 
  cout << input <<endl;
  double x1[10], y1[10], stat1[10], sys1[10],toterr1[10];
  for (int i=0;i<10;i++)
  {
     read>>x1[i]>>y1[i]>>stat1[i]>>sys1[i];
     toterr1[i]=sqrt(stat1[i]*stat1[i]+sys1[i]*sys1[i]);
     cout<<x1[i]<<" "<<y1[i]<<" "<<stat1[i]<<" "<<sys1[i]<<endl;
  }

  TGraphErrors* g1 = new TGraphErrors(10, x1 ,y1,0,toterr1);
  g1->SetName("dirpho_62_0_20");

  getline(read,input); 
  getline(read,input); 
  cout << input <<endl;
  
  double x2[8], y2[8], stat2[8], sys2[8], toterr2[8];
  for (int i=0;i<8;i++)
  {
     read>>x2[i]>>y2[i]>>stat2[i]>>sys2[i];
     toterr2[i]=sqrt(stat2[i]*stat2[i]+sys2[i]*sys2[i]);
     cout<<x2[i]<<" "<<y2[i]<<" "<<stat2[i]<<" "<<sys2[i]<<endl;
  }
  TGraphErrors* g2 = new TGraphErrors(8, x2 ,y2,0,toterr2);
  g2->SetName("dirpho_62_20_40");

  getline(read,input); 
  getline(read,input); 
  cout << input <<endl;

  double x3[11], y3[11], stat3[11], sys3[11],toterr3[11];
  for (int i=0;i<11;i++)
  {
     read>>x3[i]>>y3[i]>>stat3[i]>>sys3[i];
     cout<<x3[i]<<" "<<y3[i]<<" "<<stat3[i]<<" "<<sys3[i]<<endl;
     toterr3[i]=sqrt(stat3[i]*stat3[i]+sys3[i]*sys3[i]);
  }
  TGraphErrors* g3 = new TGraphErrors(11, x3 ,y3,0,toterr3);
  g3->SetName("dirpho_200_0_20");
  
  getline(read,input); 
  getline(read,input); 
  cout << input <<endl;

  double x4[11], y4[11], stat4[11], sys4[11],toterr4[11];
  for (int i=0;i<11;i++)
  {
     read>>x4[i]>>y4[i]>>stat4[i]>>sys4[i];
     cout<<x4[i]<<" "<<y4[i]<<" "<<stat4[i]<<" "<<sys4[i]<<endl;
     toterr4[i]=sqrt(stat4[i]*stat4[i]+sys4[i]*sys4[i]);
  }
  TGraphErrors* g4 = new TGraphErrors(11, x4 ,y4,0,toterr4);
  g4->SetName("dirpho_200_20_40");

  getline(read,input); 
  getline(read,input); 
  cout << input <<endl;

  double x5[11], y5[11], stat5[11], sys5[11],toterr5[11];
  for (int i=0;i<11;i++)
  {
     read>>x5[i]>>y5[i]>>stat5[i]>>sys5[i];
     cout<<x5[i]<<" "<<y5[i]<<" "<<stat5[i]<<" "<<sys5[i]<<endl;
     toterr5[i]=sqrt(stat5[i]*stat5[i]+sys5[i]*sys5[i]);
  }
  TGraphErrors* g5 = new TGraphErrors(11, x5 ,y5,0,toterr5);
  g5->SetName("dirpho_200_40_60");

  getline(read,input); 
  getline(read,input); 
  cout << input <<endl;

  double x6[6], y6[6], stat6[6], sys6[6],toterr6[6];
  for (int i=0;i<6;i++)
  {
     read>>x6[i]>>y6[i]>>stat6[i]>>sys6[i];
     cout<<x6[i]<<" "<<y6[i]<<" "<<stat6[i]<<" "<<sys6[i]<<endl;
     toterr6[i]=sqrt(stat6[i]*stat6[i]+sys6[i]*sys6[i]);
  }
  TGraphErrors* g6 = new TGraphErrors(6, x6 ,y6,0,toterr6);
  g6->SetName("dirpho_200_0_20_2");

  getline(read,input); 
  // cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x7[6], y7[6], stat7[6], sys7[6],toterr7[6];
  for (int i=0;i<6;i++)
  {
     read>>x7[i]>>y7[i]>>stat7[i]>>sys7[i];
     cout<<x7[i]<<" "<<y7[i]<<" "<<stat7[i]<<" "<<sys7[i]<<endl;
     toterr7[i]=sqrt(stat7[i]*stat7[i]+sys7[i]*sys7[i]);
  }
  TGraphErrors* g7 = new TGraphErrors(6, x7 ,y7,0,toterr7);
  g7->SetName("dirpho_200_20_40_2");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x8[17], y8[17], stat8[17], sys8[17],toterr8[17];
  for (int i=0;i<17;i++)
  {
     read>>x8[i]>>y8[i]>>stat8[i]>>sys8[i];
     cout<<x8[i]<<" "<<y8[i]<<" "<<stat8[i]<<" "<<sys8[i]<<endl;
     toterr8[i]=sqrt(stat8[i]*stat8[i]+sys8[i]*sys8[i]);
  }
  TGraphErrors* g8 = new TGraphErrors(17, x8 ,y8,0,toterr8);
  g8->SetName("dirpho_200_0_5");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x9[16], y9[16], stat9[16], sys9[16],toterr9[16];
  for (int i=0;i<16;i++)
  {
     read>>x9[i]>>y9[i]>>stat9[i]>>sys9[i];
     cout<<x9[i]<<" "<<y9[i]<<" "<<stat9[i]<<" "<<sys9[i]<<endl;
     toterr9[i]=sqrt(stat9[i]*stat9[i]+sys9[i]*sys9[i]);
  }
  TGraphErrors* g9 = new TGraphErrors(16, x9 ,y9,0,toterr9);
  g9->SetName("dirpho_200_5_10");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x10[16], y10[16], stat10[16], sys10[16],toterr10[16];
  for (int i=0;i<16;i++)
  {
     read>>x10[i]>>y10[i]>>stat10[i]>>sys10[i];
     cout<<x10[i]<<" "<<y10[i]<<" "<<stat10[i]<<" "<<sys10[i]<<endl;
     toterr10[i]=sqrt(stat10[i]*stat10[i]+sys10[i]*sys10[i]);
  }
  TGraphErrors* g10 = new TGraphErrors(16, x10 ,y10,0,toterr10);
  g10->SetName("dirpho_200_10_15");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x12[17], y12[17], stat12[17], sys12[17],toterr12[17];
  for (int i=0;i<16;i++)
  {
     read>>x12[i]>>y12[i]>>stat12[i]>>sys12[i];
     cout<<x12[i]<<" "<<y12[i]<<" "<<stat12[i]<<" "<<sys12[i]<<endl;
     toterr12[i]=sqrt(stat12[i]*stat12[i]+sys12[i]*sys12[i]);
  }
  TGraphErrors* g12 = new TGraphErrors(16, x12 ,y12,0,toterr12);
  g12->SetName("dirpho_200_15_20");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x13[17], y13[17], stat13[17], sys13[17],toterr13[17];
  for (int i=0;i<17;i++)
  {
     read>>x13[i]>>y13[i]>>stat13[i]>>sys13[i];
     cout<<x13[i]<<" "<<y13[i]<<" "<<stat13[i]<<" "<<sys13[i]<<endl;
     toterr13[i]=sqrt(stat13[i]*stat13[i]+sys13[i]*sys13[i]);
  }

  TGraphErrors* g13 = new TGraphErrors(17, x13 ,y13,0,toterr13);
  g13->SetName("dirpho_200_20_30");

  getline(read,input); 
  // cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x14[16], y14[16], stat14[16], sys14[16],toterr14[16];
  for (int i=0;i<16;i++)
  {
     read>>x14[i]>>y14[i]>>stat14[i]>>sys14[i];
     cout<<x14[i]<<" "<<y14[i]<<" "<<stat14[i]<<" "<<sys14[i]<<endl;
     toterr14[i]=sqrt(stat14[i]*stat14[i]+sys14[i]*sys14[i]);
  }
  TGraphErrors* g14 = new TGraphErrors(16, x14 ,y14,0,toterr14);
  g14->SetName("dirpho_200_30_40");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x15[15], y15[15], stat15[15], sys15[15],toterr15[15];
  for (int i=0;i<15;i++)
  {
     read>>x15[i]>>y15[i]>>stat15[i]>>sys15[i];
     cout<<x15[i]<<" "<<y15[i]<<" "<<stat15[i]<<" "<<sys15[i]<<endl;
     toterr15[i]=sqrt(stat15[i]*stat15[i]+sys15[i]*sys15[i]);
  }
  TGraphErrors* g15 = new TGraphErrors(15, x15 ,y15,0,toterr15);
  g15->SetName("dirpho_200_40_50");

  getline(read,input); 
  cout << input <<endl;
  getline(read,input); 
  cout << input <<endl;
  double x16[15], y16[15], stat16[15], sys16[15],toterr16[15];
  for (int i=0;i<15;i++)
  {
     read>>x16[i]>>y16[i]>>stat16[i]>>sys16[i];
     cout<<x16[i]<<" "<<y16[i]<<" "<<stat16[i]<<" "<<sys16[i]<<endl;
     toterr16[i]=sqrt(stat16[i]*stat16[i]+sys16[i]*sys16[i]);
  }
  TGraphErrors* g16 = new TGraphErrors(15, x16 ,y16,0,toterr16);
  g16->SetName("dirpho_200_50_60");


  g1->Write();
  g2->Write();
  g3->Write();
  g4->Write();
  g5->Write();
  g6->Write();
  g7->Write();
  g8->Write();
  g9->Write();
  g10->Write();
  g12->Write();
  g13->Write();
  g14->Write();
  g15->Write();
  g16->Write();
} 
