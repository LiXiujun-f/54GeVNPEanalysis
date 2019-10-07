void setHistStyle(TH1* hist,int color,int style)
{
    hist->SetLineColor(color);
    hist->SetMarkerColor(color);
    hist->SetMarkerStyle(style);
}

void plotEffHist(TString histname,TString filename,TCanvas* c,TLegend* leg,int color)
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

   leg->AddEntry(h,filename.Data(),"lep");
   file->Close();
}

void plotComponent(TString inputfileDir,TString Stotal,TString Spi0,TString Seta,TString Sgamma2pi0,TString Sgamma2eta,TString Sdirpho)
{
   TCanvas* c = new TCanvas("c","c");
   c->cd();
   int const totalNum=6;
   int color[6] = {1,2,3,7,9,6};
   TString component[]={Spi0,Seta,Sgamma2pi0,Sgamma2eta,Sdirpho};
   TFile* filetot = TFile::Open(Form("%s%s.root",inputfileDir.Data(),Stotal.Data()));
   TH1F* hPhoEff = (TH1F*)filetot->Get("hRecoEff_2_8");
   setHistStyle(hPhoEff,color[0],20);
   hPhoEff->GetFunction("fRecoEff_2_8")->Delete(); //rm the fit
   hPhoEff->Draw();
  
   TLegend* leg = new TLegend(0.2,0.6,0.5,0.88); 
   for (int i=1;i<totalNum;i++)
   {
      plotEffHist("hRecoEff_2_8",Form("%s%s.root",inputfileDir.Data(),component[i-1].Data()),c,leg,color[i]); 
   }
   leg->Draw();
  
   hPhoEff->DrawCopy("same");
   c->SaveAs("phoeff.pdf");
}
