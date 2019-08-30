#include "rootlogon.h"
#include <string>
TPDF* pdf;
TH1F* hesamp[100];
TH1F* hpsamp[100];
TH1F* hpisamp[100];
TH1F* hKsamp[100];
TH1F* htofsamp[100];

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
  c->Clear();
}
void projectionAndFit(TH2F* h, float lowpt, float highpt, float &mean,float &sigma,
                      float range1,float range2,float meanL,float meanH,float sigmaL,float sigmaH, string p)
{
  int lbin = h->GetXaxis()->FindBin(lowpt);
  int hbin = h->GetXaxis()->FindBin(highpt);
  TH1F* hx = (TH1F*)h->ProjectionY("hx",lbin,hbin);
  hx->Rebin(5);
  //hx->SetDirectory(0);
  gPad->SetLogy(1);
  hx->DrawCopy();
  TF1* fit  = new TF1("fit","gaus", range1,range2);
  hx->Fit(fit,"RN");
  fit->SetLineColor(kRed);
  mean = fit->GetParameter(1); 
  if (mean>meanH || mean <meanL) {
    fit->SetParLimits(2, meanL, meanH);
    hx->Fit(fit,"BRN");
  }
  sigma = fit->GetParameter(2); 
  if (sigma>sigmaH|| sigma<sigmaL) {
    fit->SetParLimits(2, sigmaL, sigmaH);
    hx->Fit(fit,"BRN");
  }
  fit->DrawCopy("same"); 
  mean = fit->GetParameter(1); 
  sigma = fit->GetParameter(2); 
  TLatex lat;
  lat.SetTextSize(0.035);
  lat.DrawLatexNDC(0.2,0.85,Form("%.2f<p_{T}<%.2f GeV %s",lowpt,highpt,p.c_str()));
  // addpdf(pdf);
}
void projAndFitForMg(TH2F* h, float lowpt, float highpt, float &mean,float &sigma,
                      float range1,float range2,float meanL,float meanH,float sigmaL,float sigmaH, string p)
{
  int lbin = h->GetXaxis()->FindBin(lowpt);
  int hbin = h->GetXaxis()->FindBin(highpt);
  TH1F* hx = (TH1F*)h->ProjectionY("hx",lbin,hbin);
  hx->Rebin(5);
  //hx->SetDirectory(0);
  gPad->SetLogy(1);
  hx->DrawCopy();
  hx->GetXaxis()->SetRangeUser(2,6);
  int meanbin =  hx->GetMaximumBin();
  TF1* fit  = new TF1("fit","gaus",-5 ,15);
    fit->SetParLimits(2, sigmaL, sigmaH);
    fit->SetParLimits(1, meanL, meanH);
  hx->GetXaxis()->SetRangeUser(hx->GetBinCenter(meanbin)-1.5,hx->GetBinCenter(meanbin)+1.5);
  TF1* fitall  = new TF1("fitall","gaus(0)+gaus(3)+gaus(6)",-5, 15);
  hx->Fit(fit,"BR");
  fit->SetLineColor(kRed);
  hx->GetXaxis()->SetRangeUser(-10,35);
  fit->DrawCopy("same"); 
  mean = fit->GetParameter(1);
  sigma = fit->GetParameter(2); 
  TLatex lat;
  lat.SetTextSize(0.035);
  lat.DrawLatexNDC(0.2,0.85,Form("%.2f<p_{T}<%.2f GeV %s",lowpt,highpt,p.c_str()));
  // addpdf(pdf);
}
void fitNsigE(){
  myStyle();
  // TCanvas* c = new TCanvas("c","c");
  TCanvas* c = new TCanvas("c","c",900,600);
  pdf = new TPDF("NsigE.pdf");
  pdf->Off();
  drawtitle(pdf,c,"Photonic electron plots");
  TFile* f = TFile::Open("PE.root");
  TH2F* hels = (TH2F*)f->Get("hnSigE_e_ls"); 
  TH2F* he = (TH2F*)f->Get("hnSigE_e"); 
  he->Add(hels,-1);
  he->SetDirectory(0);
  TH2F* hp = (TH2F*)f->Get("hnSigE_p");
  hp->SetDirectory(0);
  TH2F* hk = (TH2F*)f->Get("hnSigE_k");
  hk->SetDirectory(0);
  TH2F* htof = (TH2F*)f->Get("hnSigE_tof");
  htof->SetDirectory(0);
  TH2F* hpimg = (TH2F*)f->Get("hnSigE_piMg");
  hpimg->SetDirectory(0);
  // TH2F* hcut = (TH2F*)f->Get("hnSigE_Cut");
  // hcut->SetDirectory(0);
  // TH1F* hecut = (TH1F*)hcut->ProjectionX();
  // hecut->SetDirectory(0);
  f->Close();
  f = TFile::Open("Ks.root");
  TH2F* hpi = (TH2F*)f->Get("hnSigE_pi");
  TH2F* hpils = (TH2F*)f->Get("hnSigE_pi_ls");
  hpi->Add(hpils,-1);
  hpi->SetDirectory(0);
  f->Close();
  float pt[200]; //0.2-5
  int i=0;
  pt[0]=0.2;
  // while (pt[i]<1){
  //   pt[i+1]=pt[i]+0.02;
  //   i++;
  // }
  // cout<<pt[i]<<" "<<i<<endl;
  while  (pt[i]<2){
    pt[i+1]=pt[i]+0.025;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  //
  while  (pt[i]<3.2){
    pt[i+1]=pt[i]+0.025;
    i++;
  }
  cout<<pt[i]<<" "<<i<<endl;
  // while (pt[i]<4){
  //   pt[i+1]=pt[i]+0.2;
  //   i++;
  // }
  // cout<<pt[i]<<" "<<i<<endl;
  // while (pt[i]<5){
  //   pt[i+1]=pt[i]+1;
  //   i++;
  // }
  // cout<<pt[i]<<" "<<i<<endl;

  int  bin =i;
  TH1F* hpurity = new TH1F("hpurity", "hpurity;electron p_{T}(GeV);purity",bin,pt);
  
  c->Divide(3,2);
  int color[5]={kRed,kMagenta+1,kGreen+2,kBlue,kOrange+1};
  int line[5]={2,3,4,5,6};
  string pfitname[5]={"e","p","pi","k","pimg"};
  string plegname[5]={"e","p","#pi","K","merged #pi"};
  TH1F* hyield[5];
  TH1F* hmean[5];
  TH1F* hsigma[5];
  for (int ip=0;ip<5;ip++){
    hyield[ip] = new TH1F(Form("hyield_%s",pfitname[ip].c_str()),Form("fitting const. %s;%s p_{T};Const.",plegname[ip].c_str(),plegname[ip].c_str()),bin,pt);
    hsigma[ip] = new TH1F(Form("hsigma_%s",pfitname[ip].c_str()),Form("sigma of nSigE for %s;%s p_{T};n#sigma e",plegname[ip].c_str(),plegname[ip].c_str()),bin,pt);
    hmean[ip] = new TH1F(Form("hmean_%s",pfitname[ip].c_str()),Form("mean of nSigE for  %s;%s p_{T};mean",plegname[ip].c_str(),plegname[ip].c_str()),bin,pt);
  }
  TF1* ftot = new TF1("ftot","gausn(0)+gausn(3)+gausn(6)+gausn(9)+gausn(12)",-10,15);
  TF1* fpartical[5]; 
  for (int ip=0;ip<5;ip++){
    fpartical[ip] = new TF1(Form("f%s",pfitname[ip].c_str()),"gausn(0)",-10,15);
    fpartical[ip]->SetLineColor(color[ip]);
    fpartical[ip]->SetLineStyle(line[ip]);
  }
  TF1* fpimg = new TF1("fpimg","gausn(0)",-10,15);
  TLatex lax;
  TH1F* hpad = new TH1F("hpad","hpad",120,-10,15);
  TF1* frg_p = new TF1("frg_p","[0]+[1]/x/x+[2]/x+[3]*x",-10,35); 
  double frg_p_par[]={-17.8,-1.335,16.87,2.195};
  frg_p->SetParameters(frg_p_par);
  double frg_p_rg[2]={6,-7};
  TF1* frg_k = new TF1("frg_k","[0]+[1]/x/x+[2]/x+[3]*x",-10,35); 
  double frg_k_par[]={-12.5,0.289,5.469,1.946};
  frg_k->SetParameters(frg_k_par);
  double frg_k_rg[2]={6.5,-7.5};

  for (int j=0;j<bin;j++){
    cout<<pt[j]<<" "<<pt[j+1]<<endl;
    float mean[5]={0}, sigma[5]={0};
    c->cd(1);
    projectionAndFit(he,pt[j],pt[j+1],mean[0],sigma[0],-2,2,-0.6,0.1,0.6,1.1,"e");
    c->cd(2);
    projectionAndFit(hp,pt[j],pt[j+1],mean[1],sigma[1],frg_p->Eval(pt[j+1])-7,frg_p->Eval(pt[j])+6.5, -6, 30,0.5,2.5 ,"p");
    c->cd(3);
    projectionAndFit(hpi,pt[j],pt[j+1],mean[2],sigma[2],-10,10, -6, 0, 0.2, 1.2,"#pi");
    c->cd(4);
    projectionAndFit(hk, pt[j],pt[j+1],mean[3],sigma[3],frg_k->Eval(pt[j+1])-6.5,frg_k->Eval(pt[j])+7.5,-6,15,0.5,3,"K");
    c->cd(5);
    projAndFitForMg(hpimg,pt[j],pt[j+1],mean[4],sigma[4],0,10,3,5,1.1,1.4,"merged #pi");
    c->cd(6);
    double par[15];
    int lbin = htof->GetXaxis()->FindBin(pt[j]);
    int hbin = htof->GetXaxis()->FindBin(pt[j+1]);
    TH1F* htotsamp = (TH1F*)htof->ProjectionY("htotx",lbin,hbin); 
    // htotsamp->Rebin(5);
    double constL[5]={ 100,1e3,1e2,1e1,1e1};
    // double constL[5]={ 100,1e4,1e4,1e2,1e2};
    double constH[5]={ 5e7,5e9,1e9,5e8,1e6};
    // double constH[5]={ 5e6,1e9,1e8,5e7,1e5};
    //e

    if (pt[j]<0.65&&pt[j]>0.4) {
      constH[0]=1.1*exp(-5.28*pt[j]+16.256);
      constL[0]=0.8*exp(-5.28*pt[j]+16.256);
    }
    if (pt[j]<1.5&&pt[j]>0.8){
      constL[0]=exp(-3.104*pt[j]+14.6058);
      constH[0]=exp(-3.104*pt[j]+14.61);
    }
    //p
    if (pt[j]>1.7){
      constL[1]=0.3*exp(-1.72*pt[j]+16.1);
      constH[1]=1.8*exp(-1.72*pt[j]+16.1);
    }

    //pi
    // if (pt[j]>0.8){
    //   constL[2]=0.8*exp(-2.75*pt[j]+20);
    //   constH[2]=1.2*exp(-2.75*pt[j]+20);
    // }
    //pi
    if (pt[j]>1.6&&pt[j]<1.9){
      constL[2]=2.5/4*exp(-2.89*pt[j]+20);
      constH[2]=2.5/4*exp(-2.89*pt[j]+21);
    }
    if (pt[j]>0.5&&pt[j]<0.6){
      constL[2]=3.5e7;
      constH[2]=5e7;
    }
    // //K
    if (pt[j]<0.6){
      constL[3]=0.1*exp(10.4*pt[j]+6);
      constH[3]=10*exp(10.4*pt[j]+6);
    }
    if (pt[j]>0.4&&pt[j]<0.6){
      constL[3]=0.95*2.5/4*exp(10.4*pt[j]+6);
      constH[3]=1.1*2.5/4*exp(10.4*pt[j]+6);
    }
    if (pt[j]>0.8){
      constL[3]=0.005*exp(-1.74*pt[j]+16.1);
      constH[3]=exp(-1.74*pt[j]+16.1);
    }
    //merged pi
    if (pt[j]<0.5&&pt[j]>0.35)  {
      constL[4]=0.8*2.5/4*exp(10.7*pt[j]+4.69);
      constH[4]=3*2.5/4*exp(10.7*pt[j]+4.69);
    }
    for (int i=0;i<5;i++){
      ftot->FixParameter(i*3+1, mean[i]);
      ftot->FixParameter(i*3+2, sigma[i]);
      fpartical[i]->FixParameter(1,mean[i]);
      fpartical[i]->FixParameter(2,sigma[i]);
      // ftot->SetParLimits(i*3, 0,htotsamp->Integral()/htotsamp->GetBinWidth(1));
      ftot->SetParLimits(i*3, constL[i], constH[i]);
      hmean[i]->SetBinContent(j+1, mean[i]);
      hsigma[i]->SetBinContent(j+2, sigma[i]);
    }
    //e
    htotsamp->GetXaxis()->SetRangeUser(-2,2); 
    htotsamp->Fit(fpartical[0],"R");
    double e_const  = fpartical[0]->GetParameter(0);
    ftot->SetParameter(0,e_const);
    htotsamp->GetXaxis()->SetRangeUser(-10,20); 
    //p 
    // htotsamp->GetXaxis()->SetRangeUser( frg_p->Eval(pt[j+1])-7,frg_p->Eval(pt[j])+6.5); 
    // htotsamp->Fit(fpartical[1],"R");
    // double pi_const  = fpartical[1]->GetParameter(0);
    // ftot->SetParameter(3,pi_const);
    // htotsamp->GetXaxis()->SetRangeUser(-10,20); 
    //pi
    htotsamp->GetXaxis()->SetRangeUser(-10,-2); 
    htotsamp->Fit(fpartical[2],"R");
    double pi_const  = fpartical[2]->GetParameter(0);
    ftot->SetParameter(6,pi_const);
    htotsamp->GetXaxis()->SetRangeUser(-10,20); 
    // //K
    // htotsamp->GetXaxis()->SetRangeUser( frg_k->Eval(pt[j+1])-6.5,frg_k->Eval(pt[j])+7.5); 
    // htotsamp->Fit(fpartical[3],"R");
    // double k_const  = fpartical[3]->GetParameter(0);
    // ftot->SetParameter(9,k_const);
    // htotsamp->GetXaxis()->SetRangeUser(-10,20); 
    // //merged  pi
    // htotsamp->GetXaxis()->SetRangeUser(3,6); 
    // htotsamp->Fit(fpartical[4],"R");
    // double pimg_const = fpartical[4]->GetParameter(0);
    // ftot->SetParameter(12,pimg_const);
    // htotsamp->GetXaxis()->SetRangeUser(-10,20); 
    for (int ip=0;ip<5;ip++){
      ftot->SetParLimits(i*3, constL[ip], constH[ip]);
    }
    gPad->SetLogy();
    htotsamp->Draw();
    htotsamp->GetXaxis()->SetRangeUser(-10,5);
    htotsamp->Fit(ftot,"RB");
    htotsamp->GetXaxis()->SetRangeUser(-10,15);
    htotsamp->Fit(ftot,"RB");
    ftot->GetParameters(par);

    TLegend* leg = new TLegend(0.7,0.7,0.88,0.88);
    for (int ip=0;ip<5;ip++){
      fpartical[ip]->SetParameters(par+ip*3);  
      fpartical[ip]->Draw("same");
      leg->AddEntry(fpartical[ip],plegname[ip].c_str(),"l");
      hyield[ip]->SetBinContent(j+1, par[ip*3] );
      hyield[ip]->SetBinError(j+1, ftot->GetParError(ip*3) );
    }
    leg->Draw();
    c->cd();
    addpdf(pdf);
    double meanpt = 0.5*(pt[j]+pt[j+1]);
    double lowsigma = meanpt*3.5-2.8;
    double lowsigma = 0;
    // if(meanpt>0.8) lowsigma = -1.2;
    if(meanpt>0.8) lowsigma = 0;
    // float total = htotsamp->Integral(htotsamp->FindBin(lowsigma),htotsamp->FindBin(2));
    float total = ftot->Integral(lowsigma,2)/htotsamp->GetBinWidth(1);
    float electron = fpartical[0]->Integral(lowsigma,2)/htotsamp->GetBinWidth(1);
    float purity = electron*1.0/total;
    hpurity->SetBinContent(j+1,purity);
  }
  gPad->SetLogy(0);
  hpurity->Draw();
  hpurity->SetDirectory(0);
  addpdf(pdf);
  TCanvas* cp = new TCanvas("cp","cp");
  cp->Divide(2,2);
  cp->cd();
  TLegend* lp = new TLegend(0.2,0.2,0.8,0.8);
  for (int ip=0;ip<5;ip++){
    cp->cd();
    cp->cd(1);
    gPad->SetLogy();
    hyield[ip]->SetLineColor(color[ip]);
    hyield[ip]->SetMarkerColor(color[ip]);
    hyield[ip]->SetMarkerSize(0.6);
    hyield[ip]->GetYaxis()->SetRangeUser(10,1e9);
    hyield[ip]->Draw("same");
    hyield[ip]->SetDirectory(0);
    // gPad->Update();
    cp->cd(2);
    hsigma[ip]->SetLineColor(color[ip]);
    hsigma[ip]->GetYaxis()->SetRangeUser(0.5,2.5);
    hsigma[ip]->Draw("same");
    hsigma[ip]->SetDirectory(0);
    // gPad->Update();
    cp->cd(3);
    hmean[ip]->SetLineColor(color[ip]);
    hmean[ip]->GetYaxis()->SetRangeUser(-10,15);
    hmean[ip]->Draw("same");
    hmean[ip]->SetDirectory(0);
    lp->AddEntry(hyield[ip],plegname[ip].c_str(),"l");
 // gPad->Update();
  }

  cp->cd(4);
  gPad->Draw();
  lp->Draw();
  cp->cd();
  pdf->On();
  pdf->NewPage();
  cp->Update();
  pdf->Off(); 

  pdf->On();
  pdf->Close();
  TFile* fNsig = new TFile("Nsigma.root","recreate");
  hpurity->Write();
  for (int ip=0;ip<5;ip++){
    hyield[ip]->Write();
    hsigma[ip]->Write();
    hmean[ip]->Write();
  }
}
