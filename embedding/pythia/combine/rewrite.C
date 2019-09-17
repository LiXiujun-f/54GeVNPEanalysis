void rewrite()
{
  double pt[100], v2[100], statup[100],statlow[100],sysup[100],syslow[100],toterr[100];
  ifstream read;
  read.open("dirpho_lowpt.txt");
  string input;
  getline(read,input);
  for (int i=0;i<51;i++)
  {
    read>>pt[i]>>v2[i];
  }
  for (int i=0;i<51;i++)
  {
    read>>statup[i]>>statlow[i];
  }
  for (int i=0;i<51;i++)
  {
    read>>sysup[i]>>syslow[i];
    toterr[i]=sqrt(pow((syslow[i]+sysup[i]),2)/4.+pow(statlow[i]+statup[i],2)/4.); 
  }
  

  TGraphErrors* g1 = new TGraphErrors(11,pt,v2,0,toterr);
  g1->SetName("dirpho_lowpt_0_20_0");

  TGraphErrors* g2 = new TGraphErrors(6,pt+11,v2+11,0,toterr+11);
  g2->SetName("dirpho_lowpt_0_20_1");
  
  TGraphErrors* g3 = new TGraphErrors(11,pt+17,v2+17,0,toterr+17);
  g3->SetName("dirpho_lowpt_20_40_0");
 
  TGraphErrors* g4 = new TGraphErrors(6,pt+28,v2+28,0,toterr+28);
  g4->SetName("dirpho_lowpt_20_40_1");

  TGraphErrors* g5 = new TGraphErrors(11,pt+34,v2+34,0,toterr+34);
  g5->SetName("dirpho_lowpt_40_60_0");
 
  TGraphErrors* g6 = new TGraphErrors(6,pt+45,v2+45,0,toterr+45);
  g6->SetName("dirpho_lowpt_40_60_1");

  TFile* fout = new TFile("dirphov2_lowpt.root","recreate");
  // fout->Write();
  g1->Write();
  g2->Write();
  g3->Write();
  g4->Write();
  g5->Write();
  g6->Write();
}
