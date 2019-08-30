void projectMom(){
  // TFile* file = new TFile("PE.root");
  TFile* file = new TFile("hePtvsP.root");
  TH2F* hp_pt = (TH2F*)file->Get("hePtvsP");
  hp_pt->Draw("colz");
  TProfile* p_pt = (TProfile*)hp_pt->ProfileY();
  p_pt->GetXaxis()->SetTitle("p [GeV/c]");
  p_pt->GetYaxis()->SetTitle("p_{T} [GeV/c]");
  p_pt->Draw();
  p_pt->SaveAs("p_pt_MB_54.root");
}
