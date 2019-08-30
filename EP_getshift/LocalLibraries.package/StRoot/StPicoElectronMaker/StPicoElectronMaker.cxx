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
bool StPicoElectronMaker::initHists(){
  // if (DEBUG)cout<<"ok1"<<endl;
  TFile* file = TFile::Open(mRecenterFile.c_str());
  if (!file->IsOpen()){
    cout<<"Error: Cannot read recenter file!!!"<<endl;
    return kStWarn;
  }
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic] = (TProfile*)file->Get(Form("pQxRecenterMin_%d",ic));  
    if (!pQxRecenterM[ic]) cout<<"Error: Cannot find recenter hists"<<endl;
    pQxRecenterM[ic]->SetDirectory(0);
    pQyRecenterM[ic] = (TProfile*)file->Get(Form("pQyRecenterMin_%d",ic));  
    pQyRecenterM[ic]->SetDirectory(0);
    pQxRecenterP[ic] = (TProfile*)file->Get(Form("pQxRecenterPlu_%d",ic));  
    pQxRecenterP[ic]->SetDirectory(0);
    pQyRecenterP[ic] = (TProfile*)file->Get(Form("pQyRecenterPlu_%d",ic));  
    pQyRecenterP[ic]->SetDirectory(0);
  }  
  file->Close();
  // mFile->cd();
  // if (DEBUG)cout<<"ok2"<<endl;
  hevt = new TH1D("hevt","hevt",1,0.5,1.5);
  hevtcut = new TH1D("hevtcut","hevtcut",1,0.5,1.5);
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  float const PI = TMath::Pi();
  hEventPlaneCent_M = new TH2F("hEventPlaneCent_M_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_M_Sh = new TH2F("hEventPlaneCent_M_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_P = new TH2F("hEventPlaneCent_P_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_P_Sh = new TH2F("hEventPlaneCent_P_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  EPconsRes = new TProfile("EPRes","Event Plane resolution;Cent;Res",9,-0.5,8.5);
  prfQxCent = new TProfile("prfQxCent","prfQxCent", 9, -0.5, 8.5);
  prfQyCent = new TProfile("prfQyCent","prfQyCent", 9, -0.5, 8.5);
  hQyQxCent = new TH3F("hQyQxCent", "hQyQxCent;Cent;Qx;Qy", 9,-0.5,8.5,1000,-50,50,1000,-50,50);
  for (int i=0;i<20;i++) 
  {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[i][ic] = new TProfile(Form("pSinEtaM_Cent%d_%d",ic,i+1),Form("pSinEtaM_Cent%d_%d",ic,i+1),614,0,614);
      pCosEtaM[i][ic] = new TProfile(Form("pCosEtaM_Cent%d_%d",ic,i+1),Form("pCosEtaM_Cent%d_%d",ic,i+1),614,0,614);
      pSinEtaP[i][ic] = new TProfile(Form("pSinEtaP_Cent%d_%d",ic,i+1),Form("pSinEtaP_Cent%d_%d",ic,i+1),614,0,614);
      pCosEtaP[i][ic] = new TProfile(Form("pCosEtaP_Cent%d_%d",ic,i+1),Form("pCosEtaP_Cent%d_%d",ic,i+1),614,0,614);
    }
  }
  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoElectronMaker::Finish()
{
  mFile->cd();
  //write the hists
  hevt->Write();
  hevtcut->Write();

  hEventPlaneCent_M->Write();
  // hEventPlaneCent_M_Sh->Write();
  hEventPlaneCent_P->Write();
  // hEventPlaneCent_P_Sh->Write();
  EPconsRes->Write();
  prfQxCent->Write();
  prfQyCent->Write();
  hQyQxCent->Write();
  for (int i=0;i<20;i++)
  {
    for (int ic=0;ic<9;ic++){
      pCosEtaP[i][ic]->Write();
      pSinEtaP[i][ic]->Write();
      pCosEtaM[i][ic]->Write();
      pSinEtaM[i][ic]->Write();
    }
  }
  hcent->Write();
  hcentwg->Write();
  mFile->Close();
  for (int ic=0;ic<9;ic++)
  {
    delete pQxRecenterM[ic];
    delete pQyRecenterM[ic];
    delete pQxRecenterP[ic]; 
    delete pQyRecenterP[ic]; 
  }
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
  if (!isGoodTrigger(picoEvent)) return kStOK;    
  mRunId = picoEvent->runId();
  // if (DEBUG)cout<<"runid is "<<mRunId<<" "<<runnum[mRunId]<<endl;
  hevt->Fill(1);
  //event and track level QA
  if (isBadrun(mRunId)) return kStOK;
  
  if (isGoodEvent(picoEvent)){
    
   bool strangetof = picoEvent->btofTrayMultiplicity()>250&&picoEvent->refMult()<20;
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
   float weight=1;

   if (DEBUG)cout<<"ok3"<<endl;
   mCent = getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight);
   if (mCent<0) return  kStOK;
   hcent->Fill(mCent);
   hcentwg->Fill(mCent,weight);
   // loopTracks();
   recenterEventPlane();
  } //Good Event
  return kStOK;
}
bool StPicoElectronMaker::recenterEventPlane()
{
  if (DEBUG)cout<<"ok4"<<endl;
  int nTracks = mPicoDst->numberOfTracks();
  double QxEtaM=0., QxEtaP=0.,QyEtaM=0.,QyEtaP=0.;
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
    if (eta<-0.05) //eta gap = 0.05 
    {
      qx-=pQxRecenterM[mCent]->GetBinContent(runnum[mRunId]+1);
      qy-=pQyRecenterM[mCent]->GetBinContent(runnum[mRunId]+1);
      QxEtaM+=qx;
      QyEtaM+=qy;
    }
    else if (eta>0.05)
    {
      qx-=pQxRecenterP[mCent]->GetBinContent(runnum[mRunId]+1);
      qy-=pQyRecenterP[mCent]->GetBinContent(runnum[mRunId]+1);
      QxEtaP+=qx;
      QyEtaP+=qy;
    } 
  } //loop  track
  if (DEBUG)cout<<"ok5"<<endl;
    TVector2 QEta_M(QxEtaM, QyEtaM);  //minus eta recentered vector
    double  EP_M= QEta_M.Phi()*0.5;
    double EP_M_Sh = getEventPlaneShift(EP_M,-1,mCent); 
    if (EP_M_Sh<0) EP_M_Sh+=TMath::Pi();
    if(QEta_M.Mod()>0) 
    {
      hEventPlaneCent_M->Fill(mCent, EP_M);
      // hEventPlaneCent_M_Sh->Fill(mCent, EP_M_Sh);
    }

    TVector2 QEta_P(QxEtaP, QyEtaP);  //plus eta recentered vector
    float  EP_P= QEta_P.Phi()*0.5;
    double EP_P_Sh = getEventPlaneShift(EP_P,1,mCent); 
    if (EP_P_Sh<0) EP_P_Sh+=TMath::Pi();
    if(QEta_P.Mod()>0)
    {
      hEventPlaneCent_P->Fill(mCent, EP_P);
      // hEventPlaneCent_P_Sh->Fill(mCent, EP_P_Sh);
    }

    for (int i=0;i<20;i++)
    {
      pCosEtaP[i][mCent]->Fill(runnum[mRunId],std::cos(2*(i+1)*EP_P));
      pSinEtaP[i][mCent]->Fill(runnum[mRunId],std::sin(2*(i+1)*EP_P));
      pCosEtaM[i][mCent]->Fill(runnum[mRunId],std::cos(2*(i+1)*EP_M));
      pSinEtaM[i][mCent]->Fill(runnum[mRunId],std::sin(2*(i+1)*EP_M));
    }

    //event plane resolution
    float resolution = cos(2.*(EP_P_Sh-EP_M_Sh));
    EPconsRes->Fill(mCent,resolution);
    prfQxCent->Fill(mCent,QxEtaP);
    prfQyCent->Fill(mCent,QyEtaP);
    hQyQxCent->Fill(mCent,QxEtaM,QyEtaM);
    return kStOK;
}
bool StPicoElectronMaker::getIncElectronv2()
{
  //to be added in later on analysis 
  return kStOK;
}
double StPicoElectronMaker::getEventPlaneShift(double EP_Re,int side,int cent) const
{
  //to be added afer check recenter
  //side =-1 minus eta, +1 plus eta
  double EP_Sh = EP_Re;
  return EP_Sh;
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

int StPicoElectronMaker::getCentralityBin(float vz,int runId,float mult,float &weight) const
{
  // float mult_corr = mult;
  //if trigger 580001, correct the refmult
  mult+=gRandom->Rndm();
  if (runId<18156031) {
    float fvz = 0; 
    for (int i=0;i<anaCuts::nparVz_mult;i++){ 
      fvz +=anaCuts::parVz_mult[i]*std::pow(vz,i);
    }  
    mult=mult*1.0*anaCuts::parVz_mult[0]/fvz;
  }
  weight = reweight(mult);
  for (int cent=0;cent<anaCuts::nCent;cent++)
  {
    if (mult<anaCuts::Refmult_cent[cent]) return cent-1;
  }
  return anaCuts::nCent-1;
}
float StPicoElectronMaker::reweight(float x) const
{
  // float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  float p[5] = {0.811,238.9,24.31,-25,6.325e-5};
  if (x>70) return 1;
  // else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]);
}
