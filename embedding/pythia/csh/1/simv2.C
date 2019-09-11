#include "myFunction.h"
#include <iostream>
using namespace std;
void simv2(int npart=1e7,int rdnb=0,String outname="gammav2")
{
  gRandom->SetSeed(rdnb);
  pydecay = TPythia6Decayer::Instance();
  pydecay->Init();

  setDecayChannels(554,555,554);  //555 dalitz decay
  TPythia6::Instance()->SetBRAT(554,100);
  TPythia6::Instance()->SetBRAT(555,0);
  TLorentzVector* b_d = new TLorentzVector;
  
  TClonesArray ptl("TParticle", 10);
  bookHists();
  initHists();
  for (int ipart = 0; ipart < npart; ipart++)
  {
     if (!(ipart % 100000))
        cout << "____________ ipart = " << ipart / static_cast<float>(npart) << " ________________" << endl;
     getKinematics(*b_d, Cuts::M_PION_0);
     int cent=-1;
     float EP = -999;
     getCentAndEP(cent,EP);
     hCent->Fill(cent);
     hEP->Fill(EP);
     decayAndFill(Cuts::Pi0Id, b_d, ptl,EP ,cent);
  }

  TFile * file = new TFile(Form("%s.root",outname.Data()),"recreate");
  Write(file);
  file->Close();
}
void decayAndFill(int const kf, TLorentzVector* mother,  TClonesArray& daughters,float EP,int cent)
{
  double weight = getPi0Weight(mother,cent,EP);
  pydecay->Decay(kf, mother);
  pydecay->ImportParticles(&daughters);
  // cout<<"decay start"<<endl;
  TLorentzVector gammaMom[2];
  int nGamma=0;
  int nTrk = daughters.GetEntriesFast();
  for (int iTrk=0;iTrk<nTrk;++iTrk)
  {
    TParticle* ptl0 = (TParticle*)daughters.At(iTrk);
    if (abs(ptl0->GetPdgCode())==Cuts::GammaId)
    {
      if (nGamma>=2) {
         cout <<"??????????? more than 2 gamma!" <<endl;
         continue;
       }
       ptl0->Momentum(gammaMom[nGamma]);
       nGamma++; 
    }
    else continue;
   }
  daughters.Clear();
  fill(mother,&gammaMom[0],&gammaMom[1],weight,EP,cent);
}
void fill(TLorentzVector* mother,TLorentzVector* gamma1,TLorentzVector* gamma2, double weight,float EP, int cent)
{
  float mdeltaphi = getDeltaPhi(mother->Phi(),EP);
  pPi0v2->Fill(mother->Perp() ,cent,cos(2*(mother->Phi()-EP)),weight);
  hPi0v2->Fill( mdeltaphi,mother->Perp(), cent, weight );
  hPi0Spec->Fill(mother->Perp(),cent,weight);
  fillGamma(gamma1,weight,EP,cent);
  fillGamma(gamma2,weight,EP,cent);
}
void fillGamma(TLorentzVector* gamma,double weight,float EP,int cent)
{
   float gpt = gamma->Perp();
   float gphi = gamma->Phi();
   pGammav2->Fill(gpt,gamma->Eta(),cent,cos(2*(gphi-EP)) ,weight);
   hGammav2[cent]->Fill(getDeltaPhi(gphi,EP),gpt,gamma->Eta(),weight);
   hGammaSpec->Fill(gpt,gamma->Eta(),cent,weight);
}
void setDecayChannels(int const defirst,int const desecond,int const mdme)
{
   for (int idc = defirst; idc < desecond+1; idc++) TPythia6::Instance()->SetMDME(idc, 1, 0);
   TPythia6::Instance()->SetMDME(mdme, 1, 1);
}
void getKinematics(TLorentzVector& b, double const mass)
{
   float const pt = gRandom->Uniform(0,15);
   float const y = gRandom->Uniform(-1.5, 1.5);
   float const phi = TMath::TwoPi() * gRandom->Rndm();
   float const mT = sqrt(mass * mass + pt * pt);
   float const pz = mT * sinh(y);
   float const E = mT * cosh(y);
   b.SetPxPyPzE(pt * cos(phi), pt * sin(phi) , pz, E);
}
double getPi0Weight(TLorentzVector* mother,int cent,float EP)
{
  float mpt = mother->Perp();
  float mphi = mother->Phi();
  float phiweight = 1+2*fPi0v2[cent]->Eval(mpt)*std::cos(2*(mphi-EP));
  float ptweight = fpispectra[cent]->Eval(mpt); 
  return ptweight*phiweight;
}
float getDeltaPhi(float mphi,float EP)
{
   //mv  into the same period
   float mdeltaphi;
   if (mphi<0) mphi+=2*TMath::Pi();
   mdeltaphi = mphi-EP;
   if (mdeltaphi<0) mdeltaphi+=2*TMath::Pi();
   if (mdeltaphi>TMath::Pi()) mdeltaphi-=TMath::Pi();
   return mdeltaphi;
}
void getCentAndEP(int & cent, float & EP)
{
  EP = gRandom->Rndm()*TMath::Pi();
  cent = floor(gRandom->Rndm()*9);
}
void bookHists()
{
  //pi0spectra and v2
  for (int ic=0;ic<Cuts::nCent;ic++)
  {
    fpispectra[ic] = new TF1("pi0spectra","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15); 
    fpispectra[ic]->SetParameters(Cuts::SpectraParPi0[Cuts::SpectraParPi0_centbin[ic]]);
  }
  TFile* file = TFile::Open("fitpionv2.root");
  TString namev2[9]={"50_60","50_60","50_60","40_50","30_40","20_30","10_20","0_10","0_10"};
  for (int ic=0;ic<Cuts::nCent;ic++)
  {
     fPi0v2[ic] = (TF1*)file->Get(Form("fit_%s", namev2[ic].Data()));
  }
}
void initHists()
{
  //event level
  hCent = new TH1F("hCent","hCent",9,-0.5,8.5);
  hEP = new TH1F("hEP","hEP",360,0,TMath::Pi());

  //gamma v2
  pGammav2 = new TProfile3D("pGammav2","pGammav2;p_{T}[GeV/c];#eta;Cent",150,0,15,150,-1.5,1.5,9,-0.5,8.5);
  pGammav2->SetDirectory(0);
  for (int ic=0;ic<Cuts::nCent;ic++){
    hGammav2[ic] = new TH3F(Form("hGammav2_%d",ic),"hGammav2;#delta#phi;p_{T}[GeV/c];#eta;",100,0,TMath::Pi() ,150,0,15,150,-1.5,1.5);
    hGammav2[ic]->SetDirectory(0);
  }
  //gamma spectra
  hGammaSpec = new TH3F("hGammaSpec","hGammaSpec;p_{T}[GeV/c];#eta;Cent",150,0,15,150,-1.5,1.5,9,-0.5,8.5);
  hGammaSpec->SetDirectory(0);

  //for check?
  pPi0v2 = new TProfile2D("pPi0v2","pPi0v2;p_{T};Cent",150,0,15,9,-0.5,8.5);
  hPi0v2 = new TH3F("hPi0v2","hPi0v2;#delta#phi;p_{T};Cent",100,0,TMath::Pi() ,150,0,15,9,-0.5,8.5);
  hPi0Spec = new TH2F("hPi0Spec","hPi0Spec;p_{T};Cent",150,0,15,9,-0.5,8.5);
}
void Write(TFile* file)
{
   hEP->Write();
   hCent->Write();
   pGammav2->Write();
   for (int ic=0;ic<Cuts::nCent;ic++)
   {
      hGammav2[ic]->Write();
   }
   hGammaSpec->Write(); 
   pPi0v2->Write(); 
   hPi0v2->Write();
   hPi0Spec->Write();
}
