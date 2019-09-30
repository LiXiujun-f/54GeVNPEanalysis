#include "sPhenixStyle.h"
void compareRecoEv2()
{
  SetsPhenixStyle();
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  // will add this part to  HFev2
  TFile* file = new TFile("out.root");
  TH1F* hphoE = (TH1F*)file->Get("hphoE"); 
  hphoE->SetMarkerColor(kGreen+2);
  hphoE->SetLineColor(kGreen+2);
  hphoE->SetDirectory(0);
  hphoE->GetXaxis()->SetTitle("p_{T}");
  file->Close();
  // file = new TFile("RecoEff0825.root"); 
  // file = new TFile("RecoEff_0910.root"); 
  // file = new TFile("RecoEff_0924.root"); 
  file = new TFile("RecoEff_comb0929.root"); 
  TH1F*  hRecoEv2_2_8 = (TH1F*)file->Get("hRecoEv2_2_8");
  hRecoEv2_2_8->SetDirectory(0);
  hRecoEv2_2_8->SetMarkerColor(kBlue);
  hRecoEv2_2_8->SetLineColor(kBlue);
  hRecoEv2_2_8->Scale(0.95);

  // TF1* fRecoE_2_8 = (TF1*)file->Get("fRecoE_2_8");
  TH1F*  hPhoEv2_2_8= (TH1F*)file->Get("hPhoEv2_2_8");
  hPhoEv2_2_8->SetDirectory(0);
  hPhoEv2_2_8->Scale(0.95);
   
  hphoE->Draw();
  hphoE->GetYaxis()->SetRangeUser(0,0.2);
  hRecoEv2_2_8->Draw("psame");
  // fRecoE_2_8->Draw("same");
  hPhoEv2_2_8->Draw("psame");
  hphoE->Draw("psame");
// return;
  TLegend* leg = new TLegend(0.5 ,0.2 ,0.8 ,0.5 );
  leg->AddEntry(hphoE,"data: reco. e v_{2}","lp");
  leg->AddEntry(hRecoEv2_2_8,"embedd: reco. e v_{2} #times 0.95","lp");
  leg->AddEntry(hPhoEv2_2_8,"embedd: total e v_{2} #times 0.95","lp");
  leg->Draw();

  TH1F* hratio = (TH1F*)hphoE->Clone("hratio");
  hratio->GetYaxis()->SetRangeUser( -0.05, 0.05);
  hratio->GetYaxis()->SetTitle("reco. e v_{2}: (embedd-data)/data");
  // hratio->Add(hRecoEv2_2_8 , -1);
  // hratio->Divide(hphoE);
  for (int i=0;i<hphoE->GetNbinsX();i++)
  {
     double mean = hphoE->GetBinContent(i+1); 
     double fitfun = fRecoE_2_8->Eval(hphoE->GetBinCenter(i+1))*0.95;
     hratio->SetBinContent(i+1,(fitfun-mean)/mean);
     hratio->SetBinError(i+1,0);
  }
  // hratio->Draw();
}
