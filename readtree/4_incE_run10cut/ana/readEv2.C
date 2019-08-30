void readEv2()
{
  ifstream read;
  string tmp;
  read.open("photonic.txt");
  getline(read, tmp,'\n');
  cout<<tmp<<endl;
  double x1[9],y1[9],yerr1[9];
  for (int i=0;i<9;i++){
    read>>x1[i]>>y1[i]>>yerr1[i];
    cout<<x1[i]<<endl;
  }
  TGraphErrors* g1 = new TGraphErrors(9,x1,y1,0,yerr1);
  g1->SetName("phoE62");
  double x2[9],y2[9],yerr2[9];
  getline(read, tmp,'\n');
  getline(read, tmp,'\n');
  cout<<tmp<<endl;
  for (int i=0;i<9;i++){
    read>>x2[i]>>y2[i]>>yerr2[i];
    cout<<x2[i]<<endl;
  }
  TGraphErrors* g2 = new TGraphErrors(9,x2,y2,0,yerr2);
  g2->SetName("phoE39");
  double x3[3],y3[3],yerr3[3];
  getline(read, tmp,'\n');
  getline(read, tmp,'\n');
  cout<<tmp<<endl;
  for (int i=0;i<3;i++){
    read>>x3[i]>>y3[i]>>yerr3[i];
    cout<<x3[i]<<endl;
  }
  TGraphErrors* g3 = new TGraphErrors(3,x3,y3,0,yerr3);
  g3->SetName("incE62");
  double x4[3],y4[3],yerr4[3];
  getline(read, tmp,'\n');
  getline(read, tmp,'\n');
  for (int i=0;i<3;i++){
    read>>x4[i]>>y4[i]>>yerr4[i];
  }
  TGraphErrors* g4 = new TGraphErrors(3,x4,y4,0,yerr4);
  g4->SetName("incE39");
  double x5[6],y5[6],yerr5[6],ysys5[6];
  getline(read, tmp,'\n');
  getline(read, tmp,'\n');
  for (int i=0;i<6;i++){
    read>>x5[i]>>y5[i]>>yerr5[i]>>ysys5[i];
  }
  TGraphErrors* g5 = new TGraphErrors(6,x5,y5,0,yerr5);
  g5->SetName("HFe200");

  TFile* file = new TFile("phoEv2.root","recreate");
  g1->Write();
  g2->Write();
  g3->Write();
  g4->Write();
  g5->Write();
}
