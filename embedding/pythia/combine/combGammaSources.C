#include <vector>
#include "TGraph.h"
#include "TFile.h"
#include "TF1.h"
#include "sPhenixStyle.h"
TGraph* geta_gamma_sp[9];
TGraph* gpi0_gamma_sp[9];
TGraph* geta_gamma_v2[9];
TGraph* gpi0_gamma_v2[9];

double CombineSpectra(double* x,double* par)
{
  int idx = par[0];
  return geta_gamma_sp[idx]->Eval(x[0])+gpi0_gamma_sp[idx]->Eval(x[0]);
}
double Combinev2(double* x,double* par)
{
  int idx = par[0];
  double w1 = geta_gamma_sp[idx]->Eval(x[0]);
  double w2 = gpi0_gamma_sp[idx]->Eval(x[0]);
  return (geta_gamma_v2[idx]->Eval(x[0])*w1+gpi0_gamma_v2[idx]->Eval(x[0])*w2)/(w1+w2);
}
void combGammaSources()
{
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("comb_pi0_eta_gamma.pdf");
  TFile* file = new TFile("fread_pi0_eta_2gamma.root");
  TF1* fcombsp[9];
  TF1* fcombv2[9];

  for (int ic=1;ic<9;ic++)
  {
    geta_gamma_sp[ic] = (TGraph*)file->Get(Form("gGmSp_eta_%d",ic));
    gpi0_gamma_sp[ic] = (TGraph*)file->Get(Form("gGmSp_pi0_%d",ic));
    geta_gamma_v2[ic] = (TGraph*)file->Get(Form("gGmv2_eta_%d",ic));
    gpi0_gamma_v2[ic] = (TGraph*)file->Get(Form("gGmv2_pi0_%d",ic));
    
    fcombsp[ic] = new TF1(Form("fGMSp_comb_%d",ic),CombineSpectra,0,15,1);
    fcombv2[ic] = new TF1(Form("fGMv2_comb_%d",ic),Combinev2,0,15,1);
    fcombsp[ic]->SetParameter(0,ic);
    fcombv2[ic]->SetParameter(0,ic);
   
    fcombsp[ic]->Draw();
    gpi0_gamma_sp[ic]->SetLineColor(kBlue);
    gpi0_gamma_sp[ic]->Draw("same");
    geta_gamma_sp[ic]->Draw("same");
    gPad->SetLogy(1);
    addpdf(pdf);
    gPad->SetLogy(0);

    fcombv2[ic]->Draw();
    gpi0_gamma_v2[ic]->SetLineColor(kBlue);
    gpi0_gamma_v2[ic]->Draw("same");
    geta_gamma_v2[ic]->Draw("same");
    addpdf(pdf);
  }

  pdf->On();
  pdf->Close(); 
  TFile* fout = new TFile("fout_pi0_eta_gamma.root","recreate");
  for (int ic=2;ic<9;ic++)
  {
    fcombsp[ic]->Write();
    fcombv2[ic]->Write();
  }
  fout->Close();
}
