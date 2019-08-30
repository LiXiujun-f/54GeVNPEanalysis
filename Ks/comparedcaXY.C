#include  "rootlogon.h"
void comparedcaXY(){
  myrootlogon();
  int const ncomp = 2;
  int const npar = 6;
  int const Ncent=9;
  int const centedge[Ncent+1]={80,70,60,50,40,30,20,10,5,0};
  int color[ncomp] = {kRed, kBlue};
  TFile* f[npar][ncomp];
  // char pname[4][150] = {"pionplus","pionminus","kaonplus","kaonminus"};
  char pname[npar][150] = {"pionplus","pionminus","kaonplus","kaonminus","proton","antiproton"};
  char source[ncomp][150] = {"withfluc","without"};
  //book the file

  // int ipar = 0;
 for (int ipar=0;ipar<npar;ipar++){
  // f[ipar][0] = TFile::Open(Form("%s/idtruth_%s.root",source[1],pname[ipar]));
  f[ipar][0] = TFile::Open(Form("%s/%s.root",source[0],pname[ipar]));
  f[ipar][1] = TFile::Open(Form("%s/%s.root",source[1],pname[ipar]));
  TH1D* h1eff[ncomp][Ncent];
  TH1D* heffratio[Ncent];
  TH2D* h2dca[ncomp][Ncent];
  TPDF* pdf = new TPDF(Form("%scomparedcaXY.pdf",pname[ipar]));
  pdf->Off();
  // pdf->NewPage();
  TCanvas* c = new TCanvas("c","c",1000,800);
  c->cd();
  // drawtitle(pdf,c,Form("%s efficiency", pname[ipar]));
  drawtitle(pdf,c,Form("%s dcaXY", pname[ipar]));
  c->Clear();
  c->Divide(2,2);

  c->cd(1);
  // gPad->SetPad(0, 0.35,0.5,1);
  // setPad(0.15,0.05,0.1,0);
  gPad->SetLogy();
  c->cd(3);
  // gPad->SetPad(0,0,0.5,0.35);
  // setPad(0.15,0.05,0,0.2);
  gPad->SetLogy();
  c->cd(2);
  // gPad->SetPad(0.5, 0.35,1,1);
  // setPad(0.15,0.05,0.1,0);
  gPad->SetLogy();
  c->cd(4);
  // gPad->SetPad(0.5,0,1,0.35);
  // setPad(0.15,0.05,0,0.2);
  gPad->SetLogy();
  for (int ic=0;ic<9;ic++){
    for (int icomp=0;icomp<ncomp;icomp++){
      h2dca[icomp][ic] = (TH2D*)f[ipar][icomp]->Get(Form( "pTdcaXY_%d", ic))->Clone(Form("%s%sh1Ratiocent_%d",source[icomp],pname[ipar] , ic));
      h1eff[icomp][ic] = (TH1D*)h2dca[icomp][ic]->ProjectionY(Form("%s%sdca_%d",source[icomp],pname[ipar], ic));
      h1eff[icomp][ic]->SetDirectory(0);
      // h1eff[icomp][ic]->Rebin();
      // h1eff[icomp][ic]->Scale(0.5);
      // h1eff[icomp][ic]->GetXaxis()->SetRangeUser(0,10);
      // h1eff[icomp][ic]->GetYaxis()->SetRangeUser(0,1.1);
      h1eff[icomp][ic]->SetLineColor(color[icomp]);
      h1eff[icomp][ic]->SetMarkerColor(color[icomp]);
      h1eff[icomp][ic]->SetMarkerSize(0.8);
      h1eff[icomp][ic]->GetXaxis()->SetTitle("dcaXY(cm)");
      // h1eff[icomp][ic]->GetYaxis()->SetTitle("Counts");
      h1eff[icomp][ic]->GetYaxis()->SetTitle("1/N_{evt}dN/d(dcaXY)");
      if (ic%4==0)c->cd(1);
      else if (ic%4==1)c->cd(2);
      else if (ic%4==2)c->cd(3);
      else if (ic%4==3)c->cd(4);
      if (icomp == 0){ 
        gPad->Clear();
        h1eff[icomp][ic]->Draw();}
      else h1eff[icomp][ic]->Draw("same");
    }
    TLegend* l = new TLegend(0.65,0.65,0.9,0.9);
    l->AddEntry(h1eff[0][ic],"with fluct.","lep");
    l->AddEntry(h1eff[1][ic],"without fluct.","lep");

    l->SetHeader(Form("%d~%d %s",centedge[ic],centedge[ic+1],"%"));
    l->Draw("same"); 
    // //compare ratio
    // if (ic%2==0) c->cd(3);
    // else c->cd(4);
    // heffratio[ic]=(TH1D*)h1eff[0][ic]->Clone(Form("heffratio%d",ic));
    // heffratio[ic]->Divide(h1eff[1][ic]);
    // heffratio[ic]->GetYaxis()->SetRangeUser(0,2);
    // heffratio[ic]->GetYaxis()->SetTitle("with/without");
    // heffratio[ic]->SetMarkerColor(kBlack);
    // heffratio[ic]->SetMarkerSize(0.8);
    // heffratio[ic]->Draw();
    // drawLine(0,1,10,1,1.5,4,kBlue);
    if (ic%4==3 || ic == 8) addpdf(pdf,c);
  }
  pdf->On();
  // pdf->Text(0.2,0.5,"Thank you");
  // pdf->Text(0.1,0.3,pname[ipar]);
  pdf->Close();
 }
 }

void addpdf(TPDF* pdf,TCanvas* c){
  c->cd();
  pdf->On();
  pdf->NewPage();
  c->Update();
  pdf->Off();
}
void drawtitle(TPDF* pdf,TCanvas* c,string s){
  c->cd();
  c->Draw();
  setPad(0.1,0.1,0.05,0.12);
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
