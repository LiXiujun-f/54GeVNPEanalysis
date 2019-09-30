void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root.ep0");
   // TFile* file = new TFile("embeddQa_phicut0827.root");
   // TFile* file = new TFile("embeddQa_test.root");
   TFile* file = new TFile("embeddQa_gamma_0918.root");
   // TFile* file = new TFile("test.McAna.root");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   // TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   TH2F* hRc = (TH2F*)file->Get("hRcElectronMcPtvsCent");
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",3,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",3,9);

   hRcX->Divide(hMcX);
   hRcX->Draw();
   
} 
