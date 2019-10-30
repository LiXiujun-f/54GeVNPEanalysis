#include "myStyle.h"
void setHistStyle(TH1* hist,int color,int style)
{
    hist->SetLineColor(color);
    hist->SetMarkerColor(color);
    hist->SetMarkerStyle(style);
}

void plotEffHist(TString histname,TString filename,TCanvas* c,TLegend* leg,int color,TString legname)
{
   TFile* file = TFile::Open(filename.Data());
   TH1* h = (TH1*)file->Get(histname.Data())->Clone(Form("h%s",histname.Data()));
   c->cd();
   h->GetFunction(histname.Replace(0,1,'f').Data())->Delete(); //rm the fit
   h->SetLineColor(color);
   h->SetMarkerColor(color);
   h->DrawCopy("same");
   h->SetDirectory(0);
   filename.ReplaceAll( ".root","");
   filename.ReplaceAll( "output/","");
   filename.ReplaceAll( "PhoEff_","");

   leg->AddEntry(h,legname.Data(),"lep");
   file->Close();
}

void plotComponent(TString inputfileDir,TString Stotal,TString Spi0,TString Seta,TString Sgamma2pi0,TString Sgamma2eta,TString Sdirpho)
{
   SetMyStyle();
   TCanvas* c = new TCanvas("c","c");
   c->cd();
   int const totalNum=6;
   int color[6] = {1,2,3,7,9,6};
   TString component[]={Spi0,Seta,Sgamma2pi0,Sgamma2eta,Sdirpho};
   TString LegComp[]={"#pi^{0}#rightarrow#gamma ee","#eta#rightarrow#gamma ee","#pi^{0}#rightarrow2#gamma","#eta#rightarrow2#gamma","direct photon"};
   TFile* filetot = TFile::Open(Form("%s%s.root",inputfileDir.Data(),Stotal.Data()));
   TH1F* hPhoEff = (TH1F*)filetot->Get("hRecoEff_2_8");
   setHistStyle(hPhoEff,color[0],20);
   hPhoEff->GetFunction("fRecoEff_2_8")->Delete(); //rm the fit
   hPhoEff->GetXaxis()->SetTitle("p_{T} [GeV/c]");
   hPhoEff->GetYaxis()->SetTitle("Reco. Eff");
   hPhoEff->Draw();
   hPhoEff->GetXaxis()->SetRangeUser(0,3);
  
   TLegend* leg = new TLegend(0.2,0.6,0.5,0.88); 
   leg->AddEntry(hPhoEff,"total","lep");

   for (int i=1;i<totalNum;i++)
   {
     if (i==1) continue;
     if (i==2) continue;

      plotEffHist("hRecoEff_2_8",Form("%s%s.root",inputfileDir.Data(),component[i-1].Data()),c,leg,color[i],LegComp[i-1]); 
   }
   leg->Draw();
  
   hPhoEff->DrawCopy("same");
   c->SaveAs("phoeff.pdf");
}
