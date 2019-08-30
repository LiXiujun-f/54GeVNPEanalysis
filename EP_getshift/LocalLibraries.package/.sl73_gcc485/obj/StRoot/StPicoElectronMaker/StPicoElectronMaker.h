#ifndef StPicoElectronMaker_h
#define StPicoElectronMaker_h

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
// #include "StPicoDst.h"
class TString;
class TFile;
class TNtuple;
class StPicoTrack;
class StPicoDstMaker;
class StPicoDst;
class StPicoEvent;

class StPicoElectronMaker : public StMaker
{
  public:
    StPicoElectronMaker(char const * name, TString const inputFilesList,
        TString const outBaseName, StPicoDstMaker* picoDstMaker);
    virtual ~StPicoElectronMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
  private:
    StPicoElectronMaker() {}
    bool initHists();
    bool ProcessPicoEvent();
    // bool loopTracks();
    bool recenterEventPlane();   
    bool getIncElectronv2();
    double getEventPlaneShift(double EP_Re,int side,int cent) const;
    bool isGoodTrigger(StPicoEvent const*) const;
    // bool isGoodQaEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodEvent(StPicoEvent const* const picoEvent) const;
    // bool isGoodQaTrack(StPicoTrack const* const trk) const;
    bool isGoodTrack(StPicoTrack const* trk,float dca) const;
    float getTofBeta(StPicoTrack const* const trk) const;
    StPicoDstMaker* mPicoDstMaker;
    StPicoDst* mPicoDst;
    TString mInputFilesList;
    TString mOutFileBaseName;
    bool isBadrun(Int_t runId);
    int getCentralityBin(float vz,int runId, float mult,float &weight) const;
    float reweight(float x) const;
    // -------------- USER variables -------------------------
    // add your member variables here. 
    // Remember that ntuples size can be really big, use histograms where appropriate 

  public:
    void setRunNumList(string list){
      mRunNumList = list;
    }

    void getBadruns(string inputFileName);
    void setRecenterFile(string recenterfilename);
  private: 
    TFile* mFile;
    std::map<int,int> runnum;
    string mRunNumList;
    vector<int> mBadRun;
    
    int const nRunNum = 614;
    //Event level
    int  mRunId;
    float mVpdVz;
    float mRefmult;
    //primaryVertex vertex
    float mVx;
    float mVy;
    float mVz;
    float mVr;
    int mCent;

    //hists
    TH1F* hcent;
    TH1F* hcentwg;
    TH2F* hincEptCent;
    string mRecenterFile;
    TProfile* pQxRecenterM[9];
    TProfile* pQyRecenterM[9];
    TProfile* pQxRecenterP[9];
    TProfile* pQyRecenterP[9];
    TH2F* hEventPlaneCent_M;
    TH2F* hEventPlaneCent_M_Sh;
    TH2F* hEventPlaneCent_P_Sh;
    TH2F* hEventPlaneCent_P;
    TH3F* hQyQxCent;
    TProfile* EPconsRes;
    TProfile* prfQxCent;
    TProfile* prfQyCent;
    TProfile* pCosEtaM[20][9];
    TProfile* pSinEtaM[20][9];
    TProfile* pCosEtaP[20][9];
    TProfile* pSinEtaP[20][9];
    //below are used for QA, already done in recenter
    TH2F* hPhiCentEtaPlusZPlus;
    TH2F* hPhiCentEtaPlusZMinus;     //event QA
    TH2F* hPhiCentEtaMinusZPlus;
    TH2F* hPhiCentEtaMinusZMinus;

    TH1F* hVz;
    TH3F* hVxVyVz;
    TH1F* hVr;
    TH1F* hVzVpdVz;

    TH1D* hevt;
    TH1D* hevtcut;

    //track level QA
    TH1F* hnHitsFit;
    TH1F* hpDca;
    TH1F* hgDca;
    TH2F* hdEdx;
    TH1F* hpt;
    TH1F* hEta;
    TH1F* hPhi;

    ClassDef(StPicoElectronMaker, 1)
};

inline void StPicoElectronMaker::getBadruns(string inputFileName){
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
inline  bool StPicoElectronMaker::isBadrun(Int_t runId){
  vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
  return ( iter != mBadRun.end() ) ;
}
inline void StPicoElectronMaker::setRecenterFile(string recenterfilename){
  mRecenterFile = recenterfilename;
}
#endif
