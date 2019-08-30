void copyrecenter()
{
  TFile* file = TFile::Open("getshift.root");
  TProfile* pSinEtaP[9];
  TProfile* pCosEtaM[9];
  TProfile* pCosEtaP[9];
  TProfile* pSinEtaM[9];
  // for (int i=0;i<20;i++) 
  // {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[ic] = (TProfile*)file->Get(Form("pSinEtaM_Cent%d",ic));
      pCosEtaM[ic] = (TProfile*)file->Get(Form("pCosEtaM_Cent%d",ic));
      pSinEtaP[ic] = (TProfile*)file->Get(Form("pSinEtaP_Cent%d",ic));
      pCosEtaP[ic] = (TProfile*)file->Get(Form("pCosEtaP_Cent%d",ic));
      pSinEtaM[ic]->SetDirectory(0); 
      pCosEtaM[ic]->SetDirectory(0);
      pSinEtaP[ic]->SetDirectory(0);
      pCosEtaP[ic]->SetDirectory(0);

    }
  // }
  file->Close();
  // TH2F* hEventPlaneCent = (TH2F*)file->Get("hEventPlaneCent"); 
  // TH1F* mbep = (TH1F*)hEventPlaneCent->ProjectionY();
  // mbep->Draw();
  TFile* fnew = new TFile("shift.root","recreate");
  // for (int i=0;i<20;i++) 
  // {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[ic]->Write(); 
      pCosEtaM[ic]->Write();
      pSinEtaP[ic]->Write();
      pCosEtaP[ic]->Write();
    }
  // }
  fnew->Close();
}
