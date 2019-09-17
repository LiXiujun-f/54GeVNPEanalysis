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
  if ( x[0]<p[6] ) return p[0]+x[0]*p[1]+x[0]*x[0]*p[2]+x[0]*x[0]*x[0]*p[3]+pow(x[0],4)*p[4];
  if (x[0]>=p[6]) return  p[0]+p[6]*p[1]+p[6]*p[6]*p[2]+pow(p[6],3)*p[3]+pow(p[6],4)*p[4]-p[6]*p[5]+x[0]*p[5];
}
void fitdirphov2()
{
  SetsPhenixStyle();
  TFile* file = new TFile("dirpho_v2_200.root");
  TFile* filelow = new TFile("dirphov2_lowpt.root");
  TGraphErrors* g[3][2];
  TGraphErrors* glow[3][2];
  TString centname[]={"0_20","20_40","40_60"};
  TF1* fit[3];
  TMultiGraph* gmult[3];
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("dirpho_v2.pdf");
  pdf->Off();
  TH1F* h = new TH1F("h","h" ,5, 0,15);
  for (int i=0;i<3;i++)
  {
    c->Clear();
    h->Draw();
    h->GetYaxis()->SetRangeUser(-0.15,0.3);
    // fit[i] = new TF1(Form("fitdirphov2_%s",centname[i].Data()), "pol5",0,15);
    fit[i] = new TF1(Form("fitdirphov2_%s",centname[i].Data()), myFit,0,15,7);
    if (i==0) 
      fit[i]->FixParameter(6,5);
    else if (i==1)
      fit[i]->FixParameter(6,6.5);
    else if (i==2)
      fit[i]->FixParameter(6,5.5);

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
  TFile* dirphov2input = new TFile("dirphov2input.root","recreate");

  for (int i=0;i<3;i++)
  {
   gmult[i]->Write();
   fit[i]->Write(); 
  } 
}
