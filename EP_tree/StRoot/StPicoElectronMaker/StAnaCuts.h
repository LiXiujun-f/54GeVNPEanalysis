#ifndef StAnaCuts_H
#define StAnaCuts_H

/* **************************************************
 *
 *  Authors: Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>
#include <array>

namespace anaCuts
{
   std::array<unsigned int, 2> const triggers = {
    580001,580021 
   };    
   //cut before QA
   float const qavz = 30.0;// < cm.
   float const qaVerror = 1.0e-5; //
   float const qaVr = 2.0; //cm
   float const qavzVpdVz = 3; //cm

   // QA tracks cuts
   float const qaGPt = 0.20;
   int const qaNHitsFit = 20;
   int const qaNHitsDedx = 12;
   float const qaDca = 3;// < cm
   float const qaEta = 1; 
   float const qaTofPion=4;
   float const qaTpcPion=4;

   //cut 
   // float const vz = 35.0;// < cm.
   float const vz = 40.0;// < cm.
   float const Verror = 1.0e-5; //
   float const Vr = 2.0; //cm
   float const vzVpdVz = 4; //cm

   // tracks cuts
   float const GPt = 0.2;
   int const NHitsFit = 20;
   int const NHitsFit_loose = 15;
   int const NHitsDedx = 15;
   float const NHitsFit2Poss = 0.52;
   // float const Dca = 1.5;// < cm
   float const Dca = 1.5;// < cm
   float const Eta = 1; 
   float const pTof=0.04;
   float const piTof=0.01;
   float const kTof=0.04;
   float const tofe=0.04;
   float const kTpc=2;
   float const pTpc=2;
   float const piTpc=2;
   float const nESigma_partner=3.5;
   float const nESigma_Tag=2.5;
   float const tofYlocal=1.8;
   float EEdcaDaughter=1.5;  //cm

   double resolution[9]={0.156125,0.228833,0.326535,0.430296, 0.514733,0.558188,0.537687,0.451522, 0.344037};
   int const nparVz_mult = 7;
   float parVz_mult[nparVz_mult]={435.9,-0.02413,-0.003707,0.0002204,1.487e-5,-2.95e-07,-1.866e-8};
   int const nCent = 9 ;
   float Refmult_cent[nCent] = {7,16,31,54,89,138,205,299,361}; //refmult > par[i],  70-80%, 60-70%, ... ,0-5%
   // float Refmult_cent[nCent] = {6,12,24,43,70,110,165,242,292}; //refmult > par[i],  70-80%, 60-70%, ... ,0-5%

   //cuts for  event plane recentering
   // float ptMin_Re=0.15;
   float ptMin_Re=0.2;
   float ptMax_Re=2;
}
#endif
