#ifndef StMcAnaCuts_H
#define StMcAnaCuts_H

/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */
#include <vector>

#include "Rtypes.h"
#include "StEvent/StEnumerations.h"

namespace McAnaCuts
{
  std::vector<unsigned int> getAllTriggers()
  {
    std::vector<unsigned int> t;
    t.push_back(580001); 
    t.push_back(580021);

    return t;
  }

  std::vector<unsigned int> const interesting_triggers = getAllTriggers();

  float const mcTrackStartVtxR = 2.0; // maximum
  int const geantId = 2;
  float const gDCA = 1.5; //this is for test
  StDedxMethod dedxMethod = kLikelihoodFitId;
  float minPt = 0.2; 
  int const maxNumberOfTriggers = 2;
}
#endif
