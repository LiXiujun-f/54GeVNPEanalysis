#include <vector>
#include "TGraph.h"
#include "TFile.h"
#include "TF1.h"
#include "sPhenixStyle.h"
TGraph* geta_gamma_sp[9];
TGraph* gpi0_gamma_sp[9];
TGraph* geta_gamma_v2[9];
TGraph* gpi0_gamma_v2[9];
TF1* fdirpho_v2[9];
TF1* fdirpho_sp[9];
TF1* feta_gamma_sp[9];
TF1* fpi0_gamma_sp[9];
TF1* feta_gamma_v2[9];
TF1* fpi0_gamma_v2[9];

double dirphosum(double* x,double* par)
{
  double sum=0;
  for (int i=2;i<9;i++)
  {
    sum+ = fdirpho_sp[i]->Eval(x[0]);
  }
  return sum/7.0;
}
double dirphov2mb(double* x,double* par)
{
  double sum=0;
  double w[9]={0};
  double combv2=0;
  for (int i=2;i<9;i++)
  {
    sum+ = fdirpho_sp[i]->Eval(x[0]);
    w[i] = fdirpho_sp[i]->Eval(x[0]);
    combv2+=w[i]*fdirpho_v2[i]->Eval(x[0]);
  }
  return combv2/sum;
}
double Pi0Spectra(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  return gpi0_gamma_sp[idx]->Eval(x[0]);
}
double EtaSpectra(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  return geta_gamma_sp[idx]->Eval(x[0]);
}
double Etav2(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  double w1 = geta_gamma_sp[idx]->Eval(x[0]);
  double w2 = gpi0_gamma_sp[idx]->Eval(x[0]);
  double w3 = fdirpho_sp[idx_dirpho]->Eval(x[0]);
  return geta_gamma_v2[idx]->Eval(x[0]);
}
double Pi0v2(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  double w1 = geta_gamma_sp[idx]->Eval(x[0]);
  double w2 = gpi0_gamma_sp[idx]->Eval(x[0]);
  double w3 = fdirpho_sp[idx_dirpho]->Eval(x[0]);
  return gpi0_gamma_v2[idx]->Eval(x[0]);
}
double CombineSpectra(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  return geta_gamma_sp[idx]->Eval(x[0])+gpi0_gamma_sp[idx]->Eval(x[0])+fdirpho_sp[idx_dirpho]->Eval(x[0]);
}
double Combinev2(double* x,double* par)
{
  int idx = par[0];
  int idx_dirpho = par[1];
  double w1 = geta_gamma_sp[idx]->Eval(x[0]);
  double w2 = gpi0_gamma_sp[idx]->Eval(x[0]);
  double w3 = fdirpho_sp[idx_dirpho]->Eval(x[0]);
  return (geta_gamma_v2[idx]->Eval(x[0])*w1+gpi0_gamma_v2[idx]->Eval(x[0])*w2
          + fdirpho_v2[idx_dirpho]->Eval(x[0])*w3 )/(w1+w2+w3);
}
void combGammaSources()
{
  // gStyle->SetLineWidth(2.5);
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("comb_pi0_eta_gamma.pdf");
  TFile* file = new TFile("fread_pi0_eta_2gamma.root");
  TF1* fcombsp[9];
  TF1* fcombv2[9];
  TFile* filedirphosp = new TFile("dirphospinput.root");
  TFile* filedirphov2 = new TFile("dirphov2input.root");
  TString centname[9] = {"","0_60","40_60","40_60","20_40","20_40","0_20","0_20","0_20"};
  //read dirgamma
  for (int i=2;i<9;i++)
  {
    fdirpho_v2[i] = (TF1*)((TF1*)filedirphov2->Get(Form("fitdirphov2_%s",centname[i].Data())))->Clone(Form("dirpho_v2_%d",i));
    fdirpho_sp[i] = (TF1*)((TF1*)filedirphosp->Get(Form("fitdirphoInv_pt_%s",centname[i].Data() )))->Clone(Form("dirpho_sp_%d",i));
    fdirpho_sp[i]->SetLineColor(kGreen+2);
    fdirpho_v2[i]->SetLineColor(kGreen+2);
  }
  fdirpho_sp[1] = new TF1("dirpho_sp_mb",dirphosum,0,20,1);
  fdirpho_v2[1] = new TF1("dirpho_v2_mb",dirphov2mb,0,20,1);
  fdirpho_v2[1]->SetLineColor(kGreen+2);
  fdirpho_sp[1]->SetLineColor(kGreen+2);
  //2-8, 1 is the 0-60%
  for (int ic=1;ic<9;ic++)
  {
    geta_gamma_sp[ic] = (TGraph*)file->Get(Form("gGmSp_eta_%d",ic));
    gpi0_gamma_sp[ic] = (TGraph*)file->Get(Form("gGmSp_pi0_%d",ic));
    geta_gamma_v2[ic] = (TGraph*)file->Get(Form("gGmv2_eta_%d",ic));
    gpi0_gamma_v2[ic] = (TGraph*)file->Get(Form("gGmv2_pi0_%d",ic));
    
    fcombsp[ic] = new TF1(Form("fGMSp_comb_%d",ic),CombineSpectra,0,15,2);
    fcombv2[ic] = new TF1(Form("fGMv2_comb_%d",ic),Combinev2,0,15,2);
    fcombsp[ic]->SetParameter(0,ic);
    fcombsp[ic]->SetParameter(1,ic);
    fcombv2[ic]->SetParameter(0,ic);
    fcombv2[ic]->SetParameter(1,ic);
 
    feta_gamma_sp[ic] = new TF1(Form("fGMSp_eta_%d",ic),EtaSpectra,0,15,2);
    feta_gamma_v2[ic] = new TF1(Form("fGMv2_eta_%d",ic),Etav2,0,15,2);
    feta_gamma_sp[ic]->SetParameter(0,ic);
    feta_gamma_sp[ic]->SetParameter(1,ic);
    feta_gamma_v2[ic]->SetParameter(0,ic);
    feta_gamma_v2[ic]->SetParameter(1,ic);

    fpi0_gamma_sp[ic] = new TF1(Form("fGMSp_pi0_%d",ic),Pi0Spectra,0,15,2);
    fpi0_gamma_v2[ic] = new TF1(Form("fGMv2_pi0_%d",ic),Pi0v2,0,15,2);
    fpi0_gamma_sp[ic]->SetParameter(0,ic);
    fpi0_gamma_sp[ic]->SetParameter(1,ic);
    fpi0_gamma_v2[ic]->SetParameter(0,ic);
    fpi0_gamma_v2[ic]->SetParameter(1,ic);

    fcombsp[ic]->Draw();
    fcombsp[ic]->GetXaxis()->SetTitle("p_{T} [GeV]");
    gpi0_gamma_sp[ic]->SetLineColor(kBlue);
    geta_gamma_sp[ic]->SetLineWidth(2.5);
    gpi0_gamma_sp[ic]->SetLineWidth(2.5);
    fcombsp[ic]->SetLineWidth(2.5);
    fdirpho_sp[ic]->SetLineWidth(2.5);
    geta_gamma_sp[ic]->SetLineWidth(2.5);
    gpi0_gamma_sp[ic]->Draw("same");
    geta_gamma_sp[ic]->Draw("same");
    fdirpho_sp[ic]->Draw("same");
    // fcombsp[ic]->Draw("same");
    gPad->SetLogy(1);
    TLegend* legsp = new TLegend(0.6,0.7,0.88,0.88);
    legsp->AddEntry(gpi0_gamma_sp[ic],"#pi^{0}->#gamma","l");
    legsp->AddEntry(geta_gamma_sp[ic],"#eta->#gamma","l");
    legsp->AddEntry(fdirpho_sp[ic],"direct photon","l");
    legsp->AddEntry(fcombsp[ic],"combined spectra","l");
    // legsp->SetHeader(Form("%ic",ic));
    legsp->Draw("same");

    addpdf(pdf);
    gPad->SetLogy(0);
    gPad->Clear();
    fcombv2[ic]->Draw();
    gpi0_gamma_v2[ic]->SetLineColor(kBlue);
    gpi0_gamma_v2[ic]->Draw("same");
    geta_gamma_v2[ic]->Draw("same");
    fdirpho_v2[ic]->Draw("same");
    TLegend* leg = new TLegend(0.6,0.7,0.88,0.88);
    leg->AddEntry(gpi0_gamma_v2[ic],"#pi^{0}->#gamma","l");
    leg->AddEntry(geta_gamma_v2[ic],"#eta->#gamma","l");
    leg->AddEntry(fdirpho_v2[ic],"direct photon","l");
    leg->AddEntry(fcombv2[ic],"combined v2","l");
    leg->Draw("same");
    addpdf(pdf);
  }

  pdf->On();
  pdf->Close(); 
  TFile* fout = new TFile("fout_pi0_eta_gamma.root","recreate");
  for (int ic=2;ic<9;ic++)
  {
    fcombsp[ic]->Write();
    fcombv2[ic]->Write();
    feta_gamma_sp[ic]->Write();
    feta_gamma_v2[ic]->Write();
    fpi0_gamma_sp[ic]->Write();
    fpi0_gamma_v2[ic]->Write();
    fdirpho_v2[ic]->Write();
    fdirpho_sp[ic]->Write();
  }
  fout->Close();
}
