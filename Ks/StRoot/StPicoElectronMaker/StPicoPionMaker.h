#ifndef StPicoPionMaker_h
#define StPicoPionMaker_h

/* **************************************************
 *  A Maker to read a StPicoEvent and StPicoDstarEvent
 *  simultaneously and do analysis.
 * **************************************************
 */

#include "TChain.h"
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "../StRefMultCorr/StRefMultCorr.h"
// #include "StPicoDst.h"
class TString;
class TFile;
class TNtuple;
class StPicoTrack;
class StPicoDstMaker;
class StPicoDst;
class StPicoEvent;

class StPicoPionMaker : public StMaker
{
  public:
    StPicoPionMaker(char const * name, TString const inputFilesList,
        TString const outBaseName, StPicoDstMaker* picoDstMaker);
    virtual ~StPicoPionMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
  private:
    StPicoPionMaker() {}
    void initHists();
    bool ProcessPicoEvent();
    bool isGoodTrigger(StPicoEvent const*) const;
    bool isGoodQaEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodQaTrack(StPicoTrack const* const trk) const;
    bool isGoodTrack(StPicoTrack const* trk,float dca) const;
    float getTofBeta(StPicoTrack const* const trk) const;
    StPicoDstMaker* mPicoDstMaker;
    StPicoDst* mPicoDst;
    TString mInputFilesList;
    TString mOutFileBaseName;
    bool isBadrun(Int_t runId);
    bool isfirstPion(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool issecondPion(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool isElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool isTofElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const;
    bool isTofKaon(StPicoTrack const * const trk, float beta) const;
    bool isTofPion(StPicoTrack const * const trk, float beta) const;
    bool isTofProton(StPicoTrack const * const trk,  float beta) const;
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
  private: 
    TFile* mFile;
    std::map<int,int> runnum;
    string mRunNumList;
    vector<int> mBadRun;
    StRefMultCorr* mRefMultCorrUtil;

    //Event level
    int  mRunId;
    float mVpdVz;
    float mRefmult;
    //primaryVertex vertex
    float mVx;
    float mVy;
    float mVz;
    float mVr;
    float mCent;

    //hists
    // TH2F* hinvBetavsP;
    // TH2F* hnSigEvsP;  //all tracks
    // TH2F* hnSigE_e;  //select PE e
    // TH2F* hnSigE_p;  //select tof p
    // TH2F* hnSigE_pi;  //ks pi
    // TH2F* hnSigE_pi_LS;
    // TH2F* hnSigE_pi_tof;
    TH3F* hnSigE_pi;  //ks pi
    TH3F* hnSigE_pi_LS;
    TH3F* hnSigE_pi_tof;

    // TH2F* hnSigE_k;
    // TH2F* hnSigE_Cut;
    // TH2F* hnSigE_tof;
    // TH2F* hePtvsP;
    // TH2F* hKs_LS;
    // TH2F* hKs;
    TH3F* hKs_LS;
    TH3F* hKs;

    // TH2F* hnSigE_e_LS;
    // TH3F* hV0;
    // TH3F* hV0_LS;
    TH1F* hcent;
    TH1F* hcentwg;
    // TH2F* hincEptCent;
    //event QA
    // TH3F* hVxVyVz;
    // TH1F* hVz;
    // TH1F* hVr;
    // TH1F* hVzVpdVz;
    // TH2F* hnTofMulvsRef; 
    // TH2F* hnTofMatvsRef;
    //
    TH1D* hevt;
    TH1D* hevtcut;
    //
    // //tracl level QA
    // TH1F* hnHitsFit;
    // TH1F* hpDca;
    // TH1F* hgDca;
    // TH2F* hdEdx;
    // TH1F* hpt;
    // TH1F* hEta;
    // TH1F* hPhi;

    ClassDef(StPicoPionMaker, 1)
};

inline void StPicoPionMaker::getBadruns(string inputFileName){
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
inline  bool StPicoPionMaker::isBadrun(Int_t runId){
  vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
  return ( iter != mBadRun.end() ) ;
}

#endif
