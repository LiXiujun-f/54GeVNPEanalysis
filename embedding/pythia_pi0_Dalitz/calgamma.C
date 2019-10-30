#include "sPhenixStyle.h"
// #include "AnaCuts.h"
Double_t func1(Double_t *x, Double_t *par){
  Double_t arg = 0;
  Double_t val = 0;
  if(par[2]!=0 && par[1]!=1){
    arg = 1+sqrt(x[0]*x[0])/par[2]*(1-par[1]);//1.+(par[1]-1)*sqrt(x[0]*x[0]+0.149*0.149)/par[2];
    val = par[0]*(sqrt(x[0]*x[0])*TMath::Power(arg,-1/(1-par[1])));//exp(arg*(1/(1-par[1])));
  }
  return val;
}
TString centname[9]={"70-80","60-70","50-60","40-50","30-40","20-30","10-20","5-10","0-5"};
void calgamma()
{
  SetsPhenixStyle();   
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("gammav2plots.pdf");
  pdf->Off();

  TString filenamePi="gammav2.root";
  // TString filenamePi="gammav2.root";
  // TString filenameEta="eta2gamma_0918.root";
  // TString filenameEta="gammav2.root";
  TFile* fout = new TFile("fread_pi0_eta_2gamma.root","recreate");

  drawpi0(filenamePi,pdf,c,"pi0");
  // drawAndWriteGamma(filenamePi,fout,pdf,c,"pi0",0.01174);
  // drawAndWriteGamma(filenamePi,fout,pdf,c,"pi0",1.);
  drawAndWriteGamma(filenamePi,fout,pdf,c,"pi0",0.988*2);
  
  // drawpi0(filenameEta,pdf,c,"eta");
  // //0.48 is the scale factor for eta/pi0
  // drawAndWriteGamma(filenameEta,fout,pdf,c,"eta",0.3941*0.48);

  pdf->On();
  pdf->Close();
  fout->Close();
}

