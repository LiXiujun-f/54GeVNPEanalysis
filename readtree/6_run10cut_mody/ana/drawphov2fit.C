#include "sPhenixStyle.h"

void drawphov2fit()
{
  SetsPhenixStyle();
  TFile* file = new TFile("phoEv2.root","update");
  TGraphErrors* g62 = (TGraphErrors*)file->Get("phoE62");
  TF1* fit  = new TF1("fit62","pol4(0)",0,5);
  fit->SetLineColor(kBlue);
  TF1* fiterr  = new TF1("fit62err","pol4(0)*[5]",0,5);
  fiterr->SetLineColor(kRed);
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("fitphEv2.pdf"); 
  pdf->Off();
  g62->Draw("pA");
  g62->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g62->Fit(fit);
  double par[6];
  fit->GetParameters(par);
  par[5]=1.06;
  // for (int i=0;i<6;i++){ cout <<par[i] <<" ";}
  // cout<<endl;
  fiterr->SetParameters(par);
  fiterr->DrawCopy("same");
  par[5]=0.94;
  fiterr->SetParameters(par);
  fiterr->DrawCopy("same");
  addpdf(pdf);
  pdf->On();
  pdf->Close();
  TLegend* l = new TLegend(0.5,0.2,0.8,0.5);
  l->AddEntry(fit,"62GeV pho. e v_{2}","l");
  l->AddEntry(fiterr,"error","l");
  l->Draw("same");
  // fit->Write();
}
