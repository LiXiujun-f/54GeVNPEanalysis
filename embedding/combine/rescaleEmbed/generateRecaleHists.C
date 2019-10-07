template <class Hist>
Hist* readAndScale(TString histname, double scalefactor,TFile* fin)
{
  Hist* h = (Hist*)fin->Get(histname.Data());
  h->Scale(scalefactor);
  h->SetDirectory(0);
  return h;
}


template <class Hist>
void readScaleAndWrite(TString histname, double scalefactor,TFile* fin,TFile* fout)
{
  Hist* h = (Hist*)fin->Get(histname.Data());
  h->Scale(scalefactor);
  h->SetDirectory(0);
  fout->cd();
  h->Write();
}
void generateRecaleHists(TString inputfilename="data/embeddQa_pi0_0918.root", TString outputname="test.root",double branchratio=0.99)
{
  TFile* fout = new TFile(outputname.Data(),"recreate");
  TFile* fin = new TFile(inputfilename.Data());
  cout << "inputfile is: "<<inputfilename.Data()<<endl;

  //read the scale factor
  //I prefer to rescale to make it most close to the real data, so I set the Nevents=500M
  TH3F* hPi0PhiEtaPt = (TH3F*)fin->Get("hPi0PhiEtaPt");
  TH1F* heta = (TH1F*)hPi0PhiEtaPt->ProjectionZ("heta");
  //I have checked the eta distribution is flat as we required
  double etarange = (heta->Integral())/(1.0*heta->Integral(heta->FindBin(-1),heta->FindBin(1)))*2;
  cout<<"check the etarange: "<<etarange<< endl;
  
  TH2F* hPi0Pt_norm = (TH2F*)fin->Get("hPi0Pt_norm"); 
  TH1F* hPi0 = (TH1F*)hPi0Pt_norm->ProjectionX("hPi0Pt_x", 3,6); //use 20-60% as the scale range
  double ptrange = hPi0->Integral()/(1.0*hPi0->Integral(hPi0->FindBin(1),hPi0->FindBin(11)))*10; 
  cout <<"check the ptrange: "<< ptrange<< endl;

  double totalnPi0 = hPi0->Integral(); 
  cout << "total num pi0 (or eta/gamma) in the embedding: "<< totalnPi0 << endl;

  // int scaleEvents = 250e6;
  int scaleEvents = 3e8;
  double scalefactor = scaleEvents*branchratio*ptrange*etarange/totalnPi0;  
  cout <<"scalefactor: "<< scalefactor <<" branchratio: "<<branchratio<<" scale to "<<scaleEvents<<" events" << endl;
  
  //rewrite every histogram
  //when I store the hists, I scale the histogram with dN/dptdy and every event have only one pi0/eta/gamma
  //the embedding use flat phi and eta, pt distribution.
  // TH1F* hRefmult = readAndScale<TH1F>("hRefmult",scalefactor,fin,fout);
  // readScaleAndWrite<TH1F>("hRefmult",scalefactor,fin,fout);
  readScaleAndWrite<TH1F>("hEvent",scalefactor/ptrange/etarange/branchratio,fin,fout);
  readScaleAndWrite<TH2F>("hPi0Pt_weight",scalefactor,fin,fout);
  // readScaleAndWrite<TH2F>("hnHits",scalefactor,fin,fout);//global track  without any weight
  // readScaleAndWrite<TH2F>("hDCA",scalefactor,fin,fout); //global track  without any weight
  readScaleAndWrite<TH2F>("hTagElectron",scalefactor,fin,fout);
  readScaleAndWrite<TH2F>("hTagElectronPassCut",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hMassDiE",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hMassDiEMC",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hDcaPair",scalefactor,fin,fout);
  readScaleAndWrite<TH2F>("hPartEnFits",scalefactor,fin,fout);
  readScaleAndWrite<TH2F>("hPartEnFitsTagEpt",scalefactor,fin,fout);
  readScaleAndWrite<TH2F>("hPartEdca",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hPartEptetaphi",scalefactor,fin,fout);
  readScaleAndWrite<TH2F>("hPairDecayL",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hPhoEPtPhiCent",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hPhoEPtPhiCentRaw",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hRecoEPtPhiCent",scalefactor,fin,fout);
  readScaleAndWrite<TH3F>("hRecoEPtPhiCentRaw",scalefactor,fin,fout);

  fin->Close();
  fout->Close();
} 
