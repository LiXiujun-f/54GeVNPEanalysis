#include "sPhenixStyle.h"
void addpdf(TPDF* pdf,TCanvas* c)
{
  c->cd();
  pdf->On();
  c->Update();
  pdf->NewPage();
  pdf->Off();
}

void NormHist(TH1* h1, int color);
void compare2PE_my()
{
  SetsPhenixStyle();
  TCanvas* c = new TCanvas("c","c");
  c->Divide(3,2);
  TPDF* pdf = new TPDF("qa.pdf");
  pdf->Off();
  // TString realdata = "incEv2_addqa_loose0825.root";
  // TString realdata = "incEv2_0903.root";
  TString realdata = "qa5.root";
  // TString mcdata = "embeddQa0825.root";
  TString mcdata = "embeddQa_test.root";
  drawQaNhits("NFit",pdf,c,"qa5.root",mcdata);
  
  // drawQaNhits("NFit",pdf,c,"qa4.root",mcdata);
  // drawQaDca("DCA",pdf,c,"qa2.root",mcdata);
  drawQaDca("DCA",pdf,c,"qa4.root",mcdata);
  drawPartPtEtaPhi("Partner e",pdf ,c ,realdata,mcdata); 
  drawPairDca("pair DCA",pdf,c,realdata,mcdata);
  drawDecayL("pair DecayLength",pdf,c,realdata,mcdata);

  //below are QA for the data
  drawDataQa("Tag e for real data",pdf,c,realdata,mcdata);
  drawTofMatchingEff("Tof match",pdf,c,realdata,mcdata,2,8);
 
  pdf->On();
  //  pdf->NewPage();
  pdf->Close(); 
}
//----------------------------------------------------------------------
void drawQaDca(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  if (head.Contains("NFit")) 
  {  
    TString name[3]={"hnHits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hPartEnFits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hNHit_3_0_10007_2","hNFitsvsPt","hNFitsvsPt_LS"};
  }
  else if (head.Contains("DCA"))
  { 
    // TString name[3]={"hDca_3_0_10007_2","hDcavsPt","hDcavsPt_LS"};
    TString name[3]={"hDCA","hDcavsPt_Gm","hDcavsPt_Gm_LS"};
    // TString name[3]={"hPartEdca","hDcavsPt_Gm","hDcavsPt_Gm_LS"};
    // TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
    // TString name[3]={"hPartEdca","hDcavsPt","hDcavsPt_LS"};
  }
  else cout<<"???"<<endl;
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  // file = TFile::Open("forQA.root");
  // file = TFile::Open("qa.root");
  file = TFile::Open(real.Data());
  // TH3F* hDCAdata = (TH3F*)file->Get(name[1]);
  // TH3F* hDCAdataLS = (TH3F*)file->Get(name[2]);
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->Draw();
  c->Divide(3,2);  
  int ipad=1;
  // for (int ip=1;ip<=20;ip++)
  // for (int ip=1;ip<=15;ip++)
  for (int ip=1;ip<=10;ip++)
  {
    // TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip*4+1)),hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip*4+4)));;
    // TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip*4+1,ip*4+4, hDCAdata->GetZaxis()->FindBin(-1),hDCAdata->GetZaxis()->FindBin(1) );
    //    TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip)),hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip)));;
    // TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip,ip);

    TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc",ip,ip);
    // TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinLowEdge(ip)),hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinUpEdge(ip)), hDCAdata->GetZaxis()->FindBin(-1),hDCAdata->GetZaxis()->FindBin(1) );
    // TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinLowEdge(ip)),hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinUpEdge(ip)), hDCAdata->GetZaxis()->FindBin(-1),hDCAdata->GetZaxis()->FindBin(1) );
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinLowEdge(ip)),hDCAdata->GetXaxis()->FindBin(hDCArc->GetYaxis()->GetBinUpEdge(ip)) );
    // cout << hDCAdata->GetXaxis()->GetBinLowEdge(ip*4+1)<<" "<< hDCAdata->GetXaxis()->GetBinUpEdge(ip*4+4) << endl;
    cout << hrc->GetXaxis()->GetBinLowEdge(ip)<<" "<< hrc->GetXaxis()->GetBinUpEdge(ip) << endl;
    
    // hrc->Rebin(2);
    hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(0.1),hrc->GetXaxis()->FindBin(0.5)));
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(0.1),hdata->GetXaxis()->FindBin(0.5)));
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    c->cd(ipad);
    hrc->DrawCopy();
    hdata->DrawCopy("same");
    // drawLatex(0.6,0.6,Form("%0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip*4+1),hDCAdata->GetXaxis()->GetBinUpEdge(ip*4+4)),0.035);
    drawLatex(0.6,0.6,Form("%0.1f<p_{T}<%0.1f",hDCArc->GetYaxis()->GetBinLowEdge(ip),hDCArc->GetYaxis()->GetBinUpEdge(ip),0.035));
    
    ipad++;
    
    if (ip%5==0)
    {
      c->cd(6);
      TLegend* leg = new TLegend(0.2,0.2,0.7,0.7);  
      leg->AddEntry(hrc,"MC e^{-}","lep");
      leg->AddEntry(hdata,"data e^{-}","lep");
      leg->SetHeader(head);
      leg->Draw();
      addpdf(pdf,c);
      ipad=1;
      c->Clear();
      c->Draw();
      c->Divide(3,2);
    }
  }
  
}
//----------------------------------------------------------------------
void drawQaNhits(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  if (head.Contains("NFit")) 
  {  
    TString name[3]={"hnHits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hPartEnFits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hNHit_3_0_10007_2","hNFitsvsPt","hNFitsvsPt_LS"};
  }
  else if (head.Contains("DCA"))
  { 
    // TString name[3]={"hDca_3_0_10007_2","hDcavsPt","hDcavsPt_LS"};
    // TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
    // TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
    TString name[3]={"hPartEdca","hDcavsPt","hDcavsPt_LS"};
  }
  else cout<<"???"<<endl;
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  // file = TFile::Open("forQA.root");
  // file = TFile::Open("qa.root");
  // hDCAdata = (TH2F*)file->Get(name[1]);
  // hDCAdataLS = (TH2F*)file->Get(name[2]);
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->Draw();
  c->Divide(3,2);  
  int ipad=1;
  // for (int ip=1;ip<=20;ip++)
  for (int ip=1;ip<=10;ip++)
  {
    // if (ip<3) continue;
    // TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc",hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip*4+1)),hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip*4+4)), hDCArc->GetZaxis()->FindBin(-0.8),hDCArc->GetZaxis()->FindBin(0.8) );;
    // TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip)),hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip)));;
    TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc",ip,ip);
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip,ip);
    // cout << hDCAdata->GetXaxis()->GetBinLowEdge(ip)<<" "<< hDCAdata->GetXaxis()->GetBinUpEdge(ip) << endl;

    hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(21),hrc->GetXaxis()->FindBin(45)));
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(21),hdata->GetXaxis()->FindBin(45)));
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    c->cd(ipad);
    hrc->DrawCopy();
    hdata->DrawCopy("same");
    // drawLatex(0.2,0.6,Form("%0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip),hDCAdata->GetXaxis()->GetBinUpEdge(ip)),0.035);
    drawLatex(0.2,0.6,Form("%0.1f<p_{T}<%0.1f",hDCArc->GetYaxis()->GetBinLowEdge(ip),hDCArc->GetYaxis()->GetBinUpEdge(ip)),0.035);
    
    ipad++;
    
    if (ip%5==0)
    {
      c->cd(6);
      TLegend* leg = new TLegend(0.2,0.2,0.7,0.7);  
      leg->AddEntry(hrc,"MC e^{-}","lep");
      leg->AddEntry(hdata,"data e^{-}","lep");
      leg->SetHeader(head);
      leg->Draw();
      addpdf(pdf,c);
      ipad=1;
      c->Clear();
      c->Draw();
      c->Divide(3,2);
    }
  }
}
//----------------------------------------------------------------------
void drawPartPtEtaPhi(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  // TString name[3]={"hPartEptetaphi","hPartEptetaphi","hPartEptetaphi_LS"};
  TString name[3]={"hPartEptetaphi","hPartEptetaphi_Gm","hPartEptetaphi_Gm_LS"};
  // TString name[3]={"hPartEptetaphi","hPartEptetaphi_Dz","hPartEptetaphi_Dz_LS"};
  TH3F* hDCArc = (TH3F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH3F* hDCAdata = (TH3F*)file->Get(name[1]);
  TH3F* hDCAdataLS = (TH3F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  TH1* hrc_x = (TH1*)hDCArc->ProjectionX("hrc_x");
  TH1* hdata_x = (TH1*)hDCAdata->ProjectionX("hdata_x");
  NormHist(hrc_x,kBlue);
  NormHist(hdata_x,kRed);
  hrc_x->DrawCopy();
  hdata_x->DrawCopy("same");
  drawLatex(0.6,0.6,Form("%s p_{T}", head.Data()));
  addpdf(pdf,c);

  c->Clear();
  c->Divide(2,1);
  c->cd(1);
  TH1* hrc_y = (TH1*)hDCArc->ProjectionY("hrc_y");
  TH1* hdata_y = (TH1*)hDCAdata->ProjectionY("hdata_y");
  NormHist(hrc_y,kBlue);
  NormHist(hdata_y,kRed);
  hrc_y->DrawCopy();
  hdata_y->DrawCopy("same");
  drawLatex(0.2,0.6,Form("%s Eta", head.Data()));

  c->cd(2);
  TH1* hrc_z = (TH1*)hDCArc->ProjectionZ("hrc_z");
  TH1* hdata_z = (TH1*)hDCAdata->ProjectionZ("hdata_z");
  hrc_z->Rebin(4);
  NormHist(hrc_z,kBlue);
  NormHist(hdata_z,kRed);
  hrc_z->DrawCopy();
  hdata_z->DrawCopy("same");
  drawLatex(0.2,0.6,Form("%s #phi", head.Data()));
  addpdf(pdf,c);
}
//----------------------------------------------------------------------
void drawPairDca(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hDcaPair","hPairDCA","hPairDCALS"};
  TH3F* hDCArc = (TH3F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc");;
  TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata");
  // hrc->Rebin();
  // hdata->Rebin(); 
  hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(0),hrc->GetXaxis()->FindBin(0.3)));
  hrc->Scale(1./hrc->GetBinWidth(1));
  hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(0),hdata->GetXaxis()->FindBin(0.3)));
  hdata->Scale(1./hdata->GetBinWidth(1));
  hrc->SetMarkerColor(kBlue);
  hrc->SetLineColor(kBlue);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);

  hrc->DrawCopy();
  hdata->DrawCopy("same");
  drawLatex(0.6,0.6,"pair DCA",0.035);
  addpdf(pdf,c); 
}
//----------------------------------------------------------------------
void drawDecayL(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hPairDecayL","hDecayL","hDecayL_LS"};
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc");;
  TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata");

  hrc->Rebin(5);
  hdata->Rebin(); 
  hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(3.5),hrc->GetXaxis()->FindBin(30)));
  hrc->Scale(1./hrc->GetBinWidth(1));
  hdata->Scale(1./hdata->Integral());
  hdata->Scale(1./hdata->GetBinWidth(1));
  hrc->SetMarkerColor(kBlue);
  hrc->SetLineColor(kBlue);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);

  hrc->DrawCopy();
  hdata->DrawCopy("same");

  TLegend* leg = new TLegend(0.6,0.7,0.85,0.85);
  leg->AddEntry(hdata, "data","l");
  leg->AddEntry(hrc, "embedding","l");
  leg->Draw();
  drawLatex(0.6,0.6,Form("%s",head.Data()),0.035);
  addpdf(pdf,c); 
}
//----------------------------------------------------------------------
void drawDataQa(TString head,TPDF* pdf,TCanvas* c, TString real, TString mc)
{
  TFile* f = TFile::Open(real.Data()); 
  TH1F* hPhi = (TH1F*)f->Get("hPhi");
  TH1F* hPhi_hitcut = (TH1F*)f->Get("hPhi_hitcut");
  TH1F* hPhi_allcut = (TH1F*)f->Get("hPhi_allcut");
  hPhi_allcut->SetLineColor(kBlue);
  hPhi_hitcut->SetLineColor(kRed); 
  c->Clear();
  hPhi->GetYaxis()->SetRangeUser(0 ,hPhi->GetMaximum()*1.1 );
  hPhi->Draw();
  hPhi_hitcut->Draw("same");
  hPhi_allcut->Draw("same");
  TLegend* leg = new TLegend(0.5,0.7,0.88,0.92);
  leg->AddEntry(hPhi , "basic cuts","l" );
  leg->AddEntry(hPhi_hitcut , "first hit <70cm","l" );
  leg->AddEntry(hPhi_allcut, "rm 1.25<|#phi|<1.75 && first hit <70cm","l" );
  leg->Draw();
  drawLatex(0.5,0.6,"Tag e #phi distribution (data)"); 
  addpdf(pdf,c);
 
  c->Clear();
  TH1F* hEta = (TH1F*)f->Get("hEta"); 
  hEta->Draw();
  drawLatex(0.6,0.6, "Tag e #eta (data)");
  addpdf(pdf,c);
  f->Close();
  c->Clear();
} 
//----------------------------------------------------------------------
void drawTofMatchingEff(TString head,TPDF* pdf,TCanvas* c, TString real, TString mc,int centL,int centH)
{
  TFile* f = TFile::Open(real.Data()); 
  TH2F* hTPC= (TH2F*)f->Get("hPartETpc");
  TH2F* hTof= (TH2F*)f->Get("hPartETof");
  TH2F* hTofls= (TH2F*)f->Get("hPartETofLS");
  TH2F* hTPCls= (TH2F*)f->Get("hPartETpcLS");
  hTPC->Add(hTPCls,-1);
  hTof->Add(hTofls,-1);
  TH1F* hTPC_pt = (TH1F*)hTPC->ProjectionX(Form("TPC_%d_%d",centL,centH), hTPC->GetYaxis()->FindBin(centL),hTPC->GetYaxis()->FindBin(centH));
  TH1F* hTof_pt = (TH1F*)hTof->ProjectionX(Form("Tof_%d_%d",centL,centH), hTPC->GetYaxis()->FindBin(centL),hTPC->GetYaxis()->FindBin(centH));
  c->Clear();
  hTof_pt->Divide(hTPC_pt);
  // hTof_pt->SetDirectory(0);
  hTof_pt->Draw();
  drawLatex(0.5,0.75,"Tof matching Eff."); 
  drawLatex(0.5,0.85,"Use partner electron"); 
  addpdf(pdf,c);
  hTof_pt->SaveAs(Form("Tof_%d_%d.root",centL,centH));
 
  f->Close();
  c->Clear();
}
//----------------------------------------------------------------------
void NormHist(TH1* h1, int color)
{
   h1->Scale(1./h1->Integral()); 
   h1->Scale(1./h1->GetBinWidth(1)); 
   h1->SetMarkerColor(color);
   h1->SetLineColor(color); 
}

