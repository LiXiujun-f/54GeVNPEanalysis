void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   TFile* file = new TFile("embeddingQa.root");
   // TFile* file = new TFile("test.McAna.root");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",9,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",9,9);
   hMcX->Rebin();
   hRcX->Rebin();
   hRcX->Divide(hMcX);

   // for ()
   // {
    
   
   // }

   hRcX->Draw();
   
} 
