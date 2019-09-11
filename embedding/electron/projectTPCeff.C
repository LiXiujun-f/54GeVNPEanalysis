void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   // TFile* file = new TFile("embeddingQa.test.root");
   TFile* file = new TFile("embeddingQa.0829.root.bk");
   // TFile* file = new TFile("test.McAna.root");
   // TH3F* hMc = (TH3F*)file->Get("hMcElectronPtvsCent");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   hMc->Sumw2();
   TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   hRc->Sumw2();
  
   // TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",1,4,9,9);
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",3,9);
   // TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",1,4,9,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",3,9);
   hRcX->Divide(hMcX);
   hRcX->Draw();
} 
