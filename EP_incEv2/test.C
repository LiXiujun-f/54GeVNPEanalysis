void test(){
  
  // TH1F*h =  new TH1F("h","h",180,-10,35);
  // TH1F*h =  new TH1F("h","h",614,0,614);
  TProfile* h =  new TProfile("h","h",1,0,2);
  // for (int i =0;i<2e6;i++){
  //   float x = floor(gRandom->Gaus(0,0.8)*200+1.05)*0.01;
  //   float y = ceil(gRandom->Gaus(0,0.8)*200+1.05)*0.01;
  //   // cout <<x<<y<<endl;
  //   h->Fill(x);
  //   h->Fill(y);
  // }
  // h->Draw();
  // h->Rebin(4);
  for (int i=0;i<2;i++){
    h->Fill(i,3);
    h->Fill(i,2);
    h->Fill(i,1);
  }
  h->Fill(1,2);
  h->Fill(1,4);
  // h->Rebin();
  h->Draw();
  // cout<<h->GetBinContent(222)<<endl;


}
