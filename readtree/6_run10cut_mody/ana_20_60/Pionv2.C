#include "Energy.h"
#include "sPhenixStyle.h"
void addpdf(TPDF* pdf)
{ 
  pdf->On();
  gPad->cd();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void Pionv2()
{
  SetsPhenixStyle(); 
  // TFile* file = new TFile("eta0.8/incEv2.root");
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
     double res2sub = prfres->GetBinContent(ic+1);
     res2sub = TMath::Sqrt(res2sub);
     double res2suberr = prfres->GetBinError(ic+1);
     res2suberr = 0.5/res2sub*res2suberr;
     rescheck->SetBinContent(9-ic,res2sub);
     resolution->SetBinContent(ic+1,res2sub);
     rescheck->SetBinError(9-ic,res2suberr);
     resolution->SetBinError(ic+1,res2suberr);
     res_xbincenter[ic]=rescheck->GetBinCenter(ic+1);
  }
  // resolution->Draw();

  // return;
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
  c->Close();
  // hadron 
  // TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncHadronv2");
  TProfile2D* pHad2D = (TProfile2D*)file->Get("pPionPlusv2");
  // TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncEv2");
  // for ()
  // {
  int centL=5,centH=7;  //start at 1
  // int centL=1,centH=4;  //start at 1
  TProfile* pHadv2 = (TProfile*)pHad2D->ProfileX("pHadv2",centL,centH);
  // TProfile* pHadv2cent[9];
  // for (int ic =centL;ic<=centH;ic++){
  //   pHadv2cent[ic-1] = (TProfile*)pHad2D->ProfileX(Form("pHadv2_%d",ic),ic,ic);
  //   pHadv2cent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  // }

  // TH1F* pHadv2 = (TH1F*)pHadv2cent[centH]->ProjectionX("hHadv2"); 
  // TProfile* pHadv2;
    // for (int ic=centL;ic<=centH;ic++)
    // {
    //   if (ic==centL) 
    //   {
    //    // cout<<"?"<<endl;
    //    pHadv2=(TProfile*)pHadv2cent[centL-1]->Clone("pHadv2");
    //   }
    //   else 
    //   {
    //    pHadv2->Add(pHadv2cent[ic-1],1);
    //   }
    //  }
     pHadv2->GetXaxis()->SetTitle("p_{T}");
     pHadv2->GetYaxis()->SetTitle("#pi v_{2}");
     

  // for (int ib = 1;ib<=pHadv2->GetNbinsX();ib++)
  // {
    // double totnum=0,totv2=0;
    // for (int ic=centL;ic<=centH;ic++)
    // {
    //   totnum+=pHadv2cent[ic]->GetBinEntries(ib);
    //   totv2+=pHadv2cent[ic]->GetBinContent(ib)*pHadv2cent[ic]->GetBinEntries(ib);
    // }
    //
    // if (totnum!=0) pHadv2->SetBinContent(ib,totv2/(double)totnum);
    // else pHadv2->SetBinContent(ib,0);
  // }
  // double resMB=0, totmult=0;
  // double refmult[9]={11,22.8,41.7,70.2,112,170,249,329,402};
  // for (int icent=centL;icent<=centH;icent++) {
  //   // resMB+= resolution->GetBinContent(icent)*refmult[icent-1];
  //   resMB+= prfres->GetBinContent(icent)*refmult[icent-1];
  //   totmult+=refmult[icent-1];
  // }
  // resMB/=(1.0*totmult);
  // prfres->Rebin(9);
  // double resMB = prfres->GetBinContent(1);
  // resMB = sqrt(resMB);
  // cout<<sqrt(resMB)<<endl;
  // pHadv2->Scale(0.97/sqrt(resMB));
  pHadv2->SetMarkerStyle(20);
  pHadv2->SetMarkerColor(kBlue);
  pHadv2->Draw("psame");
  // }
  TGraphErrors* hadv2ShW = new TGraphErrors(31,pt_54,v2_54,0,v2err_54);
  hadv2ShW->SetMarkerStyle(20);
  // hadv2ShW->Draw("samep");
  TFile* fpre = new TFile("prev2.root");
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("pionplus_0_10_39"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("pionplus_40_80_39"); 
  // TGraphErrors* g1 = (TGraphErrors*)fpre->Get("pionplus_0_80_39"); 
  TGraphErrors* g1 = (TGraphErrors*)fpre->Get("pionplus_10_40_39"); 
  g1->SetMarkerSize(1);
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("pionplus_0_10_62"); 
  TGraphErrors* g2 = (TGraphErrors*)fpre->Get("pionplus_10_40_62"); 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("pionplus_0_80_62"); 
  // TGraphErrors* g2 = (TGraphErrors*)fpre->Get("pionplus_40_80_62"); 
  g2->SetMarkerColor(kRed);
  g2->SetMarkerStyle(20);
  g1->SetMarkerStyle(20);
  g2->SetMarkerSize(1);
  // TGraphErrors* g3 = (TGraph*)fpre->Get("pionplus_0_10_27"); 
  TGraphErrors* g3 = (TGraph*)fpre->Get("pionplus_10_40_27"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("pionplus_0_80_27"); 
  // TGraphErrors* g3 = (TGraph*)fpre->Get("pionplus_40_80_27"); 
  g3->SetMarkerColor(kGreen);
  g3->SetMarkerSize(1);
  g3->SetMarkerStyle(20);
  g3->Draw("psame");
  g2->Draw("psame");
  g1->Draw("psame");
  TLegend* l = new TLegend(0.5,0.5,0.8,0.8);
  l->AddEntry(g1,"39GeV publish","pe");
  l->AddEntry(g2,"62GeV publish","pe");
  l->AddEntry(g3,"27GeV publish","pe");
  l->AddEntry(pHadv2,"54GeV mine","pe");
  l->Draw();
}
