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
void calMee(){
  myStyle();
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("Mee.pdf");
  pdf->Off();
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = new TFile("PE.root");
  TH3F* hphotols = (TH3F*)f->Get("hphoto_LS"); 
  TH3F* hphoto = (TH3F*)f->Get("hphoto"); 
  TH3F* hphotoul = (TH3F*)f->Get("hphoto")->Clone("hphotoul"); 
  hphoto->Add(hphotols,-1);
  float pt[50]; //0.2-5
  int i=0;
  pt[0]=0.2;
  while (pt[i]<1){
    pt[i+1]=pt[i]+0.1;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while  (pt[i]<3){
    pt[i+1]=pt[i]+0.2;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while (pt[i]<4){
    pt[i+1]=pt[i]+0.5;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while (pt[i]<=5){
    pt[i+1]=pt[i]+1;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;

  int  bin =i;
  TH1F* hsp = new TH1F("hspectra", "hspectra;electron p_{T}(GeV);Counts",bin,pt);
  for(int j=0;j<bin;j++){
    int lbin = hphoto->GetYaxis()->FindBin(pt[j]);
    int hbin = hphoto->GetYaxis()->FindBin(pt[j+1]);
    TH1F* hpx = (TH1F*)hphoto->ProjectionX("hpx",lbin,hbin);
    TH1F* hpx_ul = (TH1F*)hphotoul->ProjectionX("hpx_ul",lbin,hbin);
    TH1F* hpxls = (TH1F*)hphotols->ProjectionX("hpx_ls",lbin,hbin);
    hpx_ls->SetLineColor(kBlue);
    // hpx_ul->SetLineColor();
    hpx->SetLineColor(kRed);

    hpx_ul->Draw();
    hpx_ls->Draw("same");
    hpx->Draw("same");
    TLegend* lpx = new TLegend(0.25,0.65,0.45,0.85);
    lpx->SetHeader("0-80%");
    lpx->AddEntry(hpx_ul,"UnLike","l");
    lpx->AddEntry(hpx_ls,"LikeSign","l");
    lpx->AddEntry(hpx,"UL-LS","l");
    lpx->Draw();
    TLatex lat;
    lat.SetTextSize(0.035);
    lat.DrawLatex(0.025,hpx_ul->GetMaximum()*0.6,Form("%.2f<p_{T}<%.2f GeV",pt[j],pt[j+1]));
    hsp->SetBinContent(j+1,hpx->Integral()/hsp->GetBinWidth(j+1));
    addpdf(pdf);
    }
    TH2F* hincEptCent= (TH2F*)f->Get("hincEptCent");
    // hincEptCent->Draw("colz");
    // addpdf(pdf);
    TH1F* hincE = (TH1F*)hincEptCent->ProjectionX("hincEpt_MB",1,9);
    hincE->Draw();
    hincE->Scale(1./hincE->GetBinWidth(1));
    hsp->Draw("same");
    hincE->SetLineColor(kRed);
    hincE->GetYaxis()->SetTitle("dN/dp_{T}");
    hincE->GetXaxis()->SetRangeUser(1e3,3e9);
    TLegend* lince = new TLegend(0.65,0.65,0.85,0.85);
    lince->AddEntry(hincE,"inclusive e","l");
    lince->AddEntry(hsp,"photonic e","l");
    lince->Draw();
    gPad->SetLogy(1);
    addpdf(pdf);
    pdf->On();
    pdf->Close();
}
