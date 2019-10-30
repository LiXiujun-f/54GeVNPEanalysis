#include "sPhenixStyle.h"
void drawline(double x1, double y1, double x2, double y2, int color );
void drawerrbar(double x, double y, double ysys, double xwidth, double shortlinesize, int color,float size);
void drawGraphWithSys(TString fname,TString gname , TString gnamesys, int color,int style,float size);
void plotfinal()
{
  SetsPhenixStyle();
  TFile* file = new TFile("out.root");
  TH1F* HFv2stat = (TH1F*)file->Get("hHFv2stat");
  TH1F* HFv2sys = (TH1F*)file->Get("hHFv2sys");
  TCanvas* c = new TCanvas("c","c",800,600);
   
  // HFv2stat->Draw();
  // HFv2stat->GetYaxis()->SetRangeUser(-0.05,0.2);
   
  int  nbins = HFv2stat->GetNbinsX();
  double x[50],xwidth[50] , y[50], syserr[50],staterr[50] ,tmpx,tmpy,tmperr,tmpstat;
  int npoints=0;
  for (int ib=0;ib<nbins;ib++)
  {
    tmpy = HFv2stat->GetBinContent(ib+1);
    tmpx = HFv2stat->GetBinCenter(ib+1);
    tmpstat = HFv2stat->GetBinError(ib+1);
    tmperr = HFv2sys->GetBinError(ib+1);
    if (tmpy<-0.05 || (tmpx<0.35) || (tmpx<1.15&&tmpx>0.85) ||  tmperr>0.08 ) 
    { continue; }
    else 
    { 
      y[npoints] = tmpy;
      x[npoints] = tmpx;
      syserr[npoints] = tmperr;
      staterr[npoints] = tmpstat;
      xwidth[npoints] = 0.5*HFv2sys->GetBinWidth(ib+1);
      npoints++; 
    }
  }

  TGraphErrors* gsyserr = new TGraphErrors(npoints ,x ,y ,0,syserr );
  gsyserr->SetName("gHFe54sys");
  TGraphErrors* gstaterr = new TGraphErrors(npoints ,x ,y ,xwidth,staterr );
  gstaterr->SetName("gHFe54stat");
  gstaterr->Draw("ApZ");
  gstaterr->GetYaxis()->SetRangeUser(-0.1,0.4);
  //Rapp's plot
  TFile* fRapp = new TFile("gRappNPE62.root");
  TGraph* gRappNPE62 = (TGraph*)fRapp->Get("gRappNPE62");
  gRappNPE62->Draw("sameF");
  gRappNPE62->SetFillColor(kRed-4);
  gRappNPE62->SetLineColor(kRed-4);
  gRappNPE62->SetLineWidth(2);
  fRapp->Close();
  //62 GeV
  TFile* fPhoE = TFile::Open("phoEv2.root");
  TGraphErrors* gSTAR62 = (TGraphErrors*)fPhoE->Get("HFe62");
  TGraphErrors* gSTAR62sys = (TGraphErrors*)fPhoE->Get("HFe62sys"); 
  drawGraphWithSys( gSTAR62,gSTAR62sys, kBlue,25,1);
  TGraphErrors* gSTAR200 = (TGraphErrors*)fPhoE->Get("HFe200");
  TGraphErrors* gSTAR200sys = (TGraphErrors*)fPhoE->Get("HFe200sys"); 
  drawGraphWithSys( gSTAR200,gSTAR200sys, kMagenta, 25,1);
 
  fPhoE->Close();
  // drawGraphWithSys("phoEv2.root","HFe62","HFe62sys",kBlue,21,1.2);

  //physics plot
  gstaterr->GetXaxis()->SetRangeUser(0.2,3);
  gstaterr->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  gstaterr->GetYaxis()->SetTitle("Non-photonic electron v_{2}");
  gstaterr->SetLineColor(kBlack);
  gstaterr->SetMarkerColor(kBlack);
  gstaterr->SetMarkerSize(1.2);
  gstaterr->Draw("psame");
  for (int ib=0;ib<npoints;ib++)
  { 
     drawerrbar(x[ib],y[ib],syserr[ib],HFv2sys->GetBinWidth(ib+1)*0.2, 0.005, kBlack); 
  }
  drawline(0.2,0,3,0,kGreen+2,5);

  //Add legends
  TLegend* leg = new TLegend(0.2,0.92,0.7,0.67);
  leg->AddEntry( gSTAR200, "Au+Au 200 GeV 0-60\%","lp");
  leg->AddEntry( gSTAR62, "Au+Au 62.4 GeV 0-60\%","lp");
  leg->AddEntry( gRappNPE62, "TAMU 62.4 GeV 0-60\%","l");
  leg->AddEntry(gstaterr,"Au+Au 54.4 GeV 0-60\%","lp");
  leg->Draw();
  
  c->SaveAs("physics.png");
 
  // c->Clear();
  
  // gstaterr->Draw("Ap");
  //  
  //
  //light hadron
  //
  //
  // gstaterr->Draw("psame");
  // for (int ib=0;ib<npoints;ib++)
  // { 
  //    drawerrbar(x[ib],y[ib],syserr[ib],HFv2sys->GetBinWidth(ib+1)*0.2, 0.005, kBlack); 
  // }
  // drawline(0.2,0,3,0,kGreen+2,5);
  //

  TFile* file = new TFile("final.root","recreate");  
  gstaterr->Write();
  gsyserr->Write();
  gRappNPE62->Write();

}

