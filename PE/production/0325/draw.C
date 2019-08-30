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
void draw(){
  myStyle();
TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("pe.pdf");
  pdf->Off();
  // pdf->NewPage();
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = new TFile("PE.root");
  TH2F* hsige = (TH2F*)f->Get("hnSigE_e");
  TH2F* hsigels = (TH2F*)f->Get("hnSigE_e_ls");
  hsige->Add(hsigels,-1);
  hsige->Draw("colz");
  TH1F* hsigeY = (TH1F*)hsige->ProjectionY("");
  gPad->SetLogz(1); 
  addpdf(pdf);
  // hsigeY->Rebin(8);
  hsigeY->Draw();
  addpdf(pdf);
  TH3F* hphotols = (TH3F*)f->Get("hphoto_LS"); 
  TH3F* hphoto = (TH3F*)f->Get("hphoto"); 
  TH3F* hphotoul = (TH3F*)f->Get("hphoto")->Clone("hphotoul"); 
  hphoto->Add(hphotols,-1);
  TH1F* hpx = (TH1F*)hphoto->ProjectionX("hpx");
  TH1F* hpx_ul = (TH1F*)hphotoul->ProjectionX("hpx_ul");
  TH1F* hpxls = (TH1F*)hphotols->ProjectionX("hpx_ls");
  hpx_ls->SetLineColor(kBlue);
  // hpx_ul->SetLineColor();
  hpx->SetLineColor(kRed);

  hpx_ul->Draw();
  hpx_ls->Draw("same");
  hpx->Draw("same");
  TLegend* lpx = new TLegend(0.65,0.65,0.85,0.85);
  lpx->SetHeader("0-80%");
  lpx->AddEntry(hpx_ul,"UnLike","l");
  lpx->AddEntry(hpx_ls,"LikeSign","l");
  lpx->AddEntry(hpx,"UL-LS","l");
  lpx->Draw();
  addpdf(pdf);
  TH3F* hV0 = (TH3F*)f->Get("hV0"); 
  TH2F* hV0_xy = (TH2F*)hV0->Project3D("xy");
  TH2F* hV0_zy = (TH2F*)hV0->Project3D("zy");
  hV0_xy->Draw("colz");
  addpdf(pdf);
  hV0_zy->Draw("colz");
  addpdf(pdf);

  TH2F* hincEptCent = (TH2F*)f->Get("hincEptCent");
  hincEptCent->Draw("colz");
  addpdf(pdf);
  gPad->SetLogy(1);
  for (int i=0;i<8;i++)
  {
    TH1F* hincE = (TH1F*)hincEptCent->ProjectionX(Form("hincEpt_%d",i),i+1,i+1);
    hincE->Draw();
    addpdf(pdf);
  }
 gPad->SetLogy(0); 

  TH2F* hnSigE_pi = (TH2F*)f->Get("hnSigE_pi");
  hnSigE_pi->Draw("colz");
  addpdf(pdf);

  TH2F* hnSigE_k = (TH2F*)f->Get("hnSigE_k");
  hnSigE_k->Draw("colz");
  addpdf(pdf);

  TH2F* hnSigE_p = (TH2F*)f->Get("hnSigE_p");
  hnSigE_p->Draw("colz");
  addpdf(pdf);

  TH2F* hnSigE_tof = (TH2F*)f->Get("hnSigE_tof");
  hnSigE_tof->Draw("colz"); 
  TLine* lnsigmatofcut1 = new TLine(0.2,2,5,2);
  lnsigmatofcut1->SetLineColor(kRed);
  lnsigmatofcut1->Draw("same");
    
  TLine* lnsigmatofcut2 = new TLine(0.8,-1.2,5,-1.2);
  TF1* fnsigmatofcut = new TF1("fnsigmatofcut","2*x-2.8",0.2,0.8);
  lnsigmatofcut2->SetLineColor(kRed);
  fnsigmatofcut->SetLineColor(kRed);
  lnsigmatofcut2->Draw("same");
  fnsigmatofcut->Draw("same");
  addpdf(pdf);
   
  TH2F* hinvBetavsP = (TH2F*)f->Get("hinvBetavsP");
  hinvBetavsP->Draw("colz");
  addpdf(pdf);

  pdf->On(); 
  pdf->Close();
}
// TH1D	hevt;1	hevt
// TH1D	hevtcut;1	hevtcut
// TH2F	hinvBetavsP;1	#frac{1}{#beta} vs p
// TH2F	hnTofMulvsRef;1	hnTofMul vs Ref
// TH2F	hnTofMatvsRef;1	nTofmatch VS Refmult
// TH2F	hnSigEvsP;1	all tracks
// TH2F	hnSigE_p;1	proton with tof pid
// TH2F	hnSigE_pi;1	pion with tof pid
// TH2F	hnSigE_k;1	kaon with tof pid
// TH2F	hnSigE_Cut;1	elecron after pid cut
// TH2F	hnSigE_tof;1	elecron with tof only
// TH2F	hePtvsP;1	hePtvsP
// TH3F	hphoto_LS;1	Mee like sign vs e pt vs cent
// TH3F	hphoto;1	Mee
// TH3F	hphotoVsPt;1	Mee like sign vs photon pt vs cent
// TH3F	hphotoVsPt_LS;1	Mee like sign vs photon pt vs cent
// TH2F	hinvBeta_e;1	photonic elecron
// TH2F	hinvBeta_e_ls;1	photonic elecron
// TH2F	hnSigE_e;1	photonic elecron
// TH2F	hnSigE_e_ls;1	photonic elecron like sign
// TH3F	hV0;1	hV0
// TH3F	hV0_LS;1	hV0_LS
// TH1F	hcent;1	hcent
// TH1F	hcentwg;1	hcentwg
// TH2F	hincEptCent;1	inclusive e
