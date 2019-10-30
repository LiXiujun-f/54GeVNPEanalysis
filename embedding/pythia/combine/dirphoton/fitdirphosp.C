#include "sPhenixStyle.h"
void setGraphStyle(TGraphErrors* g,int color)
{
  g->SetMarkerColor(color);
  g->SetMarkerStyle(30);
  g->SetMarkerSize(1.5);
  g->SetLineColor(color);
}
void dNdpTdY(double *x, double *p)
{
  double denominator = pow(1+x[0]*x[0]/p[2],p[3]);
  if (denominator!=0) return 2*TMath::Pi()*x[0]*p[0]*p[1]/denominator;
  else return 0;
}

void myFit(double *x, double *p)
{
  double denominator = pow(1+x[0]*x[0]/p[2],p[3]);
  if (denominator!=0) return p[0]*p[1]/denominator;
  else return 0;
}
// Double_t func1(Double_t *x, Double_t *par){
//   Double_t arg = 0;
//   Double_t val = 0;
//   if(par[2]!=0 && par[1]!=1){
//     arg = 1+sqrt(x[0]*x[0])/par[2]*(1-par[1]);//1.+(par[1]-1)*sqrt(x[0]*x[0]+0.149*0.149)/par[2];
//     val = par[0]*(sqrt(x[0]*x[0])*TMath::Power(arg,-1/(1-par[1])));//exp(arg*(1/(1-par[1])));
//   }
//   return val;
// }
void fitdirphosp()
{
  //the main purpose is to scale the 200 GeV spectra to 62 GeV and then fit with 62 GeV data
 
  SetsPhenixStyle();
  TFile* file = new TFile("dirphospectra.root");
  TGraphErrors* g62[2];
  TGraphErrors* g200_1[3];
  TGraphErrors* g200_2[2];
  TGraphErrors* g200_3[8];
  TGraphErrors* g200_comb[3]; // combine the g200_3 into 3 centbin
  TGraphErrors* g62PP_scale[3];
  TGraphErrors* g63PP1_scale[3];
  TGraphErrors* g63PP2_scale[3];

  TGraphErrors* g62PP;
  TGraphErrors* g63PP1;
  TGraphErrors* g63PP2;

  TString centname[]={"0_20","20_40","40_60","0_5","5_10","10_15","15_20","20_30","30_40","40_50","50_60"};
  TF1* fit[3];
  TF1* fit_pt[3];
  TF1* test = new TF1("func1", myFit, 0,15,4);
  double par[4]={1,8.2,2.26,2.45};
  test->SetParameters(par);
  TMultiGraph* gmult[3];
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("dirphospectra.pdf");
  pdf->Off();

  double scalefactor_200[3]={ pow(341./519., 1.225 ), pow( 151.75/225.4,1.225) , pow( 57.77/85.5,1.225) };
  // double scalefactor_200[3]={ 1./pow(519.025, 1.25 ), 1./pow( 225.425,1.25) , 1./pow( 85.475,1.25) };
  // double scalefactor_62pp[3]={1./pow(1.864,1.25)/35.11, 1./pow(1.864,1.25)/35.11, 1./pow(1.864,1.25)/35.11};
  double scalefactor_62pp[3]={ 656.59/35.61, 241.1/35.61, 70.69/35.61};
  for (int i=0;i<2;i++)
  {
    g62[i] = (TGraphErrors*)file->Get(Form("dirpho_62_%s",centname[i].Data()));
    g200_2[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s_2",centname[i].Data()));
    for (int ip=0;ip<g200_2[i]->GetN();ip++)
    {
       double x,y;
       g200_2[i]->GetPoint(ip,x,y);
       g200_2[i]->SetPoint(ip,x,y*scalefactor_200[i] );
    }
    // for (int ip=0;ip<g62[i]->GetN();ip++)
    // {
    //    double x,y;
    //    g62[i]->GetPoint(ip,x,y);
    //    g62[i]->SetPoint(ip,x,y*2*TMath::Pi()*x );
    // }
  }
  for (int i=0;i<3;i++)
  {
    g200_1[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s",centname[i].Data()));
    g200_1[i]->Draw("PA");
    for (int ip=0;ip<g200_1[i]->GetN();ip++)
    {
       double x,y;
       g200_1[i]->GetPoint(ip,x,y);
       g200_1[i]->SetPoint(ip,x,y*scalefactor_200[i] );
    }

    addpdf(pdf);
  }
  gPad->SetLogy();
  for (int i=0;i<8;i++)
  {
    g200_3[i] = (TGraphErrors*)file->Get(Form("dirpho_200_%s",centname[i+3].Data()));
    g200_3[i]->SetMarkerColor(kBlue);
  }
  g200_comb[0]=(TGraphErrors*)g200_3[2]->Clone("dirpho_200_0_20_comb");
  for (int ib=0;ib<g200_comb[0]->GetN();ib++)
  {
     double x=0,y=0,errx=0,erry=0;
     double tx,ty,terrx,terry;
     for (int ig=0;ig<4;ig++)
     {
        g200_3[ig]->GetPoint(ib,tx,ty);
        terry = g200_3[ig]->GetErrorY(ib);
        x=tx;
        // cout <<ib <<" "<<x << endl;
        y+=ty;
        erry=terry*terry;
     }
    
     y=y*scalefactor_200[0]/4.;
     erry = sqrt(erry);
     g200_comb[0]->SetPoint(ib,x,y);
     g200_comb[0]->SetPointError(ib,0 ,erry);
  }

  g200_comb[1]=(TGraphErrors*)g200_3[5]->Clone("dirpho_200_20_40_comb");
  for (int ib=0;ib<g200_comb[1]->GetN();ib++)
  {
     double x=0,y=0,errx=0,erry=0;
     double tx,ty,terrx,terry;
     
     for (int ig=0;ig<2;ig++)
     {
        // cout<< g200_3[ig+4]->GetName()<<endl;
        g200_3[ig+4]->GetPoint(ib,tx,ty);
        // cout << tx << " "<<ty<<endl;;
        terry = g200_3[ig+4]->GetErrorY(ib);
        x=tx;
        y+=ty;
        erry=terry*terry;
     }
     y=y*0.5*scalefactor_200[1];
     erry = sqrt(erry);
     g200_comb[1]->SetPoint(ib,x,y);
     // cout<<"combine: " << x <<" " << y<< endl;
     g200_comb[1]->SetPointError(ib,0 ,erry);
  }

  g200_comb[2]=(TGraphErrors*)g200_3[6]->Clone("dirpho_200_40_60_comb");
  for (int ib=0;ib<g200_comb[2]->GetN();ib++)
  {
     double x=0,y=0,errx=0,erry=0;
     double tx,ty,terrx,terry;
     for (int ig=0;ig<2;ig++)
     {  
        g200_3[ig+6]->GetPoint(ib,tx,ty);
        terry = g200_3[ig+6]->GetErrorY(ib);
        x=tx;
        // cout<< g200_3[ig+6]->GetName()<<endl;
        // cout <<ib <<" "<<x << endl;
        y+=ty;
        erry=terry*terry;
     }
     y=y*0.5*scalefactor_200[2];
     erry = sqrt(erry);
     g200_comb[2]->SetPoint(ib,x,y);
     g200_comb[2]->SetPointError(ib,0 ,erry);
  }

  TGraphErrors* g62PP = (TGraphErrors*)file->Get("dirpho_62_pp");
  TGraphErrors* g63PP1 = (TGraphErrors*)file->Get("dirpho_63_pp1");
  TGraphErrors* g63PP2 = (TGraphErrors*)file->Get("dirpho_63_pp2");

  for (int ic=0;ic<3;ic++)
  {
    g62PP_scale[ic] = (TGraphErrors*)g62PP->Clone(Form("dirpho_62pp_%s",centname[ic]));
    for (int ib=0;ib<g62PP->GetN();ib++)
    {
       double x=0,y=0,errx=0,erry=0;
       double tx,ty,terrx,terry;
       g62PP->GetPoint(ib,tx,ty);
       terry = g62PP->GetErrorY(ib);
       x=tx;
       y=ty;
       erry=terry*scalefactor_62pp[ic];
       y=y*scalefactor_62pp[ic];
       g62PP_scale[ic]->SetPoint(ib,x,y);
       g62PP_scale[ic]->SetPointError(ib,0 ,erry);
    }
    setGraphStyle(g62PP_scale[ic] ,kGreen+2 );
  }

  for (int ic=0;ic<3;ic++)
  {
    g63PP1_scale[ic] = (TGraphErrors*)g63PP1->Clone(Form("dirpho_63pp1_%s",centname[ic]));
    for (int ib=0;ib<g63PP1->GetN();ib++)
    {
       double x=0,y=0,errx=0,erry=0;
       double tx,ty,terrx,terry;
       g63PP1->GetPoint(ib,tx,ty);
       terry = g63PP1->GetErrorY(ib);
       x=tx;
       y=ty;
       erry=terry*scalefactor_62pp[ic];
       y=y*scalefactor_62pp[ic];
       g63PP1_scale[ic]->SetPoint(ib,x,y);
       g63PP1_scale[ic]->SetPointError(ib,0 ,erry);
    }
    setGraphStyle(g63PP1_scale[ic] ,kGreen+2 );
  }

  for (int ic=0;ic<3;ic++)
  {
    g63PP2_scale[ic] = (TGraphErrors*)g63PP2->Clone(Form("dirpho_63pp2_%s",centname[ic]));
    for (int ib=0;ib<g63PP2->GetN();ib++)
    {
       double x=0,y=0,errx=0,erry=0;
       double tx,ty,terrx,terry;
       g63PP2->GetPoint(ib,tx,ty);
       terry = g63PP2->GetErrorY(ib);
       x=tx;
       y=ty;
       erry=terry*scalefactor_62pp[ic];
       y=y*scalefactor_62pp[ic];
       g63PP2_scale[ic]->SetPoint(ib,x,y);
       g63PP2_scale[ic]->SetPointError(ib,0 ,erry);
    }
    setGraphStyle(g63PP2_scale[ic] ,kGreen+2 );
  } 

  TH1F* h = new TH1F("h","h",20,0,20);
  h->GetYaxis()->SetRangeUser(1e-12,1);
  h->Draw();

  //below are the fitting part
  TH1F* h = new TH1F("h","h",5,0,15);
  for (int i=0;i<3;i++)
  {
     // c->Clear();
     h->Draw("same");
     h->GetYaxis()->SetRangeUser(-0.15,0.3);
     // fit[i] = new TF1(Form("fitdirphosp_%s",centname[i].Data()), myFit,0,15,7);
     fit[i] = new TF1(Form("fitdirphoInv_%s",centname[i].Data()),myFit,0,15,4);
     fit[i]->SetNpx(15000);
     fit_pt[i] = new TF1(Form("fitdirphoInv_pt_%s",centname[i].Data()),dNdpTdY,0,15,4);
     fit_pt[i]->SetNpx(15000);
     fit_pt[i]->GetYaxis()->SetTitle("dNdp_{T}dY");
     fit[i]->SetParameters(par);
     gmult[i] = new TMultiGraph();

     setGraphStyle(g200_1[i],kRed);
     setGraphStyle(g200_comb[i],kBlue);
     if (i<2) setGraphStyle(g200_2[i],kMagenta);

     gmult[i]->Add(g200_1[i]);
     gmult[i]->Add(g62PP_scale[i]);
     gmult[i]->Add(g63PP1_scale[i]);
     gmult[i]->Add(g63PP2_scale[i]);
     //
     //we will not use 200 GeV spectra at high pt
     // gmult[i]->Add(g200_comb[i]);
     // instead we will take pp data to do the scaling
     //
     if (i<2) 
     {
       gmult[i]->Add(g200_2[i]);
       gmult[i]->Add(g62[i]);
     }
     
     gmult[i]->SetName(Form("dirpho_%s",centname[i].Data()));
     gmult[i]->Draw("psame");
     gmult[i]->Fit(fit[i]);
     fit[i]->Draw("same");
     fit[i]->GetParameters(par);
     fit_pt[i]->SetParameters(par);
     // gPad->SetLogx();
     addpdf(pdf);
  }

  pdf->On();
  pdf->Close();
  TFile* dirphov2input = new TFile("dirphospinput.root","recreate");

  for (int i=0;i<3;i++)
  {
    gmult[i]->Write();
    fit[i]->Write(); 
    fit_pt[i]->Write(); 
  } 
}
