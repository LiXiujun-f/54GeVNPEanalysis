void drawcompare()
{
  TFile* file = TFile::Open("shift_vz35.root");
  TProfile* pSinEtaP[9];
  TProfile* pCosEtaM[9];
  TProfile* pCosEtaP[9];
  TProfile* pSinEtaM[9];
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
  file->Close();
  // TFile* file = TFile::Open("../../EP_incEv2/shift.root");
  TFile* file = TFile::Open("shift_vz40.root");
  // TFile* file = TFile::Open("eta1_old/shift.root");
  TProfile* pSinEtaP1[9];
  TProfile* pCosEtaM1[9];
  TProfile* pCosEtaP1[9];
  TProfile* pSinEtaM1[9];
  for (int ic=0;ic<9;ic++)
  {
    pSinEtaM1[ic] = (TProfile*)file->Get(Form("pSinEtaM_Cent%d",ic));
    pCosEtaM1[ic] = (TProfile*)file->Get(Form("pCosEtaM_Cent%d",ic));
    pSinEtaP1[ic] = (TProfile*)file->Get(Form("pSinEtaP_Cent%d",ic));
    pCosEtaP1[ic] = (TProfile*)file->Get(Form("pCosEtaP_Cent%d",ic));
    pSinEtaM1[ic]->SetDirectory(0); 
    pCosEtaM1[ic]->SetDirectory(0);
    pSinEtaP1[ic]->SetDirectory(0);
    pCosEtaP1[ic]->SetDirectory(0);
  }
  pCosEtaP1[0]->Draw();
  pCosEtaP1[0]->SetLineColor(kRed);
  pCosEtaP[0]->Draw("same");
}
