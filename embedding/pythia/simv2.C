//this macros set the pi0-> 2gamma branch ratio 100%
//add eta mode
#include "myFunction.h"
#include "TString.h"
#include "TStopwatch.h"
#include <iostream>
using namespace std;
void simv2(int npart=1e7,int rdnb=0,int mode=0 ,TString outname="gammav2")
{
  TStopwatch*  time = new TStopwatch();
  time->Start();
  gRandom->SetSeed(rdnb);
  pydecay = TPythia6Decayer::Instance();
  pydecay->Init();

  int MomParticleId=-1;
  float Mass = -1;
  TLorentzVector* b_d;
  if (mode == 0)
  {
    setDecayChannels(554,555,554);  //555 dalitz decay
    TPythia6::Instance()->SetBRAT(554,100);
    TPythia6::Instance()->SetBRAT(555,0);
    b_d = new TLorentzVector;
    MomParticleId = Cuts::Pi0Id;
    Mass = Cuts::M_PION_0;
  }
  else if (mode == 1)
  {
    setDecayChannels(590,597,590);  //555 dalitz decay
    TPythia6::Instance()->SetBRAT(590,100);
    // TPythia6::Instance()->SetBRAT(555,0);
    b_d = new TLorentzVector;
    MomParticleId = Cuts::EtaId;
    Mass = Cuts::M_Eta;
  }
  else if (mode == 2)
  {
    setDecayChannels(590,597,593);  //eta->pipigamma
    TPythia6::Instance()->SetBRAT(593,100);
    b_d = new TLorentzVector;
    MomParticleId = Cuts::EtaId;
    Mass = Cuts::M_Eta;
  }
  else 
  {cout <<"please input the right mode(0-2)" << endl; return;}
  TClonesArray ptl("TParticle", 10);
  bookHists(mode);
  initHists();
  for (int ipart = 0; ipart < npart; ipart++)
  {
     if (!(ipart % 100000))
        cout << "____________ ipart = " << ipart / static_cast<float>(npart) << " ________________" << endl;
     getKinematics(*b_d, Mass);
     int cent=-1;
     float EP = -999;
     getCentAndEP(cent,EP);
     hCent->Fill(cent);
     hEP->Fill(EP);
     decayAndFill(MomParticleId, b_d, ptl,EP ,cent);
  }

  TFile * file = new TFile(Form("%s.root",outname.Data()),"recreate");
  Write(file);
  file->Close();
  time->Stop();
  time->Print();
}
void decayAndFill(int const kf, TLorentzVector* mother,  TClonesArray& daughters,float EP,int cent)
{
  double ptweight=0,phiweight=0;
  getPi0Weight(mother,cent,EP,ptweight,phiweight);
  pydecay->Decay(kf, mother);
  pydecay->ImportParticles(&daughters);
  // cout<<"decay start"<<endl;
  TLorentzVector gammaMom[2];
  int nGamma=0;
  int nTrk = daughters.GetEntriesFast();
  for (int iTrk=0;iTrk<nTrk;++iTrk)
  {
    TParticle* ptl0 = (TParticle*)daughters.At(iTrk);
    // cout <<ptl0->GetPdgCode() <<endl;
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
  fill(mother,&gammaMom[0],&gammaMom[1],ptweight ,phiweight,EP,cent);
}
void fill(TLorentzVector* mother,TLorentzVector* gamma1,TLorentzVector* gamma2, double ptweight,double phiweight,float EP, int cent)
{
   float mdeltaphi = getDeltaPhi(mother->Phi(),EP);
   // pPi0v2->Fill(mother->Perp() ,cent,cos(2*(mother->Phi()-EP)),weight);
   pPi0v2->Fill(mother->Perp() ,cent,cos(2*(mother->Phi()-EP)),ptweight*phiweight);
   // hPi0v2->Fill( mdeltaphi,mother->Perp(), cent, ptweight*phiweight );
   hPi0Spec->Fill(mother->Perp(),cent,ptweight);
   fillGamma(gamma1,ptweight,phiweight,EP,cent);
   fillGamma(gamma2,ptweight,phiweight,EP,cent);
}
void fillGamma(TLorentzVector* gamma,double ptweight,double phiweight,float EP,int cent)
{
   if (!gamma) return;
   float gpt = gamma->Perp();
   float gphi = gamma->Phi();
   // cout <<"weight: "<<weight<<" cent: "<<cent<<" pt: "<<gpt<<" cos(2#delta#phi):"<< cos(2*(gphi-EP))<< " deltaphi: "<<getDeltaPhi(gphi,EP) << endl;
   pGammav2->Fill(gpt,gamma->Eta(),cent,cos(2*(gphi-EP)) ,ptweight*phiweight);
   // hGammav2[cent]->Fill(getDeltaPhi(gphi,EP),gpt,gamma->Eta(),ptweight*phiweight);
   hGammaSpec->Fill(gpt,gamma->Eta(),cent,ptweight);
}
void setDecayChannels(int const defirst,int const desecond,int const mdme)
{
   for (int idc = defirst; idc < desecond+1; idc++) {
     TPythia6::Instance()->SetMDME(idc, 1, 0);
   }
   TPythia6::Instance()->SetMDME(mdme, 1, 1);
   for (int idc = defirst;idc<desecond+1;idc++){
     if (idc!=mdme) TPythia6::Instance()->SetBRAT(idc,0);
   }
}
void getKinematics(TLorentzVector& b, double const mass)
{
   float const pt = gRandom->Uniform(Cuts::momentumrange.first,Cuts::momentumrange.second);
   float const y = gRandom->Uniform( Cuts::EtaRange*-1, Cuts::EtaRange);
   float const phi = TMath::TwoPi() * gRandom->Rndm();
   float const mT = sqrt(mass * mass + pt * pt);
   float const pz = mT * sinh(y);
   float const E = mT * cosh(y);
   b.SetPxPyPzE(pt * cos(phi), pt * sin(phi) , pz, E);
   // b.SetPtEtaPhiM(pt,y,phi,mass);
}
void getPi0Weight(TLorentzVector* mother,int cent,float EP,double &ptweight,double &phiweight)
{
  double ptrangeweight = Cuts::momentumrange.second-Cuts::momentumrange.first;
   float mpt = mother->Perp();
   float mphi = mother->Phi();
   phiweight = 1+2*fPi0v2[cent]->Eval(mpt)*std::cos(2*(mphi-EP));
   ptweight = fpispectra[cent]->Eval(mpt)*(ptrangeweight)*(2*Cuts::EtaRange); 
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
  cent = floor(gRandom->Rndm()*6);
  //ignore the 60-80%, as the spectra has some problem
  cent+=2;
  if (cent==7) cent = gRandom->Rndm()>0.5?7:8;
}
void bookHists(int mode)
{
  //pi0spectra and v2
  for (int ic=0;ic<Cuts::nCent;ic++)
  {
    if (mode ==0) fpispectra[ic] = new TF1(Form("pi0spectra_%d",ic),"2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15); 
    else if (mode ==1 || mode==2) fpispectra[ic] = new TF1(Form("etaspectra_%d",ic),"2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*sqrt(x*x+0.547862*0.547862-0.134977*0.134977)-[2]*(x*x+0.547862*0.547862-0.134977*0.134977))+sqrt(x*x+0.547862*0.547862-0.134977*0.134977)/[3], -[4])",0,15);
    else { cout<<" fatal error! mode is not correct!" <<endl; return;}
    fpispectra[ic]->SetParameters(Cuts::SpectraParPi0[Cuts::SpectraParPi0_centbin[ic]]);
  }
  if (mode == 0)
  {
    TFile* file = TFile::Open("data/fitpionv2.root");
    TString namev2[9]={"50_60","50_60","50_60","40_50","30_40","20_30","10_20","0_10","0_10"};
    for (int ic=0;ic<Cuts::nCent;ic++)
    {
       fPi0v2[ic] = (TF1*)file->Get(Form("fit_%s", namev2[ic].Data()));
    }
  }
  else if (mode == 1 || mode ==2)
  {
    TFile* file = TFile::Open("data/fitKaonv2.root");
    TString namev2[9]={"40_80","40_80","40_80","40_80","10_40","10_40","10_40","0_10","0_10"};
    for (int ic=0;ic<Cuts::nCent;ic++)
    {
       fPi0v2[ic] = (TF1*)file->Get(Form("fit_%s", namev2[ic].Data()));
    }
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
     // hGammav2[ic] = new TH3D(Form("hGammav2_%d",ic),"hGammav2;#delta#phi;p_{T}[GeV/c];#eta;",100,0,TMath::Pi() ,150,0,15,150,-1.5,1.5);
     // hGammav2[ic]->SetDirectory(0);
   }
 
   //gamma spectra
   hGammaSpec = new TH3D("hGammaSpec","hGammaSpec;p_{T}[GeV/c];#eta;Cent",150,0,15,150,-1.5,1.5,9,-0.5,8.5);
   hGammaSpec->SetDirectory(0);
 
   //for check?
   pPi0v2 = new TProfile2D("pPi0v2","pPi0v2;p_{T};Cent",150,0,15,9,-0.5,8.5);
   // hPi0v2 = new TH3D("hPi0v2","hPi0v2;#delta#phi;p_{T};Cent",100,0,TMath::Pi() ,150,0,15,9,-0.5,8.5);
   hPi0Spec = new TH2D("hPi0Spec","hPi0Spec;p_{T};Cent",150,0,15,9,-0.5,8.5);
}
void Write(TFile* file)
{
   hEP->Write();
   hCent->Write();
   pGammav2->Write();
   for (int ic=0;ic<Cuts::nCent;ic++)
   {
      // hGammav2[ic]->Write();
      // fpispectra[ic]->Write();
      // fPi0v2[ic]->Write();
   }
   hGammaSpec->Write(); 
   pPi0v2->Write(); 
   // hPi0v2->Write();
   hPi0Spec->Write();
}
