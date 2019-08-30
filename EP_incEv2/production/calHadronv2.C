#include "Energy.h"
void addpdf(TPDF* pdf)
{ 
  pdf->On();
  gPad->cd();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
TGraph* divideFit(TGraphErrors* g1, TF1* fit,TString s)
{
  TGraph* gratio;
  int const tmp = g1->GetN();
  cout<<tmp<<endl;
  int const nPoint = tmp;
  double x[nPoint],y[nPoint];
  for (int ip = 0;ip<nPoint;ip++)
  {
    g1->GetPoint(ip,x[ip],y[ip]);
    cout <<x[ip]<<" "<<y[ip] <<endl;
    y[ip] /= fit->Eval(x[ip]);
  }
  gratio = new TGraph(nPoint,x,y);
  gratio->SetName(s);
  gratio->SetMarkerStyle(20);
  return gratio;

}
void calHadronv2()
{
  TFile* file = new TFile("incEv2.root");
  // TFile* file = new TFile("getshift.root");
  // TFile* file = new TFile("incEv2_0.root");
  TCanvas* c1 = new TCanvas("c1","c1");
  TCanvas* c = new TCanvas("c","c",600,1200);
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
  // resolution->Draw();
  c1->cd();
  TGraphErrors* gShWres = new TGraphErrors(9 ,res_xbincenter ,val_54GeV,0,err_54GeV );
  TGraphErrors* gShWres39 = new TGraphErrors(9 ,res_xbincenter ,val_39GeV,0,err_39GeV );
  TGraphErrors* gShWres62 = new TGraphErrors(9 ,res_xbincenter ,val_62GeV,0,err_62GeV );
  gShWres->SetMarkerStyle(20);
  gShWres->SetMarkerColor(kGreen);
  gShWres62->SetMarkerStyle(21);
  gShWres62->SetMarkerColor(kBlue);
  gShWres39->SetMarkerStyle(22);
  gShWres39->SetMarkerColor(kRed);
  rescheck->GetYaxis()->SetRangeUser(0,0.8);
  rescheck->Draw("p");
  gShWres->Draw("psame");
  gShWres62->Draw("psame");
  gShWres39->Draw("psame");
  gPad->SaveAs("res.png");
  // addpdf(pdf); 
  c1->Close();
  c->cd();
  c->Divide(1,2);
  // return;
  // resolution->SetDirectory(0);
  // file->Close();
  // file = TFile::Open("incEv2.root");
  // hadron 
  TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncHadronv2");
  // TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncEv2");
  // for ()
  // {
  int centL=3,centH=6;  //start at 1
  // int centL=3,centH=3;  //start at 1
  // TProfile* pHadv2 = (TProfile*)pHad2D->ProfileX("pHadv2",centL,centH);
  TProfile* pHadv2cent[9];
  for (int ic =centL;ic<=centH;ic++){
    pHadv2cent[ic-1] = (TProfile*)pHad2D->ProfileX(Form("pHadv2_%d",ic),ic,ic);
    pHadv2cent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  }

  // TH1F* pHadv2 = (TH1F*)pHadv2cent[centH]->ProjectionX("hHadv2"); 
  TProfile* pHadv2;
  for (int ic=centL;ic<=centH;ic++)
  {
    if (ic==centL) 
    {
     // cout<<"?"<<endl;
     pHadv2=(TProfile*)pHadv2cent[centL-1]->Clone("pHadv2");
    }
    else 
    {
     pHadv2->Add(pHadv2cent[ic-1],1);
    }
  }
  // double resMB=0, totmult=0;
  // double refmult[9]={11,22.8,41.7,70.2,112,170,249,329,402};
  // for (int icent=centL;icent<=centH;icent++) {
  //   resMB+= resolution->GetBinContent(icent)*refmult[centH];
  //   totmult+=refmult[centL];
  // }
  // resMB/=(1.0*totmult);
  // prfres->Rebin(9);
  // double resMB = prfres->GetBinContent(1);
  // resMB = sqrt(resMB);
  // cout<<resMB<<endl;
  // pHadv2->Scale(1./(resMB));
  pHadv2->SetMarkerStyle(20);
  pHadv2->SetMarkerColor(kBlue);
  c->cd(1);
  pHadv2->Draw("psame");
  c->cd(2);
  TH1F* hdraw = new TH1F("hdraw","hdraw",10,0,5);
  hdraw->GetYaxis()->SetRangeUser(0.9,1.1);
  hdraw->Draw();
  c->cd(1);
  TF1* fit = new TF1("fit","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x",0,5);
  pHadv2->Fit(fit);
  // }
  TGraphErrors* hadv2ShW = new TGraphErrors(31,pt_54,v2_54,0,v2err_54);
  hadv2ShW->SetMarkerStyle(20);
  // hadv2ShW->Draw("samep");
  TFile* fpre = new TFile("prev2.root");
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("10_20"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("10_20_sys"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("30_40_sys"); 
  int cent[10] = {80,70,60,50,40,30,20,10,5,0}; 
  TGraphErrors* g1 = (TGraphErrors*)fpre->Get(Form("%d_%d_sys",cent[centH] ,cent[centL-1] )); 
  c->cd(2);
 if (g1)  // g1 = (TGraphErrors*)fpre->Get("50_60_sys");
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("40_50_sys"); 
 {
  g1->SetMarkerSize(1);
  g1->SetMarkerStyle(20);
  c->cd(1);
  g1->Draw("psame");
  c->cd(2);
  TGraph* g1ratio = divideFit(g1,fit,"39GeVv2");
  g1ratio->SetMarkerColor(kBlack);
  g1ratio->Draw("psame");
 } 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("30_40_sys_62"); 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("40_50_sys_62"); 

  TGraphErrors* g2 = (TGraphErrors*)fpre->Get(Form("%d_%d_sys_62",cent[centH] ,cent[centL-1] )); 
  if (g2)   //g2 = (TGraphErrors*)fpre->Get("10_20_sys_62"); 
  {
    g2->SetMarkerColor(kRed);
    g2->SetMarkerStyle(20);
    g2->SetMarkerSize(1);
    c->cd(1);
    g2->Draw("psame");
    c->cd(2);
    TGraph* g2ratio = divideFit(g2,fit,"62GeVv2");
    g2ratio->SetMarkerColor(kRed);
    g2ratio->Draw("psame");
  }
  // TGraphErrors* g3 = (TGraph*)fpre->Get("30_40_star62"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("30_40_star27"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("10_20_star27"); 
  TGraphErrors* g3 = (TGraph*)fpre->Get(Form("%d_%d_star27",cent[centH] ,cent[centL-1])); 
  if (g3) //  g3 = (TGraph*)fpre->Get("50_60_star27"); 
 {
  g3->SetMarkerColor(kGreen);
  g3->SetMarkerSize(1);
  g3->SetMarkerStyle(20);
  c->cd(1);
  g3->Draw("psame");
  c->cd(2);
  TGraph* g3ratio =  divideFit(g3,fit,"27GeVv2");
  g3ratio->SetMarkerColor(kGreen);
  g3ratio->Draw("psame");
 }

  c->cd(2);
  TLine* l = new TLine(0,1,5,1);
  l->SetLineStyle(9);
  l->SetLineColor(kBlue);
  l->Draw("same");
  // TLegend* l = new TLegend(0.6,0.6,0.8,0.8);
  // l->AddEntry();
}


