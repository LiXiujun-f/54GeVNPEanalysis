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
void drawLatex(double x,double y, char* s)
{
  TLatex lat;
  lat.DrawLatexNDC(x,y,s);
}
void drawKs(int centL = 4,int centH = 4){
  myStyle();
  //change the cent bin to the hist bin number

  int const nPt = 6;
  double ptedge[nPt+1]={0.2,0.6,1,1.4,1.8,2.2,3};
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF(Form("Ksplots_%d_%d.pdf",centL,centH));
  pdf->Off();
  // centH++;
  // centL++;
  drawtitle(pdf,c,"Ks plots");
  TFile* f = TFile::Open("Ks.root");
  TH3F* hnsigpi = (TH3F*)f->Get("hnSigE_pi");
  TH3F* hnsigpils = (TH3F*)f->Get("hnSigE_pi_ls");
  hnSigE_pi->Add(hnsigpils,-1);
  // hnsigpi->Draw("colz");
  TH2F* hnsigpi_cent = (TH2F*)hnsigpi->Project3D("xy");
  hnsigpi_cent->Draw("colz");
  gPad->SetLogz();
  addpdf(pdf);
  gPad->SetLogz(0);
  TH3F* hKs = (TH3F*)f->Get("hKs");
  TH3F* hKs_LS = (TH3F*)f->Get("hKs_LS");
  hKs->Add(hKs_LS,-1);
  cout << "Get Ks" << endl;  
  for (int i=0;i<nPt;i++) 
  {
    int binL = hKs->GetYaxis()->FindBin(ptedge[i]);
    int binH = hKs->GetYaxis()->FindBin(ptedge[i+1]);
    int centBinL = hKs->GetZaxis()->FindBin(centL);
    int centBinH = hKs->GetZaxis()->FindBin(centH);
    cout << ptedge[i]<< " "<<binL<<endl;
    TH1F* hm = (TH1F*)hKs->ProjectionX("hm",binL,binH,centBinL,centBinH);
    hm->Draw();
    drawLatex(0.6,0.7,Form("%0.1f<p_{T}<%0.1f cent%d_%d",ptedge[i],ptedge[i+1],centL,centH));
    addpdf(pdf);
  }
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
