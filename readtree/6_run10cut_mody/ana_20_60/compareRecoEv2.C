#include "sPhenixStyle.h"
void addpdf(TPDF* pdf,TCanvas* c)
{
  pdf->On();
  c->cd();
  pdf->NewPage();
  c->Update();
  pdf->Off();

}
void compareRecoEv2()
{
  SetsPhenixStyle();
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("fig/comparePhov2.pdf");
  pdf->Off();
  // will add this part to  HFev2
  TFile* file = new TFile("out.root");
  TH1F* hphoE = (TH1F*)file->Get("hphoE"); 
  hphoE->SetMarkerColor(kGreen+2);
  hphoE->SetLineColor(kGreen+2);
  hphoE->SetDirectory(0);
  hphoE->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hphoE->GetYaxis()->SetTitle("v_{2}");
  file->Close();
  // file = new TFile("RecoEff0825.root"); 
  // file = new TFile("RecoEff_0910.root"); 
  // file = new TFile("RecoEff_0924.root"); 
  // file = new TFile("RecoEff_comb0929.root"); 
  file = new TFile("PhoEff_comb.root"); 
  TH1F*  hRecoEv2_2_8 = (TH1F*)file->Get("hRecoEv2_2_5");
  TH1F*  hPhoEv2_2_8= (TH1F*)file->Get("hPhoEv2_2_5");
  TF1* fRecoE_2_8 = (TF1*)file->Get("fRecoE_2_5");
  hPhoEv2_2_8->SetDirectory(0);

  double par[8];
  TF1* fscale = new TF1("fscale","pol5(0)*[6]",0,5);
  fRecoE_2_8->GetParameters(par);
  for (int i=0;i<6;i++)
    fscale->FixParameter(i,par[i]);
  hphoE->Fit(fscale,"B");
  cout << fscale->GetParameter(6)<<" "<<fscale->GetParError(6) <<endl;
  // double scalefactor=fscale->GetParameter(6);
  double scalefactor=0.95;

  hRecoEv2_2_8->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hRecoEv2_2_8->GetYaxis()->SetTitle("v_{2}");
  hRecoEv2_2_8->SetDirectory(0);
  hRecoEv2_2_8->SetMarkerColor(kBlue);
  hRecoEv2_2_8->SetLineColor(kBlue);
  hRecoEv2_2_8->GetFunction("fRecoE_2_5")->Delete();
  hRecoEv2_2_8->Scale(scalefactor);

  hPhoEv2_2_8->GetFunction("fPhoE_2_5")->Delete();
  hPhoEv2_2_8->Scale(scalefactor);
   
  hphoE->Draw();
  hphoE->GetYaxis()->SetRangeUser(0,0.2);
  hRecoEv2_2_8->Draw("psame");
  // fRecoE_2_8->Draw("same");
  hPhoEv2_2_8->Draw("psame");
  hphoE->Draw("psame");
// return;
  TLegend* leg = new TLegend(0.5 ,0.2 ,0.8 ,0.5 );
  leg->AddEntry(hphoE,"data: reco. e v_{2}","lp");
  // leg->AddEntry(hRecoEv2_2_8,"embedd: reco. e v_{2} #times 0.95","lp");
  // leg->AddEntry(hPhoEv2_2_8,"embedd: total e v_{2} #times 0.95","lp");
  leg->AddEntry(hRecoEv2_2_8,"embedd: reco. e v_{2}","lp");
  leg->AddEntry(hPhoEv2_2_8,"embedd: total e v_{2}","lp");

  leg->Draw();
  addpdf(pdf,c);
  TH1F* hratio = (TH1F*)hphoE->Clone("hratio");
  hratio->GetYaxis()->SetRangeUser( -0.05, 0.05);
  hratio->GetYaxis()->SetTitle("reco. e v_{2}: (embedd-data)/data");
  // hratio->Add(hRecoEv2_2_8 , -1);
  // hratio->Divide(hphoE);

  for (int i=0;i<hphoE->GetNbinsX();i++)
  {
     double mean = hphoE->GetBinContent(i+1); 
     // double fitfun = fRecoE_2_8->Eval(hphoE->GetBinCenter(i+1))*scalefactor;
     // double fitfun = fRecoE_2_8->Eval(hphoE->GetBinCenter(i+1))*0.94;
     double fitfun = hRecoEv2_2_8->GetBinContent(i+1);
     hratio->SetBinContent(i+1,(fitfun-mean)/mean);
     hratio->SetBinError(i+1,0);
  }
  // hratio->Draw();
  
}
