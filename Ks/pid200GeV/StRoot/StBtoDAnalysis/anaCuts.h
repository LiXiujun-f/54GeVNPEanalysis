#ifndef anaCuts_H
#define anaCuts_H

/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */

#include "TString.h"

namespace anaCuts
{
    std::array<unsigned int, 6> const triggers = {
    520001,    //VPDMB-5-p-sst_
    520011,   
    520021,  
    520031,    
    520041,
    520051
   };    // vpdmb-5-p-nobsmd
    int   const nPtBinsSig = 5;
    float const PtEdgeSig[nPtBinsSig+1] = {0., 1.5, 2., 3., 5., 15.}; //0-1, no counts
    float const meanSig[nPtBinsSig] = {1.86513, 1.86419, 1.86552, 1.86566, 1.86782};
    float const sigmaSig[nPtBinsSig] = {0.0145157, 0.0138427, 0.0153643, 0.0177216, 0.0236697};
    
    int   const nPtBins = 5;
    float const PtEdge[nPtBins+1] = {0., 1., 2., 3., 5., 15.};
    
    float const pt = 0.6;
    float const eta = 1.0;
    
    float const rapidity = 1.0;
    
    float const massMin = 1.828;
    float const massMax = 1.892;
    
    float const rightHalfLowEdge = -1.54696;
    float const rightHalfHighEdge = 1.59464;
    
    //float const dcaV0ToPv[nPtBins] = {60, 60, 60, 60, 60};
    //float const decayLength[nPtBins] = {80, 80, 80, 80, 80};
    //float const cosTheta[nPtBins] = {0.95, 0.95, 0.95, 0.95, 0.95};
    //float const dcaDaughters[nPtBins] = {100, 100, 100, 100, 100}; //0.0050;
    //float const kDca[nPtBins] = {60, 60, 60, 60, 60};//0.008, // minimum
    //float const pDca[nPtBins] = {60, 60, 60, 60, 60};//0.008
    float const dcaV0ToPv[nPtBins] = {0.0061, 0.0049, 0.0038, 0.0038, 0.0040};
    float const decayLength[nPtBins] = {0.0145, 0.0181, 0.0212, 0.0247, 0.0259};
    float const cosTheta[nPtBins] = {0.95, 0.95, 0.95, 0.95, 0.95};
    float const dcaDaughters[nPtBins] = {0.0084, 0.0066, 0.0057, 0.0050, 0.0060}; //0.0050;
    float const kDca[nPtBins] = {0.0103, 0.0091, 0.0095, 0.0079, 0.0058};//0.008, // minimum
    float const pDca[nPtBins] = {0.0110, 0.0111, 0.0086, 0.0081, 0.0062};//0.008
    
    //tight cuts
    float const dcaV0ToPv1[nPtBins] = {0.0044, 0.0036, 0.0031, 0.0026, 0.0032};
    float const decayLength1[nPtBins] = {0.0144, 0.0204, 0.0242, 0.0245, 0.0300};
    float const cosTheta1[nPtBins] = {0.95, 0.95, 0.95, 0.95, 0.95};
    float const dcaDaughters1[nPtBins] = {0.0069, 0.0048, 0.0044, 0.0049, 0.0047}; //0.0050;
    float const kDca1[nPtBins] = {0.0119, 0.0110, 0.0109, 0.0106, 0.0080};//0.008, // minimum
    float const pDca1[nPtBins] = {0.0120, 0.0102, 0.0118, 0.0109, 0.0096};//0.008
    
    //loose cuts
    float const dcaV0ToPv2[nPtBins] = {0.0072, 0.0053, 0.0047, 0.0042, 0.0062};
    float const decayLength2[nPtBins] = {0.0110, 0.0168, 0.0187, 0.0199, 0.0180};
    float const cosTheta2[nPtBins] = {0.95, 0.95, 0.95, 0.95, 0.95};
    float const dcaDaughters2[nPtBins] = {0.0077, 0.0078, 0.0074, 0.0068, 0.0066}; //0.0050;
    float const kDca2[nPtBins] = {0.0105, 0.0068, 0.0080, 0.0066, 0.0041};//0.008, // minimum
    float const pDca2[nPtBins] = {0.0092, 0.0078, 0.0086, 0.0065, 0.0047};//0.008
    
    // all variables with a phys prefix are for final physics plots
    //int const physNCentralities = 4;
    //int const physCentralityEdges[physNCentralities+1] = {0,3,5,6,9}; // 40-80, 20-40, 10-20, 0-10
    //TString const physCentralityName[physNCentralities] = {"40-80%","20-40%","10-20%","0-10%"};
    int const physNCentralities = 9;
    int const physCentralityEdges[physNCentralities+1] = {0,0,1,2,3,4,5,6,7,8}; // 70-80, 60-70, 50-60, 40-50, 30-40, 20-30, 10-20, 5-10, 0-5
    TString const physCentralityName[physNCentralities] = {"70-80%","60-70%","50-60%","40-50%","30-40%","20-30%","10-20%","5-10%","0-5%"};
    int   const physNPtBins = 10;
    double const physPtEdge[physNPtBins+1] = {0., 0.5, 1.0, 1.50, 2.0, 2.50, 3.0, 3.50, 4.0, 5.0, 8.};
    //int   const physNPtBins = 7;
    //double const physPtEdge[physNPtBins+1] = {0., 0.7, 1.1, 1.6, 2.2, 3.0, 5.0, 8.0};
}
#endif
