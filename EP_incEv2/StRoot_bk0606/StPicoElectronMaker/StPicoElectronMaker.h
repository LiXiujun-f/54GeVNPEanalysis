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
#include "TProfile2D.h"
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
    bool getIncElectronv2(float bField, StThreeVectorF pVtx);
    double getEventPlaneShift(double EP_Re,int side,int cent,int runnum) const;
    bool isGoodTrigger(StPicoEvent const*) const;
    // bool isGoodQaEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodEvent(StPicoEvent const* const picoEvent) const;
    // bool isGoodQaTrack(StPicoTrack const* const trk) const;
    bool isGoodTrack(StPicoTrack const* trk,float dca) const;
    float getTofBeta(StPicoTrack const* const trk) const; 
    bool isProton(StPicoTrack const * const trk,  float beta) const;
    bool isPion(StPicoTrack const * const trk, float beta) const;
    bool isKaon(StPicoTrack const * const trk, float beta) const;
    bool isPEfirstElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool isPEsecondElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool isElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    StPicoDstMaker* mPicoDstMaker;
    StPicoDst* mPicoDst;
    TString mInputFilesList;
    TString mOutFileBaseName;
    bool isBadrun(Int_t runId);
    int getCentralityBin(float vz,int runId, float mult,float &weight, double &refmultcor) const;
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
    void setShiftFile(string shiftfilename);
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
    float mWeight;
    double mEP_P_Re;
    double mEP_P_Sh;
    double mEP_M_Re;
    double mEP_M_Sh; 
    // float bField;

    //hists
    TH1F* hcent;
    TH1F* hcentwg;
    TH1F* hrefmult;
    TH1F* hrefmultcor;
    TH2F* hincEptCent;
    string mRecenterFile;
    string mShiftFile;
    //recenter
    TProfile* pQxRecenterM[9];
    TProfile* pQyRecenterM[9];
    TProfile* pQxRecenterP[9];
    TProfile* pQyRecenterP[9];
    //shift
    TProfile* pSinEtaP[9];
    TProfile* pCosEtaM[9];
    TProfile* pCosEtaP[9];
    TProfile* pSinEtaM[9];
    
    TH2F* hEventPlaneCent_M;
    TH2F* hEventPlaneCent_M_Re;
    TH2F* hEventPlaneCent_M_Sh;
    TH2F* hEventPlaneCent_P_Sh;
    TH2F* hEventPlaneCent_P;
    TH2F* hEventPlaneCent_P_Re;
    TH3F* hQyQxCent;
    TProfile* EPconsRes;
    TH3F* hIncEv2vsPtvsCent;
    TH3F* hIncHadronv2vsPtvsCent;
    TH3F* hPhEv2vsPtvsCent;
    TH3F* hPhEv2vsPtvsCentLS;
    TProfile2D* pIncHadronv2;
    TProfile2D* pPionMinusv2;
    TProfile2D* pPionPlusv2;
    TProfile2D* pTagEv2;
    TProfile2D* pTagEv2_LS;
    TProfile2D* pIncEv2;
    TH3F* hphoto;
    TH3F* hphoto_LS;
    TH3F* hphotoVsPt;
    TH3F* hphotoVsPt_LS; 
    
    TProfile* prfQxCent;
    TProfile* prfQyCent;
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
inline void StPicoElectronMaker::setShiftFile(string shiftfilename){
  mShiftFile = shiftfilename;
}
#endif
