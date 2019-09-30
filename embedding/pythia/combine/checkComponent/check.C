void addpdf(TPDF* pdf, TCanvas* c)
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
    TPDF* pdf = new TPDF("check.pdf");
    TF1* fpispectra[9];
    int const SpectraParPi0_centbin[9]={0,0,1,1,2,2,3,4,4};
    double const SpectraParPi0[5][5]={ 
                                 {1.67607, -1.77146, 0.0759459, -0.369874, 2.01742},
                                 {264.787, 0.349324, 0.0475088, 1.01322, 10.903},
                                 {406.429, 0.361361, 0.0558035, 1.10099, 11.2414},
                                 {682.788, 0.396488, 0.0690007, 1.06078, 11.0674},
                                 {992.598, 0.406828, 0.0831998, 1.04464, 11.0797}
                             };

    for (int ic=0;ic<9;ic++)
    {
       // fpispectra[ic] = new TF1(Form("pi0spectra_%d",ic),"2*2*0.99*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15);
       fpispectra[ic] = new TF1(Form("etaspectra_%d",ic),"2*0.4*0.48*2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*sqrt(x*x+0.547862*0.547862-0.134977*0.134977)-[2]*(x*x+0.547862*0.547862-0.134977*0.134977))+sqrt(x*x+0.547862*0.547862-0.134977*0.134977)/[3], -[4])",0,15);
       fpispectra[ic]->SetParameters(SpectraParPi0[SpectraParPi0_centbin[ic]]);
    }
   
    TFile* file = new TFile("fout_pi0_eta_gamma_0926.root");
    TF1* fpi2gamma[9];
    for (int ic=2;ic<9;ic++)
    {
       // fpi2gamma[ic] = (TF1*)file->Get(Form("fGMSp_pi0_%d",ic));
       fpi2gamma[ic] = (TF1*)file->Get(Form("fGMSp_eta_%d",ic));
       // fpi2gamma[ic] = (TF1*)file->Get(Form("fGMSp_comb_%d",ic));
       fpi2gamma[ic]->SetLineColor(kBlue);
       fpi2gamma[ic]->Draw();
       fpispectra[ic]->Draw("same");
       // gPad->Update();
       addpdf(pdf,c);
    } 
    pdf->On();
    pdf->Close();
}
