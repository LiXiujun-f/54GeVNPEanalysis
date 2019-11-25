void drawline(double x1, double y1, double x2, double y2, int color);
void drawerrbar(double x, double y, double ysys, double xwidth, double shortlinesize, int color,float size);
void drawGraphWithSys(TString fname,TString gname , TString gnamesys, int color,int style,float size);
void drawLatex(double x,double y,const char* txt,double size)
{
  TLatex lat;
  lat.SetTextSize(size);
  lat.DrawLatexNDC ( x, y, txt);
}
void plotNPEv2()
{
  gROOT->Reset();

  TFile* fpsys = new TFile("out_sys.root");
  TH1F* hHFv2_psys =  (TH1F*)fpsys->Get("hHFv2stat");
  hHFv2_psys->SetDirectory(0);
  TH1F* hs2b_psys = (TH1F*)fpsys->Get("hratioCor");
  hs2b_psys->SetDirectory(0);
  fpsys->Close();

  TFile* file = new TFile("out.root");
  TH1F* HFv2stat = (TH1F*)file->Get("hHFv2stat");
  TH1F* HFv2sys = (TH1F*)file->Get("hHFv2sys");
  TH1F* hs2b = (TH1F*)file->Get("hratioCor");
  TH1F* hs2berr = (TH1F*)file->Get("hS2Berror");
   
  int  nbins = HFv2stat->GetNbinsX();
  double x[50],xwidth[50] , y[50], syserr[50],staterr[50] ,tmpx,tmpy,tmperr,tmpstat,tmppsys;
  double  s2b[50], s2berr[50],s2bstaterr[50] ,tmpx,tmps2b,tmps2berr,tmps2bstat, tmps2bpsys;
  int npoints=0;
  for (int ib=0;ib<nbins;ib++)
  {
    tmpy = HFv2stat->GetBinContent(ib+1);
    tmpx = HFv2stat->GetBinCenter(ib+1);
    tmpstat = HFv2stat->GetBinError(ib+1);
    tmperr = HFv2sys->GetBinError(ib+1);
    tmps2b = hs2b->GetBinContent(ib+1);
    tmps2bstat = hs2b->GetBinError(ib+1);
    tmps2berr = hs2berr->GetBinContent(ib+1);
    tmps2bpsys = hs2b_psys->GetBinContent(ib+1)-hs2b->GetBinContent(ib+1);
    tmppsys = hHFv2_psys->GetBinContent(ib+1)-HFv2stat->GetBinContent(ib+1);

    tmperr = sqrt(tmperr*tmperr+tmppsys*tmppsys);
    tmps2berr = sqrt(tmps2bpsys*tmps2bpsys+tmps2berr*tmps2berr);

    // if (tmpy<-0.05 || tmpx<0.35|| (tmpx<1.15&&tmpx>0.85) || tmperr>0.1 ) 
    if (tmpy<-0.05 || (tmpx<0.65&&tmpx>0.4)|| (tmpx<1.2&&tmpx>0.7) || tmperr>0.1 ) 
    { continue; }
    else 
    { 
      y[npoints] = tmpy;
      x[npoints] = tmpx;
      syserr[npoints] = tmperr;
      staterr[npoints] = tmpstat;
      xwidth[npoints] = 0.5*HFv2sys->GetBinWidth(ib+1);
     
      s2b[npoints] = tmps2b; 
      s2berr[npoints] = tmps2berr; 
      s2bstaterr[npoints] = tmps2bstat; 

      npoints++; 
    }
  }

  TGraphErrors* gSTAR54sys = new TGraphErrors(npoints ,x ,y ,0,syserr );
  gSTAR54sys->SetName("gHFe54sys");
  TGraphErrors* gSTAR54 = new TGraphErrors(npoints ,x ,y ,0,staterr );
  gSTAR54->SetName("gHFe54stat");
  TGraphErrors* g54S2Bsys = new TGraphErrors(npoints ,x ,s2b ,0,s2berr );
  g54S2Bsys->SetName("g54S2Bsys");
  TGraphErrors* g54S2B = new TGraphErrors(npoints ,x ,s2b ,0,s2bstaterr );
  g54S2B->SetName("g54S2Bstat");

  TFile* fRapp = new TFile("model/gRappNPE62.root");
  TGraph* gRappNPE62 = (TGraph*)fRapp->Get("gRappNPE62");
  // gRappNPE62->SetFillColor(kBlue-4);
  // gRappNPE62->SetLineColor(kBlue-4);
  gRappNPE62->SetFillColor(kRed-4);
  gRappNPE62->SetLineColor(kRed-4);

  gRappNPE62->SetLineWidth(3);
  fRapp->Close();
  TGraph* gRappNPE200 = new TGraph("model/ev2_200_Rapp.txt","%lg %lg");
  gRappNPE200->SetLineColor(kBlack);
  gRappNPE200->SetLineWidth(3);
  
  //62 GeV
  TFile* fPhoE = TFile::Open("phoEv2.root");
  TGraphErrors* gSTAR62 = (TGraphErrors*)fPhoE->Get("HFe62");
  TGraphErrors* gSTAR62sys = (TGraphErrors*)fPhoE->Get("HFe62sys"); 
  TGraphErrors* gSTAR200 = (TGraphErrors*)fPhoE->Get("HFe200");
  TGraphErrors* gSTAR200sys = (TGraphErrors*)fPhoE->Get("HFe200sys");  
  fPhoE->Close();

  TCanvas *c1 = new TCanvas("c1","c1",0,0,800,600);

   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetEndErrorSize(0.01);
   c1->SetFillColor(10);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameFillColor(0);
   c1->SetFrameBorderMode(0);


   c1->SetLeftMargin(0.13);
   c1->SetBottomMargin(0.15);
   c1->SetTopMargin(0.02);
   c1->SetRightMargin(0.02);

   double x1 = 0.0;
   double x2 = 2.9;
   double y1 = -0.07;
   double y2 = 0.23;
   TH1D *d0 = new TH1D("d0","",1,x1,x2);
   d0->SetMinimum(y1);
   d0->SetMaximum(y2);
   d0->GetXaxis()->SetNdivisions(208);
   d0->GetXaxis()->SetTitle("p_{T} (GeV/c)");
   d0->GetXaxis()->SetTitleOffset(1.0);
   d0->GetXaxis()->SetTitleSize(0.06);
   d0->GetXaxis()->SetLabelOffset(0.01);
   d0->GetXaxis()->SetLabelSize(0.045);
   d0->GetXaxis()->SetLabelFont(42);
   d0->GetXaxis()->SetTitleFont(42);
   d0->GetYaxis()->SetNdivisions(205);
   d0->GetYaxis()->SetTitle("HF electron v_{2}");
   d0->GetYaxis()->SetTitleOffset(1.0);
   d0->GetYaxis()->SetTitleSize(0.06);
   d0->GetYaxis()->SetLabelOffset(0.005);
   d0->GetYaxis()->SetLabelSize(0.045);
   d0->GetYaxis()->SetLabelFont(42);
   d0->GetYaxis()->SetTitleFont(42);
   d0->SetLineWidth(2);
   d0->Draw("c");
   drawline(x1,0,x2,0,1,2,2);
   
   drawline(x1,y1,x2,y1,1,1,3);
   drawline(x1,y2,x2,y2,1,1,3);
   drawline(x1,y1,x1,y2,1,1,3);
   drawline(x2,y1,x2,y2,1,1,3);

   drawGraphWithSys(gSTAR200, gSTAR200sys, 1, 30, 2.1);
   drawGraphWithSys(gSTAR62, gSTAR62sys, 1, 24, 1.7);
   drawGraphWithSys(gSTAR54, gSTAR54sys, kBlue, 20, 1.8);

   TLatex *tex = new TLatex(2.0, 0.15, "Au+Au, 0-60%");
   tex->SetTextFont(42);
   tex->SetTextSize(0.05);
   tex->Draw("same");
   
   TLegend *leg = new TLegend(0.18, 0.76, 0.5, 0.94);
   leg->SetFillColor(10);
   leg->SetFillStyle(10);
   leg->SetLineStyle(4000);
   leg->SetLineColor(10);
   leg->SetLineWidth(0.);
   leg->SetTextFont(42);
   leg->SetTextSize(0.045);
   leg->AddEntry(gSTAR200,"200 GeV","p");
   leg->AddEntry(gSTAR62,"62.4 GeV","p");
   leg->AddEntry(gSTAR54,"54.4 GeV","p");
   leg->Draw();

   drawSTAR(0.65,0.88);

   c1->Update();
   c1->SaveAs("fig/NPEv2_200_62_54.pdf");
   c1->SaveAs("fig/NPEv2_200_62_54.eps");
   c1->SaveAs("fig/NPEv2_200_62_54.png");

   //draw theory curve
   gRappNPE200->Draw("same");
   gRappNPE62->Draw("sameF");

   TLegend *leg = new TLegend(0.65, 0.18, 0.9, 0.32);
   leg->SetFillColor(10);
   leg->SetFillStyle(10);
   leg->SetLineStyle(4000);
   leg->SetLineColor(10);
   leg->SetLineWidth(0.);
   leg->SetTextFont(42);
   leg->SetTextSize(0.045);
   leg->AddEntry(gRappNPE200,"TAMU @200 GeV","l");
   leg->AddEntry(gRappNPE62,"TAMU @62.4 GeV","l");
   leg->Draw();

   c1->Update();
   c1->SaveAs("fig/NPEv2_200_62_54_model.pdf");
   c1->SaveAs("fig/NPEv2_200_62_54_model.eps");
   c1->SaveAs("fig/NPEv2_200_62_54_model.png");

   
   TFile* gplots = new TFile("finalDatapoints.root","recreate");
   g54S2B->Write();
   g54S2Bsys->Write();
   gSTAR54->Write();
   gSTAR54sys->Write();
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
    drawerrbar(x,y,yerr,0.03,0.005,color); 
  }
}
void drawSTAR(double x,double y)
{
  TLatex lat;
  lat.SetTextSize(0.05);
  lat.SetTextFont(72);
  lat.SetTextColor(kRed);
  lat.DrawLatexNDC ( x, y, "STAR Preliminary");
}
