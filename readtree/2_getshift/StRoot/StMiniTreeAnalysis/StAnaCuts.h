#ifndef StAnaCuts_H
#define StAnaCuts_H

#include "Rtypes.h"
#include <string>
#include <array>

namespace anaCuts
{
   std::array<unsigned int, 2> const triggers = {
    580001,580021
   };    
   
   int const  nRunNum=614;
   //cut 
   // float const vz = 35.0;// < cm.
   float const vz = 40.0;// < cm.
   // float const vz = 60.0;// < cm.
   float const Verror = 1.0e-5; //
   float const Vr = 1.0; //cm
   float const vzVpdVz = 3; //cm

   // tracks cuts
   float const GPt = 0.20;
   // float const GPt = 0.15;
   int const NHitsFit = 20;
   int const NHitsDedx = 15;
   float const NHitsFit2Poss = 0.52;
   float const Dca = 1.5;// < cm
   // float const Dca = 1;// < cm
   // float const Eta = 0.8; 
   float const Eta = 1; 
   float const pTof=0.02;
   float const piTof=0.02;
   float const kTof=0.02;
   float const kTpc=2;
   float const pTpc=2;
   float const piTpc=2;
   float const nESigma_partner=3.5;
   float const nESigma_Tag=2; 
   
   float const tofYlocal=1.8;
   float EEdcaDaughter=1;  //cm

   // int const nparVz_mult = 7;
   // float parVz_mult[nparVz_mult]={435.9,-0.02413,-0.003707,0.0002204,1.487e-5,-2.95e-07,-1.866e-8};
   int const nCent = 9 ;
   // float Refmult_cent[nCent] = {6,12,24,43,70,110,165,242,292}; //refmult > par[i],  70-80%, 60-70%, ... ,0-5%
   float Refmult_cent[nCent] = {7,16,31,54,89,138,205,299,361};
   // double resolution[9]={0.155569,0.229148,0.327245,0.43032,0.514879,0.558227,0.538259,0.452528,0.343969};

   //cuts for  event plane recentering
   // float ptMin_Re=0.15;
   float ptMin_Re=0.2;
   float ptMax_Re=2;
}
#endif
