#include "sPhenixStyle.h"

void drawtageff()
{
  SetsPhenixStyle();
  // TGraph* g62 = new TGraph("tageff.txt");
  TGraph* g62 = new TGraph("tageff200.txt");
  TF1* fit  = new TF1("fit62","pol2(0)",0,5);
  fit->SetLineColor(kBlue);
  g62->Draw("pA");
  g62->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g62->Fit(fit);
  TLegend* l = new TLegend(0.5,0.2,0.8,0.5);
  l->AddEntry(fit,"62GeV tag eff","l");
  l->Draw("same");
  // fit->Write();
}
