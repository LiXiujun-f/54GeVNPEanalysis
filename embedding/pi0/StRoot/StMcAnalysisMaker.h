#ifndef ST_MCANALYSISMAKER_H
#define ST_MCANALYSISMAKER_H

#include <vector>
#include "TString.h"

#include "StChain/StMaker.h"
#include "TH1.h" 
#include "TH2.h" 
#include "TH3.h" 
#include "TF1.h"
#include "StThreeVectorF.hh"
#include "TProfile2D.h"
class TFile;
class TNtuple;
class TH3F;
class TF1;

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
   std::vector<int> mBadRun;
   double mField; //.. magnetic field
   int    mCentrality;
   float  mCentWeight;
   StThreeVectorF mpVtx;
   float mEP;
   TFile* mFile;
   TF1* fPi0v2;
   // TF1* fPi0spectra;

   int nPi0;
   TH2F* hnHitsvsPt;
   TH2F* hVzvsmcVz;
   TH2F* hDCAvsPt;
   TH1F* hnPi0;
   TH1F* hRefmult;
   TH1F* hEvent;
   TH1F* hEP;
   TH1F* hPi2tot;
   TH2F* hPi0Pt;
   TH2F* hPi0Pt_norm;
   TH2F* hPi0Pt_weight;
   TH3F* hPi0PhiEtaPt; 

   TH2F* hMcElectronPtvsCent;
   // TH2F* hMcElectronPtvsCent_test;
   TH2F* hRcElectronPtvsCent;
   TH2F* hRcElectronMcPtvsCent;
   TH2F* hMomResolution;
   TH3F* hDuplicated;
   TH2F* hDupTracksDpt;
   TH2F* hDupTracksDhits;
   TH2F* hDupTracksCharge;
   TF1*  mPi0Spectra;  
  
   TH2F* hElectron;
   TH2F* hElectronPassCut; 
   TH2F* hElectronVsParent; 
                
   //check the part
   TH3F* hPartEptetaphi;
   TH2F* hPartEnFits;
   TH2F* hPartEdca; 
   TH2F* hPairDecayL;
   TH3F* hDcaPair;
   TH3F* hMassDiE;
   TH3F* hMassDiEMC;
   TProfile2D* pPhoEv2;
   TProfile2D* pPi0Ev2;
   TH3F* hPhoEPtPhiCent;
   TH3F* hPhoEPtPhiCentRaw;
   TProfile2D* pRecoEv2;
   TH3F* hRecoEPtPhiCent;
   TH3F* hRecoEPtPhiCentRaw;

   TH3F* hPi0PtPhiCent;
   TH3F* hPi0PtPhiCentRaw;

   StMcEvent* mMcEvent;
   StEvent* mEvent;
   StAssociationMaker* mAssoc;
 private:
   bool InitHists();
   void bookSpectra(int );
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
   void pairPartnerElectron(StMuTrack const* const TagE,StMcTrack const * const McTagE, std::map<int,int>& mc_rcpair);
   bool passReconstructionCut(StMuTrack const * const pe_1, StMuTrack const * const pe_2, float weight);
   void fillPhoEv2(StMuTrack const* const TagE,StMcTrack const * const mother,float weight,bool isReco); 
   void fillPi0v2(StMcTrack const * const mcTrack, int nPi0 );
   bool isGoodTagE(const StMuTrack*) const ;
   bool isGoodPartE(const StMuTrack*) const;
   bool checkTheDecayChannel();
   int getCentralityBin(float vz,int runId,float mult,float &weight,double &refmultcor) const; 
   float reweight(float x) const;
   bool isBadrun(Int_t runId); 

public:
   StMcAnalysisMaker (const char *name="StMcAnalysisMaker", const char *title="event/StMcAnalysisMaker");
   int Init();
   int Make();
   int Finish();
   void getBadruns(string inputFileName);
   void setOutFileName(std::string);
   void setRefMultCorr(StRefMultCorr*);

   ClassDef(StMcAnalysisMaker, 0)
};
inline void StMcAnalysisMaker::getBadruns(string inputFileName){
  ifstream fin(inputFileName.c_str());
  if(!fin){
    cout <<"no Bad runs list" << endl;
    return;
  }
  cout << "  " << inputFileName << flush;

  Int_t runId = 0 ;
  while( fin >> runId ) {
    mBadRun.push_back(runId);
  }
  cout << "get Bad runs list [OK]" << endl;
}
inline  bool StMcAnalysisMaker::isBadrun(Int_t runId){
  vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
  return ( iter != mBadRun.end() ) ;
}
inline void StMcAnalysisMaker::setOutFileName(std::string s){ mOutfileName = s.c_str();}
inline void StMcAnalysisMaker::setRefMultCorr(StRefMultCorr* grefmultCorrUtil){ mGRefMultCorrUtil = grefmultCorrUtil;}
#endif

