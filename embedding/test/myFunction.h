#include "TPythia6Decayer.h"
#include "TPythia6.h"
#include "TParticle.h"
#include "TF1.h"
#include "TH3.h"
#include "TH1.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TProfile.h"
#include "TProfile3D.h"
#include "TProfile2D.h"
#include "TH2.h"
#include "TFile.h"

//constants

namespace Cuts
{
  double M_PION_0 = 0.134977;
  int Pi0Id=111;
  int GammaId = 22;
  int nCent = 9;
  int const SpectraParPi0_centbin[9]={0,0,0,1,2,3,4,5,5};
  // double const SpectraParPi0[5][5]={ 
  //                              {1.67607, -1.77146, 0.0759459, -0.369874, 2.01742},
  //                              {264.787, 0.349324, 0.0475088, 1.01322, 10.903},
  //                              {406.429, 0.361361, 0.0558035, 1.10099, 11.2414},
  //                              {682.788, 0.396488, 0.0690007, 1.06078, 11.0674},
  //                              {992.598, 0.406828, 0.0831998, 1.04464, 11.0797}
  //                            };
     double const SpectraParPi0[6][5]={ 
                               {196.3,0.511,0.04127,0.7721,8.314},
                               {311,0.4898,0.05927,0.7918,8.425},
                               {486.5,0.5034,0.13,0.7511,8.22},
                               {656.5,0.518,0.084,0.7946,8.436},
                               {853,0.5369,0.06,0.8268,8.626},
                               {1320,0.564,0.1222,0.7899,8.532}
                             };

};

TH1F* hEP;
TH1F* hCent;
TPythia6Decayer* pydecay;
TProfile2D* pPi0v2;
TH3D* hPi0v2;
TH2D* hPi0Spec;
TF1* fPi0v2[9];
TF1* fpispectra[9];
TProfile3D* pGammav2;
TH3D* hGammav2[9];
TH3D* hGammaSpec; 

void decayAndFill(int const kf, TLorentzVector* mother, TClonesArray& daughters,float EP ,int cent);
void fill(TLorentzVector* mother,TLorentzVector* gamma1,TLorentzVector* gamma2, double weight,float EP, int cent);
void fillGamma(TLorentzVector* gamma,double weight,float EP,int cent);
void setDecayChannels(int const defirst,int const desecond,int const mdme);
void getKinematics(TLorentzVector& b, double const mass);
double getPi0Weight(TLorentzVector* mother,int cent,float EP);
float getDeltaPhi(float mphi,float EP);
void bookHists();
void getCentAndEP(int & cent,float & EP);
void initHists();
void Write(TFile* file);