void drawpi0(TString filename, TPDF* pdf,TCanvas* c,TString particlename)
{
  cout << "draw "<< particlename.Data() <<endl;
  c->cd();
  c->Clear();
  TFile* fgamma = new TFile(filename.Data());
  fgamma->cd();
  TProfile* pPi0v2[9];
  TH1F* hPi0Sp[9];
  TH1F* hCent = (TH1F*)fgamma->Get("hCent");
  TProfile2D* gPi0 = (TProfile2D*)fgamma->Get("pPi0v2");
  if (!gPi0) 
  {
    cout <<"cannot find gPi0" <<endl;
  }
  else {
    for (int i=2;i<9;i++){
      pPi0v2[i] = (TProfile*)gPi0->ProfileX(Form("Cent_%d",i),i+1,i+1);
      pPi0v2[i]->GetYaxis()->SetRangeUser(0,0.25);
      pPi0v2[i]->GetYaxis()->SetTitle("v_{2}");
      pPi0v2[i]->DrawCopy();
      drawLatex( 0.2,0.3 ,Form("%s %s",particlename.Data(),centname[i].Data()) ,0.035);
      addpdf(pdf);
    }
  }

  TH2F* hPi0Spec= (TH2F*)fgamma->Get("hPi0Spec");
  if (!hPi0Spec)
  {
    cout << "cannot find pi0 spectra" <<endl;
  } 
  else 
  {
    for (int i=2;i<9;i++){
      hPi0Sp[i] = (TH1F*)hPi0Spec->ProjectionX("cc",i+1,i+1);
      hPi0Sp[i]->GetYaxis()->SetTitle("Counts");
      gPad->SetLogy(1);
      drawLatex(0.2,0.3,Form("%s %s",particlename.Data(),centname[i].Data()),0.035);
     TF1* f;
    if (particlename.Contains("pi0")) f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])", 0,15);
    else if (particlename.Contains("eta")) f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*sqrt(x*  x+0.547862*0.547862-0.134977*0.134977)-[2]*(x*x+0.547862*0.547862-0  .134977*0.134977))+sqrt(x*x+0.547862*0.547862-0.134977*0.134977)/[3  ], -[4])", 0,15);
    else {cout <<"please input the correct particle" << endl; return;}
   
        // TF1* f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])", 0,15);
    int const SpectraParPi0_centbin[9]={0,0,1,1,2,2,3,4,4};
    double const SpectraParPi0[5][5]={ 
                                  {1.67607, -1.77146, 0.0759459, -0.369874, 2.01742},
                                  {264.787, 0.349324, 0.0475088, 1.01322, 10.903},
                                  {406.429, 0.361361, 0.0558035, 1.10099, 11.2414},
                                  {682.788, 0.396488, 0.0690007, 1.06078, 11.0674},
                                  {992.598, 0.406828, 0.0831998, 1.04464, 11.0797}
                                 };
      f->SetParameters(SpectraParPi0[SpectraParPi0_centbin[i]] );
      //dNdptdy 
      hPi0Sp[i]->Scale(1./3.0/hPi0Sp[i]->GetBinWidth(1)); 
      double funInt = f->Integral(0,15);
      double gammaInt = hPi0Sp[i]->Integral()*hPi0Sp[i]->GetBinWidth(1);
      double scale = gammaInt/funInt;
      hPi0Sp[i]->Scale(1./scale); 
      cout << i << " "<<scale<<endl;
      hPi0Sp[i]->DrawCopy(); //change to a higher pt
      f->Draw("same");
      addpdf(pdf);
    }

  }
  // gPad->SetLogy(0);
  fgamma->Close();
}
void drawAndWriteGamma(TString filename, TFile* fout,TPDF* pdf, TCanvas* c,TString particlename,double branchratio)
{
  cout << "start "<< particlename.Data()<<"->gamma" <<endl;
  TH1F* hGmSp[9];
  TProfile* pGmv2[9];
  TGraph* gGmSp[9];
  TGraph* gGmv2[9];
  TFile* fgamma = new TFile(filename.Data());
  fgamma->cd();
  TH1F* hCent = (TH1F*)fgamma->Get("hCent");
  TH3F* hGammaSpec = (TH3F*)fgamma->Get("hGammaSpec");
  TProfile3D* pgammav2 = (TProfile3D*)fgamma->Get("pGammav2"); 
    // c->Divide(3,2);
  int colors[8]={kBlack,kRed,kOrange+8,kMagenta,kViolet,kBlue,kCyan,kGreen+2};
  // TF1* fgammaSpec[7];
  double etarange = 2;
  
  double x[1000],y[1000];
  int Nbins=0;

  //spectra
  for (int i=1;i<9;i++){
    // c->cd(i-1);
    if (i>=2)
      hGmSp[i] = (TH1F*)hGammaSpec->ProjectionX(Form("h%s2Gamma%d",particlename.Data(),i),hGammaSpec->GetYaxis()->FindBin(etarange*-0.5),hGammaSpec->GetYaxis()->FindBin(etarange*0.5) ,i+1,i+1);
    else if (i==1)
      hGmSp[1] = (TH1F*)hGammaSpec->ProjectionX(Form("h%s2Gamma_mb",particlename.Data()),hGammaSpec->GetYaxis()->FindBin(etarange*-0.5),hGammaSpec->GetYaxis()->FindBin(etarange*0.5) ,3,9);
    TF1* f;
    if (particlename.Contains("pi0")) f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])", 0,15);
    else if (particlename.Contains("eta")) f = new TF1("f","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*sqrt(x*x+0.547862*0.547862-0.134977*0.134977)-[2]*(x*x+0.547862*0.547862-0.134977*0.134977))+sqrt(x*x+0.547862*0.547862-0.134977*0.134977)/[3], -[4])", 0,15);
    else {cout <<"please input the correct particle" << endl; return;}
    int const SpectraParPi0_centbin[9]={0,0,1,1,2,2,3,4,4};
    double const SpectraParPi0[5][5]={ 
                                 {1.67607, -1.77146, 0.0759459, -0.369874, 2.01742},
                                 {264.787, 0.349324, 0.0475088, 1.01322, 10.903},
                                 {406.429, 0.361361, 0.0558035, 1.10099, 11.2414},
                                 {682.788, 0.396488, 0.0690007, 1.06078, 11.0674},
                                 {992.598, 0.406828, 0.0831998, 1.04464, 11.0797}
    };
    double events=1;
    if (i>=2) events = hCent->GetBinContent(i+1); //some problem happens the hCent does not represent real events, I donot know why
    else if (i==1) events = hCent->GetEntries();
    // if (i>=2) events = 1.67e10; // directly use the scale factor taken from pi0 spectra
    // if (i>=7) events = 8.33e9;
    hGmSp[i]->Scale(1./(hGmSp[i]->GetBinWidth(1)*etarange));
    if (i>=2){
       f->SetParameters(SpectraParPi0[SpectraParPi0_centbin[i]] );
       double funInt = f->Integral(0,15);
       double gammaInt = hGmSp[i]->Integral()*hGmSp[i]->GetBinWidth(1);
       cout << "scale by mother pt: " <<gammaInt/funInt<< " "<<events<< endl;
    }

    hGmSp[i]->SetDirectory(0);
    hGmSp[i]->Scale(1.*branchratio/(events));
    hGmSp[i]->GetYaxis()->SetTitle("dN/dydp_{T}");
    hGmSp[i]->GetXaxis()->SetTitle("p_{T} [GeV]");
    hGmSp[i]->SetLineColor(colors[i-1]);
    hGmSp[i]->DrawCopy(); 
    f->DrawCopy("same");
    gPad->SetLogy(1);
    drawLatex(0.2,0.3,Form("#%s->#gamma %s",particlename.Data(),centname[i].Data()),0.05);
    addpdf(pdf);

    Nbins=hGmSp[i]->GetNbinsX();
    for (int ib=0;ib<Nbins;ib++)
    {
      x[ib]=hGmSp[i]->GetBinCenter(ib);
      y[ib]=hGmSp[i]->GetBinContent(ib);
      gGmSp[i] = new TGraph(Nbins,x,y);
      gGmSp[i]->SetName(Form("gGmSp_%s_%d",particlename.Data(),i));
    }
  }

  gPad->SetLogy(0); 
  //gamma v2
  TProfile2D* pgamma2d = (TProfile2D*)pGammav2->Project3DProfile("zx");
  for (int i=1;i<9;i++) {
    if (i>=2) pGmv2[i]=(TProfile*)pgamma2d->ProfileX(Form("p%s2gammav2_%d",particlename.Data(),i),i+1,i+1);
    else if (i==1)
        pGmv2[1]=(TProfile*)pgamma2d->ProfileX(Form("p%s2gammav2_mb",particlename.Data()),3,9);
    pGmv2[i]->SetDirectory(0);
    pGmv2[i]->GetYaxis()->SetRangeUser(-0.05,0.25);
    pGmv2[i]->DrawCopy();
    pGmv2[i]->GetYaxis()->SetTitle("v_{2}");
    drawLatex(0.2,0.3,Form("#%s->#gamma %s",particlename.Data(),centname[i].Data()),0.05);
    addpdf(pdf);

    Nbins=pGmv2[i]->GetNbinsX();
    for (int ib=0;ib<Nbins;ib++)
    {
      x[ib]=pGmv2[i]->GetBinCenter(ib);
      y[ib]=pGmv2[i]->GetBinContent(ib);
      gGmv2[i] = new TGraph(Nbins,x,y);
      gGmv2[i]->SetName(Form("gGmv2_%s_%d",particlename.Data(),i));
    }
  }

  fgamma->Close();

  cout <<"start writing..." <<endl;
  fout->cd();
  for (int ic=1;ic<9;ic++){  
    hGmSp[ic]->Write();
    gGmSp[ic]->Write();
    pGmv2[ic]->Write();
    gGmv2[ic]->Write();
  }
}
