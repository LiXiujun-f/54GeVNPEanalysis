#ifndef StBtoDAnalysis_h
#define StBtoDAnalysis_h

#include "StMaker.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>
#include "StThreeVectorF.hh"
#include "mTreeDst.h"
#define  MAX_NUM 1000
class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StMyUtilities;
class StRefMultCorr;
class TString;
class TH1F;
class TH2F;
class TH3F;
class TH2I;
class TProfile;
class TProfile2D;

class StBtoDAnalysis : public StMaker
{
public:
   StBtoDAnalysis(const char *name, StPicoDstMaker *picoMaker, const char *outName);
   virtual ~StBtoDAnalysis();

   virtual Int_t Init();
   virtual Int_t Make();
   virtual void  Clear(Option_t *opt = "");
   virtual Int_t Finish();
   int     passEvent();
   int     loopTrack();
   void    MakeRealEvent();

   void    initTree();
   void    clearTree();
   void    writeTree();
   void    DeclareHistograms();
   void    WriteHistograms();
   void    DeleteHistograms();
   void    setEnergyAndList();
   void    setUseTree(bool condi)
   {
      mUseTree = condi;
   };
   void    setUseHist(bool condi)
   {
      mUseHist = condi;
   };
   void    setUtilities(StMyUtilities *in)
   {
      mUtilities = in;
   };
   float   getDca(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex);
   float   getDcaXY(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex);
   float   getDcaZ(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex);
   int     getD0PtIndex(float const pt);
   int     getMeaSigPtIndex(float const pt);

private:
   StPicoDstMaker *mPicoDstMaker;
   StPicoDst      *mPicoDst;
   StPicoEvent    *mEvent;
   StPicoTrack    *mTrack;
   StPicoTrack    *mTrack1;
   StMyUtilities  *mUtilities;
   static StRefMultCorr  *mRefMultCorr;

   float           B;
   StThreeVectorF  Vertex;
   float           gweight;

   TFile *mHistoutFile;

   TString  mOutName;
   double   PI;
   double   twoPI;
   int      runnumberPointer;
   int      centrality;
   int      nEvents;

   float    massPi;
   float    massK;
   float    massP;
   float    massPhi;
   float    massK0S;
   float    massLambda0;

   bool     mUseHist;
   bool     mUseTree;
   TTree*   mTree;
   mTreeDst v0Tree;

   //reconstuct parent
   int   nPar1;  //particle 1--target, strict cut
   int   mcharge1[MAX_NUM];
   float gmomx1[MAX_NUM];  //global
   float gmomy1[MAX_NUM];
   float gmomz1[MAX_NUM];
   float moriginx1[MAX_NUM];
   float moriginy1[MAX_NUM];
   float moriginz1[MAX_NUM];
   float mDiffInvBeta1[MAX_NUM];
   float mNsigmaX1[MAX_NUM];
   //bool  mNdEdxCut1[MAX_NUM];
   bool  mTofMatch1[MAX_NUM];
   int   mtrackID1[MAX_NUM];
   float mdca1[MAX_NUM];
   bool  mClean1[MAX_NUM];
   float mEta1[MAX_NUM];
   int   nPar2;  //particle 2
   int   mcharge2[MAX_NUM];
   float gmomx2[MAX_NUM];  //global
   float gmomy2[MAX_NUM];
   float gmomz2[MAX_NUM];
   float moriginx2[MAX_NUM];
   float moriginy2[MAX_NUM];
   float moriginz2[MAX_NUM];
   float mDiffInvBeta2[MAX_NUM];
   float mNsigmaX2[MAX_NUM];
   //bool  mNdEdxCut2[MAX_NUM];
   bool  mTofMatch2[MAX_NUM];
   int   mtrackID2[MAX_NUM];
   float mdca2[MAX_NUM];
   bool  mClean2[MAX_NUM];
   float mEta2[MAX_NUM];

   //test
   TH1F* hCentrality;
   TH2I* hNPar1VsNPar2;
   TH1F* hgRefMult;
   TH1F* hgRefMultWg;

   //tof match efficiency
   TH2F* h2pTCent_tpcPi;
   TH2F* h2pTCent_tofPi;
   TH2F* h2pTCent_tpcPi2;
   TH2F* h2pTCent_tofPi2;

   TH1F* hmassUL;
   TH1F* hmassLS;
   TH1F* hmass;

   TH2F* h2massUL;
   TH2F* h2massLS;
   TH2F* h2mass;

   TH2F* h2NsigmaVsP_cleanUL;
   TH2F* h2NsigmaVsP_cleanLS;
   TH2F* h2NsigmaVsP_cleanSB;
   TH2F* h2NsigmaVsP_clean;

   TH2F* h2InvBetaVsP_cleanUL;
   TH2F* h2InvBetaVsP_cleanLS;
   TH2F* h2InvBetaVsP_cleanSB;
   TH2F* h2InvBetaVsP_clean;

   //nsigma cut efficiency -- dE/dx
   TH3F* hNsigmaVsPCentUL;
   TH3F* hNsigmaVsPCentLS;
   TH3F* hNsigmaVsPCentSB;
   TH3F* hNsigmaVsPCent;

   //TOF matching efficiency
   TH1F* hpT_tofMatchUL;
   TH1F* hpT_tofMatchLS;
   TH1F* hpT_tofMatch;
   TH1F* hpT_tofMatchCutUL;
   TH1F* hpT_tofMatchCutLS;
   TH1F* hpT_tofMatchCutSB;
   TH1F* hpT_tofMatchCut;
   TH2F* hpTCent_tofMatchUL;
   TH2F* hpTCent_tofMatchLS;
   TH2F* hpTCent_tofMatch;
   TH2F* hpTCent_tofMatchCutUL;
   TH2F* hpTCent_tofMatchCutLS;
   TH2F* hpTCent_tofMatchCutSB;
   TH2F* hpTCent_tofMatchCut;

   //TOF pid cut efficiency
   TH3F* hInvBetaVsPCentUL;
   TH3F* hInvBetaVsPCentLS;
   TH3F* hInvBetaVsPCentSB;
   TH3F* hInvBetaVsPCent;
   TH2F* hInvBetaVsP;

   ClassDef(StBtoDAnalysis, 1)
};

#endif




















