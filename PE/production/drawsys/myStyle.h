//
//   @file    MyStyle.h         
//            original file comes from sPHENIX and copyright to sPHENIX collaboration 
//            I modified some parameters
//  original  @author Peter Steinberg
//

#ifndef  __MYSTYLE_H
#define __MYSTYLE_H

#include "TStyle.h"

void SetMyStyle();

TStyle* MyStyle(); 

void SetMyStyle ()
{
  static TStyle* myStyle = 0;
  std::cout << "MyStyle: Applying nominal settings." << std::endl ;
  if ( myStyle==0 ) myStyle = MyStyle();
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
}

TStyle* MyStyle() 
{
  TStyle *myStyle = new TStyle("MyStyle","modified sPHENIX style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  myStyle->SetFrameBorderMode(icol);
  myStyle->SetFrameFillColor(icol);
  myStyle->SetCanvasBorderMode(icol);
  myStyle->SetCanvasColor(icol);
  myStyle->SetPadBorderMode(icol);
  myStyle->SetPadColor(icol);
  myStyle->SetStatColor(icol);
  //myStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  myStyle->SetPaperSize(20,26);

  // set margin sizes
  myStyle->SetPadTopMargin(0.1);
  myStyle->SetPadRightMargin(0.1);
  myStyle->SetPadBottomMargin(0.16);
  myStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  myStyle->SetTitleXOffset(1.3);
  myStyle->SetTitleYOffset(1.3);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  myStyle->SetTextFont(font);

  myStyle->SetTextSize(tsize);
  myStyle->SetLabelFont(font,"x");
  myStyle->SetTitleFont(font,"x");
  myStyle->SetLabelFont(font,"y");
  myStyle->SetTitleFont(font,"y");
  myStyle->SetLabelFont(font,"z");
  myStyle->SetTitleFont(font,"z");
  
  myStyle->SetLabelSize(tsize,"x");
  myStyle->SetTitleSize(tsize,"x");
  myStyle->SetLabelSize(tsize,"y");
  myStyle->SetTitleSize(tsize,"y");
  myStyle->SetLabelSize(tsize,"z");
  myStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  myStyle->SetMarkerStyle(20);
  // myStyle->SetMarkerSize(1.2);
  myStyle->SetMarkerSize(1.0);
  myStyle->SetHistLineWidth(2.);
  myStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars 
  //myStyle->SetErrorX(0.001);
  // get rid of error bar caps
  myStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  myStyle->SetOptTitle(0);
  //myStyle->SetOptStat(1111);
  myStyle->SetOptStat(0);
  //myStyle->SetOptFit(1111);
  myStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);

  // legend modificatin
  myStyle->SetLegendBorderSize(0);
  myStyle->SetLegendFillColor(0);
  myStyle->SetLegendFont(font);


#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
  std::cout << "MyStyle: ROOT6 mode" << std::endl;
  myStyle->SetLegendTextSize(tsize);
  myStyle->SetPalette(kBird);
#else
  std::cout << "MyStyle: ROOT5 mode" << std::endl;
  // color palette - manually define 'kBird' palette only available in ROOT 6
  Int_t alpha = 0;
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
  Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
  TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
#endif

  myStyle->SetNumberContours(80);

  return myStyle;

}
void addpdf(TPDF* pdf)
{ 
  pdf->On();
  gPad->cd();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void setPad(Double_t left, Double_t right, Double_t top, Double_t bottom, int color=10)
{
    gPad->SetFillColor(color);
    gPad->SetBorderMode(0);
    gPad->SetBorderSize(0);
    gPad->SetFrameFillColor(10);
    gPad->SetFrameBorderMode(0);
    gPad->SetFrameBorderSize(0);
    gPad->SetLeftMargin(left);
    gPad->SetRightMargin(right);
    gPad->SetTopMargin(top);
    gPad->SetBottomMargin(bottom);
}

void setGraphStyle(TGraphErrors* gr, int color, int marker, int mSize, int lSize)
{
    gr->SetLineColor(color);
    gr->SetMarkerColor(color);
    gr->SetMarkerStyle(marker);
    gr->SetMarkerSize(mSize);
    gr->SetLineWidth(lSize);
}

//__________________________________________________
TLine* drawLine(Double_t xlow,Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineStyle,Int_t lineColor)
{
    TLine *l1 = new TLine(xlow,ylow,xup,yup);
    l1->SetLineWidth(lineWidth);
    l1->SetLineColor(lineColor);
    l1->SetLineStyle(lineStyle);
    l1->Draw("same");
    return l1;
}
void drawLatex(double x,double y,const char* txt,double size=0.05,int color=kBlack)
{
  TLatex lat;
  lat.SetTextSize(size);
  lat.SetTextColor(color);
  lat.DrawLatexNDC ( x, y, txt);
}
void drawSTAR(double x,double y)
{
  TLatex lat;
  lat.SetTextSize(0.05);
  lat.SetTextFont(72);
  lat.SetTextColor(kRed);
  lat.DrawLatexNDC ( x, y, "STAR Preliminary");

}
#endif // __MYSTYLE_H
