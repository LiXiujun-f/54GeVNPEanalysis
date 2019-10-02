//---2019/10/01----add tag e pt bin for nHits and decayL and pair DCA
//---2019/09/29----add sys calculation TGraph
#include "sPhenixStyle.h"
int pagenumber=0; //global variable
void addpdf(TPDF* pdf,TCanvas* c)
{
  c->cd();
  pdf->On();
  c->Update();
  pagenumber++;
  pdf->SetTextSize(0.03);
  pdf->SetTextColor(kGray+1);
  pdf->TextNDC(0.975,0.05,Form("%d",pagenumber));
  pdf->NewPage();
  pdf->Off();
}
//store systematics
TGraph* gDcasys;
TGraph* gnHitsSys;
TGraph* gInvMassSys;
TGraph* gPairDca;
TGraph* gPartPtSys;

void NormHist(TH1* h1, int color, int x1=0, int x2=0);
void calSys(TH1F* hdata,TH1F* hmc, double x0, double x1, double x2,double x3);
void compare2PE_my()
{
  SetsPhenixStyle();
  TCanvas* c = new TCanvas("c","c");
  c->Divide(3,2);
  TPDF* pdf = new TPDF("qa2.pdf");
  pdf->Off();
  // TString realdata = "incEv2_addqa_loose0825.root";
  // TString realdata = "incEv2_0905.root";
  // TString realdata = "incEv2_0924.root";
  // TString realdata = "incEv2_0925.root";
  // TString realdata = "incEv2_0929.root";
  TString realdata = "incEv2_0930.root";
  // TString realdata = "checkMisPID.root";
  // TString realdata = "qa5.root";
  // TString mcdata = "embeddQa0825.root";
  // TString mcdata = "embeddQa_phicut0827.root";
  TString mcdata = "rescale_combine.root";
  // TString mcdata = "rescaleFile/rescale_gamma.root";
  // TString mcdata = "rescaleFile/rescale_pi0.root";
    // drawQaNhits("NFit",pdf,c,"incEv2_0905.root",mcdata);
  TString pi0Mc="rescaleFile/rescale_pi0.root", etaMc="rescaleFile/rescale_eta.root";
  TString gammaMc1="rescaleFile/rescale_gamma_pi0.root", gammaMc2="rescaleFile/rescale_gamma_eta.root",gammaMc3="rescaleFile/rescale_gamma_dirpho.root";
  drawElectronComponent(mcdata, pi0Mc,etaMc,gammaMc1,gammaMc2,gammaMc3,pdf,c);
  drawQaNhits("NFit",pdf,c,realdata,mcdata);
  drawNhitsTagePt("NFit",pdf,c,realdata,mcdata);
  drawQaDca("DCA",pdf,c,realdata,mcdata);
  drawPartPtEtaPhi("Partner e",pdf ,c ,realdata,mcdata); 
  drawInvMass("Inv mass of dielectron",pdf ,c ,realdata,mcdata); 
  drawPairDca("pair DCA",pdf,c,realdata,mcdata);
  drawDecayL("pair DecayLength",pdf,c,realdata,mcdata);
  //
  // //below are QA for the data
  drawDataQa("Tag e for real data",pdf,c,realdata,mcdata);
  drawTofMatchingEff("Tof match",pdf,c,realdata,mcdata,2,8);
  
  drawSysGraph(pdf,c );

  pdf->On();
  //  pdf->NewPage();
  pdf->Close(); 
  TFile* sysFromEmbedd = new TFile("sysFromEmbedd.root","recreate");
  gDcasys->Write();
  gnHitsSys->Write();
  gInvMassSys->Write();
  sysFromEmbedd->Close();
}
//----------------------------------------------------------------------
void drawElectronComponent(TString totalMc, TString pi0Mc, TString etaMc, TString gammaMc1,TString gammaMc2,TString gammaMc3, TPDF* pdf, TCanvas* c)
{
   // TFile* ftot = TFile::Open(totalMc.Data());
   // TFile* fpi0 = TFile::Open(pi0Mc.Data());
   // TFile* feta = TFile::Open(etaMc.Data());
   // TFile* fgamma = TFile::Open(gammaMc.Data());
   // TFile* file[4];
   // TH2F* hTagE2D[4];
   // TH2F* hRecoE2D[4];
   // TH1F* hTagE[4];
   // TH1F* hRecoE[4];
   // TString filename[4]={totalMc,pi0Mc,etaMc,gammaMc};
   // TString histname[4]={"total","pi0","eta","gamma"};
   // TString legendname[4]={"total","#pi^{0} Dalitz","#eta Dalitz","#gamma (#pi^{0}/#eta->#gamma,direct photon)"};
   // int color[4]={kBlack,kRed,kGreen+2,kBlue};

   int const nfiles=6;
   TFile* file[nfiles];
   TH2F* hTagE2D[nfiles];
   TH2F* hRecoE2D[nfiles];
   TH1F* hTagE[nfiles];
   TH1F* hRecoE[nfiles];
   TString filename[nfiles]={totalMc,pi0Mc,etaMc,gammaMc1,gammaMc2,gammaMc3};
   TString histname[nfiles]={"total","pi0","eta","gamma2pi0","gamma2eta","dirpho"};
   // TString legendname[nfiles]={"total","#pi^{0} Dalitz","#eta Dalitz","#gamma (#pi^{0}/#eta->#gamma,direct photon)"};
   TString legendname[nfiles]={"total","#pi^{0} Dalitz","#eta Dalitz","#pi^{0}->#gamma->e","#eta->#gamma->e","direct photon"};
   int color[nfiles]={kBlack,kRed,kGreen+2,kCyan+2,kBlue,kMagenta};

   //book hists
   for (int ip=0;ip<nfiles;ip++)
   {
      file[ip] = TFile::Open(filename[ip].Data());
      cout << filename[ip].Data()<< endl;
      hTagE2D[ip]=(TH2F*)file[ip]->Get("hTagElectron");
      // hTagE2D[ip]=(TH2F*)file[ip]->Get("hPi0Pt_weight");
      hTagE2D[ip]->SetDirectory(0);
      // if (ip==1) hTagE2D[ip]->Scale(2*99);
      // hRecoE2D[ip]=(TH2F*)file[ip]->Get("hTagElectronPassCut");
      hRecoE2D[ip]=(TH2F*)file[ip]->Get("hPi0Pt_weight");
      hRecoE2D[ip]->SetDirectory(0);
      hTagE[ip] = (TH1F*)hTagE2D[ip]->ProjectionX(Form("hTagE_%s",histname[ip].Data()),3,9);
      hTagE[ip]->SetDirectory(0);
      hRecoE[ip] = (TH1F*)hRecoE2D[ip]->ProjectionX(Form("hRecoE_%s",histname[ip].Data()),3,9);
      hRecoE[ip]->SetDirectory(0);
   }
   c->cd(); 
   gPad->SetLogy(1);
   //Draw total photonic electron
   TLegend* leg1 = new TLegend(0.6,0.5,0.85,0.85);
   TLegend* leg2 = new TLegend(0.6,0.5,0.85,0.85);
   for (int ip=0;ip<nfiles;ip++)
   {
      hTagE[ip]->SetMarkerColor(color[ip]);
      hTagE[ip]->SetLineColor(color[ip]);
      hTagE[ip]->Scale(1./hTagE[ip]->GetBinWidth(1));
      if (ip==0) hTagE[ip]->Draw();
      else if (ip>0) hTagE[ip]->Draw("same");
      leg1->AddEntry(hTagE[ip],legendname[ip].Data(),"lp");
   }
   leg1->Draw();
   addpdf(pdf,c);
   //Draw reconstructed electrons
   // c->Clear();
   for (int ip=0;ip<nfiles;ip++)
   {
      hRecoE[ip]->SetLineColor(color[ip]);
      hRecoE[ip]->SetMarkerColor(color[ip]);
      // if (ip==0) hRecoE[ip]->Draw(); 
      // else if (ip>0) hRecoE[ip]->Draw("same"); 
      hRecoE[ip]->Scale(1./hRecoE[ip]->GetBinWidth(1));
      hRecoE[ip]->Draw("same"); 
      leg2->AddEntry(hRecoE[ip],legendname[ip].Data(),"lp");
   }
   leg2->Draw();
   addpdf(pdf,c);
   gPad->SetLogy(0);

   //close the files
   for (int ip=0;ip<nfiles;ip++)
   {
      file[ip]->Close();
      delete hTagE[ip];
      delete hRecoE[ip];
      delete hTagE2D[ip];
      delete hRecoE2D[ip];
   }
}
//----------------------------------------------------------------------
void drawQaDca(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  cout <<"starting Dca QA..." << endl;
  c->Clear();
  drawLatex(0.3,0.6,"Partner e Dca projection");
  drawLatex(0.4,0.45,"in partner e pt bin");
  addpdf(pdf,c); 

  TFile* file = TFile::Open(mc.Data());
  if (head.Contains("NFit")) 
  {  
    // TString name[3]={"hnHits","hNFitsvsPt","hNFitsvsPt_LS"};
    TString name[3]={"hPartEnFits","hNFitsvsPt","hNFitsvsPt_LS"};
    // TString name[3]={"hNHit_3_0_10007_2","hNFitsvsPt","hNFitsvsPt_LS"};
  }
  else if (head.Contains("DCA"))
  { 
    // TString name[3]={"hDca_3_0_10007_2","hDcavsPt","hDcavsPt_LS"};
    // TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
    // TString name[3]={"hDCA","hDcavsPt","hDcavsPt_LS"};
    TString name[3]={"hPartEdca","hDcavsPt","hDcavsPt_LS"};
  }
  else cout<<"???"<<endl;
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  // file = TFile::Open("forQA.root");
  // file = TFile::Open("qa.root");
  // hDCAdata = (TH2F*)file->Get(name[1]);
  // hDCAdataLS = (TH2F*)file->Get(name[2]);
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->Draw();
  c->Divide(3,2);  
  int ipad=1;
  double x[20],y[20];
  int ibin=0;
  for (int ip=1;ip<=10;ip++)
  {
    TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip)),hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip)));;
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip,ip);
    cout << hDCAdata->GetXaxis()->GetBinLowEdge(ip)<<" "<< hDCAdata->GetXaxis()->GetBinUpEdge(ip) << endl;

    hrc->Scale(1./hrc->Integral());
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral());
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);
    
    //calculate the error
    x[ibin] = hDCAdata->GetXaxis()->GetBinCenter(ip);
    y[ibin] = calSys(hdata, hrc, 0.1 , 2,0.1, 3);
    ibin++;

    c->cd(ipad);
    hdata->DrawCopy();
    hrc->DrawCopy("same");
    drawLatex(0.6,0.6,Form("%0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip),hDCAdata->GetXaxis()->GetBinUpEdge(ip)),0.035);
    
    ipad++;
    
    if (ip%5==0)
    {
      c->cd(6);
      TLegend* leg = new TLegend(0.2,0.2,0.7,0.7);  
      leg->AddEntry(hrc,"MC e^{-}","lep");
      leg->AddEntry(hdata,"data e^{-}","lep");
      leg->SetHeader(head);
      leg->Draw();
      addpdf(pdf,c);
      ipad=1;
      c->Clear();
      c->Draw();
      c->Divide(3,2);
    }
  }
  gDcasys = new TGraph(ibin,x,y);
  gDcasys->SetName("gPartEDcaSys"); 
}
//----------------------------------------------------------------------
void drawQaNhits(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hPartEnFits","hNFitsvsPt","hNFitsvsPt_LS"};
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  // file = TFile::Open("forQA.root");
  // file = TFile::Open("qa.root");
  // hDCAdata = (TH2F*)file->Get(name[1]);
  // hDCAdataLS = (TH2F*)file->Get(name[2]);
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
 
  c->Clear();
  drawLatex(0.3,0.6,"Partner e nHits projection");
  drawLatex(0.4,0.45,"in partner e pt bin");
  addpdf(pdf,c); 

  c->Clear();
  c->Draw();
  c->Divide(3,2);  
  int ipad=1;
  double  x[20],y[20];
  int ibin=0;
   
  // for (int ip=1;ip<=20;ip++)
  for (int ip=1;ip<=10;ip++)
  {

    // if (ip<3) continue;
    // TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc",hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip*4+1)),hDCArc->GetYaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip*4+4)), hDCArc->GetZaxis()->FindBin(-0.8),hDCArc->GetZaxis()->FindBin(0.8) );;
    TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinLowEdge(ip)),hDCArc->GetXaxis()->FindBin(hDCAdata->GetXaxis()->GetBinUpEdge(ip)));;
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",ip,ip);
    // cout << hDCAdata->GetXaxis()->GetBinLowEdge(ip)<<" "<< hDCAdata->GetXaxis()->GetBinUpEdge(ip) << endl;

    hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(21),hrc->GetXaxis()->FindBin(45)));
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(21),hdata->GetXaxis()->FindBin(45)));
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    c->cd(ipad);
    hrc->DrawCopy();
    hdata->DrawCopy("same");
    drawLatex(0.2,0.6,Form("Part e: %0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip),hDCAdata->GetXaxis()->GetBinUpEdge(ip)),0.035);
    
    ipad++;
    
    if (ip%5==0)
    {
      c->cd(6);
      TLegend* leg = new TLegend(0.2,0.2,0.7,0.7);  
      leg->AddEntry(hrc,"MC e^{-}","lep");
      leg->AddEntry(hdata,"data e^{-}","lep");
      leg->SetHeader(head);
      leg->Draw();
      addpdf(pdf,c);
      ipad=1;
      c->Clear();
      c->Draw();
      c->Divide(3,2);
    }
  }
  TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc");
  TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata");
  // cout << "nHits sys(full pt bin):  " <<calSys(hdata,hrc,20,50,25,50)<<endl;
}
//----------------------------------------------------------------------
void drawNhitsTagePt(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hPartEnFitsTagEpt","hNFitsvsTagPt","hNFitsvsTagPt_LS"};
  TH2F* hDCArc = (TH2F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  // file = TFile::Open("forQA.root");
  // file = TFile::Open("qa.root");
  // hDCAdata = (TH2F*)file->Get(name[1]);
  // hDCAdataLS = (TH2F*)file->Get(name[2]);
  file = TFile::Open(real.Data());
  TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
  TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->Draw();
  drawLatex(0.3,0.6,"Partner e nHits projection");
  drawLatex(0.4,0.45,"in Tag e pt bin");
  addpdf(pdf,c);
  c->Clear();
  c->Divide(3,2);  
  int ipad=1;
  double  x[20],y[20];
  int ibin=0;
  
  int const nbins = 7;
  double ptedge[nbins+1]={0.25,0.3,0.4,0.6,1.0,1.4,1.8,3};

  // for (int ip=1;ip<=20;ip++)
  for (int ip=0;ip<=nbins;ip++)
  {
    TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc",hDCArc->GetXaxis()->FindBin(ptedge[ip]),hDCArc->GetXaxis()->FindBin(ptedge[ip+1]));;
    TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata",hDCAdata->GetXaxis()->FindBin(ptedge[ip]),hDCAdata->GetXaxis()->FindBin(ptedge[ip+1]));
    // cout << hDCAdata->GetXaxis()->GetBinLowEdge(ip)<<" "<< hDCAdata->GetXaxis()->GetBinUpEdge(ip) << endl;

    hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(21),hrc->GetXaxis()->FindBin(45)));
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(21),hdata->GetXaxis()->FindBin(45)));
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    y[ibin]=calSys(hdata,hrc,20,50,25,50);
    x[ibin]=hDCAdata->GetXaxis()->GetBinCenter(ip);
    ibin++;

    c->cd(ipad);
    hrc->DrawCopy();
    hdata->DrawCopy("same");
    drawLatex(0.2,0.6,Form("Tag e: %0.1f<p_{T}<%0.1f",hDCAdata->GetXaxis()->GetBinLowEdge(ip),hDCAdata->GetXaxis()->GetBinUpEdge(ip)),0.035);
    ipad++;
    
    if (ip%5==0)
    {
      c->cd(6);
      TLegend* leg = new TLegend(0.2,0.2,0.7,0.7);  
      leg->AddEntry(hrc,"MC e^{-}","lep");
      leg->AddEntry(hdata,"data e^{-}","lep");
      leg->SetHeader(head);
      leg->Draw();
      addpdf(pdf,c);
      ipad=1;
      c->Clear();
      c->Draw();
      c->Divide(3,2);
    }
  }
  gnHitsSys = new TGraph(ibin,x,y);
  gnHitsSys->SetName("gPartEnHitsSys");
  TH1* hrc = (TH1*)hDCArc->ProjectionY("hrc");
  TH1* hdata = (TH1*)hDCAdata->ProjectionY("hdata");
  cout << "nHits sys:  " <<calSys(hdata,hrc,20,50,25,50)<<endl;
}
//----------------------------------------------------------------------
void drawPartPtEtaPhi(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  c->Clear();
  drawLatex(0.3,0.6,"Partner e Eta/Phi projection");
  drawLatex(0.4,0.45,"in part e/Tag e pt bin");
  addpdf(pdf,c); 

  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hPartEptetaphi","hPartEptetaphi","hPartEptetaphi_LS"};
  // TString name[3]={"hPartEptetaphi","hPartEptetaphi_Dz","hPartEptetaphi_Dz_LS"};
  TH3F* hDCArc = (TH3F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH3F* hDCAdata = (TH3F*)file->Get(name[1]);
  TH3F* hDCAdataLS = (TH3F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  int const nbins = 9;
  double ptedge[nbins+1]={0.25,0.3,0.4,0.6,1.0,1.4,1.8,2.2,2.6,3};

  // c->Clear();
  TH1* hrc_x = (TH1*)hDCArc->ProjectionX("hrc_x");
  TH1* hdata_x = (TH1*)hDCAdata->ProjectionX("hdata_x");
  NormHist(hrc_x,kBlue,hrc_x->GetXaxis()->FindBin(0.4),hrc_x->GetXaxis()->FindBin(2.5));
  NormHist(hdata_x,kRed,hdata_x->GetXaxis()->FindBin(0.4),hdata_x->GetXaxis()->FindBin(2.5));
  hrc_x->DrawCopy();
  hdata_x->DrawCopy("same");
  drawLatex(0.6,0.75,Form("%s p_{T}(full range)", head.Data()));
  gPad->SetLogy(1);
  addpdf(pdf,c);
  gPad->SetLogy(0);
  cout <<"partner e pt cut sys "<< calSys(hdata_x,hrc_x,0.25,5,0.3,5 )<< endl;
  c->Clear();
  
  c->cd();
  int numOfPlots=4;
  c->Divide(2,2);
  bool finishThispage=false;
  for (int i=0;i<nbins;i++)
  {
    c->cd(i%numOfPlots+1);
    TH1* hrc_y = (TH1*)hDCArc->ProjectionY("hrc_y", hDCArc->GetXaxis()->FindBin(ptedge[i]),  hDCArc->GetXaxis()->FindBin(ptedge[i+1]), 1, hDCArc->GetNbinsZ());
    TH1* hdata_y = (TH1*)hDCAdata->ProjectionY("hdata_y",hDCAdata->GetXaxis()->FindBin(ptedge[i]), hDCAdata->GetXaxis()->FindBin(ptedge[i]),1,hDCAdata->GetNbinsZ());
    hdata_y->Rebin();
    if (ptedge[i]>1.7) hdata_y->Rebin(5); 
    NormHist(hrc_y,kBlue);
    NormHist(hdata_y,kRed);
    hrc_y->DrawCopy();
    hdata_y->DrawCopy("same");
    // drawLatex(0.2,0.6,Form("%s Eta", head.Data()));
    drawLatex(0.2,0.8,Form("%s Eta %0.1f<pt<%0.1f", head.Data(), ptedge[i], ptedge[i+1]),0.05);
    if ( i==(nbins-1) ) finishThispage=true;
    else if ( i%numOfPlots==(numOfPlots-1) )  finishThispage = true;
    else finishThispage=false;
    if (finishThispage) { addpdf(pdf,c); c->Clear();c->Divide(2,2);}
  }

  // c->cd(2);
  c->Clear();
  c->cd();
  TH1* hrc_z = (TH1*)hDCArc->ProjectionZ("hrc_z",1,hDCArc->GetNbinsX(),hDCArc->GetYaxis()->FindBin(-1),hDCArc->GetYaxis()->FindBin(1));
  TH1* hdata_z = (TH1*)hDCAdata->ProjectionZ("hdata_z",1,hDCAdata->GetNbinsX(),hDCAdata->GetYaxis()->FindBin(-1),hDCAdata->GetYaxis()->FindBin(1));
  hrc_z->Rebin(4);
  NormHist(hrc_z,kBlue);
  NormHist(hdata_z,kRed);
  hrc_z->DrawCopy();
  hdata_z->DrawCopy("same");
  drawLatex(0.2,0.8,Form("%s #phi", head.Data()));
  addpdf(pdf,c);
}
//----------------------------------------------------------------------
void drawPairDca(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
  TFile* file = TFile::Open(mc.Data());
  TString name[3]={"hDcaPair","hPairDCA","hPairDCALS"};
  TH3F* hDCArc = (TH3F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH3F* hDCAdata = (TH3F*)file->Get(name[1]);
  TH3F* hDCAdataLS = (TH3F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc");;
  TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata");
 
  hrc->Scale(1./hrc->Integral());
  hrc->Scale(1./hrc->GetBinWidth(1));
  hdata->Scale(1./hdata->Integral());
  hdata->Scale(1./hdata->GetBinWidth(1));
  hrc->SetMarkerColor(kBlue);
  hrc->SetLineColor(kBlue);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);

  double sys = calSys(hdata,hrc,0,1,0,1.5);
  cout <<"pair DCA sys(full pt): (1.5) "<< sys << endl;  
  
  double sys = calSys(hdata,hrc,0,1,0,0.8);
  cout <<"pair DCA sys(full pt): (0.8) "<< sys << endl;  

  hrc->DrawCopy();
  hdata->DrawCopy("same");
  drawLatex(0.6,0.6,"pair DCA (full p_{T} range)",0.035);
  addpdf(pdf,c); 

  int const nbins = 7;
  double ptedge[nbins+1]={0.25,0.3,0.4,0.6,1.0,1.4,1.8,3};

  c->Clear();
  int iv=3,ih=2;
  c->Divide(iv,ih);
  int ipad=1;
  double x[nbins],y[nbins];
  for (int ip=0;ip<nbins;ip++)
  {
    c->cd(ipad);
    TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc", hDCArc->GetYaxis()->FindBin(ptedge[ip]),hDCArc->GetYaxis()->FindBin(ptedge[ip+1]),3,6);
    TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata", hDCAdata->GetYaxis()->FindBin(ptedge[ip]),hDCAdata->GetYaxis()->FindBin(ptedge[ip+1]),3,6);
    hrc->Scale(1./hrc->Integral());
    hrc->Scale(1./hrc->GetBinWidth(1));
    hdata->Scale(1./hdata->Integral());
    hdata->Scale(1./hdata->GetBinWidth(1));
    hrc->SetMarkerColor(kBlue);
    hrc->SetLineColor(kBlue);
    hdata->SetLineColor(kRed);
    hdata->SetMarkerColor(kRed);

    y[ip] = calSys(hdata,hrc,0,1,0,1.5);
    x[ip] = 0.5*(ptedge[ip]+ptedge[ip+1]);
    // cout <<"pair DCA sys: (1.5) "<< sys << endl;  

    hrc->DrawCopy();
    hdata->DrawCopy("same");
    drawLatex(0.4,0.7,Form("pair DCA %0.1f<pt<%0.1f", ptedge[ip],ptedge[ip+1]),0.035);
    if (ipad==iv*ih || ip==nbins-1) 
    {  
      addpdf(pdf,c); 
      c->Clear();
      c->Divide(iv,ih);
      ipad=1;
    }
    ipad++;
  }
  gPairDca = new TGraph(nbins, x, y);
  gPairDca->SetName("gPairDca");
}
//----------------------------------------------------------------------
void drawDecayL(TString head, TPDF* pdf, TCanvas* c,TString real,TString mc)
{
   TFile* file = TFile::Open(mc.Data());
   TString name[3]={"hPairDecayL","hDecayL","hDecayL_LS"};
   TH2F* hDCArc = (TH2F*)file->Get(name[0]);
   hDCArc->SetDirectory(0);
   file->Close();
   file = TFile::Open(real.Data());
   TH2F* hDCAdata = (TH2F*)file->Get(name[1]);
   TH2F* hDCAdataLS = (TH2F*)file->Get(name[2]);

   hDCAdataLS->SetDirectory(0);
   hDCAdata->SetDirectory(0);
   file->Close();
   hDCAdata->Add(hDCAdataLS,-1);
   
   c->Clear();
   TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc");;
   TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata");
   
   hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(0),hrc->GetXaxis()->FindBin(2)));
   hrc->Scale(1./hrc->GetBinWidth(1));
   hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(0),hdata->GetXaxis()->FindBin(2)));
   hdata->Scale(1./hdata->GetBinWidth(1));
   hrc->SetMarkerColor(kBlue);
   hrc->SetLineColor(kBlue);
   hdata->SetLineColor(kRed);
   hdata->SetMarkerColor(kRed);

   hrc->DrawCopy();
   hdata->DrawCopy("same");

   TLegend* leg = new TLegend(0.6,0.7,0.85,0.85);
   leg->AddEntry(hdata, "data","l");
   leg->AddEntry(hrc, "embedding","l");
   leg->Draw();
   drawLatex(0.6,0.6,Form("%s",head.Data()),0.035);
   addpdf(pdf,c); 

   int const nbins = 7;
   double ptedge[nbins+1]={0.25,0.3,0.4,0.6,1.0,1.4,1.8,3};
   c->Clear();
   c->Divide(3,2);
   int ipad=1;
   for (int ip=0;ip<nbins;ip++)
   {
      c->cd(ipad);
      TH1* hrc = (TH1*)hDCArc->ProjectionX("hrc", hDCArc->GetYaxis()->FindBin(ptedge[ip]),hDCArc->GetYaxis()->FindBin(ptedge[ip+1]));
      TH1* hdata = (TH1*)hDCAdata->ProjectionX("hdata",hDCAdata->GetYaxis()->FindBin(ptedge[ip]),hDCAdata->GetYaxis()->FindBin(ptedge[ip+1]));
      
      hrc->Scale(1./hrc->Integral(hrc->GetXaxis()->FindBin(0),hrc->GetXaxis()->FindBin(2)));
      hrc->Scale(1./hrc->GetBinWidth(1));
      hdata->Scale(1./hdata->Integral(hdata->GetXaxis()->FindBin(0),hdata->GetXaxis()->FindBin(2)));
      hdata->Scale(1./hdata->GetBinWidth(1));
      hrc->SetMarkerColor(kBlue);
      hrc->SetLineColor(kBlue);
      hdata->SetLineColor(kRed);
      hdata->SetMarkerColor(kRed);
      hrc->DrawCopy();
      hdata->DrawCopy("same");

      TLegend* leg = new TLegend(0.6,0.7,0.85,0.85);
      leg->AddEntry(hdata,"data","l");
      leg->AddEntry(hrc,"embedding","l");
      leg->Draw();
      drawLatex(0.6,0.6,Form("%s %0.1f<pt<%0.1f",head.Data(),ptedge[ip],ptedge[ip+1]),0.035);
      if (ipad==6 || ip==nbins-1)
      {
        addpdf(pdf,c); 
        c->Clear();
        c->Divide(3,2);
        ipad=1;
      }
      ipad++;
   }
}
//----------------------------------------------------------------------
void drawDataQa(TString head,TPDF* pdf,TCanvas* c, TString real, TString mc)
{
   gStyle->SetPalette(1);
   TFile* f = TFile::Open(real.Data()); 
   TH1F* hPhi = (TH1F*)f->Get("hPhi");
   TH1F* hPhi_hitcut = (TH1F*)f->Get("hPhi_hitcut");
   TH1F* hPhi_allcut = (TH1F*)f->Get("hPhi_allcut");
   hPhi_allcut->SetLineColor(kBlue);
   hPhi_hitcut->SetLineColor(kRed); 
   c->Clear();
   hPhi->GetYaxis()->SetRangeUser(0 ,hPhi->GetMaximum()*1.1 );
   hPhi->Draw();
   hPhi_hitcut->Draw("same");
   hPhi_allcut->Draw("same");
   TLegend* leg = new TLegend(0.5,0.7,0.88,0.92);
   leg->AddEntry(hPhi , "basic cuts","l" );
   leg->AddEntry(hPhi_hitcut , "first hit <70cm","l" );
   leg->AddEntry(hPhi_allcut, "rm 1.25<|#phi|<1.95 && first hit <70cm","l" );
   leg->Draw();
   drawLatex(0.5,0.6,"Tag e #phi distribution (data)"); 
   addpdf(pdf,c);
   
   c->Clear();
   TH1F* hEta = (TH1F*)f->Get("hEta"); 
   hEta->Draw();
   drawLatex(0.6,0.6, "inclusive e #eta (data)");
   addpdf(pdf,c);
   // c->Clear();
   // new tag e eta distribution array
   THnSparseF* hEtaArray = (THnSparseF*)f->Get("hPartETagEetapt");
   hEtaArray->GetAxis(3)->SetTitle("partner e #eta");
   THnSparseF* hEtaArrayLS = (THnSparseF*)f->Get("hPartETagEetapt_LS");
   cout << "add likesign" << endl;
   // hEtaArray->Add(hEtaArrayLS,-1);
   int numOfPlots=4;
   c->Divide(2,2);
   // bool finishThispage=false;
   int const nbins = 8;
   double ptedge[nbins+1]={0.2,0.3,0.4,0.6,1.0,1.4,1.8,2.2,2.5};
   for (int i=0;i<nbins;i++)
   {
     c->Clear();
     c->cd();
     c->Divide(2,2);
     cout <<"tag e : "<<ptedge[i]<<" "<< hEtaArray->GetAxis(0)->FindBin(ptedge[i])<<" "<< hEtaArray->GetAxis(0)->FindBin(ptedge[i+1])<<endl;
     hEtaArray->GetAxis(0)->SetRange(hEtaArray->GetAxis(0)->FindBin(ptedge[i]),hEtaArray->GetAxis(0)->FindBin(ptedge[i+1]));
     hEtaArrayLS->GetAxis(0)->SetRange(hEtaArray->GetAxis(0)->FindBin(ptedge[i]),hEtaArray->GetAxis(0)->FindBin(ptedge[i+1]));

     // hEtaArray->GetAxis(1)->SetRange(hEtaArray->GetAxis(1)->FindBin(0),hEtaArray->GetAxis(1)->FindBin(2.5));
     // hEtaArrayLS->GetAxis(1)->SetRange(hEtaArray->GetAxis(1)->FindBin(0),hEtaArray->GetAxis(1)->FindBin(2.5));
     // hEtaArray->GetAxis(2)->SetRange(hEtaArray->GetAxis(2)->FindBin(-1),hEtaArray->GetAxis(2)->FindBin(1));
     // hEtaArrayLS->GetAxis(2)->SetRange(hEtaArray->GetAxis(2)->FindBin(-1),hEtaArray->GetAxis(2)->FindBin(1));
     // hEtaArray->GetAxis(3)->SetRange(hEtaArray->GetAxis(3)->FindBin(-1),hEtaArray->GetAxis(3)->FindBin(1));
     // hEtaArrayLS->GetAxis(3)->SetRange(hEtaArray->GetAxis(3)->FindBin(-1),hEtaArray->GetAxis(3)->FindBin(1));
      
     TH2D* heta2d = (TH2D*)hEtaArray->Projection(2,3);
     TH2D* heta2dls = (TH2D*)hEtaArrayLS->Projection(2,3);
     heta2d->Add(heta2dls,-1);
     c->cd(1);
     gPad->SetLogz();
     heta2d->DrawCopy("COLZ");
     gPad->SetRightMargin(0.15);
     drawLatex(0.2,0.85,Form("Tag e %0.1f<pt<%0.1f", ptedge[i], ptedge[i+1]),0.05);
     
     c->cd(2);
     TH1D* hpartEpt = (TH1D*)hEtaArray->Projection(1);
     TH1D* hpartEptls = (TH1D*)hEtaArrayLS->Projection(1);
     hpartEpt->Add(hpartEptls,-1);
     // hpartEpt->DrawCopy();
     heta2d->Scale(-1);
     heta2d->DrawCopy("colz");
     gPad->SetRightMargin(0.15);
     // gPad->SetLeftMargin();
     gPad->SetLogz();
     delete hpartEpt;
     delete hpartEptls;
     // drawLatex(0.4,0.85,Form("Tag e %0.1f<pt<%0.1f, part. e p_{T}", ptedge[i], ptedge[i+1]),0.05);
     drawLatex(0.2,0.85,"left plot scale with -1",0.05);
     c->cd(3);
     TH1F* hTagEeta = (TH1F*)hEtaArray->Projection(2);
     TH1F* hTagEetals = (TH1F*)hEtaArrayLS->Projection(2);
     hTagEeta->Add(hTagEetals,-1);
     hTagEeta->DrawCopy();
     delete hTagEeta;
     delete hTagEetals;
     drawLatex(0.2,0.85,Form("Tag e %0.1f<pt<%0.1f, tag e #eta", ptedge[i], ptedge[i+1]),0.05);
     c->cd(4);
     TH1F* hPartEeta = (TH1F*)hEtaArray->Projection(3);
     TH1F* hPartEetals = (TH1F*)hEtaArrayLS->Projection(3);
     hPartEeta->Add(hPartEetals,-1);
     // TH1F* hPartEeta = (TH1F*)heta2d->ProjectionX();
     hPartEeta->DrawCopy();
     drawLatex(0.2,0.85,Form("Tag e %0.1f<pt<%0.1f, part. e #eta", ptedge[i], ptedge[i+1]),0.05);
     delete hPartEeta;
     delete hPartEetals;
     addpdf(pdf,c);
     delete heta2d;
     delete heta2dls;

   }
   f->Close();
   c->Clear();
}
//----------------------------------------------------------------------
void drawInvMass(TString head,TPDF* pdf,TCanvas* c, TString real, TString mc)
{
  cout <<"drawInvMass..." << endl;
  TFile* file = new TFile(mc.Data());
  TString name[3]={"hMassDiE","hphoto","hphoto_LS"};
  TH3F* hDCArc = (TH3F*)file->Get(name[0]);
  hDCArc->SetDirectory(0);
  file->Close();
  file = TFile::Open(real.Data());
  TH3F* hDCAdata = (TH3F*)file->Get(name[1]);
  TH3F* hDCAdataLS = (TH3F*)file->Get(name[2]);

  hDCAdataLS->SetDirectory(0);
  hDCAdata->SetDirectory(0);
  file->Close();
  hDCAdata->Add(hDCAdataLS,-1);
  
  c->Clear();
  c->cd();
  int numOfPlots=4;
  c->Divide(2,2);
  bool finishThispage=false;

  // int const nbins = 9;
  // double ptedge[nbins+1]={0.2,0.3,0.4,0.6,1.0,1.4,1.8,2.2,2.6,3.2};
  
  int const nbins = 7;
  double ptedge[nbins+1]={0.2,0.3,0.4,0.6,1.0,1.4,1.8,3.2};

  double x[nbins],y[nbins];
  for (int i=0;i<nbins;i++)
  {
    c->cd(i%numOfPlots+1);
    TH1* hrc_x = (TH1*)hDCArc->ProjectionX("hrc_X", hDCArc->GetYaxis()->FindBin(ptedge[i]),  hDCArc->GetYaxis()->FindBin(ptedge[i+1]), 1, hDCArc->GetNbinsZ());
    TH1* hdata_x = (TH1*)hDCAdata->ProjectionX("hdata_X",hDCAdata->GetYaxis()->FindBin(ptedge[i]), hDCAdata->GetYaxis()->FindBin(ptedge[i]),1,hDCAdata->GetNbinsZ());
    NormHist(hrc_x,kBlue);
    NormHist(hdata_x,kRed);
    hrc_x->GetXaxis()->SetRangeUser(0,0.2);
    hrc_x->DrawCopy();
    hdata_x->DrawCopy("same");
    // drawLatex(0.2,0.6,Form("%s Eta", head.Data()));
    drawLatex(0.2,0.85,Form("%s, Tag e %0.1f<pt<%0.1f", head.Data(), ptedge[i], ptedge[i+1]),0.05);
    // addpdf(pdf);
    if ( i==(nbins-1) ) finishThispage=true;
    else if ( i%numOfPlots==(numOfPlots-1) )  finishThispage = true;
    else finishThispage=false;
    if (finishThispage) { addpdf(pdf,c); c->Clear();c->Divide(2,2);}
    x[i]=(ptedge[i]+ptedge[i+1])*0.5;
    cout << "pt " << x[i] << endl;
    y[i]=calSys(hdata_x, hrc_x, 0,0.1,0,0.15 );
  }
  gInvMassSys = new TGraph( nbins, x, y);
  gInvMassSys->SetName("gInvMassSys");
  gInvMassSys->GetXaxis()->SetTitle("Tag e p_{T} [GeV]");
}
//----------------------------------------------------------------------
void drawTofMatchingEff(TString head,TPDF* pdf,TCanvas* c, TString real, TString mc,int centL,int centH)
{
  TFile* f = TFile::Open(real.Data()); 
  TH2F* hTPC= (TH2F*)f->Get("hPartETpc");
  TH2F* hTof= (TH2F*)f->Get("hPartETof");
  TH2F* hTofls= (TH2F*)f->Get("hPartETofLS");
  TH2F* hTPCls= (TH2F*)f->Get("hPartETpcLS");
  hTPC->Add(hTPCls,-1);
  hTof->Add(hTofls,-1);
  TH1F* hTPC_pt = (TH1F*)hTPC->ProjectionX(Form("TPC_%d_%d",centL,centH), hTPC->GetYaxis()->FindBin(centL),hTPC->GetYaxis()->FindBin(centH));
  TH1F* hTof_pt = (TH1F*)hTof->ProjectionX(Form("Tof_%d_%d",centL,centH), hTPC->GetYaxis()->FindBin(centL),hTPC->GetYaxis()->FindBin(centH));
  c->Clear();
  hTof_pt->Divide(hTPC_pt);
  // hTof_pt->SetDirectory(0);
  hTof_pt->Draw();
  drawLatex(0.5,0.75,"Tof matching Eff."); 
  drawLatex(0.5,0.85,"Use partner electron"); 
  addpdf(pdf,c);
  hTof_pt->SaveAs(Form("Tof_%d_%d.root",centL,centH));
 
  f->Close();
  c->Clear();
}
//----------------------------------------------------------------------
void NormHist(TH1* h1, int color, int x1, int x2)
{
   if (x1==0 && x2==0)  h1->Scale(1./h1->Integral()); 
   else  h1->Scale(1./h1->Integral(x1,x2)); 
   h1->Scale(1./h1->GetBinWidth(1)); 
   h1->SetMarkerColor(color);
   h1->SetLineColor(color); 
}
double calSys(TH1* hdata,TH1* hmc, double x0, double x1, double x2,double x3)
{
    double dataratio = hdata->Integral(hdata->GetXaxis()->FindBin(x0),hdata->GetXaxis()->FindBin(x1))/hdata->Integral(hdata->GetXaxis()->FindBin(x2),hdata->GetXaxis()->FindBin(x3));
    double mcratio = hmc->Integral(hmc->GetXaxis()->FindBin(x0),hmc->GetXaxis()->FindBin(x1))/hmc->Integral(hmc->GetXaxis()->FindBin(x2),hmc->GetXaxis()->FindBin(x3));
    cout << "data: "<< dataratio << " mc: "<<mcratio<<" sys:"<< fabs(mcratio-dataratio)/dataratio<< endl;
    return fabs(mcratio-dataratio)/dataratio;
}
void drawSysGraph(TPDF* pdf, TCanvas* c)
{
   pdf->On();
   c->cd();
   TH1F* hbkgd =  new TH1F("hbkgd","hbkgd;p_{T} [GeV/c];Sys. Err" , 10,0, 4);
   hbkgd->GetYaxis()->SetRangeUser(0,0.1);
   // hbkgd->Draw();
   gDcasys->Draw();
   addpdf(pdf,c);
   c->Clear();
   gInvMassSys->Draw();
   addpdf(pdf,c);
}
