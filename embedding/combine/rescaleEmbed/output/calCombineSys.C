void calCombineSys()
{
  TFile* fdef = TFile::Open("PhoEff_comb.root");
  // TFile* f85 = TFile::Open("PhoEff_comb_0.85.root");
  // TFile* f115 = TFile::Open("PhoEff_comb_1.15.root");
  TFile* f85 = TFile::Open("PhoEff_comb_0.7.root");
  TFile* f115 = TFile::Open("PhoEff_comb_1.3.root");

  TH1F* hdef = (TH1F*)fdef->Get("hRecoEff_2_8")->Clone("hdef"); 
  hdef->SetDirectory(0);
  TH1F* h85 = (TH1F*)f85->Get("hRecoEff_2_8")->Clone("h85"); 
  h85->SetDirectory(0); 
  TH1F* h115 = (TH1F*)f115->Get("hRecoEff_2_8")->Clone("h115"); 
  h115->SetDirectory(0); 

  fdef->Close();
  f85->Close();
  f115->Close();

  h85->Add(hdef,-1);
  h85->Divide(hdef);
  h85->Draw();
  h115->Add(hdef,-1);
  h115->Divide(hdef);
  h115->SetMarkerColor(kRed);
  h115->Draw("same");

  int tmp = hdef->GetNbinsX();
  int const nbins=tmp;
  double x[nbins],y[nbins];
  double err1,err2;

  for (int i=1;i<nbins+1;i++)
  {
    //choose the larger one as the syserr from the combine
    err1 = h85->GetBinContent(i);
    err2 = h115->GetBinContent(i);
    x[i-1] = hdef->GetBinCenter(i);
    y[i-1] = fabs(err1)>fabs(err2)?fabs(err1):fabs(err2); 
  }

  TGraph* g = new TGraph(nbins,x,y);
  g->SetName("gCombineSys");
  TFile* f = new TFile("fCombineSys.root","recreate");
  g->Write();
}
