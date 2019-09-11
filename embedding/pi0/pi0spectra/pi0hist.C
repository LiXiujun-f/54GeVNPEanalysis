#include "sPhenixStyle.h"

void pi0hist()
{
  SetsPhenixStyle();
  gStyle->SetOptFit(111);
  TCanvas* c =new TCanvas("c","c");
  TPDF* pdf = new TPDF("PionSpectra.pdf");
  pdf->Off();
  TFile* file = new TFile("sppi0.root");
  // TF1* f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,8);
  TF1* f[5],f2[5];
  TMultiGraph* gmul[5];
  TGraphErrors* g[3][5];
   // c->Divide(3,2);
  double SpectraParPi0[5][5]={ 
    {1052,0.408,0.091,1.021,10.98},
    {698.6,0.3983,0.064,1.058,11.06},
    {381.4,0.3689,0.03409,1.141,11.42},
    {147.2,0.3094,0.1883,0.927,10.19},
    {1,1,1,1,1}
  };
  double par[5];
  TString centname[5] = {"0_10","10_20","20_40","40_60","60_86"};
  TString pionname[3] = {"pi0","piplus","piminus"};
  int color[3]={kRed,kBlue,kMagenta};
  for (int i=0;i<5;i++)
  {
    //if have pionminus
    c->Clear();
    c->cd(i+1);
    gmul[i] = new TMultiGraph(Form("gCom_%s",centname[i].Data()),Form("gCom_%s",centname[i].Data())); 
    for (int j=0;j<3;j++)
    {
      if (file->GetListOfKeys()->Contains(Form("%s_%s",pionname[j].Data(),centname[i].Data())))
      { 
        g[j][i] = (TGraphErrors*)file->Get(Form("%s_%s",pionname[j].Data(),centname[i].Data()));
        g[j][i]->Draw();
        cout<< Form("%s_%s",pionname[j].Data(),centname[i].Data())<< endl;
        g[j][i]->SetMarkerColor(color[j]);
        g[j][i]->SetLineColor(color[j]);
        g[j][i]->GetXaxis()->SetTitle("p_{T}[GeV/c]");
        g[j][i]->GetYaxis()->SetTitle("Inv. yield");

        gmul[i]->Add(g[j][i]);
      }
    } 

    f[i] = new TF1(Form("fitpi_%s",centname[i].Data() ),"[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15);
    f2[i] = new TF1(Form("fitpi_pt_%s",centname[i].Data() ),"2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15);
    // double par[5]={1052,0.408,0.091,1.021,10.98};
    f[i]->SetParameters(SpectraParPi0[i]);
    gmul[i]->Draw("p");
    gmul[i]->Fit(f[i]);
    gmul[i]->Fit(f[i]);
    gPad->SetLogy();
    drawLatex(0.2,0.2,Form("%s",centname[i].Data()),0.05);
    f[i]->GetParameters(par);
    f2[i]->SetParameters(par);
    cout <<centname[i].Data() <<" ";
    for (int ip = 0;ip<5;ip++)
    {
      cout <<par[ip] << ", ";
    }
    cout<<endl;
    addpdf(pdf);
  }
    // addpdf(pdf);
  pdf->On();
  pdf->Close();
  // file->Close();
  TFile* fpi0sp = new TFile("fpi0spectra.root","recreate");
  for (int i=0;i<5;i++)
  {
    gmul[i]->Write();
    f[i]->Write();
    f2[i]->Write();
  }
  fpi0sp->Close();
}
