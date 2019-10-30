void test()
{
  TFile* file = new TFile("fout_pi0_eta_gamma_0926.root");
  TF1* f = (TF1*)file->Get("dirpho_sp_8");
  TH1F* h = new TH1F("h","h",2000,0,4);
  for (int i=1;i<2000+1;i++)
  {
    h->SetBinContent(i,f->Eval(h->GetBinCenter(i)));
  }
  h->Draw();

}
