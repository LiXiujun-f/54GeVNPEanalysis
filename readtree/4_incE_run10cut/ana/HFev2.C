#include "Energy.h"
#include "sPhenixStyle.h"
TH1F* getPhev2(TH3F* phe, TH3F* phels, int centL,int centH, double* ptedge,int const nBins,char* hname);
double getHFev2Err(double v1,double v1err,double n1,double n1err,double v2,double v2err,double n2,double n2err,double eff2,double eff2err);
double funv2(double*x, double *par);

void HFev2()
{
  SetsPhenixStyle(); 
  //book hists
  TH1F* hpurity;
  // TFile* fpurity = TFile::Open("",);

  // TFile* file = TFile::Open("incEv2.root");
  TFile* file = TFile::Open("incEv2_fulleta.root");
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf"); 
  pdf->Off();
  int const nbin = 12;
  double ptedge[nbin+1]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.8,4.0};
  // int const nbin = 5;
  // double ptedge[nbin+1]={0.2,0.4,0.6,1.0,2.0,4.0};
  int centL = 3,centH=9;
  // int centL = 3,centH=7;
  
  //hists
  TH3F* hphotols = (TH3F*)file->Get("hphoto_LS_hitcut"); 
  // TH3F* hphotols = (TH3F*)file->Get("hphoto_LS"); 
  // TH3F* hphoto = (TH3F*)file->Get("hphoto"); 
  TH3F* hphoto = (TH3F*)file->Get("hphoto_hitcut"); 
  TH3F* hphotoul = (TH3F*)file->Get("hphoto_hitcut")->Clone("hphotoul"); 
  hphoto->Add(hphotols,-1);
  TH1F* centcorr = (TH1F*)file->Get("hcentwg");
  double nEvents = centcorr->Integral(centL,centH);
  cout<<nEvents<<endl;
  TH1F* hPhe = new TH1F("hPheectra", "hPheectra;electron p_{T}(GeV);Counts",nbin,ptedge);
  
  for(int j=0;j<nbin;j++){
    int lbin = hphoto->GetYaxis()->FindBin(ptedge[j]);
    int hbin = hphoto->GetYaxis()->FindBin(ptedge[j+1]);
    TH1F* hpx = (TH1F*)hphoto->ProjectionX("hpx",lbin,hbin,centL,centH);
    TH1F* hpx_ul = (TH1F*)hphotoul->ProjectionX("hpx_ul",lbin,hbin,centL,centH);
    TH1F* hpxls = (TH1F*)hphotols->ProjectionX("hpx_ls",lbin,hbin,centL,centH);
    hpx_ls->SetLineColor(kBlue);
    // hpx_ul->SetLineColor();
    hpx->SetLineColor(kRed);

    hpx_ul->Draw();
    hpx_ls->Draw("same");
    hpx->Draw("same");
    TLegend* lpx = new TLegend(0.25,0.65,0.45,0.85);
    lpx->SetHeader("0-60%");
    lpx->AddEntry(hpx_ul,"UnLike","l");
    lpx->AddEntry(hpx_ls,"LikeSign","l");
    lpx->AddEntry(hpx,"UL-LS","l");
    lpx->Draw();
    TLatex lat;
    lat.SetTextSize(0.035);
    lat.DrawLatex(0.025,hpx_ul->GetMaximum()*0.6,Form("%.2f<p_{T}<%.2f GeV",ptedge[j],ptedge[j+1]));

    //hPhe->SetBinContent(j+1,hpx->Integral()/nEvents);
    int highbin=hpx->GetXaxis()->FindBin(0.15);
    // int lowbin=hpx->GetXaxis()->FindBin(0);
    hPhe->SetBinContent(j+1,hpx->Integral(1,highbin));
    addpdf(pdf);
  }
    // TH2F* hincEptCent= (TH2F*)file->Get("hIncEvsPtvsCent");
    // TH3F* hincEptCent= (TH3F*)file->Get("hIncEv2vsPtvsCent_hitcut");
    TH2F* hincEptCent= (TH2F*)file->Get("hIncEvsPtvsCent_hitcut");
    // hincEptCent->Draw("colz");
    // addpdf(pdf);
    TH1F* hincE = (TH1F*)hincEptCent->ProjectionX("hincEpt",  centL,centH);
    TH1F* hEsp4eff = (TH1F*)hincE->Clone("hEsp4eff");
    hincE = (TH1F*)hincE->Rebin(nbin,"hincE",ptedge);
    // hincE->Scale(1./hincE->GetBinWidth(1));
    // hincE->Scale(1./nEvents);
    // hPhe->Scale(1./nEvents);
    hincE->Draw();
    hPhe->Draw("same");
    hincE->SetLineColor(kRed);
    hincE->GetYaxis()->SetTitle("N/N_{events}");
    hincE->GetXaxis()->SetRangeUser(1e3,3e9);
    TLegend* lince = new TLegend(0.65,0.65,0.85,0.85);
    lince->AddEntry(hincE,"pass cut inclusive e","l");
    lince->AddEntry(hPhe,"reconstructed pho.c e","l");
    lince->Draw();
    gPad->SetLogy(1);
    addpdf(pdf);
    gPad->SetLogy(0);
    TH1F* hratio = (TH1F*)hPhe->Clone("ratio");
    hratio->Divide(hincE);
    hratio->Draw();
    addpdf(pdf);

  // inclusive electron
  // TProfile2D* pE2D = (TProfile2D*)file->Get("pIncEv2");
  TProfile2D* pE2D = (TProfile2D*)file->Get("pIncEv2_hitcut");
  TProfile2D* pPhoE2D = (TProfile2D*)file->Get("pTagEv2");
  TProfile2D* pPhoE2Dls = (TProfile2D*)file->Get("pTagEv2_LS");
  // TProfile*  pEv2;
  TProfile*  pPhoEls;
  TProfile*  pPhoEul;
  pEv2 = (TProfile*)pE2D->ProfileX(Form("pEv2_%d_%d",centL,centH),centL,centH); 
  pEv2 = (TProfile*)pEv2->Rebin(nbin,"pEv2",ptedge);
  pPhoEul = (TProfile*)pPhoE2D->ProfileX(Form("pPhoEul_%d_%d",centL,centH),centL,centH);
  pPhoEul = (TProfile*)pPhoEul->Rebin(nbin,"pPhoEul",ptedge);
  pPhoEls = (TProfile*)pPhoE2Dls->ProfileX(Form("pPhoEls_%d_%d",centL,centH),centL,centH);
  pPhoEls = (TProfile*)pPhoEls->Rebin(nbin,"pPhoEls",ptedge);
  
  // TH3F* hinc = (TH3F*)file->Get("hIncEv2vsPtvsCent_hitcut");
  // TH1F* pEv2 = getIncev2(hinc, centL,centH, ptedge,nbin ,"pEv2",pdf);
  // cout<<"???"<<endl;
  //photonic electron
  // TH3F* hphe = (TH3F*)file->Get("hPhEv2vsPtvsCent");
  // TH3F* hphels = (TH3F*)file->Get("hPhEv2vsPtvsCentLS");
  // TH1F* hphoE = getPhev2(hphe, hphels, centL,centH, ptedge,nbin ,"phoEv2",pdf);
  
  TH1F* hphoE = (TH1F*)pPhoEul->ProjectionX("hphoE");
  for (int i=1;i<=hphoE->GetNbinsX();i++)
  {
     double Nls = pPhoEls->GetBinEntries(i);
     double v2ls = pPhoEls->GetBinContent(i);
     double v2lserr = pPhoEls->GetBinError(i);
     double Nul = pPhoEul->GetBinEntries(i);
     double v2ul = pPhoEul->GetBinContent(i);   
     double v2ulerr = pPhoEul->GetBinError(i);   
     double v2PhE=0;
     if (Nul!=0 && (Nul-Nls)!=0) 
       v2PhE = (Nul*v2ul-Nls*v2ls)*1./(double)(Nul-Nls);
     cout<<Nls << "  "<< Nul << " " <<v2ls <<" "<<v2ul <<endl;
     hphoE->SetBinContent(i,v2PhE);
     
     double err = v2ulerr*v2ulerr*Nul*Nul+v2lserr*v2lserr*Nls*Nls;
     err = sqrt(err);
     err/=(1.*(Nul-Nls)); 
     hphoE->SetBinError(i,err);
  }

  TFile* fprevious = TFile::Open("phoEv2.root");
  TGraphErrors* gHFe200 = fprevious->Get("HFe200"); 
  // TGraphErrors* gPhoE62 = fprevious->Get("phoE62"); 
  TGraphErrors* gIncE62 = fprevious->Get("incE62"); 
  // TGraphErrors* gPhoE39 = fprevious->Get("phoE39"); 
  // TGraphErrors* gIncE39 = fprevious->Get("incE39"); 
  gIncE62->SetMarkerColor(kMagenta); 
  gIncE62->SetMarkerStyle(20); 
  // gIncE62->Draw("psame");
  // gPhoE62->SetMarkerColor(kOrange+5); 
  // gPhoE62->SetMarkerStyle(20); 
  // gPhoE62->Draw("psame");
  fprevious->Close();
  //
  TF1* phe62v2 = new TF1("phe62v2","pol4(0)",0,5);
  double par62[5] = {0.00338,0.18,-0.1,0.026,-0.0028};
  phe62v2->SetParameters(par62); 
  phe62v2->Draw();
  addpdf(pdf);
  // TF1* pheeff = new TF1("pheeff","pol2(0)",0,5);
  TF1* pheeff = new TF1("pheeff","pol3(0)",0,5);
  // double pareff[3]={0.119,0.097,-0.0046}; 
  // double pareff[3]={0.1225,0.0937,-0.00384}; 
  // double pareff[3]={0.107604,0.114847, -0.00713215}; 
  double pareff[4]={0.098,0.12,-0.0079,6e-5}; 
  pheeff->SetParameters(pareff);
  pheeff->Draw();
  TH1F* hPheEff = (TH1F*)hEsp4eff->Clone("hPheEff");
  for (int i=1;i<=hEsp4eff->GetNbinsX();i++)
  {
    double tmp = hPheEff->GetBinContent(i);
    tmp*=pheeff->Eval(hPheEff->GetBinCenter(i));
    hPheEff->SetBinContent(i,tmp);
  }
  TH1F* hPheEff = (TH1F*)hPheEff->Rebin(nbin,"hPheEff",ptedge);
  TH1F* hEsp4eff = (TH1F*)hEsp4eff->Rebin(nbin,"hEsp4eff",ptedge);
  hPheEff->Divide(hEsp4eff);
  hPheEff->Draw("same");
  addpdf(pdf);

  TH1F* hPheCor = (TH1F*)hPhe->Clone("hPheCor");  
  TH1F* hHFv2 = new TH1F("hHFv2","hHFv2",nbin,ptedge);
  TH1F* hHFv2_data = new TH1F("hHFv2_data","hHFv2_data",nbin,ptedge);

  for (int ib=0;ib<nbin;ib++) 
  {
    double nphe =  hPhe->GetBinContent(ib+1);
    // nphe/=pheeff->Eval(0.5*(ptedge[ib]+ptedge[ib+1]));
    // double eff =pheeff->Eval(hPhe->GetBinCenter(ib+1));
    // double eff =pheeff->Eval(ptedge[ib]);
    double eff = hPheEff->GetBinContent(ib+1);
    nphe/=eff;
    hPheCor->SetBinContent(ib+1,nphe);
    double nince =  hincE->GetBinContent(ib+1);
    double phev2 = phe62v2->Eval(hPhe->GetBinCenter(ib+1));
    // double phev2 = hphoE->GetBinContent(ib+1);
    
    double incev2 = pEv2->GetBinContent(ib+1);
    double hfev2 = (nince*incev2-phev2*nphe)/(nince-nphe);
    // double hfev2err = getHFev2Err(incev2, pEv2->GetBinError(ib+1), nince,0,phev2,phev2*0.06,nphe,sqrt(nphe),eff,eff*0.05); 
    // double hfev2err = getHFev2Err(incev2, pEv2->GetBinError(ib+1), nince*nEvents,sqrt(nince*nEvents),phev2,phev2*0.06,-1*nphe*nEvents,sqrt(nphe*nEvents),eff,eff*0.05); 
    double hfev2err = getHFev2Err(incev2, pEv2->GetBinError(ib+1), nince,sqrt(nince),phev2,phev2*0.06,-1*nphe,sqrt(nphe),eff,eff*0.05); 
    hHFv2->SetBinContent(ib+1,hfev2);
    hHFv2->SetBinError(ib+1,hfev2err);
  }

  gPad->SetLogy(1);
  hincE->SetLineColor(kBlack);
  hPhe->SetLineColor(kRed);
  hPheCor->SetLineColor(kBlue);
  hincE->Scale(1./nEvents);
  hPheCor->Scale(1./nEvents);
  hPhe->Scale(1./nEvents);
  hincE->GetYaxis()->SetRangeUser(1e-6,1);
  hincE->Draw();
  hPheCor->Draw("same");
  hPhe->Draw("same");
  TLegend* lsp = new TLegend(0.5,0.6,0.85,0.86);
  lsp->AddEntry(hincE,"inc. e","l");
  lsp->AddEntry(hPheCor,"corrected pho. e","l");
  lsp->AddEntry(hPhe,"pho. e","l");
  lsp->Draw();

  addpdf(pdf);
  
  hratio = (TH1F*)hincE->Clone("hratioCor");
  hratio->Add(hPheCor,-1); 
  hratio->Divide(hPheCor); 
  hratio->GetYaxis()->SetTitle("HF e/pho e");
  hratio->Draw();
  addpdf(pdf);

  gPad->SetLogy(0); 
  pEv2->SetMarkerStyle(21);
  pEv2->SetMarkerColor(kRed);
  pEv2->GetYaxis()->SetRangeUser(-0.1,0.2);
  pEv2->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  pEv2->GetYaxis()->SetTitle("v_{2}");
  pEv2->Draw();

  hphoE->SetMarkerStyle(21);
  hphoE->SetMarkerColor(kGreen);
  hphoE->Draw("psame");
  phe62v2->Draw("same"); 
  hHFv2->SetMarkerStyle(21);
  hHFv2->SetMarkerColor(kBlue);
  hHFv2->Draw("psame");   
  gHFe200->SetMarkerColor(kBlack); 
  gHFe200->SetMarkerStyle(20); 
  gHFe200->Draw("psame");
  //gIncE62->Draw("psame");
  TLegend* l = new TLegend(0.5,0.2,0.8,0.5);
  l->AddEntry(hphoE,"54GeV pho. e v2","pl");
  l->AddEntry(pEv2,"54GeV inc. e v2","pl");
  l->AddEntry(phe62v2,"62GeV pho. e v2","pl");
  l->AddEntry(hHFv2,"54GeV NPE v2","pl");
  l->AddEntry(gHFe200,"200GeV NPE v2","pl");
  l->Draw();
  addpdf(pdf);
  pdf->On();
  pdf->Close();
}

