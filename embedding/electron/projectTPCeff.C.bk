void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   // TFile* file = new TFile("embeddingQa.0829.root");
   TFile* file = new TFile("embeddingQa.test.root");
   // TFile* file = new TFile("test.McAna.root");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",1,1);
   hMcX->Rebin(10);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",1,1);

   hRcX->Rebin(10);
   hRcX->Divide(hMcX);

   // for ()
   // {
    
   
   // }

   hRcX->Draw();
   
} 
