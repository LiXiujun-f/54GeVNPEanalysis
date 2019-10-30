//
//   @file    sPhenixStyle.h         
//   
//            sPHENIX Style, based on a style file from ATLAS
//
//
//   @author Peter Steinberg
// 
//   Copyright (C) 2017 sPhenix Collaboration
//
//   Version 0.1

#ifndef  __SPHENIXSTYLE_H
#define __SPHENIXSTYLE_H

#include "TStyle.h"

void SetsPhenixStyle();

TStyle* sPhenixStyle(); 

void SetsPhenixStyle ()
{
  static TStyle* sphenixStyle = 0;
  std::cout << "sPhenixStyle: Applying nominal settings." << std::endl ;
  if ( sphenixStyle==0 ) sphenixStyle = sPhenixStyle();
  gROOT->SetStyle("sPHENIX");
  gROOT->ForceStyle();
}

TStyle* sPhenixStyle() 
{
  TStyle *sphenixStyle = new TStyle("sPHENIX","sPHENIX style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  sphenixStyle->SetFrameBorderMode(icol);
  sphenixStyle->SetFrameFillColor(icol);
  sphenixStyle->SetCanvasBorderMode(icol);
  sphenixStyle->SetCanvasColor(icol);
  sphenixStyle->SetPadBorderMode(icol);
  sphenixStyle->SetPadColor(icol);
  sphenixStyle->SetStatColor(icol);
  //sphenixStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  sphenixStyle->SetPaperSize(20,26);

  // set margin sizes
  sphenixStyle->SetPadTopMargin(0.06);
  sphenixStyle->SetPadRightMargin(0.05);
  sphenixStyle->SetPadBottomMargin(0.16);
  sphenixStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  sphenixStyle->SetTitleXOffset(1.4);
  sphenixStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  Int_t font=42; // Helvetica
  Double_t tsize=0.05;
  sphenixStyle->SetTextFont(font);

  sphenixStyle->SetTextSize(tsize);
  sphenixStyle->SetLabelFont(font,"x");
  sphenixStyle->SetTitleFont(font,"x");
  sphenixStyle->SetLabelFont(font,"y");
  sphenixStyle->SetTitleFont(font,"y");
  sphenixStyle->SetLabelFont(font,"z");
  sphenixStyle->SetTitleFont(font,"z");
  
  sphenixStyle->SetLabelSize(tsize,"x");
  sphenixStyle->SetTitleSize(tsize,"x");
  sphenixStyle->SetLabelSize(tsize,"y");
  sphenixStyle->SetTitleSize(tsize,"y");
  sphenixStyle->SetLabelSize(tsize,"z");
  sphenixStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  sphenixStyle->SetMarkerStyle(20);
  // sphenixStyle->SetMarkerSize(1.2);
  sphenixStyle->SetMarkerSize(1.0);
  sphenixStyle->SetHistLineWidth(2.);
  sphenixStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars 
  //sphenixStyle->SetErrorX(0.001);
  // get rid of error bar caps
  sphenixStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  sphenixStyle->SetOptTitle(0);
  //sphenixStyle->SetOptStat(1111);
  sphenixStyle->SetOptStat(0);
  //sphenixStyle->SetOptFit(1111);
  sphenixStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  sphenixStyle->SetPadTickX(1);
  sphenixStyle->SetPadTickY(1);

  // legend modificatin
  sphenixStyle->SetLegendBorderSize(0);
  sphenixStyle->SetLegendFillColor(0);
  sphenixStyle->SetLegendFont(font);


#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
  std::cout << "sPhenixStyle: ROOT6 mode" << std::endl;
  sphenixStyle->SetLegendTextSize(tsize);
  sphenixStyle->SetPalette(kBird);
#else
  std::cout << "sPhenixStyle: ROOT5 mode" << std::endl;
  // color palette - manually define 'kBird' palette only available in ROOT 6
  Int_t alpha = 0;
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
  Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
  TColor::CreateGradientColorTable(9, stops, red, green, blue, 255, alpha);
#endif

  sphenixStyle->SetNumberContours(80);

  return sphenixStyle;

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
void drawLatex(double x,double y,const char* txt,double size)
{
  TLatex lat;
  lat.SetTextSize(size);
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
#endif // __SPHENIXSTYLE_H
