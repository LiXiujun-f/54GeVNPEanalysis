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
   std::array<unsigned int, 3> const triggers = {
    580001,580011,580021
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
   float const vz = 60.0;// < cm.
   float const Verror = 1.0e-5; //
   float const Vr = 4.0; //cm
   float const vzVpdVz = 6; //cm

   // QA tracks cuts
   float const GPt = 0.20;
   int const NHitsFit = 15;
   int const NHitsDedx = 15;
   int const NHitsFit2Poss = 0.52;
   float const Dca = 3;// < cm
   float const Eta = 1.5; 
   float const TofPion=4;
   float const TpcPion=4;
}
#endif
