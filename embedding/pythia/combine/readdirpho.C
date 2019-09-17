void readdirpho()
{
  ifstream read;
  read.open("dirphoton_v2_200.txt");
  TFile* file = new TFile("dirpho_v2_200.root","recreate");
  double x1[14],stat1[14],sys11[14],sys12[14],err1[14],y1[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout<<input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x1[i]>>y1[i]>>stat1[i]>>sys11[i]>>sys12[i];
    err1[i] = sqrt(stat1[i]*stat1[i]+(sys12[i]+sys11[i])*(sys11[i]+sys12[i])/4.);
    cout<<x1[i]<<" " <<y1[i] <<endl;
  }
  TGraphErrors* g1 = new TGraphErrors(14, x1, y1, 0, err1);
  g1->SetName(Form("dirphoton_0_20_0"));
  g1->Write();

  double x2[14],stat2[14],sys21[14],sys22[14],err2[14],y2[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout << input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x2[i]>>y2[i]>>stat2[i]>>sys21[i]>>sys22[i];
    err2[i] = sqrt(stat2[i]*stat2[i]+(sys22[i]+sys21[i])*(sys21[i]+sys22[i])/4.);
    cout<<x2[i]<<" " <<y2[i] <<endl;
  }
  TGraphErrors* g2 = new TGraphErrors(14, x2, y2, 0, err2);
  g2->SetName(Form("dirphoton_0_20_1"));
  g2->Write();

  double x3[14],stat3[14],sys31[14],sys32[14],err3[14],y3[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout << input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x3[i]>>y3[i]>>stat3[i]>>sys31[i]>>sys32[i];
    err3[i] = sqrt(stat3[i]*stat3[i]+(sys32[i]+sys31[i])*(sys31[i]+sys32[i])/4.);
    cout<<x3[i]<<" " <<y3[i] <<endl;
  } 
  TGraphErrors* g3 = new TGraphErrors(14, x3, y3, 0, err3);
  g3->SetName(Form("dirphoton_20_40_0"));
  g3->Write();

  double x4[14],stat4[14],sys41[14],sys42[14],err4[14],y4[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout << input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x4[i]>>y4[i]>>stat4[i]>>sys41[i]>>sys42[i];
    err4[i] = sqrt(stat4[i]*stat4[i]+(sys42[i]+sys41[i])*(sys41[i]+sys42[i])/4.);
    cout<<x4[i]<<" " <<y4[i] <<endl;
  }
  TGraphErrors* g4 = new TGraphErrors(14, x4, y4, 0, err4);
  g4->SetName(Form("dirphoton_20_40_1"));
  g4->Write();

  double x5[14],stat5[14],sys51[14],sys52[14],err5[14],y5[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout << input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x5[i]>>y5[i]>>stat5[i]>>sys51[i]>>sys52[i];
    err5[i] = sqrt(stat5[i]*stat5[i]+(sys52[i]+sys51[i])*(sys51[i]+sys52[i])/4.);
    cout<<x5[i]<<" " <<y5[i] <<endl;
  }
  TGraphErrors* g5 = new TGraphErrors(14, x5, y5, 0, err5);
  g5->SetName(Form("dirphoton_40_60_0"));
  g5->Write();

  double x6[14],stat6[14],sys61[14],sys62[14],err6[14],y6[14];
  string input;
  getline(read,input);
  getline(read,input);
  cout << input<<endl;

  for (int i=0;i<14;i++)
  {
    read>>x6[i]>>y6[i]>>stat6[i]>>sys61[i]>>sys62[i];
    err6[i] = sqrt(stat6[i]*stat6[i]+(sys62[i]+sys61[i])*(sys61[i]+sys62[i])/4.);
    cout<<x6[i]<<" " <<y6[i] <<endl;
  }
  TGraphErrors* g6 = new TGraphErrors(14, x6, y6, 0, err6);
  g6->SetName(Form("dirphoton_40_60_1"));
  g6->Write();
}
