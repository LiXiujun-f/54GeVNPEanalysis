#include "myStyle.h"
void plotCorr()
{
  SetMyStyle();
  TFile* file = TFile::Open("test_200GeVNCR.root"); 
  TProfile* pf = (TProfile*)file->Get("D_B_e"); 
  pf->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  double xmax = 3.5,xlow=0;
  pf->GetXaxis()->SetRangeUser(xlow,xmax);
  pf->GetYaxis()->SetTitle("<cos(#phi_{c/b#rightarrow e}#minus #phi_{parent})>");
  pf->Draw();
  drawLine(xlow,0.95, xmax,0.95, 1.5,9,kRed);
  drawLatex(0.6,0.7,"PYTHIA",0.08);
  drawLatex(0.58,0.6,"p+p 54 GeV",0.08);
  drawLatex(0.22,0.77,"0.95",0.08,kRed);
  gPad->SaveAs("HFeCorr.pdf");
  gPad->SaveAs("HFeCorr.png");
}
