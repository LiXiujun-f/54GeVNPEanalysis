void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   // TFile* file = new TFile("embeddingQa.0829.root");
   TFile* file = new TFile("embeddingQa.test.root");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",3,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",3,9);

   hRcX->Divide(hMcX);

   // for ()
   // {
    
   
   // }

   hRcX->Draw();
   
} 
