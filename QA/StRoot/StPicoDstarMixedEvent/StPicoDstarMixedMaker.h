#ifndef StPicoDstarMixedMaker_h
#define StPicoDstarMixedMaker_h

/* **************************************************
 *  A Maker to read a StPicoEvent and StPicoDstarEvent
 *  simultaneously and do analysis.
 *
 *  Authors: Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
 */

#include "TChain.h"
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
class TString;
class TFile;
class TNtuple;
class StPicoTrack;
class StPicoDstMaker;
class StPicoEvent;

class StPicoDstarMixedMaker : public StMaker
{
  public:
    StPicoDstarMixedMaker(char const * name, TString const inputFilesList,
        TString const outBaseName, StPicoDstMaker* picoDstMaker);
    virtual ~StPicoDstarMixedMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
  private:

    StPicoDstarMixedMaker() {}
    void initHists();
    bool isGoodTrigger(StPicoEvent const*) const;
    bool isGoodQaEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodQaTrack(StPicoTrack const* const trk) const;
    bool isGoodTrack(StPicoTrack const* const trk) const;
    float getTofBeta(StPicoTrack const* const trk) const;
    StPicoDstMaker* mPicoDstMaker;
    TString mInputFilesList;
    TString mOutFileBaseName;
    bool isBadrun(Int_t runId);
    
    // -------------- USER variables -------------------------
    // add your member variables here. 
    // Remember that ntuples size can be really big, use histograms where appropriate 

  public:
    void setRunNumList(string list){
      mRunNumList = list;
    }
    void setRunbyRunQA(bool b){
      mRunbyRunQA = b;
    }

    void getBadruns(string inputFileName);
  private: 
    TFile* mFile;
    std::map<int,int> runnum;
    string mRunNumList;
    vector<int> mBadRun;

    //Event level
    int  mRunId;
    float mVpdVz;
    float mRefmult;
    float mVpdHitEast;
    float mVpdHitWest;
    //primaryVertex vertex
    float mVx;
    float mVy;
    float mVz;
    float mVr;
    //event QA
    TH3F* hVxVyVz;
    TH1F* hVz;
    TH1F* hVr;
    TH1F* hVzVpdVz;
    // TH3F* hnEvsEtavsVz;
    TH2F* hnEvsEtavsVz;
    TH2F* hnTofMulvsRef; 
    TH2F* hnTofMatvsRef;
    TH2F* hnTofHitvsRef;

    TH1D* hevt;
    TH1D* hevtcut;
    TH1D* hevtbadcut;
    
   //tracl level QA
    TH1F* hnHitsFit;
    TH1F* hpDca;
    TH1F* hgDca;
    TH2F* hinvBetavsP;
    TH2F* hinvBetavsY;
    TH2F* hdEdx;
    TH1F* hpt;
    TH1F* hEta;
    TH1F* hPhi;

    //Run by run QA
    bool mRunbyRunQA;
    TProfile* pVpdVz;
    TProfile* pVzVpdVz;
    TProfile* pRefmult;
    TProfile* pVpdHitEast;
    TProfile* pVpdHitWest;
    TProfile* pVx;
    TProfile* pVy;
    TProfile* pVz;
    TProfile* pVr;
    //Run by run track level
    TProfile* pTof;
    TProfile* pDedx;
    TProfile* pRMSDedx;
    TProfile* pgDCA;
    TProfile* pgPt;
    TProfile* pgPhi;
    TProfile* pgEta;
    TProfile* pNFits; 
    TProfile* ppPt;
    TProfile* ppEta;
    TProfile* ppPhi;
    ClassDef(StPicoDstarMixedMaker, 1)
};

inline void StPicoDstarMixedMaker::getBadruns(string inputFileName){
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
inline  bool StPicoDstarMixedMaker::isBadrun(Int_t runId){
    vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
    return ( iter != mBadRun.end() ) ;
}

#endif
