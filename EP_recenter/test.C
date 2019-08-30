void test(){
  std::map<int,int> mm;
  for (int i=0;i<614;i++){
    mm.insert(pair<int,int>(i,i));
  }  
  TH1F*h =  new TH1F("h","h",614,0,614);
  // for (int i =0;i<2e6;i++){
  //   float x = floor(gRandom->Gaus(0,0.8)*614)*0.01;
  //   float y = ceil(gRandom->Gaus(0,0.8)*614)*0.01;
  //   // cout <<x<<y<<endl;
  //   h->Fill(x);
  //   h->Fill(y);
  // }
  for (int i=0;i<614;i++){
    h->Fill(mm[i],i);
  }
  h->Draw();
  // h->Rebin(4);
  // cout<<h->GetBinWidth(1)<<endl;
  cout<<h->GetBinContent(1)<<endl;


}
