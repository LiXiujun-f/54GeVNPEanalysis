#include "sPhenixStyle.h"
#include "AnaCuts.h"

double myfitv2(double* x, double* p)
{
  if (x[0]<p[6]) return p[0]+p[1]*x[0]+p[2]*x[0]*x[0]+p[3]*x[0]*x[0]*x[0];
  else return (p[0]+p[1]*p[6]+p[2]*pow(p[6],2)+p[3]*pow(p[6],3)-p[4]-p[5]*p[6])+p[4]+p[5]*x[0];
}

void fitPhoEv2(TH3F* hPhev2, int centL, int centH, TString name, TPDF* pdf, TFile* fout);
void fitPhoEv2(TH3F* hPhev2,TH3F* hPhev2raw, int centL, int centH, TString name , TPDF* pdf, TFile* fout);
void ProjectionAndFit(TH2F* hMc,TH2F* hRc,int centL,int centH,TString name ,TPDF* pdf,TFile* fout);
void CalPhoEv2(TProfile2D* pPhev2_2d, int centL, int centH, TString name , TPDF* pdf, TFile* fout);
void calPhoeff()
{
  SetsPhenixStyle();
  // TFile* file = new TFile("embeddingQa.phoE.root.ep0");
  // TFile* file = new TFile("embeddingQa.phoE.root");
  // TFile* file = new TFile("test.root");
  TFile* file = new TFile("embeddQa_0910.root");
  // TFile* file = new TFile("embeddQa_phicut0827.root");
  // TFile* file = new TFile("embeddingQa.phoE.root.0818");

  // TFile* file = new TFile("test.McAna.root");
  TCanvas* c = new TCanvas("c","c",800,600);
  TPDF* pdf = new TPDF("PhoErecoEff.pdf");
  pdf->Off();
  TH2F* hTagE = (TH2F*)file->Get("hTagElectron");
  TH2F* hRecoTagE = (TH2F*)file->Get("hTagElectronPassCut");
  hTagE->SetDirectory(0);
  hRecoTagE->SetDirectory(0);
  // hTagE->Sumw2();
  // hRecoTagE->Sumw2(); 

  TFile* fout = new TFile("RecoEff.root","recreate");
  // 0->80%,1->70%,2->60%,3->50%,...,6->10%,7->5%,8->0
  gStyle->SetOptFit(111);
  ProjectionAndFit(hTagE,hRecoTagE,2,5,"RecoEff",pdf,fout);
  ProjectionAndFit(hTagE,hRecoTagE,2,8,"RecoEff",pdf,fout);
  ProjectionAndFit(hTagE,hRecoTagE,6,8,"RecoEff",pdf,fout);
  gStyle->SetOptFit(0);
 
  //cal v2
  TProfile2D* pPhev2_2d = (TProfile2D*)file->Get("pPhoEv2");
  TProfile2D* pPi0v2_2d = (TProfile2D*)file->Get("pPi0Ev2");
  TH3F* hPhEv2 = (TH3F*)file->Get("hPhoEPtPhiCent");
  TH3F* hRecoEv2 = (TH3F*)file->Get("hRecoEPtPhiCent");
  TH3F* hPi0v2 = (TH3F*)file->Get("hPi0PtPhiCent");
  TH3F* hPi0v2raw = (TH3F*)file->Get("hPi0PtPhiCentRaw");
  TH3F* hPhEv2raw = (TH3F*)file->Get("hPhoEPtPhiCentRaw");
  TH3F* hRecoEv2raw = (TH3F*)file->Get("hRecoEPtPhiCentRaw");
  // TH3F* hPhEv2 = (TH3F*)file->Get("hPhoEPtPhiCentRaw");
  // hPhEv2->Divide(hPhEv2raw);
  hPhEv2->SetDirectory(0);
  hPhEv2raw->SetDirectory(0);
  hRecoEv2->SetDirectory(0);
  hRecoEv2raw->SetDirectory(0);

  gStyle->SetOptFit(111);
  CalPhoEv2(pPhev2_2d,2,5,"PhoE",pdf,fout);
  CalPhoEv2(pPhev2_2d,2,8,"PhoE",pdf,fout);
  CalPhoEv2(pPhev2_2d,7,8,"PhoE",pdf,fout);
  

  fitPhoEv2(hPhEv2,hPhEv2raw, 2, 8,"PhoE",  pdf, fout);
  fitPhoEv2(hPhEv2,hPhEv2raw, 2, 5,"PhoE",  pdf, fout);
  fitPhoEv2(hPhEv2,hPhEv2raw, 7, 8,"PhoE",  pdf, fout);
 
  fitPhoEv2(hPhEv2raw, 2, 8,"Raw",  pdf, fout);
  // fitPhoEv2(hPhEv2raw, 2, 5,"Raw",  pdf, fout);
  // fitPhoEv2(hPhEv2raw, 7, 8,"Raw",  pdf, fout);

  fitPhoEv2(hPhEv2, 2, 8,"NoWg",  pdf, fout);
  // fitPhoEv2(hPhEv2, 2, 5,"NoWg",  pdf, fout);
  // fitPhoEv2(hPhEv2, 7, 8,"NoWg",  pdf, fout);

  fitPhoEv2(hRecoEv2,hRecoEv2raw, 2, 8,"RecoE",  pdf, fout);
  fitPhoEv2(hPi0v2,hPi0v2raw, 2, 8,"Pi0",  pdf, fout);
  // fitPhoEv2(hPi0v2, 4, 5,"Pi0",  pdf, fout);

  CalPhoEv2(pPi0v2_2d,2,8,"Pi0",pdf,fout);
  
  gStyle->SetOptFit(0);
  
  pdf->On();
  pdf->Close();
  fout->Close();
  file->Close();
}

