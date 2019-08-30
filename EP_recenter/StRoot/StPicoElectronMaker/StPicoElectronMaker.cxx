/* **************************************************
 *
 *  Authors: Yuanjing Ji
 *
 * **************************************************
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TNtuple.h"

#include "StEvent/StDcaGeometry.h"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StBTofUtil/tofPathLength.hh"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoElectronMaker.h"
#include "StAnaCuts.h"
#include "StMemStat.h"
#include "StLorentzVector.hh"
// #include "calmean.h"
#include "TVector3.h"
#include "TVector2.h"
#include "TRandom.h"
#ifndef DEBUG
#define DEBUG 0
#endif
ClassImp(StPicoElectronMaker)
  StPicoElectronMaker::StPicoElectronMaker(char const * name, TString const inputFilesList, TString const outFileBaseName, StPicoDstMaker* picoDstMaker):
    StMaker(name), mPicoDstMaker(picoDstMaker),
    mInputFilesList(inputFilesList), mOutFileBaseName(outFileBaseName)
{}

Int_t StPicoElectronMaker::Init()
{
  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");
  // -------------- USER VARIABLES -------------------------
  mFile = new TFile(mOutFileBaseName+".recenter.root", "RECREATE");
  ifstream readnum;
  readnum.open(mRunNumList);
  int tmp=0;
  if (DEBUG) cout<<"start initial run number..."<<endl;
  for (int i=0;i<nRunNum;i++){
    readnum>>tmp;
    runnum.insert(pair<int,int>(tmp,i));
    if (DEBUG) cout <<"run number : " << tmp <<" id :" <<runnum[tmp] <<endl;
  }
  readnum.close();
  //initialize trees
  initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoElectronMaker::~StPicoElectronMaker()
{}
//-----------------------------------------------------------------------------
void StPicoElectronMaker::initHists(){
  hevt = new TH1D("hevt","hevt",1,0.5,1.5);
  hevtcut = new TH1D("hevtcut","hevtcut",1,0.5,1.5);
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic] = new TProfile(Form("pQxRecenterMin_%d",ic),Form("pRecenterMin_%d",ic),nRunNum,0,nRunNum);  
    pQyRecenterM[ic] = new TProfile(Form("pQyRecenterMin_%d",ic),Form("pRecenterMin_%d",ic),nRunNum,0,nRunNum);  
    pQxRecenterP[ic] = new TProfile(Form("pQxRecenterPlu_%d",ic),Form("pRecenterPlu_%d",ic),nRunNum,0,nRunNum);  
    pQyRecenterP[ic] = new TProfile(Form("pQyRecenterPlu_%d",ic),Form("pRecenterPlu_%d",ic),nRunNum,0,nRunNum);  
  }  
  float const PI = TMath::Pi();
  hPhiCentEtaPlusZPlus = new TH2F("hPhiCentEtaPlusZPlus","hPhiCentEtaPlusZPlus",9,0,9,120,-PI,PI);
  hPhiCentEtaPlusZMinus = new TH2F("hPhiCentEtaPlusZMinus","hPhiCentEtaPlusZMinus",9,0,9,120,-PI,PI);
  hPhiCentEtaMinusZPlus = new TH2F("hPhiCentEtaMinusZPlus","hPhiCentEtaMinusZPlus",9,0,9,120,-PI,PI);
  hPhiCentEtaMinusZMinus = new TH2F("hPhiCentEtaMinusZMinus","hPhiCentEtaMinusZMinus",9,0,9,120,-PI,PI);
  hEventPlaneCent = new TH2F("hEventPlaneCent","hEventPlaneCent",9,0,9,60,0,PI); 
  hQyQxCent = new TH3F("hQyQxCent", "hQyQxCent", 9,0,9,1000,-50,50,1000,-50,50);
}
//-----------------------------------------------------------------------------
Int_t StPicoElectronMaker::Finish()
{
  mFile->cd();
  //write the hists
  hevt->Write();
  hevtcut->Write();
  
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic]->Write();
    pQyRecenterM[ic]->Write();
    pQxRecenterP[ic]->Write();
    pQyRecenterP[ic]->Write();
  }

  hEventPlaneCent->Write();
  hQyQxCent->Write();
  hPhiCentEtaPlusZPlus->Write();
  hPhiCentEtaPlusZMinus->Write();     //event QA
  hPhiCentEtaMinusZPlus->Write();
  hPhiCentEtaMinusZMinus->Write();

  hcent->Write();
  hcentwg->Write();
  mFile->Close();
  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoElectronMaker::Make()
{
  // StMemStat mem;
  if (mPicoDstMaker)
  {
    mPicoDst = mPicoDstMaker->picoDst();
  }
  else {
    LOG_WARN << " StPicoElectronMaker - No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }

  if (!ProcessPicoEvent()) return kStOK;
  return kStOK;
}
bool StPicoElectronMaker::ProcessPicoEvent()
{
  if (!mPicoDst)
  {
    LOG_WARN << "StPicoElectronMaker - No PicoDst! Skip! " << endm;
    return kStWarn;
  }
  // -------------- USER ANALYSIS -------------------------
  StPicoEvent const * picoEvent = mPicoDst->event();
  //trigger
  if (!isGoodTrigger(picoEvent)) return 0;    
  mRunId = picoEvent->runId();
  hevt->Fill(1);
  //event and track level QA
  if (isBadrun(mRunId)) return kStOK;
  
  if (isGoodEvent(picoEvent)){
    
   bool strangetof = picoEvent->btofTrayMultiplicity()>400&&picoEvent->refMult()<20;
   // hnTofMulvsRef->Fill(picoEvent->btofTrayMultiplicity(),picoEvent->refMult());
   // hnTofMatvsRef->Fill(picoEvent->nBTOFMatch(),picoEvent->refMult());  
   if (strangetof) return kStOK;
    StThreeVectorF pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    hevtcut->Fill(1);
    float bField=picoEvent->bField(); 
    // centrality definition
    double weight=1;

    mCent = getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight);
    if (mCent<0) return  kStOK;
    hcent->Fill(mCent);
    hcentwg->Fill(mCent,weight);
    loopTracks();
  } //Good Event
  return kStOK;
}
bool StPicoElectronMaker::loopTracks()
{
  int nTracks = mPicoDst->numberOfTracks();
    float Qx=0., Qy=0.;
  for (int itrack=0;itrack<nTracks;itrack++){
    StPicoTrack* trk = mPicoDst->track(itrack);
    if (!trk) {
      cout<<"Warning: input track is NAN!!!"<<endl;
      return kStWarn;
    }
    bool isprimary = trk->isPrimary();
    if (!isprimary) continue;
    bool goodtrack = isGoodTrack(trk,trk->gDCA(mVx,mVy,mVz));
    if (!goodtrack) continue;
    StThreeVectorF mom = trk->pMom(); //primary momentum
    float phi = mom.phi();
    float eta = mom.pseudoRapidity();
    float pt = mom.perp();
    if (pt>anaCuts::ptMax_Re || pt<anaCuts::ptMin_Re) continue; 
    float qx = cos(2*phi)*pt;
    float qy = sin(2*phi)*pt;
    if(eta>0 && mVz>0) hPhiCentEtaPlusZPlus->Fill(mCent, phi);
    if(eta>0 && mVz<0) hPhiCentEtaPlusZMinus->Fill(mCent, phi);
    if(eta<0 && mVz>0) hPhiCentEtaMinusZPlus->Fill(mCent, phi);
    if(eta<0 && mVz<0) hPhiCentEtaMinusZMinus->Fill(mCent, phi); 
    if (eta<-0.05) 
    {
      pQxRecenterM[mCent]->Fill(runnum[mRunId],qx); 
      pQyRecenterM[mCent]->Fill(runnum[mRunId],qy); 
    }
    else if (eta>0.05)
    {
      pQxRecenterP[mCent]->Fill(runnum[mRunId],qx); 
      pQyRecenterP[mCent]->Fill(runnum[mRunId],qy); 
    } 
    
    Qx+=qx;
    Qy+=qy; 
  } //loop track
    TVector2 Q(Qx, Qy);
    float eventPlane = Q.Phi()*0.5;
    if(Q.Mod()>0)
      hEventPlaneCent->Fill(mCent, eventPlane);
    hQyQxCent->Fill(mCent, Qx, Qy);
}

bool StPicoElectronMaker::isGoodTrigger(StPicoEvent const* const picoEvent) const
{
  for (auto trg : anaCuts::triggers)
  {
    if (picoEvent->isTrigger(trg)) return true;
  }

  return false;
}
bool StPicoElectronMaker::isGoodTrack(StPicoTrack const* trk, float dca) const
{
  // StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::GPt && fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::Eta &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && fabs(dca)<=anaCuts::Dca &&
    trk->nHitsFit()/trk->nHitsMax() >=anaCuts::NHitsFit2Poss;
  // fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx &&
  // fabs( trk->gDCA(vtx.x() , vtx.y(), vtx.z() )) <= anaCuts::Dca;
}
bool StPicoElectronMaker::isGoodQaTrack(StPicoTrack const* const trk) const
{
  StThreeVectorF vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::qaGPt && fabs(trk->nHitsFit()) >= anaCuts::qaNHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::qaEta &&
    // fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx && fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::qaDca;
    fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx ;
}
bool StPicoElectronMaker::isGoodQaEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::qavz &&
    fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::qavzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::qaVerror && fabs(pVtx.y()) < anaCuts::qaVerror &&
        fabs(pVtx.z()) < anaCuts::qaVerror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::qaVr;
}
bool StPicoElectronMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  double refmult = picoEvent->refMult();
  double Tofmult = picoEvent->btofTrayMultiplicity();
  bool pileup = Tofmult < refmult*2.88-155;
  return (!pileup) && fabs(pVtx.z()) < anaCuts::vz &&  fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
        fabs(pVtx.z()) < anaCuts::Verror) && sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}
float StPicoElectronMaker::getTofBeta(StPicoTrack const* const trk) const
{
  int index2tof = trk->bTofPidTraitsIndex();
  // float beta = std::numeric_limits<float>::quiet_NaN();
  float beta = -9999;
  if (index2tof >= 0)
  {
    StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
    if (tofPid){
      bool btofMatchFlag = tofPid->btofMatchFlag();
      float btofYLocal = tofPid->btofYLocal();
      int tofcellId = tofPid->btofCellId();
      bool badtofcell = tofcellId==8994 || tofcellId==8998 || tofcellId == 8999;
      if (fabs(btofYLocal)<anaCuts::tofYlocal && btofMatchFlag && (!badtofcell))
      {
        beta = tofPid->btofBeta();
        // if (beta < 1e-4)
        // {
        //   StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
        //   // StThreeVectorF vtx(vtx3.x(),vtx3.y(),vtx3.z());
        //   // TVector3 const btofHitPos3 = tofPid->btofHitPos();
        //   StThreeVectorF const btofHitPos = tofPid->btofHitPos();
        //   // StThreeVectorF btofHitPos(btofHitPos3.x(),btofHitPos3.y(),btofHitPos3.z());
        //   // StPicoPhysicalHelix helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        //   StPhysicalHelixD helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        //   float L = tofPathLength(&vtx, &btofHitPos, helix.curvature());
        //   float tof = tofPid->btof();
        //   if (tof > 0) beta = L / (tof * (C_C_LIGHT / 1.e9));
        //   else beta = std::numeric_limits<float>::quiet_NaN();
        // }
      }
    }
  } 
  return beta;
}

int StPicoElectronMaker::getCentralityBin(float z,int runId,double mult,double &weight) const
{
  // float mult_corr = mult;
  //if trigger 580001, correct the refmult
  // mult+=gRandom->Rndm();
  if (runId<=18156031) {
    double fvz = 0; 
    
    const Double_t par_z[7] = {435.9, -0.02413, -0.003707, 0.0002204, 1.487e-5, -2.95e-7, -1.867e-8};
    const Double_t RefMult_ref = par_z[0];
    const Double_t  RefMult_z = par_z[0] + par_z[1]*z + par_z[2]*z*z + par_z[3]*z*z*z + par_z[4]*z*z*z*z + par_z[5]*z*z*z*z*z + par_z[6]*z*z*z*z*z*z; // Parametrization of mean RefMult vs. z_vertex position
    if  (RefMult_z>0.0) mult=mult*RefMult_ref/RefMult_z;
    //  for (int i=0;i<anaCuts::nparVz_mult;i++){ 
    //    fvz +=anaCuts::parVz_mult[i]*std::pow((double)vz,i);
    //  }  
    //
    // mult=mult*1.0*anaCuts::parVz_mult[0]/fvz;
  }
  weight = reweight(mult);
  if  (mult<7)  return  -1;
  for (int cent=0;cent<anaCuts::nCent;cent++)
  {
    if (mult<anaCuts::Refmult_cent[cent]) return cent-1;
  }
  return anaCuts::nCent-1;
}
double StPicoElectronMaker::reweight(float x) const
{
  // float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  double p[5] = {0.811,238.9,24.31,-25.16,6.325e-5};
  if (x>=70 ||  x == -(p[3]/p[2]) ) return 1.;
  // else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]);
}
