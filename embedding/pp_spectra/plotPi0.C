// #include "TRatioPlot.h"
void plotPi0()
{
   TFile* f54 = TFile::Open("hpi0_54.root");
   TH1F* h54 = (TH1F*)f54->Get("hpi0_54");
   h54->SetDirectory(0);
   f54->Close();
   TFile* f62 = TFile::Open("pi0_62.root");
   // TH1F* h62 = (TH1F*)f62->Get("hpi0");
   TH1F* h62 = (TH1F*)f62->Get("hpi0_62");
   h62->SetDirectory(0);
   f62->Close();
   
   h54->SetLineColor(kRed);
   h62->Scale(1./h62->GetBinWidth(1));
   h54->Scale(1./h54->GetBinWidth(1));
   h62->Draw();
   h54->Draw("same");
   TH1F* rp = (TH1F*)h54->Clone("hratio"); 
   rp->Divide(h62);
   rp->Draw();
   cout <<h54->Integral()*1.0/h62->Integral() << endl;

} 
