// #include ""
void addpdf(TPDF* pdf)
{
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off(); 
}
void draw(){
  TPDF* pdf = new TPDF("pe.pdf");
  pdf->NewPage();
  pdf->Off();
  TFile* f = new TFile("test_out_.Ks.root");
  TH2F* hsige = (TH2F*)f->Get("hnSigE_pi");
  TH2F* hsigels = (TH2F*)f->Get("hnSigE_pi_ls");
  // hsige->Add(hsigels,-1);
  hsige->Draw("colz");
  TH1F* hsigeY = (TH1F*)hsige->ProjectionY("");
  gPad->SetLogz(); 
  addpdf(pdf);
  hsigeY->Draw();
  addpdf(pdf);
  TH2F* hphotols = (TH2F*)f->Get("hKs_LS"); 
  TH2F* hphoto = (TH2F*)f->Get("hKs"); 
  TH2F* hphotoul = (TH2F*)f->Get("hKs")->Clone("hphotoul"); 
  hphoto->Add(hphotols,-1);
  TH1F* hpx = (TH1F*)hphoto->ProjectionX("hpx");
  TH1F* hpx_ul = (TH1F*)hphotoul->ProjectionX("hpx_ul");
  TH1F* hpxls = (TH1F*)hphotols->ProjectionX("hpx_ls");
  hpx_ls->SetLineColor(kBlue);
  // hpx_ul->SetLineColor();
  hpx->SetLineColor(kRed);
  hpx_ul->Draw();
  hpx_ls->Draw("same");
  hpx->Draw("same");
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}
