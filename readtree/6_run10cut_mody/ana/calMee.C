// #include "rootlogon.h"
#include "sPhenixStyle.h"
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
  // myStyle();
  SetsPhenixStyle();
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("Mee.pdf");
  pdf->Off();
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = new TFile("incEv2.root");
  // TH3F* hphotols = (TH3F*)f->Get("hphoto_LS_hitcut"); 
  TH3F* hphotols = (TH3F*)f->Get("hphoto_LS"); 
  TH3F* hphoto = (TH3F*)f->Get("hphoto"); 
  // TH3F* hphoto = (TH3F*)f->Get("hphoto_hitcut"); 
  TH3F* hphotoul = (TH3F*)f->Get("hphoto")->Clone("hphotoul"); 
  hphoto->Add(hphotols,-1);
  int const bin =12;
  double ptedge[bin+1]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.8,4.0};
  int centL = 3,centH=9;
  TH1F* centcorr = (TH1F*)f->Get("hcentwg");
  double nEvents = centcorr->Integral(centL,centH);
  cout<<nEvents<<endl;
  TH1F* hsp = new TH1F("hspectra", "hspectra;electron p_{T}(GeV);Counts",bin,ptedge);
  
  for(int j=0;j<bin;j++){
    int lbin = hphoto->GetYaxis()->FindBin(ptedge[j]);
    int hbin = hphoto->GetYaxis()->FindBin(ptedge[j+1]);
    TH1F* hpx = (TH1F*)hphoto->ProjectionX("hpx",lbin,hbin,centL,centH);
    TH1F* hpx_ul = (TH1F*)hphotoul->ProjectionX("hpx_ul",lbin,hbin,centL,centH);
    TH1F* hpxls = (TH1F*)hphotols->ProjectionX("hpx_ls",lbin,hbin,centL,centH);
    hpx_ls->SetLineColor(kBlue);
    // hpx_ul->SetLineColor();
    hpx->SetLineColor(kRed);

    hpx_ul->Draw();
    hpx_ls->Draw("same");
    hpx->Draw("same");
    TLegend* lpx = new TLegend(0.25,0.65,0.45,0.85);
    lpx->SetHeader("0-60%");
    lpx->AddEntry(hpx_ul,"UnLike","l");
    lpx->AddEntry(hpx_ls,"LikeSign","l");
    lpx->AddEntry(hpx,"UL-LS","l");
    lpx->Draw();
    TLatex lat;
    lat.SetTextSize(0.035);
    lat.DrawLatex(0.025,hpx_ul->GetMaximum()*0.6,Form("%.2f<p_{T}<%.2f GeV",ptedge[j],ptedge[j+1]));
    // hsp->SetBinContent(j+1,hpx->Integral()/hsp->GetBinWidth(j+1));
    hsp->SetBinContent(j+1,hpx->Integral()/nEvents);
    addpdf(pdf);
    }
    TH3F* hincEptCent= (TH3F*)f->Get("hIncEv2vsPtvsCent");
    // TH3F* hincEptCent= (TH3F*)f->Get("hIncEv2vsPtvsCent_hitcut");
    // hincEptCent->Draw("colz");
    // addpdf(pdf);
    TH1F* hincE = (TH1F*)hincEptCent->ProjectionY("hincEpt_MB", 1 , hincEptCent->GetNbinsX() , centL,centH);
    hincE = (TH1F*)hincE->Rebin(12,"hincE",ptedge);
    // hincE->Scale(1./hincE->GetBinWidth(1));
    hincE->Scale(1./nEvents);
    hincE->Draw();
    hsp->Draw("same");
    hincE->SetLineColor(kRed);
    hincE->GetYaxis()->SetTitle("N/N_{events}");
    hincE->GetXaxis()->SetRangeUser(1e3,3e9);
    TLegend* lince = new TLegend(0.65,0.65,0.85,0.85);
    lince->AddEntry(hincE,"pass cut inclusive e","l");
    lince->AddEntry(hsp,"reconstructed pho.c e","l");
    lince->Draw();
    gPad->SetLogy(1);
    addpdf(pdf);
    hsp->Divide(hincE);
    hsp->Draw();
    pdf->On();
    pdf->Close();
}
