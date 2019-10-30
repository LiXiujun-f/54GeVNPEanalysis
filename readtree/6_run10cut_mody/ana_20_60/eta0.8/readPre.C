#include <fstream>
void readPre(){
  TFile* file = new TFile("prev2.root","recreate");
  double x1[14],y1[14],err1[14],errsys1[14];
  double y2[14],err2[14],errsys2[14];
  double y3[14],err3[14],errsys3[14];
  ifstream in;
  in.open("Previousv2.txt");
  for (int i=0;i<14;i++)
  {
    in>>x1[i]>>y1[i]>>err1[i]>>errsys1[i]>>y2[i]>>err2[i]>>errsys2[i]>>y3[i]>>err3[i]>>errsys3[i]; 
  }
  
  double x10[14],y10[14],err10[14],errsys10[14];
  double x11[14],y11[14],err11[14],errsys11[14];
  double x12[14],y12[14],err12[14],errsys12[14];
  for (int i=0;i<14;i++)
  {
    in>>x10[i]>>y10[i]>>err10[i]>>errsys10[i]>>y11[i]>>err11[i]>>errsys11[i]>>y12[i]>>err12[i]>>errsys12[i]; 
  }
  double x4[6],y4[6],err4[6],errsys4[6];
  for (int i=0;i<6;i++)
  {
    in>>x4[i]>>y4[i]>>err4[i]>>errsys4[i];
  }
  double x5[6],y5[6],err5[6],errsys5[6];
  for (int i=0;i<6;i++)
  {
    in>>x5[i]>>y5[i]>>err5[i]>>errsys5[i];
  }
  double x6[6],y6[6],err6[6],errsys6[6];
  for (int i=0;i<6;i++)
  {
    in>>x6[i]>>y6[i]>>err6[i]>>errsys6[i];
  }
  double x7[6],y7[6],err7[6],errsys7[6];
  for (int i=0;i<6;i++)
  {
    in>>x7[i]>>y7[i]>>err7[i]>>errsys7[i];
  }
  double x8[6],y8[6],err8[6],errsys8[6];
  for (int i=0;i<6;i++)
  {
    in>>x8[i]>>y8[i]>>err8[i]>>errsys8[i];
  }
  in.close();
  double x9[12],y9[12];
  in.open("xyscan.txt");
  for (int i=0;i<12;i++){
    in>>x9[i]>>y9[i];
    cout<<x9[i]<<endl;
    y9[i]/=100.;
  }
  in.close();
  TGraphErrors* g1=new TGraphErrors(14,x1,y1,0,err1);
  g1->SetName("10_20");
  g1->Write();
  TGraphErrors* g1sys=new TGraphErrors(14,x1,y1,0,errsys1);
  g1sys->SetName("10_20_sys");
  g1sys->Write();
  TGraphErrors* g2=new TGraphErrors(14,x1,y2,0,err2);
  g2->SetName("30_40");
  g2->Write();
  TGraphErrors* g2sys=new TGraphErrors(14,x1,y2,0,errsys2);
  g2sys->SetName("30_40_sys");
  g2sys->Write();
  TGraphErrors* g3=new TGraphErrors(14,x1,y3,0,err3);
  g3->SetName("50_60");
  g3->Write();
  TGraphErrors* g3sys=new TGraphErrors(14,x1,y3,0,errsys3);
  g3sys->SetName("50_60_sys");
  g3sys->Write();
   TGraphErrors* g4sys=new TGraphErrors(6,x4,y4,0,err4);
  g4sys->SetName("0_10_sys_62");

  g4sys->Write();
  TGraphErrors* g5sys=new TGraphErrors(6,x5,y5,0,err5);
  g5sys->SetName("10_20_sys_62");
  g5sys->Write();
  TGraphErrors* g6sys=new TGraphErrors(6,x6,y6,0,err6);
  g6sys->SetName("20_30_sys_62");
  g6sys->Write();
  TGraphErrors* g7sys=new TGraphErrors(6,x7,y7,0,err7);
  g7sys->SetName("30_40_sys_62");
  g7sys->Write();
  TGraphErrors* g8sys=new TGraphErrors(6,x8,y8,0,err8);
  g8sys->SetName("40_50_sys_62");
  g8sys->Write();
  TGraph* g9 = new TGraph(12,x9,y9);
  g9->SetName("30_40_star62");
  g9->Write();
  TGraph* g10 = new TGraph(14,x10,y10);
  g10->SetName("10_20_star27");
  g10->Write();
  TGraph* g11 = new TGraph(14,x10,y11);
  g11->SetName("30_40_star27");
  g11->Write();
  TGraph* g12 = new TGraph(14,x10,y12);
  g12->SetName("50_60_star27");
  g12->Write();

}