void drawline(double x1, double y1, double x2, double y2, int color,int s=0 ){
  TLine* l = new  TLine(x1,y1,x2,y2);
  l->SetLineStyle(s);
  l->SetLineColor(color);
  l->Draw("same");
}
void drawerrbar(double x, double y, double ysys, double xwidth, double shortlinesize, int color){
  double xw = xwidth; //x-width or x sys
  // double yl = shortlinesize*y;
  double yl = shortlinesize;
  int s = 1;
  //top vertical line
  drawline(x-xw,y+ysys, x+xw, y+ysys, color,s);
  //bottom vertical line
  drawline(x-xw,y-ysys, x+xw, y-ysys, color,s);
  //top left line
  drawline(x-xw,y+ysys, x-xw, y+ysys-yl, color,s);
  //top right line
  drawline(x+xw,y+ysys, x+xw, y+ysys-yl, color,s);
  //bottom left line
  drawline(x-xw,y-ysys, x-xw, y-ysys+yl, color,s);
  //bottom right line
  drawline(x+xw,y-ysys, x+xw, y-ysys+yl, color,s);
}
void drawGraphWithSys(TString fname,TString gname , TString gnamesys, int color,int style,float size)
{
  TFile* f = TFile::Open(fname.Data());
  TGraphErrors* g = (TGraphErrors*)f->Get(gname.Data());
  TGraphErrors* gsys = (TGraphErrors*)f->Get(gnamesys.Data());
  cout<<"start draw..." <<endl;
  g->SetMarkerStyle(style);
  g->SetMarkerColor(color);
  g->SetMarkerSize(size);
  g->Draw("psame");
  int npoints=g->GetN();
  for (int ip=0;ip<npoints;ip++)
  {
    double x,y,yerr;
    gsys->GetPoint(ip,x,y);
    yerr = gsys->GetErrorY(ip);
    drawerrbar(x,y,yerr,0.03,0.005,color); 
  }
}
void drawGraphWithSys(TGraphErrors* g,TGraphErrors* gsys , int color,int style,float size)
{
  // TFile* f = TFile::Open(fname.Data());
  // TGraphErrors* g = (TGraphErrors*)f->Get(gname.Data());
  // TGraphErrors* gsys = (TGraphErrors*)f->Get(gnamesys.Data());
  cout<<"start draw..." <<endl;
  g->SetMarkerStyle(style);
  g->SetMarkerColor(color);
  g->SetLineColor(color);
  g->SetMarkerSize(size);
  g->SetLineWidth(1.5);
  g->Draw("psame");
  int npoints=g->GetN();
  for (int ip=0;ip<npoints;ip++)
  {
    double x,y,yerr;
    gsys->GetPoint(ip,x,y);
    yerr = gsys->GetErrorY(ip);
    drawerrbar(x,y,yerr,0.03,0.005,color); 
  }
}
