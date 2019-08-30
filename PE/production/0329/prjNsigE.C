#include "rootlogon.h"
#include <string>
TH1F* hesamp[50];
TH1F* hpsamp[50];
TH1F* hpisamp[50];
TH1F* hKsamp[50];
TH1F* htofsamp[50];
TPDF* pdf;
void addpdf(TPDF* pdf)
{
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off(); 
}
void drawtitle(TPDF* pdf,TCanvas* c,string s){
  c->cd();
  c->Draw();
  // setPad(0.1,0.1,0.05,0.12);
  TLatex t;
  t.SetTextSize(0.05);
  t.DrawText(0.2,0.5,s.c_str());
  TLatex la;
  la.SetTextSize(0.035);
  la.DrawText(0.1,0.3,(new TDatime())->AsSQLString());
  la.DrawText(0.1,0.2,"by Yuanjing");
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void projectionAndFit(TH2F* h, float lowpt, float highpt, float &mean,float &sigma,string p, int j)
{
  int lbin = h->GetXaxis()->FindBin(lowpt);
  int hbin = h->GetXaxis()->FindBin(highpt);
  TH1F* hx = (TH1F*)h->ProjectionY("hx",lbin,hbin);
  //hx->SetDirectory(0);
  hx->DrawCopy();
  // TF1* fit  = new TF1("fit","gaus", -10,15);
  // hx->Fit(fit);
  // fit->SetLineColor(kRed);
  // fit->DrawCopy("same"); 
  // mean = fit->GetParameter(1); 
  // sigma = fit->GetParameter(2); 
  TLatex lat;
  lat.SetTextSize(0.035);
  lat.DrawLatex(0.025,hx->GetMaximum()*0.6,Form("%.2f<p_{T}<%.2f GeV %s",lowpt,highpt,p.c_str()));
  addpdf(pdf);
  hx->Scale(1./hx->GetBinWidth(1));
  // j=0;
  if (p.find("tot")!=std::string::npos) {
    htofsamp[j] = (TH1F*)hx->Clone(Form("h%s_%.1f_%.1f",p.c_str() ,lowpt,highpt));
    htofsamp[j]->SetDirectory(0);
  }
  float total = hx->Integral();
  hx->Scale(1./total); 
  // return hx;
  if (p.find("e")!=std::string::npos)     {
    hesamp[j] = (TH1F*)hx->Clone(Form("h%s_%.1f_%.1f",p.c_str() ,lowpt,highpt));
    hesamp[j]->SetDirectory(0);
  }
  if (p.find("K")!=std::string::npos) {
    hKsamp[j] = (TH1F*)hx->Clone(Form("h%s_%.1f_%.1f",p.c_str() ,lowpt,highpt));
    hKsamp[j]->SetDirectory(0);
  }
  if (p.find("pi")!=std::string::npos) {
    hpisamp[j] = (TH1F*)hx->Clone(Form("h%s_%.1f_%.1f",p.c_str() ,lowpt,highpt));
    hpisamp[j]->SetDirectory(0);
  }
  if (p.find("p")!=std::string::npos&&p.find("pi")==std::string::npos) {
    hpsamp[j] = (TH1F*)hx->Clone(Form("h%s_%.1f_%.1f",p.c_str() ,lowpt,highpt));
    hpsamp[j]->SetDirectory(0);
  }
 }

void funesamp(double *x, double *p)
{
  int idx = p[0];
  // return p[0]*hesamp[p[1]]->GetBinContent(hesamp[p[1]]->FindBin(x[0]));
  return p[0]*hesamp[0]->GetBinContent(hesamp[0]->FindBin(x[0]));
} 
void funpisamp(double *x, double *p)
{ 
  int idx = p[1];
  return p[0]*hpisamp[idx]->GetBinContent(hpisamp[idx]->FindBin(x[0]));
}
void funKsamp(double *x, double *p)
{
  // p[1]=0; 
  int idx = p[1];
  return p[0]*hKsamp[idx]->GetBinContent(hKsamp[idx]->FindBin(x[0]));
}
void funpsamp(double *x, double *p)
{
  int idx =p[1];
  return p[0]*hpsamp[idx]->GetBinContent(hpsamp[idx]->FindBin(x[0]));
}

void funtofsamp(double* x,double *p )
{
  int idx = p[1];
  float e_part = p[0]*hesamp[idx]->GetBinContent(hesamp[idx]->FindBin(x[0]));
  float pi_part = p[2]*hpisamp[idx]->GetBinContent(hpisamp[idx]->FindBin(x[0]));
  float K_part = p[3]*hKsamp[idx]->GetBinContent(hKsamp[idx]->FindBin(x[0]));
  float p_part = p[4]*hpsamp[idx]->GetBinContent(hpsamp[idx]->FindBin(x[0]));
  return e_part+pi_part+K_part+p_part;
}
void prjNsigE(){
  myStyle();
  TCanvas* c = new TCanvas("c","c");
  pdf = new TPDF("NsigE.pdf");
  pdf->Off();
  // TPDF* pdf = new TPDF("NsigE.pdf");
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = TFile::Open("PE.root");
  TH2F* hels = (TH2F*)f->Get("hnSigE_e_ls_Dz"); 
  TH2F* he = (TH2F*)f->Get("hnSigE_e"); 
  he->Add(hels,-1);
  he->SetDirectory(0);
  TH2F* hp = (TH2F*)f->Get("hnSigE_p");
  hp->SetDirectory(0);
  TH2F* hk = (TH2F*)f->Get("hnSigE_k");
  hk->SetDirectory(0);
  TH2F* htof = (TH2F*)f->Get("hnSigE_tof");
  htof->SetDirectory(0);

  TH2F* hcut = (TH2F*)f->Get("hnSigE_Cut");
  hcut->SetDirectory(0);
  TH1F* hecut = (TH1F*)hcut->ProjectionX();
  hecut->SetDirectory(0);
  f->Close();
  f = TFile::Open("Ks.root");
  TH2F* hpi = (TH2F*)f->Get("hnSigE_pi");
  TH2F* hpils = (TH2F*)f->Get("hnSigE_pi_ls");
  hpi->Add(hpils,-1);
  hpi->SetDirectory(0);
  f->Close();
  float pt[50]; //0.2-5
  int i=0;
  pt[0]=0.2;
  while (pt[i]<1){
    pt[i+1]=pt[i]+0.2;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while  (pt[i]<3){
    pt[i+1]=pt[i]+0.5;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  while (pt[i]<5){
    pt[i+1]=pt[i]+1;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  int  bin =i;
  TH1F* hpurity = new TH1F("hpurity", "hpurity;electron p_{T}(GeV);purity",bin,pt);
  TH1F* hmean_e = new TH1F("hmean_e","mean of nSigE for e;p_{T};mean",bin,pt);
  TH1F* hmean_pi = new TH1F("hmean_pi","mean of nSigE for pi;p_{T};mean",bin,pt);
  TH1F* hmean_k = new TH1F("hmean_k","mean of nSigE for k;p_{T};mean",bin,pt);
  TH1F* hmean_p = new TH1F("hmean_p","mean of nSigE for p;p_{T};mean",bin,pt);
  TH1F* hsigma_e = new TH1F("hsigma_e","sigma of nSigE for e",bin,pt); 
  TH1F* hsigma_pi = new TH1F("hsigma_pi","sigma of nSigE for pi",bin,pt); 
  TH1F* hsigma_p = new TH1F("hsigma_p","sigma of nSigE for p",bin,pt); 
  TH1F* hsigma_k = new TH1F("hsigma_k","sigma of nSigE for k",bin,pt); 
  TCanvas* c = new TCanvas("c","c");
  TCanvas* c2 = new TCanvas("c2","c2");
  // c->Divide(2,2);
  TLatex lax;
  TH1F* hpad = new TH1F("hpad","hpad",120,-10,15);
  double par[5]={1,1,1,1,1};
  // TF1* fitfun = new TF1(Form("fit",pt[j],pt[j+1]),funtofsamp,-4,4,5);
  // TF1* fitfune = new TF1(Form("fite",pt[j],pt[j+1]),funesamp,-4,4,2);
  // TF1* fitfunK = new TF1(Form("fitK",pt[j],pt[j+1]),funKsamp,-4,4,2);
  // TF1* fitfunpi = new TF1(Form("fitpi",pt[j],pt[j+1]),funpisamp,-4,4,2);
  // TF1* fitfunp = new TF1(Form("fitp",pt[j],pt[j+1]),funpsamp,-4,4,2);
  TF1* fitfun = new TF1("fit",funtofsamp,-10,10,5);
  TF1* fitfune = new TF1("fite",funesamp,-10,10,2);
  TF1* fitfunK = new TF1("fitK",funKsamp,-10,10,2);
  TF1* fitfunpi = new TF1("fitpi",funpisamp,-10,10,2);
  TF1* fitfunp = new TF1("fitp",funpsamp,-10,10,2);
  for (int j=0;j<bin;j++){
    cout<<pt[j]<<" "<<pt[j+1]<<endl;
    float mean=0, sigma=0;
    c->cd();
gPad->SetLogy();
    // c->cd(1);
    // projectionAndFit(he,hesamp[j] ,pt[j],pt[j+1],mean,sigma,"e");
    projectionAndFit(he,pt[j],pt[j+1],mean,sigma,"e",j);
    // hesamp[j]->Draw();
    // addpdf(pdf);
    // c->cd(2);
    // hpsamp[j] = projectionAndFit(hp,pt[j],pt[j+1],mean,sigma,"p",j);
    projectionAndFit(hp,pt[j],pt[j+1],mean,sigma,"p",j);
    // addpdf(pdf);
    // c->cd(3);
    projectionAndFit(hpi,pt[j],pt[j+1],mean,sigma,"pi",j);
    // addpdf(pdf);
    // c->cd(4);
    projectionAndFit(hk, pt[j],pt[j+1],mean,sigma,"K",j);
    // addpdf(pdf);   

    projectionAndFit(htof,pt[j],pt[j+1],mean,sigma,"tot",j);
    htofsamp[j]->Draw(); 
    // cout<<"ok"<<endl;
    // fitfun->SetParameters(par);
    fitfun->FixParameter(1,j); 
    fitfunp->FixParameter(1,j);
    fitfunK->FixParameter(1,j);
    fitfunpi->FixParameter(1,j);
    fitfune->FixParameter(1,j);
    fitfune->SetParLimits(0,0,1e31);
    fitfunpi->SetParLimits(0,0,1e31);
    fitfunp->SetParLimits(0,0,1e31);
    fitfunK->SetParLimits(0,0,1e31);
    // fitfun->SetParLimits(2,0,1e31);
    // fitfun->SetParLimits(0,0,1e31);
    // fitfun->SetParLimits(3,0,1e31);
    // fitfun->SetParLimits(4,0,1e31);
    htofsamp[j]->Fit(fitfune,"RB");
    // gPad->SaveAs("dddd.png");
    fitfun->SetParameter(0,fitfune->GetParameter(0));
    htofsamp[j]->Fit(fitfun,"RB");
    fitfun->Draw("same");
    fitfun->GetParameters(par); 
    cout<<par[0]<<" "<<par[1]<<" "<<par[2]<<" "<<par[3]<<" "<<par[4];
    fitfune->SetParameter(0,par[0]);
    fitfunpi->SetParameter(0,par[2]);
    fitfunK->SetParameter(0,par[3]);
    fitfunp->SetParameter(0,par[4]);
    fitfune->SetLineColor(kRed);
    fitfunpi->SetLineColor(kMagenta);
    fitfunK->SetLineColor(kGreen);
    fitfunp->SetLineColor(kBlue);
    fitfune->Draw("same");
    fitfunpi->Draw("same");
    fitfunp->Draw("same");
    fitfunK->Draw("same"); 
    addpdf(pdf);
    double etot =  hecut->Integral(hecut->FindBin(pt[j]),hecut->FindBin(pt[j+1])); 
    double meanpt = (pt[j]+pt[j+1])/2.0;
    double nsig = 0;
    if (meanpt>0.8) nsig=-1.2;    
    else nsig = (meanpt*2-2.8);
    double epure = fitfune->Integral(nsig,2);
    hpurity->SetBinContent(j+1,epure/etot);
  }
  gPad->SetLogy(0);
  hpurity->Draw();
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}
