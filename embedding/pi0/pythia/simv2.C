#include "myFunction.h"
void simv2()
{
  gRandom->SetSeed(0);
  TPythia6Decayer* pydecay = TPythia6Decayer::Instance();
  pydecay->Init();

  setDecayChannels(554,555,554);  //555 dalitz decay
  TPythia6::Instance()->SetBRAT(554,100);
  TPythia6::Instance()->SetBRAT(555,0);
  TLorentzVector* b_d = new TLorentzVector;
  
  TClonesArray ptl("TParticle", 10);
  bookHists();
  for (int ipart = 0; ipart < npart; ipart++)
  {
     if (!(ipart % 100000))
        cout << "____________ ipart = " << ipart / static_cast<float>(npart) << " ________________" << endl;
      getKinematics(*b_d, M_PION_0);
      int cent=-1;
      float EP = -999;
      getCentAndEP(cent,EP);
      decayAndFill(111, b_d,getPi0Weight(b_d->Perp()), ptl,EP ,cent);
  }

  TFile * file = new TFile("gammav2.root","recreate");
  pGammav2->Write();
  file->Close();
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
void decayAndFill(int const kf, TLorentzVector* mother, double const weight, TClonesArray& daughters,float EP,int cent)
{
  pydecay->Decay(kf, mother);
  pydecay->ImportParticles(&daughters);
  // cout<<"decay start"<<endl;
  TLorentzVector gammaMom[2];
  int nGamma=0;
  int nTrk = daughters.GetEntriesFast();
  for (int iTrk=0;iTrk<nTrk;++iTrk)
  {
    TParticle* ptl0 = (TParticle*)daughters.At(iTrk);
    if (abs(ptl0->GetPdgCode()))
    {
      if (nGamma>2) {
         cout <<"??????????? more than 2 gamma!" <<endl;
         continue;
       }
       ptl0->Momentum(gammaMom[nGamma]);
       nGamma++; 
    }
    else continue;
   }
  daughters.Clear();
  fill(mother,gammaMom[0], weight,EP,cent);
  fill(mother,gammaMom[1], weight,EP,cent);
}
void fill(TLorentzVector* mother,TLorentzVector* gamma, double weight,float EP, int cent)
{
  float mpt = ;
  float mphi = ;
  float phiweight = ;
  float ptweight = ;
  float gpt = ;
  float gphi = ;

  //have to decide whether we need to consider eta dependence

}
void bookHists()
{
  //gamma v2
  pGammav2 = new TProfile2D();
  pGammav2->SetDirectory(0);

  //pi0spectra
  for (int i=0;i<9;i++)
  {
    
  }

  //pi0v2
  for (int i=0;i<9;i++)
  {
    
  }
}
void getCentAndEP(int & cent, float & EP)
{
  EP = gRandom->Rndm()*TMath::Pi();
  cent = floor(gRandom->Rndm()*9);
}
