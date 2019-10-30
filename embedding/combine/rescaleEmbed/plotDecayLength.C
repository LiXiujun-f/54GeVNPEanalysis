#include "myStyle.h"
void plotDecayLength()
{
  SetMyStyle();
  TFile* f100 = TFile::Open("fdecatLforPlots_1.root");
  TFile* f85 =  TFile::Open("fdecatLforPlots_0.85.root");
  TFile* f115 =  TFile::Open("fdecatLforPlots_1.15.root");
  // TFile* f85 =  TFile::Open("fdecatLforPlots_0.8.root");
  // TFile* f115 =  TFile::Open("fdecatLforPlots_1.2.root");
  // TFile* f85 =  TFile::Open("fdecatLforPlots_0.7.root");
  // TFile* f115 =  TFile::Open("fdecatLforPlots_1.3.root");

  TH1F* hdata = (TH1F*)f100->Get("hdata");
  hdata->SetDirectory(0);
  TH1F* hMC100 = (TH1F*)f100->Get("hrc")->Clone("hrc100");
  hMC100->SetDirectory(0);
  TH1F* hMC85 = (TH1F*)f85->Get("hrc")->Clone("hrc85");
  hMC85->SetDirectory(0);
  hMC85->SetMarkerColor(kRed);
  TH1F* hMC115 = (TH1F*)f115->Get("hrc")->Clone("hrc115");
  hMC115->SetDirectory(0);
  hMC115->SetMarkerColor(kBlue);
  f100->Close();
  f85->Close();
  f115->Close();

  // int const tmp  = hMC100->GetNbinsX();
  // int const nbins = tmp;
  // double x[nbins],y[nbins],errl[nbins],errh[nbins];
  // for (int i=1;i<=hMC100->GetNbinsX();i++) 
  // {
  //     x[i-1] = hMC100->GetBinCenter(i);
  //     y[i-1] = hMC100->GetBinContent(i);
  //     double e1 = hMC115->GetBinContent(i);
  //     double e2 = hMC85->GetBinContent(i);
  //     if (e1>e2) {  
  //       errh[i-1] = e1-y[i-1];
  //       errl[i-1] = y[i-1]-e2;
  //       // errh[i-1] = e1;
  //       // errl[i-1] = e2;
  //     }
  //     else {
  //       errl[i-1] = y[i-1]-e1;
  //       errh[i-1] = e2-y[i-1];
  //       // errl[i-1] = e1;
  //       // errh[i-1] = e2;
  //     }
  // }
  // TGraphAsymmErrors* gasymm = new TGraphAsymmErrors(nbins,x,y,0,0,errl,errh);
  for (int i=1;i<=hMC100->GetNbinsX();i++) 
  {
      // x[i-1] = hMC100->GetBinCenter(i);
      // y[i-1] = hMC100->GetBinContent(i);
      double y = hMC100->GetBinContent(i);
      double e1 = hMC115->GetBinContent(i);
      double e2 = hMC85->GetBinContent(i);
      double err1 = fabs(e1-y)>fabs(e2-y)?fabs(e1-y):fabs(e2-y);
      double err2 = hMC100->GetBinError(i);;
      hMC100->SetBinError(i,sqrt(err2*err2+err1*err1));
  }
   
  // hdata->SetMarkerStyle(24);
  hdata->SetMarkerSize(1);
  hdata->GetYaxis()->SetTitle("Arb. unit");
  hdata->GetXaxis()->SetTitle("Pair decay length [cm]");
  hdata->Draw("p");
  // gasymm->SetFillStyle(3004);
  // gasymm->SetFillColor(kRed);
  // gasymm->Draw("AsameF");
  // hMC85->Draw("same");

  hMC100->SetMarkerSize(0);
  hMC100->SetLineColor(kRed+2);

  hMC100->SetFillColor(kRed);
  hMC100->SetFillStyle(3009);
  hMC100->DrawCopy("sameE3");
  hMC100->DrawCopy("sameC");
  
  TLegend* leg = new TLegend(0.65,0.67,0.87,0.85);
  leg->AddEntry(hMC100,"MC","lef");
  leg->AddEntry(hdata,"Data","ep");
  leg->Draw();
  drawLatex(0.6,0.6,"0.2 GeV<p_{T}<2.5 p_{T}");
  drawSTAR(0.6,0.5);
  
  gPad->SaveAs("pdf/decayL.pdf");
  gPad->SaveAs("pdf/decayL.png");
}
void drawSTAR(double x,double y)
{
  TLatex lat;
  lat.SetTextSize(0.05);
  lat.SetTextFont(72);
  lat.SetTextColor(kRed);
  lat.DrawLatexNDC ( x, y, "STAR Preliminary");

}
