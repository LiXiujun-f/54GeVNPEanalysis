#include "sPhenixStyle.h"
void setGraphStyle(TGraphErrors* g,int color)
{
  g->SetMarkerColor(color);
  g->SetMarkerStyle(30);
  g->SetMarkerSize(1.5);
  g->SetLineColor(color);
}
void myFit(double *x, double *p)
{
}
Double_t func1(Double_t *x, Double_t *par){
  Double_t arg = 0;
  Double_t val = 0;
  if(par[2]!=0 && par[1]!=1){
    arg = 1+sqrt(x[0]*x[0])/par[2]*(1-par[1]);//1.+(par[1]-1)*sqrt(x[0]*x[0]+0.149*0.149)/par[2];
    val = par[0]*(sqrt(x[0]*x[0])*TMath::Power(arg,-1/(1-par[1])));//exp(arg*(1/(1-par[1])));
  }
  return val;
}
void fitdirphosp()
{
  //the main purpose is to scale the 200 GeV spectra to 62 GeV and then fit with 62 GeV data
 
  SetsPhenixStyle();
  TFile* file = new TFile("dirphospectra.root");
  TGraphErrors* g62[2];
  TGraphErrors* g200_1[3];
  TGraphErrors* g200_2[2];
  TGraphErrors* g200_3[8];
  TGraphErrors* g200_comb[3]; // combine the g200_3 into 3 centbin
  TString centname[]={"0_20","20_40","40_60","0_5","5_10","10_15","15_20","20_30","30_40","40_50","50_60"};
  TF1* fit[3];
  TF1* test = new TF1("func1",func1,0,15,3);
  TMultiGraph* gmult[3];
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("dirphospectra.pdf");
  pdf->Off();

  for (int i=0;i<2;i++)
  {
    g62[i] = (TGraphErrors*)file->Get(Form("dirpho_62_%s",centname[i].Data()));
    g200_2[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s_2",centname[i].Data()));
  }
  for (int i=0;i<3;i++)
  {
    g200_1[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s",centname[i].Data()));
  }
  for (int i=0;i<8;i++)
  {
    g200_3[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s",centname[i].Data()));
    g200_3[i]->SetMarkerColor(kBlue);
    g200_3[i]->Draw("pA");
    g200_3[i]->Fit(test);
    addpdf(pdf);
  }
  


  return;
  //below are the fitting part
  TH1F* h = new TH1F("h","h" ,5, 0,15);
  for (int i=0;i<3;i++)
  {
     c->Clear();
     h->Draw();
     h->GetYaxis()->SetRangeUser(-0.15,0.3);
     fit[i] = new TF1(Form("fitdirphosp_%s",centname[i].Data()), myFit,0,15,7);

     gmult[i] = new TMultiGraph();
     g[i][0] = (TGraphErrors*)file->Get(Form("dirphoton_%s_0",centname[i].Data()));
     g[i][1] = (TGraphErrors*)file->Get(Form("dirphoton_%s_1",centname[i].Data()));
     glow[i][0] = (TGraphErrors*)filelow->Get(Form("dirpho_lowpt_%s_0",centname[i].Data()));
     glow[i][1] = (TGraphErrors*)filelow->Get(Form("dirpho_lowpt_%s_1",centname[i].Data()));

     setGraphStyle(g[i][0],kRed);
     setGraphStyle(g[i][1],kBlue);
     setGraphStyle(glow[i][0],kMagenta);
     setGraphStyle(glow[i][1],kGreen);

     gmult[i]->Add(g[i][0]);
     gmult[i]->Add(g[i][1]);
     gmult[i]->Add(glow[i][0]);
     gmult[i]->Add(glow[i][1]);
     
     gmult[i]->SetName(Form("dirpho_%s",centname[i].Data()));
     gmult[i]->Draw("psame");
     gmult[i]->Fit(fit[i],"B");
     fit[i]->Draw("same");
     addpdf(pdf);
  }

  pdf->On();
  pdf->Close();
  TFile* dirphov2input = new TFile("dirphoinput.root","recreate");

  for (int i=0;i<3;i++)
  {
   gmult[i]->Write();
   fit[i]->Write(); 
  } 
}
