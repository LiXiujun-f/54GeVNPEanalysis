void test(){
  
  TH1F*h =  new TH1F("h","h",180,-10,35);
  for (int i =0;i<2e6;i++){
    float x = floor(gRandom->Gaus(0,0.8)*200+1.05)*0.01;
    float y = ceil(gRandom->Gaus(0,0.8)*200+1.05)*0.01;
    // cout <<x<<y<<endl;
    h->Fill(x);
    h->Fill(y);
  }
  h->Draw();
  // h->Rebin(4);
  cout<<h->GetBinWidth(1)<<endl;


}
