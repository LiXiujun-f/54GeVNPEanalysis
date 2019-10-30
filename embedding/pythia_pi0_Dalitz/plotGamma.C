void plotGamma()
{
   TFile* file1 = new TFile("fread_pi0_Dz_gamma.root");
   TFile* file2 = new TFile("fread_pi0_eta_2gamma.root");

   TGraph* f1 = (TGraph*)file1->Get("gGmSp_pi0_1");
   TGraph* f2 = (TGraph*)file2->Get("gGmSp_pi0_1");
   
   TH1F* h1 = (TH1F*)file1->Get("hpi02Gamma_mb")->Clone("h1");
   TH1F* h2 = (TH1F*)file2->Get("hpi02Gamma_mb")->Clone("h2");
   h1->Draw();
   gPad->SetLogy();
   f2->SetMarkerSize(0.5);
   f1->SetMarkerSize(0.5);
   f1->SetMarkerColor(kRed);
   f1->Draw("psame");
   f2->Draw("psame");
   
   TLegend* leg = new TLegend(0.6,0.7,0.88,0.88);
   leg->AddEntry( f1,"pi0 Dalitz gamma","lp");
   leg->AddEntry( f2,"pi0#leftarrow2#gamma","lp");
   leg->Draw();
   cout << "scale factor: Dz: "<<h1->Integral(h1->GetXaxis()->FindBin(1.5),h1->GetXaxis()->FindBin(5))<< " 2#gamma: "<<f2->Integral(1.5,5)<<" "<<h1->Integral(h1->GetXaxis()->FindBin(1.5),h1->GetXaxis()->FindBin(5))/h2->Integral(h1->GetXaxis()->FindBin(1.5),h1->GetXaxis()->FindBin(5))<< endl;


   TH1F* ratio = (TH1F*)h1->Clone("ratio");
   ratio->Divide(h2);
   // ratio->Draw(); 
}
