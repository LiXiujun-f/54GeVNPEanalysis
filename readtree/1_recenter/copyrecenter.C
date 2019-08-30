void copyrecenter()
{
  TFile* file = TFile::Open("getrecenter_vz40.root");
  TProfile* pQxRecenterM[9];
  TProfile* pQyRecenterM[9];
  TProfile* pQxRecenterP[9];
  TProfile* pQyRecenterP[9];
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic] = (TProfile*)file->Get(Form("pQxRecenterMin_%d",ic));
    pQxRecenterM[ic]->SetDirectory(0);
    pQyRecenterM[ic] = (TProfile*)file->Get(Form("pQyRecenterMin_%d",ic));
    pQyRecenterM[ic]->SetDirectory(0);
    pQxRecenterP[ic] = (TProfile*)file->Get(Form("pQxRecenterPlu_%d",ic));
    pQxRecenterP[ic]->SetDirectory(0);
    pQyRecenterP[ic] = (TProfile*)file->Get(Form("pQyRecenterPlu_%d",ic));
    pQyRecenterP[ic]->SetDirectory(0);
  } 
  file->Close();
  // TH2F* hEventPlaneCent = (TH2F*)file->Get("hEventPlaneCent"); 
  // TH1F* mbep = (TH1F*)hEventPlaneCent->ProjectionY();
  // mbep->Draw();
  TFile* fnew = new TFile("recenter_vz40.root","recreate");
  for (int ic=0;ic<9;ic++){
    pQyRecenterP[ic]->Write();
    pQxRecenterP[ic]->Write();
    pQxRecenterM[ic]->Write();
    pQyRecenterM[ic]->Write();
  }
  fnew->Close();
}
