void addpdf(TPDF* pdf,TCanvas* c)
{
   pdf->On();
   c->cd();
   c->Update();
   pdf->NewPage();
   pdf->Off();
}
void check()
{
  TCanvas* c = new TCanvas("c","c"); 
  TPDF* pdf = new TPDF("comparespectra.pdf");
  // drawSpectra("rescale_pi0.root",pdf,c);
  // drawSpectra("rescale_eta.root",pdf,c);
  // drawSpectra("rescale_gamma.root",pdf,c);
  drawSpectra("rescale_gamma_pi0.root",pdf,c);
  pdf->On();
  pdf->Close();

}

void drawSpectra(TString name, TPDF* pdf,TCanvas* c)
{
   c->cd();
   TFile* file = new TFile(name.Data());
   TH2F* h2D = (TH2F*)file->Get("hPi0Pt_weight");
   h2D->SetDirectory(0);
   TH1F* hcent = (TH1F*)file->Get("hEvent");
   hcent->SetDirectory(0);
   int const SpectraParPi0_centbin[9]={0,0,1,1,2,2,3,4,4};
   double const SpectraParPi0[5][5]={
                                 {1.67607, -1.77146, 0.0759459, -0.369874, 2.01742},
                                 {264.787, 0.349324, 0.0475088, 1.01322, 10.903},
                                 {406.429, 0.361361, 0.0558035, 1.10099, 11.2414},
                                 {682.788, 0.396488, 0.0690007, 1.06078, 11.0674},
                                 {992.598, 0.406828, 0.0831998, 1.04464, 11.0797}
                             };
   TF1* fpispectra[9];
   
   // TFile* fgamma = new TFile("fout_pi0_eta_gamma_0926.root");
   TFile* fgamma = new TFile("fout_pi0_eta_gamma1016.root");
   for (int ic=2;ic<9;ic++)
   {
     if (name.Contains("gamma")) 
     {
        if (name.Contains("gamma_pi0")) fpispectra[ic] = (TF1*)fgamma->Get(Form("fGMSp_pi0_%d",ic));
     }

     else {  
     if (name.Contains("pi0"))  fpispectra[ic] = new TF1(Form("pi0spectra_%d",ic),"2*0.01174*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15);
     if (name.Contains("eta"))  {
       fpispectra[ic] = new TF1(Form("etaspectra_%d",ic),"0.0069*0.48*2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*sqrt(x*x+0.547862*0.547862-0.134977*0.134977)-[2]*(x*x+0.547862*0.547862-0.134977*0.134977))+sqrt(x*x+0.547862*0.547862-0.134977*0.134977)/[3], -[4])",0,15);
       // cout<<name.Data() <<endl;
       }
       fpispectra[ic]->SetParameters(SpectraParPi0[SpectraParPi0_centbin[ic]]);
     }
   }
   for (int i=2;i<9;i++)
   {
     TH1F* h = (TH1F*)h2D->ProjectionX("h",i+1,i+1);
     h->Scale(1./h->GetBinWidth(1));
     h->Scale(1./hcent->GetBinContent(i+1));
     // h->Scale(4./250e6);
     h->Scale(1./2.99);
     h->DrawCopy();
     fpispectra[i]->Draw("same");
     gPad->SetLogy();
     fpispectra[i]->SetLineColor(kRed);
     addpdf(pdf,c);
     // delete h;
   }
   file->Close();
 }
