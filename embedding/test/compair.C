#include "sPhenixStyle.h"
#include ""
Double_t func1(Double_t *x, Double_t *par){
    Double_t arg = 0;
    Double_t val = 0;
    if(par[2]!=0 && par[1]!=1){
	arg = 1+sqrt(x[0]*x[0])/par[2]*(1-par[1]);//1.+(par[1]-1)*sqrt(x[0]*x[0]+0.149*0.149)/par[2];
	val = par[0]*(sqrt(x[0]*x[0])*TMath::Power(arg,-1/(1-par[1])));//exp(arg*(1/(1-par[1])));
    }
    return val;
}

void compair()
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
  
  TString filename="gammav2.root";
  // drawpi0(filename,pdf,c);
  cout << "start gamma"<<endl;
  TFile* fgamma = new TFile("gammav2.root");
  TH1F* hCent = (TH1F*)fgamma->Get("hCent");
  TH3F* hGammaSpec = (TH3F*)fgamma->Get("hGammaSpec");

  // c->Divide(3,2);
  int colors[7]={kRed,kOrange+8,kMagenta,kViolet,kBlue,kCyan,kGreen+2};
  TF1* fgammaSpec[7];
  for (int i=7;i<8;i++){
    c->cd(i-1);
    // hGmSp[i] = (TH1F*)hGammaSpec->ProjectionX(Form("%d",i),hGammaSpec->GetYaxis()->FindBin(-1),hGammaSpec->GetYaxis()->FindBin(0.5) ,i+1,i+1);
    // hGmSp[i] = (TH1F*)hGammaSpec->ProjectionX(Form("%d",i),hGammaSpec->GetYaxis()->FindBin(-0.5),hGammaSpec->GetYaxis()->FindBin(0.5) ,3,9);
    hGmSp[i] = (TH1F*)hGammaSpec->ProjectionX(Form("%d",i),hGammaSpec->GetYaxis()->FindBin(-0.5),hGammaSpec->GetYaxis()->FindBin(0.5) ,8,9);
    // float events = hCent->GetBinContent(i+1);
    // float events = hCent->GetEntries();
    float events = hCent->Integral(8,9);
    hGmSp[i]->Scale(1./(events*hGmSp[i]->GetBinWidth(1)));
    hGmSp[i]->SetLineColor(colors[i-2]);
    TH1F* h=(TH1F*)hGmSp[i]->Clone("aaa");
    for (int ib=1;ib<=h->GetNbinsX();ib++)
    {
       h->SetBinContent(ib,h->GetBinContent(ib)/h->GetBinCenter(ib));
    
    }
    
    // fgammaSpec[i] = new TF1(Form("fgammaSpec_%d",i),"2*TMath::Pi()*[0]*sqrt(x*x-0.14*0.14)*pow(TMath::Exp(-1*[1]*sqrt(x*x-0.14*0.14)-[2]*(x*x-0.14*0.14))+sqrt(x*x-0.14*0.14)/[3], -[4])", 0, 10 );
    fgammaSpec[i] = new TF1(Form("fgammaSpec_%d",i),"2*TMath::Pi()*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])", 0.2, 10 );
    // fgammaSpec[i] = new TF1(Form("fgammaSpec_%d",i),func1, 0, 10,3 );
    double par[5]={698.6,0.3983,0.064,1.058,11.06};
    fgammaSpec[i]->SetParameters(par);
    // hGmSp[i]->Fit(fgammaSpec[i]);
    // h->Fit(fgammaSpec[i]);
    hGmSp[i]->DrawCopy(); //change to a higher pt
    // h->DrawCopy(); //change to a higher pt
    gPad->SetLogy();

    addpdf(pdf);

  }
 
  TFile* f = new TFile("pi0_eta_gamma_decay_cent0_10p.root");
  TH1F* hhhh = (TH1F*)f->Get("h_pi0");
  hhhh->Scale(6.28/hhhh->GetBinWidth(1)/1.0e6); 
  hhhh->Draw("same");


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
