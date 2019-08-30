#include <stdio.h>
#include <iostream.h>
#include <math.h>
#include <TMath.h>

double chi(double res);
double resEventPlane(double chi);

void eventPlaneResolution()
{
  gStyle->SetOptDate(0);
  gStyle->SetOptFit(111);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TCanvas* c1 = new TCanvas();
  c1->SetBottomMargin(0.14);
  c1->SetLeftMargin(0.14);
  c1->SetGrid(0,0);
  
  TFile* f2 = new TFile("eventPlaneResolution.root", "RECREATE");
  float centBins[10] = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80};
  TH1D* resolution = new TH1D("resolution", "resolution", 9, centBins);
  TH1D* resolutionEtaGap[8];
  char histName[256];
  for(int i=0; i<8; i++)
    {
      sprintf(histName, "resolution_etaGap0%i", i);
      resolutionEtaGap[i] = new TH1D(histName, histName, 9, centBins);
    }
  TFile f("FullRun14.mixedevent.root");
  TProfile *prof=(TProfile*)f.Get("prfCosResolutionEtaCent");
  ofstream fout("eventPlaneResolution.dat");
  for(int i=0;i<9;i++){
    double deltaRes2Sub = 0.005;  // differential for the error propagation
    double res2Sub = ::sqrt(prof->GetBinContent(i+1));
    double res2SubErr = prof->GetBinError(i+1) / (2. * res2Sub);
    double chiSub2 = chi(res2Sub);
    double chiSub2Delta = chi(res2Sub + deltaRes2Sub);
    double res2 = resEventPlane(::sqrt(2.) * chiSub2); // full event plane res.
    double mRes2Delta = resEventPlane(::sqrt(2.) * chiSub2Delta);
    double res2error = res2SubErr * fabs((double)res2 - mRes2Delta)/ deltaRes2Sub;
    
    // cout<<"resAB:"<<prof->GetBinContent(i+1)<<"    resSub:"<<res2Sub<<"   chiSub:"<<chiSub2<<"   resCheck:"<<resEventPlane(chiSub2)<<"   res:"<<res2<<endl;
    
    cout << res2 << "+/-" << res2error << endl;
    fout<<res2<<endl;
    resolution->SetBinContent(9-i, res2);
    resolution->SetBinError(9-i, res2error);
  
    for(int j=0; j<8; j++)
      {
        double etaScale = 2.;
        if(j>0) etaScale = 2.1-0.2*j;
        double res2EtaGap = resEventPlane(::sqrt(etaScale) * chiSub2);
        double mRes2EtaGapDelta = resEventPlane(::sqrt(etaScale) * chiSub2Delta);
        double res2EtaGapError = res2SubErr * fabs((double)res2EtaGap - mRes2EtaGapDelta)/ deltaRes2Sub;
        resolutionEtaGap[j]->SetBinContent(9-i, res2EtaGap);
        resolutionEtaGap[j]->SetBinError(9-i, res2EtaGapError);
        resolutionEtaGap[j]->SetMarkerStyle(20);
        resolutionEtaGap[j]->SetMarkerColor(j+1);
        resolutionEtaGap[j]->SetLineColor(j+1);
      }
  }
  
  resolution->SetMarkerStyle(20);
  resolution->GetXaxis()->SetTitle("centrality (%)");
  resolution->GetYaxis()->SetTitle("event plane resolution");
  resolution->GetXaxis()->SetLabelSize(0.06);
  resolution->GetYaxis()->SetLabelSize(0.06);
  resolution->GetXaxis()->SetTitleSize(0.06);
  resolution->GetYaxis()->SetTitleSize(0.06);
  resolution->GetXaxis()->SetTitleOffset(0.9);
  resolution->GetYaxis()->SetTitleOffset(0.9);
  resolution->GetYaxis()->SetRangeUser(0, 1);
  resolution->GetXaxis()->SetRangeUser(0, 79);
  resolution->Draw("e");
  for(int i=0; i<8; i++)
    resolutionEtaGap[i]->Draw("esame");
  c1->SaveAs("eventPlaneResolution.gif");
  c1->SaveAs("eventPlaneResolution.eps");
  f2->Write();
  f2->Close();
  fout.close();
}

//-----------------------------------------------------------------------
double chi(double res) {
  // Calculates chi from the event plane resolution

  double chi   = 2.0;
  double delta = 1.0;

  for (int i = 0; i < 15; i++) {
    chi   = (resEventPlane(chi) < res) ? chi + delta : chi - delta;
    delta = delta / 2.;
  }

  return chi;
}

//-----------------------------------------------------------------------

double resEventPlane(double chi) {
  // Calculates the event plane resolution as a function of chi

  double con = 0.626657;                   // sqrt(pi/2)/2
  double arg = chi * chi / 4.;

  double res = con * chi * exp(-arg) * (TMath::BesselI0(arg) +
					  TMath::BesselI1(arg)); 

  return res;
}