void ProjectionAndFit(TH2F* hMc,TH2F* hRc,int centL,int centH,TString name,TPDF* pdf,TFile* fout)
{
  // TFile* file = new TFile("test.McAna.root");
  int centHbin = hMc->GetYaxis()->FindBin(centH);
  int centLbin = hMc->GetYaxis()->FindBin(centL);
  int const nbin =11;
  double ptedge[nbin+1]={0.2,0.3,0.4,0.5,0.6,0.7,0.85,1,1.2,1.6,2.0,2.8};
  TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",centLbin,centHbin);
  TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",centLbin,centHbin);
  hMcX->SetDirectory(0);
  hRcX->SetDirectory(0);
  hRcX->Rebin(4);
  hMcX->Rebin(4);
  // hRcX = (TH1F*)hRcX->Rebin(11,"hRcX",ptedge);
  // hMcX = (TH1F*)hMcX->Rebin(11,"hMcX",ptedge);
  TH1F* hRecoEff = (TH1F*)hRcX->Clone(Form("h%s_%d_%d",name.Data(),centL,centH));
  hRecoEff->SetDirectory(0); 
  hRecoEff->Divide(hMcX);
  hRecoEff->GetYaxis()->SetRangeUser(0,0.9);
  hRecoEff->Draw();
  TF1* fit = new TF1(Form("f%s_%d_%d",name.Data(),centL,centH),"pol3",0,4); 
  fit->SetLineColor(kRed);
  hRecoEff->GetXaxis()->SetRangeUser(0.2,4);
  hRecoEff->Fit(fit,"R");

  drawLatex(0.2,0.8,Form("%s %d-%d \%",name.Data(),CentNum[centH+1],CentNum[centL]), 0.035);
  addpdf(pdf);
  fout->cd();
  hRecoEff->Write();
  fit->Write(); 
} 
void fitPhoEv2(TH3F* hPhev2,TH3F* hPhev2raw, int centL, int centH, TString name , TPDF* pdf, TFile* fout)
{
  int centHbin = hPhev2->GetZaxis()->FindBin(centH);
  int centLbin = hPhev2->GetZaxis()->FindBin(centL);
  cout <<centHbin<<" "<<centLbin << endl;
  // int const nbin = 8;
  // int const nbin = 21;
  int const nbin = 11;
  // double ptedge[22]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.4,2.8,3.2,3.6,4.0,5,6,7,8,9,10};
  //
  double ptedge[12]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,1.6,2.4,3.2,4.0};
  // double ptedge[nbin+1]={0.2,0.42,0.67,0.85,1,1.2,1.6,2.0,2.8};
  // double ptedge[10]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,2.4,4.0};
  TH1F* hPhEv2 = new TH1F(Form("h%sv2_%d_%d",name.Data(),centL,centH),"hPhEv2",nbin,ptedge);
  TF1* fitfun = new TF1("fitfun","[0]*(1+2*[1]*cos(2*x))",0,3.14);
  for (int i=0;i<nbin;i++){
    TH1F* h= (TH1F*)hPhev2->ProjectionY("h", hPhev2->GetXaxis()->FindBin(ptedge[i]), hPhev2->GetXaxis()->FindBin(ptedge[i+1]),centLbin,centHbin);
    TH1F* hraw= (TH1F*)hPhev2raw->ProjectionY("hraw", hPhev2->GetXaxis()->FindBin(ptedge[i]), hPhev2->GetXaxis()->FindBin(ptedge[i+1]),centLbin,centHbin);
    // h->Divide(hraw);
    h->Rebin(5);
    hraw->Rebin(5);
    float scale = h->Integral()/(1.0*h->GetNbinsX());
    cout <<"mean value="<<scale  << endl;
    for (int ibin = 0 ;ibin<hraw->GetNbinsX();ibin++)
    {
      h->SetBinContent(ibin+1,h->GetBinContent(ibin+1)*scale/(1.0*hraw->GetBinContent(ibin+1)));
    }
    h->Draw();

    h->Fit(fitfun);
    hPhEv2->SetBinContent(i+1,fitfun->GetParameter(1));
    hPhEv2->SetBinError(i+1,fitfun->GetParError(1));
    drawLatex(0.2,0.8,Form("%0.1f<pt<%0.1f %d-%d", ptedge[i],ptedge[i+1],CentNum[centH+1],CentNum[centL]), 0.035);
    addpdf(pdf);
    delete h;
    delete hraw;
  }
  TF1* f = new TF1(Form("f%s_%d_%d",name.Data(),centL,centH),"pol5",0,4);
  f->SetLineColor(kRed);
  // f->FixParameter(6,1.3);
  hPhEv2->Draw();
  hPhEv2->GetYaxis()->SetRangeUser(0,0.3);
  // hPhEv2->GetYaxis()->SetRangeUser(0,0.35);
  hPhEv2->Fit(f,"R");

  drawLatex(0.2,0.8,Form("%s e v_{2} %d-%d \%",name.Data(),CentNum[centH+1],CentNum[centL]), 0.035);
  addpdf(pdf);
  fout->cd();
  hPhEv2->Write();
  f->Write();
  
}
void fitPhoEv2(TH3F* hPhev2, int centL, int centH, TString name, TPDF* pdf, TFile* fout)
{
  int centHbin = hPhev2->GetZaxis()->FindBin(centH);
  int centLbin = hPhev2->GetZaxis()->FindBin(centL);
  // double ptedge[13]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.8,4.0};
  double ptedge[12]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,1.6,2.4,3.2,4.0};
  // double ptedge[10]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,2.4,4.0};
  TH1F* hPhEv2 = new TH1F(Form("h%sv2_%d_%d",name.Data(),centL,centH),"hPhEv2",11,ptedge);
  TF1* fitfun = new TF1("fitfun","[0]*(1+2*[1]*cos(2*x))",0,3.14);
  for (int i=0;i<11;i++){
    TH1F* h= (TH1F*)hPhev2->ProjectionY("h", hPhev2->GetXaxis()->FindBin(ptedge[i]), hPhev2->GetXaxis()->FindBin(ptedge[i+1]),centLbin,centHbin);
    h->Rebin(5);
    h->Draw();

    h->Fit(fitfun);
    hPhEv2->SetBinContent(i+1,fitfun->GetParameter(1));
    hPhEv2->SetBinError(i+1,fitfun->GetParError(1));
    drawLatex(0.2,0.8,Form("%0.1f<pt<%0.1f %d-%d", ptedge[i],ptedge[i+1],CentNum[centH+1],CentNum[centL]), 0.035);
    addpdf(pdf);
  }
  TF1* f = new TF1(Form("f%s_%d_%d",name.Data() , centL,centH),"pol3",0.2,4);
  f->SetLineColor(kRed);
  // f->FixParameter(6,1.3);
  hPhEv2->Draw();
  hPhEv2->GetYaxis()->SetRangeUser(0,0.25);
  // hPhEv2->GetYaxis()->SetRangeUser(0,0.35);
  hPhEv2->Fit(f,"R");

  drawLatex(0.2,0.8,Form("%s e v_{2} %d-%d \%", name.Data(),CentNum[centH+1],CentNum[centL]), 0.035);
  addpdf(pdf);
  fout->cd();
  hPhEv2->Write();
  f->Write();

}
void CalPhoEv2(TProfile2D* pPhev2_2d, int centL, int centH, TString name , TPDF* pdf, TFile* fout)
{
  int centHbin = pPhev2_2d->GetYaxis()->FindBin(centH);
  int centLbin = pPhev2_2d->GetYaxis()->FindBin(centL);
  TProfile* pPhEv2 = (TProfile*)pPhev2_2d->ProfileX("pPhEv2",centLbin,centHbin);
  pPhEv2->SetDirectory(0);
  // double ptedge[13]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.8,4.0};
  double ptedge[12]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,1.6,2.4,3.2,4.0};
  // double ptedge[10]={0,0.2,0.3,0.4,0.5,0.6,0.8,1.2,2.4,4.0};
  pPhEv2 = (TProfile*)pPhEv2->Rebin(11, Form("p%sv2_%d_%d", name.Data(),centL,centH),ptedge);
  // pPhEv2 = (TProfile*)pPhEv2->Rebin(9, Form("pPhEv2_%d_%d",centL,centH),ptedge);
  pPhEv2->Draw();
  pPhEv2->GetYaxis()->SetRangeUser(0,0.45);
  // TF1* f = new TF1(Form("fPhpE_%d_%d",centL,centH),myfitv2,0,4,7);
  TF1* f = new TF1(Form("f%sv2_%d_%d",name.Data(),centL,centH),"pol4",0,4);
  f->SetLineColor(kRed);
  // f->FixParameter(6,1.3);
  pPhEv2->Fit(f);
  cout << " pt "<< pPhEv2->GetBinCenter(3)<<" "<<pPhEv2->GetBinEntries(3)<<endl;
  drawLatex(0.2,0.8,Form("photonic e v_{2} %d-%d \%",CentNum[centH+1],CentNum[centL]), 0.035);
  addpdf(pdf);
  fout->cd();
  pPhEv2->Write();

}
