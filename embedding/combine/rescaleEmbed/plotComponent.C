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
   h->SetMarkerStyle(24);
   h->DrawCopy("same");
   h->SetDirectory(0);
   filename.ReplaceAll( ".root","");
   filename.ReplaceAll( "output/","");
   filename.ReplaceAll( "PhoEff_","");

   leg->AddEntry(h,legname.Data(),"lep");
   file->Close();
}

void plotComponent(TString inputfileDir,TString Stotal,TString Spi0,TString Seta,TString Sgamma2pi0,TString Sgamma2eta,TString Sdirpho,int mode)
{
  if (mode==0)
    plotMode0(inputfileDir,Stotal,Spi0, Seta, Sgamma2pi0, Sgamma2eta, Sdirpho);
  else if (mode==1)
    plotMode1(inputfileDir,Stotal,Spi0, Seta, Sgamma2pi0); //in mode 1 put the gamma file name at gamma2pi0 position
  else cout<<"please inpout the right mode" <<endl; 
  
}
void plotMode0(TString inputfileDir,TString Stotal,TString Spi0,TString Seta,TString Sgamma2pi0,TString Sgamma2eta,TString Sdirpho)
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
void plotMode1(TString inputfileDir,TString Stotal,TString Spi0,TString Seta,TString Sgamma)
{
   SetMyStyle();
   TCanvas* c = new TCanvas("c","c");
   c->cd();
   int const totalNum=4;
   int color[6] = {kBlack,kRed,kGreen+2,kBlue,9,6};
   TString component[]={Spi0,Seta,Sgamma};
   TString LegComp[]={"#pi^{0}#rightarrowe from Dalitz decay","#eta#rightarrowe from Dalitz decay","Photon conversion"};
   TFile* filetot = TFile::Open(Form("%s%s.root",inputfileDir.Data(),Stotal.Data()));
   TH1F* hPhoEff = (TH1F*)filetot->Get("hRecoEff_2_8");
   setHistStyle(hPhoEff,color[0],20);
   hPhoEff->GetFunction("fRecoEff_2_8")->Delete(); //rm the fit
   hPhoEff->GetXaxis()->SetTitle("p_{T} [GeV/c]");
   hPhoEff->GetYaxis()->SetTitle("Reco. Eff");
   hPhoEff->Draw();
   hPhoEff->GetXaxis()->SetRangeUser(0,2.8);
   hPhoEff->GetYaxis()->SetRangeUser(0,0.75);
  
   TLegend* leg = new TLegend(0.2,0.6,0.55,0.88); 
   leg->AddEntry(hPhoEff,"total eff.","lep");

   for (int i=1;i<totalNum;i++)
   {
      plotEffHist("hRecoEff_2_8",Form("%s%s.root",inputfileDir.Data(),component[i-1].Data()),c,leg,color[i],LegComp[i-1]); 
   }
   leg->Draw();
   drawLatex(0.65,0.8,"Au+Au 54.4 GeV",0.05);
  
   hPhoEff->DrawCopy("same");

   TFile* f = new TFile("output/fitRecoEff_2_8.root");
   TF1* fit = (TF1*)f->Get("fitfun2");
   fit->SetLineColor(kMagenta);
   // fit->Draw("same");
   drawSTAR(0.55,0.2);
   c->SaveAs("phoeff.pdf");
   c->SaveAs("phoeff.png");
}
