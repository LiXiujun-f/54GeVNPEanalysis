#ifndef StMiniTreeAnalysis_h
#define StMiniTreeAnalysis_h
// #include <cstdio>
// #include <cstdlib>
// #include <ctime>
#include <algorithm>
// #include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TTree.h"
#include "TRandom.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
// #include "THnSparse.h"
#include "myTree.h"
using namespace std;

// class StMiniTreeAnalysis : public TObject
class StMiniTreeAnalysis 
{
  public:
    StMiniTreeAnalysis(TString const  inputfilelist, TString const outfilename);
    virtual ~StMiniTreeAnalysis();

    int Init();
    int Make(); 
    int Finish();

    //function
    void getBadruns(string inputFileName);
    void setRunList(string runlist);
    void setRecenterFile(string recenterfilename);
    void setShiftFile(string shiftfilename);
  private:
    //function 
    void setRunMap(string mRunNumList, int nRunNum);
    void initHists(int nRunNum);
    void WriteHists(TFile* out);
    void bookHists();
    bool isBadrun(Int_t runId);
    int getCentralityBin(float z,int runId,double mult,float &weight); 
    double getEventPlaneShift(double EP_Re,int side,int cent) const;
    int EventPlane(int cent, int runId, float &EP_P_sh,float &EP_M_sh);
    int getIncEv2(float EP_M_sh,float EP_P_sh,int cent,float weight);
    bool isSecondPE(float nSigE,float beta,float pt);
    bool isElectron(float nSigE,float beta,float pt);
    float reweight(float x) const;
  private:
    //read tree
    TStopwatch* stopWatch;
    myTree* mTree;
    TChain* mChain; 
    TFile* mOutfile; 
    TString mInputlist;
    TString mOutputname;
    string mRunNumList; 
    string mRecenterFile;
    string mShiftFile;

    std::vector<int> mBadRun;
    std::map<int,int> runnum;

    TH1F* hcent;
    TH1F* hcentwg;
    // TH2F* hincEptCent;
    TH2F* hEventPlaneCent_P;
    TH2F* hEventPlaneCent_M;
    TH2F* hEventPlaneCent_P_re;
    TH2F* hEventPlaneCent_M_re;
    TH2F* hEventPlaneCent_P_sh;
    TH2F* hEventPlaneCent_M_sh;
    TProfile* EPconsRes;
   
    TH3F* hIncEv2vsPtvsCent;
    TH2F* hIncEPtvsCent;
    TH3F* hIncEv2vsPtvsCent_hitcut;
    TH2F* hIncEPtvsCent_hitcut;
    TH3F* hPhEv2vsPtvsCent;
    TH3F* hPhEv2vsPtvsCentLS;
    TProfile2D* pTagEv2;
    TProfile2D* pTagEv2_LS;
    TProfile2D* pIncEv2;
    TProfile2D* pIncEv2_hitcut;
    TH3F* hphoto;
    TH3F* hphoto_LS;
    TH3F* hphoto_hitcut;
    TH3F* hphoto_LS_hitcut;

    TH3F* hphotoVsPt;
    TH3F* hphotoVsPt_LS; 
    TH3F* hV0_LS;
    TH3F* hV0;
   
    TH1F* hEta;
    TH1F* hPhi;
    TH1F* hPhi_hitcut; 
    TH1F* hPhi_allcut; 
    
    TH2F* hePtvsP; 
    TH3F* hPartEptetaphi;
    TH3F* hPartEptetaphi_LS;
    TH3F* hPartEptetaphi_Dz_LS;
    TH3F* hPartEptetaphi_Dz;
    TH2F* hPairDCA;
    TH2F* hPairDCALS;
    TH2F* hDecayL_LS;
    TH2F* hDecayL;
    TH2F* hPartETpc; 
    TH2F* hPartETpcLS;
    TH2F* hPartETof;
    TH2F* hPartETofLS;

    TProfile* pQxRecenterM[9];
    TProfile* pQyRecenterM[9];
    TProfile* pQxRecenterP[9];
    TProfile* pQyRecenterP[9];

    TProfile* pCosEtaM[9];
    TProfile* pSinEtaM[9];
    TProfile* pCosEtaP[9];
    TProfile* pSinEtaP[9];

    ClassDef(StMiniTreeAnalysis,1)
};

inline  void StMiniTreeAnalysis::getBadruns(string inputFileName)
{
  ifstream fin(inputFileName.c_str());
  if(!fin){
    //      Error("StRefMultCorr::readBadRuns", "can't open %s", inputFileName);
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
inline void StMiniTreeAnalysis::setRunList(string runlist)
{
  mRunNumList = runlist;
}
inline  bool StMiniTreeAnalysis::isBadrun(Int_t runId)
{
  vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
  return ( iter != mBadRun.end() ) ;
}

inline void StMiniTreeAnalysis::setRunMap(string RunNumList, int nRunNum)
{
  ifstream readnum;
  readnum.open(RunNumList);
  int tmp=0;
  cout<<"start initial run number..."<<endl;
  for (int i=0;i<nRunNum;i++){
    readnum>>tmp;
    runnum.insert(pair<int,int>(tmp,i));
    // cout <<"run number : " << tmp <<" id :" <<runnum[tmp] <<endl;
  }
  readnum.close();
}
inline void StMiniTreeAnalysis::setRecenterFile(string recenterfilename)
{
  mRecenterFile = recenterfilename;
}
inline void StMiniTreeAnalysis::setShiftFile(string shiftfilename)
{
  mShiftFile = shiftfilename;
}

#endif
