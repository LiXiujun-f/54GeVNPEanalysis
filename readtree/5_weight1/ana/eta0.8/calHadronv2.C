#include "Energy.h"
void addpdf(TPDF* pdf)
{ 
  pdf->On();
  gPad->cd();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void calHadronv2()
{
  // TFile* file = new TFile("incEv2.root");
  TFile* file = new TFile("incEv2_0.root");
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf"); 
  pdf->Off();
  //resolution
  TProfile* prfres= (TProfile*)file->Get("EPRes"); 
  double res_x[10] = {0,5,10,20,30,40,50,60,70,80};
  double res_xbincenter[9];
  TH1F* rescheck = new TH1F("rescheck","res sqrt(deltasub)",9,res_x);
  TH1F* resolution = new TH1F("res","res sqrt(deltasub)",9,-0.5,8.5);
  for (int ic=0;ic<9;ic++)
  {
     double res2sub = prfres->GetBinContent(prfres->FindBin(ic));
     res2sub = sqrt(res2sub);
     double res2suberr = prfres->GetBinError(prfres->FindBin(ic));
     res2suberr = 0.5/res2sub*res2suberr;
     rescheck->SetBinContent(9-ic,res2sub);
     resolution->SetBinContent(ic+1,res2sub);
     rescheck->SetBinError(9-ic,res2suberr);
     resolution->SetBinError(ic+1,res2suberr);
     res_xbincenter[ic]=rescheck->GetBinCenter(ic+1);
  }
  resolution->Draw();
  // return;
  TGraphErrors* gShWres = new TGraphErrors(9 ,res_xbincenter ,val_54GeV,0,err_54GeV );
  TGraphErrors* gShWres39 = new TGraphErrors(9 ,res_xbincenter ,val_39GeV,0,err_39GeV );
  TGraphErrors* gShWres62 = new TGraphErrors(9 ,res_xbincenter ,val_62GeV,0,err_62GeV );
  gShWres->SetMarkerStyle(20);
  gShWres62->SetMarkerStyle(21);
  gShWres39->SetMarkerStyle(22);
  rescheck->Draw();
  gShWres->Draw("psame");
  gShWres62->Draw("psame");
  gShWres39->Draw("psame");
  addpdf(pdf); 
  c->Close();
  // hadron 
  TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncHadronv2");
  // TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncEv2");
  // for ()
  // {
  TProfile* pHadv2 = (TProfile*)pHad2D->ProfileX("pHadv2",5,5);
  double resMB = resolution->GetBinContent(5);
  // prfres->Rebin(9);
  // double resMB = prfres->GetBinContent(1);
  // resMB = sqrt(resMB);
  cout<<resMB<<endl;
  pHadv2->Scale(1./(resMB));
  pHadv2->SetMarkerStyle(20);
  pHadv2->SetMarkerColor(kBlue);
  pHadv2->Draw("psame");
  // }
  TGraphErrors* hadv2ShW = new TGraphErrors(31,pt_54,v2_54,0,v2err_54);
  hadv2ShW->SetMarkerStyle(20);
  // hadv2ShW->Draw("samep");
  TFile* fpre = new TFile("prev2.root");
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("10_20"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("10_20_sys"); 
  TGraphErrors* g1 = (TGraphErrors*)fpre->Get("30_40_sys"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("50_60_sys"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("40_50_sys"); 
  g1->SetMarkerSize(1);
  TGraphErrors* g2 = (TGraphErrors*)fpre->Get("30_40_sys_62"); 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("40_50_sys_62"); 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("10_20_sys_62"); 
  g2->SetMarkerColor(kRed);
  g2->SetMarkerStyle(20);
  g1->SetMarkerStyle(20);
  g2->SetMarkerSize(1);
  // TGraphErrors* g3 = (TGraph*)fpre->Get("30_40_star62"); 
  TGraphErrors* g3 = (TGraph*)fpre->Get("30_40_star27"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("10_20_star27"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("50_60_star27"); 
  g3->SetMarkerColor(kGreen);
  g3->SetMarkerSize(1);
  g3->SetMarkerStyle(20);
  g1->Draw("psame");
  g2->Draw("psame");
  g3->Draw("psame");
}
