#include "sPhenixStyle.h"
void plotS2B()
{  
  SetsPhenixStyle();

  // TCanvas* c1 = new TCanvas("c1","c1", 800,600);
  TFile* fpsys = new TFile("out_sys.root");
  TH1F* hs2b_psys = (TH1F*)fpsys->Get("hratioCor");
  hs2b_psys->SetDirectory(0);
  fpsys->Close();

  TFile* file = new TFile("out.root");
  TH1F* hs2b = (TH1F*)file->Get("hratioCor");
  TH1F* hs2berr = (TH1F*)file->Get("hS2Berror");
   
  int  nbins = hs2b->GetNbinsX();
  double x[50],xwidth[50] , y[50], ;
  double  s2b[50], s2berr[50],s2bstaterr[50] ,tmpx,tmps2b,tmps2berr,tmps2bstat,tmps2bpsys;
  int npoints=0;
  for (int ib=0;ib<nbins;ib++)
  {
    tmpx = hs2b->GetBinCenter(ib+1);
    tmps2b = hs2b->GetBinContent(ib+1);
    tmps2bstat = hs2b->GetBinError(ib+1);
    tmps2berr = hs2berr->GetBinContent(ib+1);
    tmps2bpsys = hs2b_psys->GetBinContent(ib+1)-hs2b->GetBinContent(ib+1);

    tmps2berr = sqrt(tmps2bpsys*tmps2bpsys+tmps2berr*tmps2berr);

    // if (tmpy<-0.05 || tmpx<0.35|| (tmpx<1.15&&tmpx>0.85) || tmperr>0.1 ) 
    if ( (tmpx<0.649&&tmpx>0.4)|| (tmpx<1.2&&tmpx>0.7)  ) 
    { continue; }
    else 
    { 
      x[npoints] = tmpx;
      xwidth[npoints] = 0.5*hs2b->GetBinWidth(ib+1);
     
      s2b[npoints] = tmps2b; 
      s2berr[npoints] = tmps2berr; 
      s2bstaterr[npoints] = tmps2bstat; 

      npoints++; 
    }
  }

  TGraphErrors* g54S2Bsys = new TGraphErrors(npoints ,x ,s2b ,0,s2berr );
  g54S2Bsys->SetName("g54S2Bsys");
  TGraphErrors* g54S2B = new TGraphErrors(npoints ,x ,s2b ,0,s2bstaterr );
  g54S2B->SetName("g54S2Bstat");

  double pt62[4]={0.279,0.623,1.433,2.303};
  double stat62[4]={0.004,0.009,0.028,0.065};
  double sys62[4]={0.068,0.088,0.075,0.082};
  double s2b62[4]={0.268,0.229,0.356,0.414};
  TGraphErrors* g62S2Bstat = new TGraphErrors(4,pt62,s2b62,0,stat62); 
  g62S2Bstat->SetName("g62S2Bstat");
  TGraphErrors* g62S2Bsys = new TGraphErrors(4,pt62,s2b62,0,sys62); 
  g62S2Bsys->SetName("g62S2Bsys");
  
  double pt39[4]={0.279,0.623,1.433,2.303};
  double stat39[4]={0.003,0.006,0.021,0.058};
  double sys39[4]={0.068,0.076,0.074,0.091};
  double s2b39[4]={0.275,0.158,0.204,0.352};
  TGraphErrors* g39S2Bstat = new TGraphErrors(4,pt39,s2b39,0,stat39); 
  g39S2Bstat->SetName("g39S2Bstat");
  TGraphErrors* g39S2Bsys = new TGraphErrors(4,pt39,s2b39,0,sys39); 
  g39S2Bsys->SetName("g39S2Bsys");

   double x1=0,x2=3,y1=0.0,y2=0.6;
   TH1F* d0 = new TH1F("d0","d0;p_{T} [GeV/c];Y",10,x1,x2);
   d0->GetYaxis()->SetRangeUser(y1,y2);
   d0->GetYaxis()->SetTitle("e^{HF}/e^{pho}");

   d0->Draw(); 
   drawline(x1,y1,x1,y2,kBlack,1,2);
   drawline(x1,y2,x2,y2,kBlack,1,2);
   drawline(x2,y2,x2,y1,kBlack,1,2);
   drawline(x2,y1,x1,y1,kBlack,1,2);
   drawGraphWithSys(g54S2B, g54S2Bsys, kRed, 20, 1.5);
   drawGraphWithSys(g39S2Bstat, g39S2Bsys, kGreen+2, 25, 1.5);
   drawGraphWithSys(g62S2Bstat, g62S2Bsys, kBlue+2, 30, 2);
   TLegend* leg = new TLegend(0.2,0.65,0.45,0.88);
   leg->AddEntry(g62S2Bstat,"62.4 GeV","lep");
   leg->AddEntry(g54S2B,"54.4 GeV","lep");
   leg->AddEntry(g39S2Bstat,"39 GeV","lep");
   leg->Draw();

   gPad->Update();
   drawSTAR(0.6,0.25);

   gPad->SaveAs("fig/S2B.pdf");
   gPad->SaveAs("fig/S2B.png");
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
  g->SetLineColor(color);
  g->SetLineWidth(2);
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
  cout<<"start draw..." <<g->GetName()<<endl;
  g->SetMarkerStyle(style);
  g->SetMarkerColor(color);
  g->SetLineColor(color);
  g->SetMarkerSize(size);
  g->SetLineWidth(2.0);
  g->Draw("psame");
  int npoints=g->GetN();
  for (int ip=0;ip<npoints;ip++)
  {
    double x,y,yerr;
    gsys->GetPoint(ip,x,y);
    yerr = gsys->GetErrorY(ip);
    // drawerrbar(x,y,yerr,0.04,0.02,color); 
    drawerrbox(x,y,yerr,0.04,color,3002); 
  }
}
void drawline(double x1, double y1, double x2, double y2, int color, int s=0, int width=1 ){
  TLine* l = new  TLine(x1,y1,x2,y2);
  l->SetLineStyle(s);
  l->SetLineColor(color);
  l->SetLineWidth(width);
  l->Draw("same");
}
void drawerrbar(double x, double y, double ysys, double xwidth, double shortlinesize, int color){
  double xw = xwidth; //x-width or x sys
  // double yl = shortlinesize*y;
  double yl = shortlinesize;
  int s = 1;
  int w = 2;
  //top vertical line
  drawline(x-xw,y+ysys, x+xw, y+ysys, color,s,w);
  //bottom vertical line
  drawline(x-xw,y-ysys, x+xw, y-ysys, color,s,w);
  w = 1;
  //top left line
  drawline(x-xw,y+ysys, x-xw, y+ysys-yl, color,s,w);
  //top right line
  drawline(x+xw,y+ysys, x+xw, y+ysys-yl, color,s,w);
  //bottom left line
  drawline(x-xw,y-ysys, x-xw, y-ysys+yl, color,s,w);
  //bottom right line
  drawline(x+xw,y-ysys, x+xw, y-ysys+yl, color,s,w);
}
void drawerrbox(double x, double y, double ysys, double xwidth,  int color,int fillstyle){
  double xw = xwidth; //x-width or x sys
  TBox* box = new TBox(x-xw ,y-ysys,x+xw ,y+ysys ); 
  box->SetFillStyle(fillstyle);
  box->SetFillColor(color);
  box->Draw();
}
