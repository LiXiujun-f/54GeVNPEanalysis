#ifndef ST_MCANALYSISMAKER_H
#define ST_MCANALYSISMAKER_H

#include <vector>
#include "TString.h"

#include "StChain/StMaker.h"
#include "TH1.h" 
#include "TH2.h" 
#include "TH3.h" 
class TFile;
class TNtuple;
class TH3F;

class StMcTrack;
class StTrack;
class StGlobalTrack;
class StAssociationMaker;
class StMcEvent;
class StMuTrack;
class StEvent;
class StMuDst;
class StDedxPidTraits;
class StRefMultCorr;

class StMcAnalysisMaker : public StMaker
{
private:
   TString mOutfileName;
   StRefMultCorr* mGRefMultCorrUtil;
   StMuDst*       mMuDst;
   std::vector<float> firedTriggersIndices;
   double mField; //.. magnetic field
   int    mCentrality;
   double mCentWeight;
   TFile* mFile;

   int nPi0;
   TH2F* hnHitsvsPt;
   TH2F* hDCAvsPt;
   TH1F* hnPi0;
   TH1F* hRefmult;
   TH1F* hPi2tot;
   TH1F* hPi0Pt;
   TH2F* hMcElectronPtvsCent;
   TH2F* hMcElectronPtvsCentPlu;
   TH2F* hMcElectronPtvsCentMin;
   TH2F* hMcElectronPtvsCentGamma;
   TH2F* hRcElectronPtvsCentGamma;
   TH2F* hRcElectronPtvsCentPlu;
   TH2F* hRcElectronPtvsCentMin;
   TH2F* hRcElectronPtvsCent;
   TH3F* hRcPtEtaPhiGamma;
   TH3F* hRcPtEtaPhiDalitz;
   TH3F* hMcPtEtaPhiGamma;
   TH3F* hMcPtEtaPhiDalitz;
   TH2F* hMomResolution;
   TH3F* hDuplicated;
   TH2F* hDupTracksDpt;
   TH2F* hDupTracksDhits;
   TH2F* hDupTracksCharge;

   StMcEvent* mMcEvent;
   StEvent* mEvent;
   StAssociationMaker* mAssoc;

   bool InitHists();
   StTrack const* findPartner(StMcTrack*, int&) const;
   StMcTrack const* findPartner(StGlobalTrack*, int&) const;
   int getNHitsDedx(StTrack const*) const;
   bool passTrigger();
   int  fillTracks(int& nRTracks, int& nMcTracks);
   void fillMcTrack(StMcTrack const*);
   void fillRcTrack(StMuTrack const* rcTrack, StMcTrack const* const mcTrack);
   void getDca(StTrack const*,float& dca, float& dcaXY, float& dcaZ) const;
   bool isGoodMcTrack(StMcTrack const*) const;
   bool isGoodRcTrack(StMuTrack const* const rcTrack) const;

public:
   StMcAnalysisMaker (const char *name="StMcAnalysisMaker", const char *title="event/StMcAnalysisMaker");
   int Init();
   int Make();
   int Finish();

   void setOutFileName(std::string);
   void setRefMultCorr(StRefMultCorr*);

   ClassDef(StMcAnalysisMaker, 0)
};

inline void StMcAnalysisMaker::setOutFileName(std::string s){ mOutfileName = s.c_str();}
inline void StMcAnalysisMaker::setRefMultCorr(StRefMultCorr* grefmultCorrUtil){ mGRefMultCorrUtil = grefmultCorrUtil;}
#endif
