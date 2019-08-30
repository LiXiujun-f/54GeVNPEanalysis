#include "rootlogon.h"
#include <string>
TPDF* pdf;
void addpdf(TPDF* pdf)
{
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off(); 
}
void drawtitle(TPDF* pdf,TCanvas* c,string s){
  c->cd();
  c->Draw();
  // setPad(0.1,0.1,0.05,0.12);
  TLatex t;
  t.SetTextSize(0.05);
  t.DrawText(0.2,0.5,s.c_str());
  TLatex la;
  la.SetTextSize(0.035);
  la.DrawText(0.1,0.3,(new TDatime())->AsSQLString());
  la.DrawText(0.1,0.2,"by Yuanjing");
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void projectionAndFit(TH2F* h, float lowpt, float highpt, float &mean,float &sigma,float range1,float range2)
{
  int lbin = h->GetXaxis()->FindBin(lowpt);
  int hbin = h->GetXaxis()->FindBin(highpt);
  TH1F* hx = (TH1F*)h->ProjectionY("hx",lbin,hbin);
  //hx->SetDirectory(0);
  hx->DrawCopy();
  TF1* fit  = new TF1("fit","gaus", range1,range2);
  hx->Fit(fit);
  fit->SetLineColor(kRed);
  fit->DrawCopy("same"); 
  mean = fit->GetParameter(1); 
  sigma = fit->GetParameter(2); 
  TLatex lat;
  lat.SetTextSize(0.035);
  lat.DrawLatex(0.025,hx->GetMaximum()*0.6,Form("%.2f<p_{T}<%.2f GeV %s",lowpt,highpt,p.c_str()));
  addpdf(pdf);

 }

void funtofsamp(double* x,double *p )
{
  int idx = p[1];
  float e_part = p[0]*hesamp[idx]->GetBinContent(hesamp[idx]->FindBin(x[0]));
  float pi_part = p[2]*hpisamp[idx]->GetBinContent(hpisamp[idx]->FindBin(x[0]));
  float K_part = p[3]*hKsamp[idx]->GetBinContent(hKsamp[idx]->FindBin(x[0]));
  float p_part = p[4]*hpsamp[idx]->GetBinContent(hpsamp[idx]->FindBin(x[0]));
  return e_part+pi_part+K_part+p_part;
}
void fitNsigE(){
  myStyle();
  TCanvas* c = new TCanvas("c","c");
  pdf = new TPDF("NsigE.pdf");
  pdf->Off();
  // TPDF* pdf = new TPDF("NsigE.pdf");
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = TFile::Open("PE.root");
  TH2F* hels = (TH2F*)f->Get("hnSigE_e_ls_Dz"); 
  TH2F* he = (TH2F*)f->Get("hnSigE_e_Dz"); 
  he->Add(hels,-1);
  he->SetDirectory(0);
  TH2F* hp = (TH2F*)f->Get("hnSigE_p");
  hp->SetDirectory(0);
  TH2F* hk = (TH2F*)f->Get("hnSigE_k");
  hk->SetDirectory(0);
  TH2F* htof = (TH2F*)f->Get("hnSigE_tof");
  htof->SetDirectory(0);

  TH2F* hcut = (TH2F*)f->Get("hnSigE_Cut");
  hcut->SetDirectory(0);
  TH1F* hecut = (TH1F*)hcut->ProjectionX();
  hecut->SetDirectory(0);
  f->Close();
  f = TFile::Open("Ks.root");
  TH2F* hpi = (TH2F*)f->Get("hnSigE_pi");
  TH2F* hpils = (TH2F*)f->Get("hnSigE_pi_ls");
  hpi->Add(hpils,-1);
  hpi->SetDirectory(0);
  f->Close();
  float pt[50]; //0.2-5
  int i=0;
  pt[0]=0.2;
  while (pt[i]<1){
    pt[i+1]=pt[i]+0.1;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while  (pt[i]<3){
    pt[i+1]=pt[i]+0.5;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while (pt[i]<5){
    pt[i+1]=pt[i]+1;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  int  bin =i;
  TH1F* hpurity = new TH1F("hpurity", "hpurity;electron p_{T}(GeV);purity",bin,pt);
  TH1F* hmean_e = new TH1F("hmean_e","mean of nSigE for e;p_{T};mean",bin,pt);
  TH1F* hmean_pi = new TH1F("hmean_pi","mean of nSigE for pi;p_{T};mean",bin,pt);
  TH1F* hmean_k = new TH1F("hmean_k","mean of nSigE for k;p_{T};mean",bin,pt);
  TH1F* hmean_p = new TH1F("hmean_p","mean of nSigE for p;p_{T};mean",bin,pt);
  TH1F* hsigma_e = new TH1F("hsigma_e","sigma of nSigE for e",bin,pt); 
  TH1F* hsigma_pi = new TH1F("hsigma_pi","sigma of nSigE for pi",bin,pt); 
  TH1F* hsigma_p = new TH1F("hsigma_p","sigma of nSigE for p",bin,pt); 
  TH1F* hsigma_k = new TH1F("hsigma_k","sigma of nSigE for k",bin,pt); 
  TCanvas* c = new TCanvas("c","c");
  // TCanvas* c2 = new TCanvas("c2","c2");
  // c->Divide(2,2);
  TF1* ftot = new TF1("ftot","gausn(0)+gausn(3)+gausn(6)+gausn(9)",-10,15);
  TLatex lax;
  TH1F* hpad = new TH1F("hpad","hpad",120,-10,15);
  double par[5]={1,1,1,1,1};
  for (int j=0;j<bin;j++){
    cout<<pt[j]<<" "<<pt[j+1]<<endl;
    float mean[3]={4}, sigma[4]={0};
    c->cd();
    gPad->SetLogy();
    projectionAndFit(he,pt[j],pt[j+1],mean[0],sigma[0],"e",j);
    projectionAndFit(hp,pt[j],pt[j+1],mean[1],sigma[1],"p",j);
    projectionAndFit(hpi,pt[j],pt[j+1],mean[2],sigma[2],"pi",j);
    projectionAndFit(hk, pt[j],pt[j+1],mean[3],sigma[3],"K",j);
    float par[12];
    for (int i=0;i<4;i++){
      ftot->FixParameter(i*3+1,mean[i]);
      ftot->FixParameter(i*3+2,sigma[i]);
    }
    int lbin = hnSigE_tof->GetXaxis()->FindBin(lowpt);
    int hbin = hnSigE_tof->GetXaxis()->FindBin(highpt);
    TH1F* htotsamp = (TH1F*)hnSigE_tof->ProjectionY("htotx",lbin,hbin); 
  }
  gPad->SetLogy(0);
  hpurity->Draw();
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}
