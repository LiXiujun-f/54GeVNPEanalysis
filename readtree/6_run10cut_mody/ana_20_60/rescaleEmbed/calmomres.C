void calmomres()
{
  TFile* file = new TFile("embeddingQa.phoE.root.ep0");
  TH2F* momres = (TH2F*)file->Get("hMomResolution");
  TF1* fit = new TF1("fit","gaus",0,10);
  momres->FitSlicesY(fit);
  TH1D *h2_2 = (TH1D*)gDirectory->Get("hMomResolution_2");
  h2_2->Draw();

}
