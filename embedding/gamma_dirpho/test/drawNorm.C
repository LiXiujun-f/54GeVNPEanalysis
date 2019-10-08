void drawNorm()
{
   // TFile* file = new TFile("test_gamma_dirpho_1004.root");
   // TFile* file = new TFile("test_gamma_dirpho_1007.root");
   // TFile* file = new TFile("embeddQa_pi0_0930.root");
   TFile* file = new TFile("test_pi0_1007.root");
   // TFile* file = new TFile("test_gamma2pi0_1007.root");
   // TFile* file = new TFile("embeddQa_gamma_dirpho_1002.root");
   TH2F* hpi2d = (TH2F*)file->Get("hPi0Pt_ptwt");
   // TH2F* hpi2d = (TH2F*)file->Get("hPi0Pt");
   TH1F* hpi0 = (TH1F*)hpi2d->ProjectionX("hpi0",3,9);
   hpi0->Draw();
   // hpi0->GetXaxis()->SetRangeUser(0,4);

   TH2F* hpi2dweight = (TH2F*)file->Get("hPi0Pt_weight");
   TH1F* hpiw = (TH1F*)hpi2dweight->ProjectionX("hpiw");
   // hpiw->Draw();

   // TH2F* hpi2dnoweight = (TH2F*)file->Get("hPi0Pt");
   TH2F* hpi2dnoweight = (TH2F*)file->Get("hPi0Pt_norm");
   TH1F* hpinw = (TH1F*)hpi2dnoweight->ProjectionX("hpinw",3,9);
   double avgtracks = hpiw->Integral()*1.0/hpinw->Integral()*1.;
   cout <<avgtracks << endl;
   double totevents = hpinw->Integral(hpinw->GetXaxis()->FindBin(0.2),hpinw->GetXaxis()->FindBin(15));
   TH1F* href = (TH1F*)file->Get("hRefmult");
   cout <<"events: "<<" "<< totevents<< endl;


   TH2F* hMc2d = (TH2F*)file->Get("hMcElectronPtvsCent");
   TH1F* hMc = (TH1F*)hMc2d->ProjectionX("hMc",3,9);
   cout <<"electron integral: "<<hMc->Integral(hMc->GetXaxis()->FindBin(0.2),hMc->GetXaxis()->FindBin(4)) << endl;
   cout << "pi0 weight: " << hpi0->Integral(hpi0->GetXaxis()->FindBin(0.2),hpi0->GetXaxis()->FindBin(15))<<endl;
   // hMc->Scale(1./avgtracks);
   hMc->Draw("same");
   gPad->SetLogy();

}
