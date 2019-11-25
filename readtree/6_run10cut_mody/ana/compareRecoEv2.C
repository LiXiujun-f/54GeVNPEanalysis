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
  TH1F*  hRecoEv2_2_8 = (TH1F*)file->Get("hRecoEv2_2_8");
  TH1F*  hPhoEv2_2_8= (TH1F*)file->Get("hPhoEv2_2_8");
  TF1* fRecoE_2_8 = (TF1*)file->Get("fRecoE_2_8");
  hPhoEv2_2_8->SetDirectory(0);

  double par[8];
  TF1* fscale = new TF1("fscale","pol5(0)*[6]",0,5);
  fRecoE_2_8->GetParameters(par);
  for (int i=0;i<6;i++)
    fscale->FixParameter(i,par[i]);
  hphoE->Fit(fscale,"B");
  cout << fscale->GetParameter(6)<<" "<<fscale->GetParError(6) <<endl;
  // double scalefactor=fscale->GetParameter(6);
  double scalefactor=0.963;

  hRecoEv2_2_8->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  hRecoEv2_2_8->GetYaxis()->SetTitle("v_{2}");
  hRecoEv2_2_8->SetDirectory(0);
  hRecoEv2_2_8->SetMarkerColor(kBlue);
  hRecoEv2_2_8->SetLineColor(kBlue);
  hRecoEv2_2_8->GetFunction("fRecoE_2_8")->Delete();
  hRecoEv2_2_8->Scale(scalefactor);

  hPhoEv2_2_8->GetFunction("fPhoE_2_8")->Delete();
  hPhoEv2_2_8->Scale(scalefactor);
   
  hphoE->Draw();
  hphoE->GetFunction("fscale")->Delete();
  hphoE->GetYaxis()->SetRangeUser(0,0.18);
  //reset the error bar

  for (int i=0;i<hPhoEv2_2_8->GetNbinsX();i++)
  {
     double mean = hPhoEv2_2_8->GetBinContent(i+1);
     hPhoEv2_2_8->SetBinError(i+1,mean*0.032);
     mean = hRecoEv2_2_8->GetBinContent(i+1);
     hRecoEv2_2_8->SetBinError(i+1,mean*0.032);
  }

  hRecoEv2_2_8->SetFillColorAlpha(kBlue,0.3);
  hRecoEv2_2_8->SetMarkerColor(kBlue);
  hRecoEv2_2_8->SetFillStyle(1001);
  hPhoEv2_2_8->SetFillColorAlpha(kRed,0.3);
  hPhoEv2_2_8->SetMarkerColor(kRed);
  hPhoEv2_2_8->SetLineColor(kRed);
  hPhoEv2_2_8->SetFillStyle(1001);
  hPhoEv2_2_8->Draw("sameE3");
  hRecoEv2_2_8->Draw("sameE3");
  // fRecoE_2_8->Draw("same");
  hphoE->Draw("psame");
  
  drawSTAR(0.2,0.85); 
  drawLatex(0.5,0.54,"Au+Au 54.4 GeV",0.055);
  drawLatex(0.55,0.46,"0-60%",0.055);

// return;
  TLegend* leg = new TLegend(0.45 ,0.2,0.88,0.45 );
  leg->AddEntry(hphoE,"Data: e^{pho} (w/ cut on partner)","lp");
  // leg->AddEntry(hRecoEv2_2_8,"embedd: reco. e v_{2} #times 0.95","lp");
  // leg->AddEntry(hPhoEv2_2_8,"embedd: total e v_{2} #times 0.95","lp");
  leg->AddEntry(hRecoEv2_2_8,"MC: e^{pho} (w/ cut on partner)","lpf");
  leg->AddEntry(hPhoEv2_2_8,"MC: e^{pho} (all)","lpf");

  leg->Draw();
  gPad->SaveAs("fig/phov2sys.pdf");
  gPad->SaveAs("fig/phov2sys.png");
  gPad->SaveAs("fig/phov2sys.eps");

  addpdf(pdf,c);
  TH1F* hratio = (TH1F*)hphoE->Clone("hratio");
  hratio->GetYaxis()->SetRangeUser( -0.05, 0.05);
  hratio->GetYaxis()->SetTitle("reco. e v_{2}: (embedd-data)/data");
  // hratio->Add(hRecoEv2_2_8 , -1);
  // hratio->Divide(hphoE);

  double RMS=0; 
  int num=0;
  for (int i=0;i<hphoE->GetNbinsX();i++)
  {
     double mean = hphoE->GetBinContent(i+1); 
     // double fitfun = fRecoE_2_8->Eval(hphoE->GetBinCenter(i+1))*scalefactor;
     // double fitfun = fRecoE_2_8->Eval(hphoE->GetBinCenter(i+1))*0.94;
     double fitfun = hRecoEv2_2_8->GetBinContent(i+1);
     double ratio = (fitfun-mean)/mean;
     hratio->SetBinContent(i+1,ratio);
     hratio->SetBinError(i+1,0);
     if (ratio>0.15) continue; 
     RMS+=ratio*ratio;
     num++;
  }

  RMS/=(1.*num);
  RMS=sqrt(RMS);
  cout <<"RMS:  " <<RMS << " scale factor="<<scalefactor<<endl;
  // hratio->Draw();
  
}
