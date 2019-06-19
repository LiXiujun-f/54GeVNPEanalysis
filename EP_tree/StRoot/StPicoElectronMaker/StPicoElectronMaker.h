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
#include "TVector3.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile2D.h"
// #include "StPicoDst.h"

#define Nphe 300
#define NincE 200

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
    bool bookHists();
    bool initTree();
    bool WriteHists();
    bool WriteTree();
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
    bool isGoodTrackGlobal(StPicoTrack const* trk,float dca) const;
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
    TFile* mTreeFile;
    std::map<int,int> runnum;
    string mRunNumList;
    vector<int> mBadRun;
    bool mWriteTree;
    
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

    //tree
    TTree* mTree;
    int m_runId;
    int m_eventId;
    float m_bfield;
    int m_cent;
    float m_weight;
    float m_refMult;
    float m_pVtx_x;
    float m_pVtx_y;
    float m_pVtx_z;
    float m_BBCx;
    float m_ZDCx;
    float m_nTofMult; 
    float m_nTofMat;
//
    float m_nQvec_M;
    float m_nQvec_P;
    double m_Qx_M;
    double m_Qy_M;
    double m_Qx_P;
    double m_Qy_P;
    float mEP_M_raw;
    float mEP_M_re;
    float mEP_M_sh;
    float mEP_P_raw;
    float mEP_P_re;
    float mEP_p_sh;
 
    // int const NincE=80; 
    int m_nIncE;
    int m_idx_inc[NincE];
    float m_nSigmaE[NincE];
    float m_beta_inc[NincE];
    float m_pt_inc[NincE];
    float m_eta_inc[NincE];
    float m_phi_inc[NincE];
    float m_deltaphi_inc[NincE];
    float m_cos2phi_inc[NincE];
    float m_gDca_inc[NincE];
    float m_charge_inc[NincE];
    float m_nFits_inc[NincE]; 
    float m_ndEdx_inc[NincE]; 
    UInt_t m_topomap0_inc[NincE]; 

    bool m_has1hit_inc[NincE]; 
    bool m_has2hit_inc[NincE]; 
    bool m_has3hit_inc[NincE]; 
    bool m_has4hit_inc[NincE]; 

    // int const Nphe=60;
    int m_nPhoE;
    int m_idx_phe[Nphe];
    float m_nSigE_phe[Nphe];
    float m_beta_phe[Nphe];
    float m_pt_phe[Nphe];
    float m_eta_phe[Nphe];
    float m_phi_phe[Nphe];
    float m_gDca_phe[Nphe];
    float m_charge_phe[Nphe];
    float m_nFit_phe[Nphe];
    float m_ndEdx_phe[Nphe];
    float m_deltaphi_phe[Nphe];
    float m_cos2phi_phe[Nphe];
    int m_idx_parte[Nphe];
    float m_nSigE_parte[Nphe];
    float m_beta_parte[Nphe];
    float m_pt_parte[Nphe];
    float m_eta_parte[Nphe];
    float m_phi_parte[Nphe];
    float m_gDca_parte[Nphe];
    float m_charge_parte[Nphe];
    float m_nFit_parte[Nphe];
    float m_ndEdx_parte[Nphe];
    float m_DCA_pair[Nphe];
    float m_decayL_pair[Nphe];
    float m_V0x_pair[Nphe];
    float m_V0y_pair[Nphe];
    float m_V0z_pair[Nphe];
    float m_vtx2V0x_pair[Nphe];
    float m_vtx2V0y_pair[Nphe];
    float m_vtx2V0z_pair[Nphe];
    float m_M_pair[Nphe];
    float m_px_pair[Nphe];
    float m_py_pair[Nphe];
    float m_pz_pair[Nphe];
    
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
    TProfile2D* pIncEv2_hitcut;
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