double funv2(double*x, double *par)
{
  return par[0]*(1+par[1]*cos(2*x[0]));
}
TH1F* getIncev2(TH3F* phe,  int centL,int centH, double* ptedge,int const nBins,char* hname,TPDF* pdf)
{
  TH1F* hphev2 = new TH1F(hname,hname,nBins,ptedge);
  TF1* fitfun = new TF1("fitfun","[0]*(1+[1]*cos(2*x))", 0,5); 
  for (int ib=0;ib<nBins;ib++)
  {
    TH1F* h = (TH1F*)phe->ProjectionX(Form("ince%d_%d",ptedge[ib],ptedge[ib+1]),phe->GetYaxis()->FindBin(ptedge[ib]),phe->GetYaxis()->FindBin(ptedge[ib+1]),centL,centH);
    // TH1F* hls = (TH1F*)phels->ProjectionX(Form("%d_%d_ls",ptedge[ib],ptedge[ib+1]),phe->GetYaxis()->FindBin(ptedge[ib]),phe->GetYaxis()->FindBin(ptedge[ib+1]),centL,centH);
    // h->Add(hls,-1); 
    h->Rebin(20);
    if (ptedge[ib]>2.5) h->Rebin();
    h->Fit(fitfun);
    h->Draw("p");
    drawLatex(0.4,0.8,Form("%0.f<p_{T}<%0.f inc. e",ptedge[ib],ptedge[ib+1]),0.05);
    addpdf(pdf);
    // gPad->Update();
    // gSystem->Sleep(500);
    hphev2->SetBinContent( ib+1, fitfun->GetParameter(1));
    hphev2->SetBinError( ib+1, fitfun->GetParError(1));
  } 
  return hphev2;
}
TH1F* getPhev2(TH3F* phe, TH3F* phels, int centL,int centH, double* ptedge,int const nBins,char* hname,TPDF* pdf)
{
  TH1F* hphev2 = new TH1F(hname,hname,nBins,ptedge);
  TF1* fitfun = new TF1("fitfun","[0]*(1+[1]*cos(2*x))", 0,5); 
  for (int ib=0;ib<nBins;ib++)
  {
    TH1F* h = (TH1F*)phe->ProjectionX(Form("phe%d_%d",ptedge[ib],ptedge[ib+1]),phe->GetYaxis()->FindBin(ptedge[ib]),phe->GetYaxis()->FindBin(ptedge[ib+1]),centL,centH);
    TH1F* hls = (TH1F*)phels->ProjectionX(Form("%d_%d_ls",ptedge[ib],ptedge[ib+1]),phe->GetYaxis()->FindBin(ptedge[ib]),phe->GetYaxis()->FindBin(ptedge[ib+1]),centL,centH);
    h->Add(hls,-1); 
    h->Rebin(30);
    if (ptedge[ib]>2.5) h->Rebin();
    h->Fit(fitfun);
    h->Draw("p");
    drawLatex(0.4,0.8,Form("%0.f<p_{T}<%0.f pho. e",ptedge[ib],ptedge[ib+1]),0.05);
    addpdf(pdf);
    // gPad->Update();
    // gSystem->Sleep(500);
    hphev2->SetBinContent( ib+1, fitfun->GetParameter(1));
    hphev2->SetBinError( ib+1, fitfun->GetParError(1));
  } 
  return hphev2;
}
double getHFev2Err(double v1,double v1err,double n1,double n1err,double v2,double v2err,double n2,double n2err,double eff,double eff2err)
{
  double nperr2 = eff*eff*n2err*n2err+n2*n2*eff2err*eff2err; 
  double sumerr = (n1*n1/(n1+n2)/(n1+n2))*v1err*v1err; //invE v2
  sumerr += (n2*n2/(n1+n2)/(n1+n2))*v2err*v2err; // phe v2
  sumerr += (v1*v1*n2*n2)/pow(n1+n2,4)*n1err*n1err;  //number of incE
  sumerr += pow(v2*n1,2)/pow(n1+n2,4)*nperr2;
  return sqrt(sumerr);
}

