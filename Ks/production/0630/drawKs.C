#include "rootlogon.h"
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
void drawKs(){
  myStyle();
  TPDF* pdf = new TPDF("Ksplots.pdf");
  pdf->Off();
  TCanvas* c = new TCanvas("c","c");
  drawtitle(pdf,c,"Ks plots");
  TFile* f = TFile::Open("Ks.root");
  TH2F* hnsigpi = (TH2F*)f->Get("hnSigE_pi");
  TH2F* hnsigpils = (TH2F*)f->Get("hnSigE_pi_ls");
  hnSigE_pi->Add(hnsigpils,-1);
  hnsigpi->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);
  TH2F* hKs = (TH2F*)f->Get("hKs");
  TH2F* hKs_LS = (TH2F*)f->Get("hKs_LS");
  hKs->Add(hKs_LS,-1);
  TH1F* hm = (TH1F*)hKs->ProjectionX();
  hm->Draw();
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}

// TH1D	hevt;1	hevt
// TH1D	hevtcut;1	hevtcut
// TH2F	hnSigE_pi;1	pion from Ks
// TH2F	hnSigE_pi_tof;1	pion with tof only
// TH2F	hKs_LS;1	Mpipi like sign
// TH2F	hKs;1	M#pi#pi
// TH2F	hnSigE_pi_ls;1	pion from Ks like sign
// TH1F	hcent;1	hcent
// TH1F	hcentwg;1	hcentwg
