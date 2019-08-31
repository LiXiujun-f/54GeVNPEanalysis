void projectTPCeff()
{
   // TFile* file = new TFile("embeddingQa.phoE.root");
   TFile* file = new TFile("embeddingQa.root");
   // TFile* file = new TFile("test.McAna.root");
   TH2F* hMc = (TH2F*)file->Get("hMcElectronPtvsCent");
   hMc->Sumw2();
   TH2F* hRc = (TH2F*)file->Get("hRcElectronPtvsCent");
   hRc->Sumw2();
   TH1F* hMcX =(TH1F*)hMc->ProjectionX("hMcX",0,9);
   TH1F* hRcX =(TH1F*)hRc->ProjectionX("hRcX",0,9);
   hMcX->Rebin();
   hRcX->Rebin();
   hRcX->Divide(hMcX);

   // for ()
   // {
    
   
   // }

   hRcX->Draw();
   
} 
