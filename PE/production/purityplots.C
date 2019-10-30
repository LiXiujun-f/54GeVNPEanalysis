void purityplots()
{
    TFile* fMB = new TFile("Nsigma_6_8.root");
    TFile* fmid = new TFile("Nsigma_2_8.root");
    TH1F* hmb = (TH1F*)fMB->Get("hpurity");
    hmb->SetDirectory(0);
    TH1F* hmid = (TH1F*)fmid->Get("hpurity");
    hmid->SetDirectory(0);
    hmb->SetLineColor(kRed);
    hmb->SetLineColor(kBlack);
    hmb->Draw();
    hmid->Draw("same");
    TLegend * leg = new TLegend(0.5,0.5,0.8,0.8);
    leg->AddEntry( hmid,"20-60%","l");
    leg->AddEntry( hmb,"0-20%","l");
    leg->Draw();
}
