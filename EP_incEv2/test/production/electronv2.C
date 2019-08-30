#include "Energy.h"
#include "sPhenixStyle.h"
void addpdf(TPDF* pdf)
{ 
  pdf->On();
  gPad->cd();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void electronv2()
{
  SetsPhenixStyle(); 
  // TFile* file = new TFile("incEv2.root");
  // TFile* file = new TFile("eta0.8/incEv2.root");
  TFile* file = new TFile("incEv2_0.root");
  // TFile* file = new TFile("test_out_.recenter.root");
  TCanvas* c = new TCanvas("c","c");
  TPDF* pdf = new TPDF("plots.pdf"); 
  pdf->Off();
  double ptedge[14]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,1.0,1.2,1.6,2.0,2.8,4.0};
  //resolution
  TProfile* prfres= (TProfile*)file->Get("EPRes"); 
  double res_x[10] = {0,5,10,20,30,40,50,60,70,80};
  double res_xbincenter[9];
  TH1F* rescheck = new TH1F("rescheck","res sqrt(deltasub)",9,res_x);
  TH1F* resolution = new TH1F("res","res sqrt(deltasub)",9,-0.5,8.5);
  for (int ic=0;ic<9;ic++)
  {
     double res2sub = prfres->GetBinContent(prfres->FindBin(ic));
     res2sub = sqrt(res2sub);
     double res2suberr = prfres->GetBinError(prfres->FindBin(ic));
     res2suberr = 0.5/res2sub*res2suberr;
     rescheck->SetBinContent(9-ic,res2sub);
     resolution->SetBinContent(ic+1,res2sub);
     rescheck->SetBinError(9-ic,res2suberr);
     resolution->SetBinError(ic+1,res2suberr);
     res_xbincenter[ic]=rescheck->GetBinCenter(ic+1);
     cout << res2sub  << endl;
  }
  // resolution->Draw();

  // hadron 
  // TProfile2D* pHad2D = (TProfile2D*)file->Get("pIncHadronv2");
  TProfile2D* pHad2D = (TProfile2D*)file->Get("pPionPlusv2");
  // inclusive electron
  // TProfile2D* pE2D = (TProfile2D*)file->Get("pIncEv2");
  TProfile2D* pE2D = (TProfile2D*)file->Get("pIncEv2_hitcut");
  TProfile2D* pPhoE2D = (TProfile2D*)file->Get("pTagEv2");
  TProfile2D* pPhoE2Dls = (TProfile2D*)file->Get("pTagEv2_LS");
  // TProfile* pPhoEul = (TProfile*)pPhoE2D->ProfileX("pPhoEul",centL,centH);
  // TProfile* pPhoEls = (TProfile*)pPhoE2Dls->ProfileX("pPhoEls",centL,centH);
  int centL=3,centH=9;  //start at 1
  TProfile* pHadv2cent[9],* pHadv2;
  TProfile* pEv2cent[9],* pEv2;
  TProfile* pPhoElscent[9],* pPhoEls;
  TProfile* pPhoEulcent[9],* pPhoEul;
  pEv2 = (TProfile*)pE2D->ProfileX(Form("pEv2_%d_%d",centL,centH),centL,centH); 
  pHadv2 = (TProfile*)pHad2D->ProfileX(Form("pHadv2_%d_%d",centL,centH),centL,centH);
  pPhoEul = (TProfile*)pPhoE2D->ProfileX(Form("pPhoEul_%d_%d",centL,centH),centL,centH);
  pPhoEul = (TProfile*)pPhoEul->Rebin(12,"pPhoEul",ptedge);
  pPhoEls = (TProfile*)pPhoE2Dls->ProfileX(Form("pPhoEls_%d_%d",centL,centH),centL,centH);
  pPhoEls = (TProfile*)pPhoEls->Rebin(12,"pPhoEls",ptedge);

  // for (int ic=centL;ic<=centH;ic++)
  // {
  //    pEv2cent[ic-1] = (TProfile*)pE2D->ProfileX(Form("pEv2_%d",ic),ic,ic); 
  //    // pEv2cent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  //    pHadv2cent[ic-1] = (TProfile*)pHad2D->ProfileX(Form("pHadv2_%d",ic),ic,ic);
  //    // pHadv2cent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  //    pPhoEulcent[ic-1] = (TProfile*)pPhoE2D->ProfileX(Form("pPhoEul_%d",ic),ic,ic);
  //    // pPhoEulcent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  //    pPhoElscent[ic-1] = (TProfile*)pPhoE2Dls->ProfileX(Form("pPhoEls_%d",ic),ic,ic);
  //    // pPhoElscent[ic-1]->Scale(1./resolution->GetBinContent(ic));
  //
  //    if (ic==centL) 
  //    {
  //      // cout<<"?"<<endl;
  //      pHadv2=(TProfile*)pHadv2cent[centL-1]->Clone("pHadv2");
  //      pEv2=(TProfile*)pEv2cent[centL-1]->Clone("pEv2");
  //      pPhoEls=(TProfile*)pPhoElscent[centL-1]->Clone("pPhoEls");
  //      pPhoEul=(TProfile*)pPhoEulcent[centL-1]->Clone("pPhoEul");
  //    }
  //    else 
  //    {
  //      // cout<<"??"<<endl;
  //      pHadv2->Add(pHadv2cent[ic-1],1);
  //      pEv2->Add(pEv2cent[ic-1],1 );
  //      pPhoEul->Add(pPhoEulcent[ic-1],1);
  //      pPhoEls->Add(pPhoElscent[ic-1],1);
  //    }
  // }
  //
  // cout<<"???"<<endl;
  //photonic electron
  
  TH1F* hphoE = (TH1F*)pPhoEul->ProjectionX("hphoE");
  // TProfile* hphoE = (TProfile*)pPhoEul->Clone("hphoE");
  // TProfile* hphoE = (TProfile*)pPhoEul->Clone("hphoEls");
  // hphoE->Add(pPhoEls,-1);
  for (int i=1;i<=hphoE->GetNbinsX();i++)
  {
     double Nls = pPhoEls->GetBinEntries(i);
     double v2ls = pPhoEls->GetBinContent(i);
     double Nul = pPhoEul->GetBinEntries(i);
     double v2ul = pPhoEul->GetBinContent(i);   
     double v2PhE=0;
     if (Nul!=0 && (Nul-Nls)!=0) 
       v2PhE = (Nul*v2ul-Nls*v2ls)*1./(double)(Nul-Nls);
     cout<<Nls << "  "<< Nul << " " <<v2ls <<" "<<v2ul <<endl;
     hphoE->SetBinContent(i,v2PhE);
  }
  // cout<<"????"<<endl;
  // double resMB=0, totmult=0;
  // double refmult[9]={11,22.8,41.7,70.2,112,170,249,329,402};
  // for (int icent=centL;icent<=centH;icent++) {
  //   resMB+= resolution->GetBinContent(icent)*refmult[icent-1];
  //   totmult+=refmult[icent-1];
  // }
  // resMB/=(1.0*totmult);
  // cout<<resMB<<endl;
  // resMB = sqrt(resMB); 
  // pHadv2->Scale(1./(resMB));
 pHadv2->SetMarkerStyle(20);
 pHadv2->SetMarkerColor(kBlue);
 pHadv2->Draw("psame");
 pHadv2->GetXaxis()->SetTitle("p_{T}");
 pHadv2->GetYaxis()->SetTitle("v_{2}");
 // pEv2->Scale(1./(resMB));
 pEv2->SetMarkerStyle(21);
 pEv2->SetMarkerColor(kRed);
 pEv2 = (TProfile*)pEv2->Rebin(12,"pEv2",ptedge);
//  pEv2->Rebin();
 pEv2->Draw("psame");
 // hphoE->Scale(1./(resMB));
 hphoE->SetMarkerStyle(21);
 hphoE->SetMarkerColor(kGreen);
 hphoE->Draw("psame");
  
 TFile* fprevious = TFile::Open("phoEv2.root");
 TGraphErrors* gPhoE62 = fprevious->Get("phoE62"); 
 TGraphErrors* gIncE62 = fprevious->Get("incE62"); 
 TGraphErrors* gPhoE39 = fprevious->Get("phoE39"); 
 TGraphErrors* gIncE39 = fprevious->Get("incE39"); 
 gIncE62->SetMarkerColor(kMagenta); 
 gIncE62->SetMarkerStyle(20); 
 gIncE62->Draw("psame");
 gPhoE62->SetMarkerColor(kOrange+5); 
 gPhoE62->SetMarkerStyle(20); 
 gPhoE62->Draw("psame");
 
 TFile* fxj = TFile::Open("inclusive.root");
 TProfile* incE_xj = (TProfile*)fxj->Get("hev2");
 incE_xj = (TProfile*)incE_xj->Rebin(12,"hxj",ptedge);
 incE_xj->SetMarkerColor(kOrange);
 incE_xj->SetMarkerStyle(22);
 incE_xj->Draw("psame");
 
 hphoE->Draw("psame");
 
 pEv2->Draw("psame");
 TLegend* l = new TLegend(0.6,0.6,0.8,0.8);
 l->AddEntry(gIncE62,"62GeV inclusice electron v2{2}", "pe");
 l->AddEntry(gPhoE62,"62GeV photonic electron data+embedding","pe");
 l->AddEntry(hphoE,"photonic electron v2","pe");
 l->AddEntry(pEv2,"inclusive electron v2","pe");
 l->AddEntry(incE_xj,"inc. e v2 0-80% from Xiujun","pe");
 l->AddEntry(pHadv2,"inc. charged hadron v2","pe");
 l->SetHeader("0-60 \%");
 l->Draw();
}
