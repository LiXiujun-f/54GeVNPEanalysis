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
    ~StMiniTreeAnalysis();

    int Init();
    int Make(); 
    int Finish();

    //function
    void getBadruns(string inputFileName);
    void setRunList(string runlist);


  private:
    //function
    
    void setRunMap(string mRunNumList, int nRunNum);
    void initHists(int nRunNum);
    void WriteHists(TFile* out);
    void bookHists();
    bool isBadrun(Int_t runId);
    int getCentralityBin(float z,int runId,double mult,double &weight); 
  private:
    //read tree
    TStopwatch* stopWatch;
    myTree* mTree;
    TChain* mChain; 
    TFile* mOutfile; 
    TString mInputlist;
    TString mOutputname;
    string mRunNumList; 

    std::vector<int> mBadRun;
    std::map<int,int> runnum;

    TH1F* hcent;
    TH1F* hcentwg;
    // TH2F* hincEptCent;
    TProfile* pQxRecenterM[9];
    TProfile* pQyRecenterM[9];
    TProfile* pQxRecenterP[9];
    TProfile* pQyRecenterP[9];
    TH2F* hEventPlaneCent_P;
    TH2F* hEventPlaneCent_M;
    // TH3F* hQyQxCent;
    // TH2F*  hPhiCentEtaPlusZPlus;
    // TH2F*  hPhiCentEtaPlusZMinus;     //event QA
    // TH2F*  hPhiCentEtaMinusZPlus;
    // TH2F*  hPhiCentEtaMinusZMinus;
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
#endif
