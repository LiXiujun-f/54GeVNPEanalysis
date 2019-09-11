void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   TFile* file = new TFile("embeddingQa.root");
   // TFile* file = new TFile("test.McAna.root");
   TH3F* hMc = (TH3F*)file->Get("hMcElectronPtvsCent");
   hMc->Sumw2();
   TH3F* hRc = (TH3F*)file->Get("hRcElectronPtvsCent");
   hRc->Sumw2();
  
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",2,2,8,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",2,2,8,9);
   hRcX->Divide(hMcX);
   hRcX->Draw();
} 
