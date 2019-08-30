void copyrecenter()
{
  TFile* file = TFile::Open("getshift.root");
  TProfile* pSinEtaP[20][9];
  TProfile* pCosEtaM[20][9];
  TProfile* pCosEtaP[20][9];
  TProfile* pSinEtaM[20][9];
  for (int i=0;i<20;i++) 
  {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[i][ic] = (TProfile*)file->Get(Form("pSinEtaM_Cent%d_%d",ic,i+1));
      pCosEtaM[i][ic] = (TProfile*)file->Get(Form("pCosEtaM_Cent%d_%d",ic,i+1));
      pSinEtaP[i][ic] = (TProfile*)file->Get(Form("pSinEtaP_Cent%d_%d",ic,i+1));
      pCosEtaP[i][ic] = (TProfile*)file->Get(Form("pCosEtaP_Cent%d_%d",ic,i+1));
      pSinEtaM[i][ic]->SetDirectory(0); 
      pCosEtaM[i][ic]->SetDirectory(0);
      pSinEtaP[i][ic]->SetDirectory(0);
      pCosEtaP[i][ic]->SetDirectory(0);
      pSinEtaM[i][ic]->Rebin(614); 
      pCosEtaM[i][ic]->Rebin(614);
      pSinEtaP[i][ic]->Rebin(614);
      pCosEtaP[i][ic]->Rebin(614);

    }
  }
  file->Close();
  // TH2F* hEventPlaneCent = (TH2F*)file->Get("hEventPlaneCent"); 
  // TH1F* mbep = (TH1F*)hEventPlaneCent->ProjectionY();
  // mbep->Draw();
  TFile* fnew = new TFile("shift.root","recreate");
  for (int i=0;i<20;i++) 
  {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[i][ic]->Write(); 
      pCosEtaM[i][ic]->Write();
      pSinEtaP[i][ic]->Write();
      pCosEtaP[i][ic]->Write();
    }
  }
  fnew->Close();
}
