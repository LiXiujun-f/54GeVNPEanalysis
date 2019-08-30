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
  TFile* f = new TFile("PE.root");
  TH2F* hsige = (TH2F*)f->Get("hnSigE_e");
  TH2F* hsigels = (TH2F*)f->Get("hnSigE_e_ls");
  // hsige->Add(hsigels,-1);
  hsige->Draw("colz");
  TH1F* hsigeY = (TH1F*)hsige->ProjectionY("");
  gPad->SetLogz(); 
  addpdf(pdf);
  hsigeY->Rebin(8);
  hsigeY->Draw();
  addpdf(pdf);
  TH3F* hphotols = (TH3F*)f->Get("hphoto_LS"); 
  TH3F* hphoto = (TH3F*)f->Get("hphoto"); 
  TH3F* hphotoul = (TH3F*)f->Get("hphoto")->Clone("hphotoul"); 
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
  TH3F* hV0 = (TH3F*)f->Get("hV0"); 
  TH2F* hV0_xy = (TH2F*)hV0->Project3D("xy");
  TH2F* hV0_zy = (TH2F*)hV0->Project3D("zy");
  hV0_xy->Draw("colz");
  addpdf(pdf);
  hV0_zy->Draw("colz");
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}
