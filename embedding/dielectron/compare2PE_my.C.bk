#include "sPhenixStyle.h"
void addpdf(TPDF* pdf,TCanvas* c)
{
  c->cd();
  pdf->On();
  c->Update();
  pdf->NewPage();
  pdf->Off();
}

void compare2PE_my()
{
  SetsPhenixStyle();
  TCanvas* c = new TCanvas("c","c");
  c->Divide(3,2);
  TPDF* pdf = new TPDF("qa.pdf");
  pdf->Off();
  drawQa("NFit",pdf,c);
  drawQa("DCA",pdf,c);
  pdf->On();
  //  pdf->NewPage();
  pdf->Close(); 

}

void drawQa(TString head, TPDF* pdf, TCanvas* c)
{
  TFile* file = TFile::Open("embeddingQa.root");
  if (head.Contains("NFit")) 
  {  
    TString name[3]={"hnHits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hNHit_3_0_10007_2","hNFitsvsPt","hNFitsvsPt_LS"};
  }
  else if (head.Contains("DCA"))
  { 
    // TString name[3]={"hDca_3_0_10007_2","hDcavsPt","hDcavsPt_LS"};
    TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
  }
  else cout<<"???"<<endl;
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open("forQA.root");
  hDCAdata = (TH2F*)file->Get(name[1]);
  hDCAdataLS = (TH2F*)file->Get(name[2]);
  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->Draw();
  c->Divide(3,2);  
  int ipad=1;
  for (int ip=1;ip<=10;ip++)
  {
    TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc",hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip)),hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip)));;
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip,ip);

    hrc->Scale(1./hrc->Integral());
    hdata->Scale(1./hdata->Integral());
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    c->cd(ipad);
    hrc->DrawCopy();
    hdata->DrawCopy("same");
    drawLatex(0.2,0.6,Form("%0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip),hDCAdata->GetXaxis()->GetBinUpEdge(ip)),0.035);
    
    ipad++;
    
    if (ip==5 || ip == 10)
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
