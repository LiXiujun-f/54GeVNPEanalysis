#include "sPhenixStyle.h"
#include ""
void calgamma()
{
  SetsPhenixStyle();   
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("gammav2plots.pdf");
  pdf->Off();
  // TFile* fgamma = new TFile("gammav2.root");
  // TFile* fout = new TFile("pi2gammainputs.root","recreate");
  // fgamma->Delete("pi0spectra_*;*"); 
  // fgamma->Delete("fit_*_*;*"); 
  TH1F* hGmSp[9];
  TH1F* hGmv2[9];
  TProfile* pGmv2[9];
  TProfile* pPi0v2[9];
  
  TString filename="gammav2_0906.root";
  // drawpi0(filename,pdf,c);
  cout << "start gamma"<<endl;
  TFile* fgamma = new TFile("gammav2_0906.root");
  TH1F* hCent = (TH1F*)fgamma->Get("hCent");
  TH3F* hGammaSpec = (TH3F*)fgamma->Get("hGammaSpec");

  // c->Divide(3,2);
  int colors[7]={kRed,kOrange+8,kMagenta,kViolet,kBlue,kCyan,kGreen+2};
  TF1* fgammaSpec[7];
  for (int i=2;i<8;i++){
    c->cd(i-1);
    hGmSp[i] = (TH1F*)hGammaSpec->ProjectionX(Form("%d",i),hGammaSpec->GetYaxis()->FindBin(-0.2),hGammaSpec->GetYaxis()->FindBin(0.2) ,i+1,i+1);
    float events = hCent->GetBinContent(i+1);
    hGmSp[i]->Scale(1./(events*hGmSp[i]->GetBinWidth(1)));
    hGmSp[i]->SetLineColor(colors[i-2]);
    
    fgammaSpec[i] = new TF1(Form("fgammaSpec_%d",i),"2*TMath::Pi()*[0]*x*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])", 0, 15 );
    double par[5]={1052,0.408,0.091,1.021,10.98};
    fgammaSpec[i]->SetParameters(par);
    hGmSp[i]->Fit(fgammaSpec[i]);
    hGmSp[i]->Draw(); //change to a higher pt
    gPad->SetLogy();

    addpdf(pdf);

  }
  
  pdf->On();
  pdf->Close();
}

void drawpi0(TString filename, TPDF* pdf,TCanvas* c)
{
  c->cd();
  c->Clear();
  TFile* fgamma = new TFile(filename.Data());
  TH1F* hPi0v2[9];
  TH1F* hPi0Sp[9];

  TProfile2D* gPi0 = (TProfile2D*)fgamma->Get("pPi0v2");
  
  for (int i=2;i<9;i++){
    pPi0v2[i] = (TProfile*)gPi0->ProfileX("cent",i+1,i+1);
    pPi0v2[i]->GetYaxis()->SetRangeUser(0,0.25);
    pPi0v2[i]->DrawCopy();
    addpdf(pdf);
  }
  
  TH2F* hPi0Spec= (TH2F*)fgamma->Get("hPi0Spec");
  for (int i=2;i<9;i++){
    hPi0Sp[i] = (TH1F*)hPi0Spec->ProjectionX("cc",i+1,i+1);
    hPi0Sp[i]->DrawCopy(); //change to a higher pt
    gPad->SetLogy();
    addpdf(pdf);
  }
  fgamma->Close();
}
