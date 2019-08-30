/* **************************************************
 *
 *  Authors: Yuanjing Ji
 Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
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
#include "StPicoDstarMixedMaker.h"
#include "StAnaCuts.h"
#include "StMemStat.h"
#include "calmean.h"
// #include "TVector3.h"
#ifndef DEBUG
#define DEBUG 0
#endif
ClassImp(StPicoDstarMixedMaker)
  StPicoDstarMixedMaker::StPicoDstarMixedMaker(char const * name, TString const inputFilesList, TString const outFileBaseName, StPicoDstMaker* picoDstMaker):
    StMaker(name), mPicoDstMaker(picoDstMaker),
    mInputFilesList(inputFilesList), mOutFileBaseName(outFileBaseName),mRunbyRunQA(false)
{}

Int_t StPicoDstarMixedMaker::Init()
{
  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");
  // -------------- USER VARIABLES -------------------------
  mFile = new TFile(mOutFileBaseName+".QA.root", "RECREATE");
  //initialize trees
  initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoDstarMixedMaker::~StPicoDstarMixedMaker()
{}
//-----------------------------------------------------------------------------
void StPicoDstarMixedMaker::initHists(){
  int totalNum = 614;
  if (mRunbyRunQA){
    ifstream readnum;
    readnum.open(mRunNumList);
    int tmp=0;
    if (DEBUG) cout<<"start initial run number..."<<endl;
    for (int i=0;i<totalNum;i++){
      readnum>>tmp;
      runnum.insert(pair<int,int>(tmp,i));
      if (DEBUG) cout <<"run number : " << tmp <<" id :" <<runnum[tmp] <<endl;
    }
    readnum.close();
  }
  // event level QA
  hevt = new TH1D("hevt","hevt",totalNum,0,totalNum);
  hevtcut = new TH1D("hevtcut","hevtcut",totalNum,0,totalNum);
  hevtbadcut = new TH1D("hevtbadcut","Events after remove bad run;Run;Counts",totalNum,0,totalNum);
  //run by run QA
  if (mRunbyRunQA){ 
    pVpdVz = new TProfile("VpdVz","VpdVz vs runId;runId;VpdVz(cm)",totalNum,0,totalNum);
    pVzVpdVz = new TProfile("VzVpdVz","VzVpdVz vs runId;runId;VpdVz-Vz(cm)",totalNum,0,totalNum);
    pRefmult = new TProfile("Refmult","Refmult vs runId;runId;Refmult",totalNum,0,totalNum);
    pVpdHitEast = new TProfile("pVpdHitEast","pVpdHitEast;runId;pVpdHitEast",totalNum,0,totalNum);
    pVpdHitWest = new TProfile("pVpdHitWest","pVpdHitWest;runId;pVpdHitWest",totalNum,0,totalNum);
    pVz  = new TProfile("pVz","pVz;runId;pVz(cm)",totalNum,0,totalNum);
    pVx = new TProfile("pVx","pVx;runId;pVx(cm)",totalNum,0,totalNum);
    pVy = new TProfile("pVy","pVy;runId;pVy(cm)",totalNum,0,totalNum);
    pVr = new TProfile("pVr","pVr;runId;pVr(cm)",totalNum,0,totalNum);
    //track level QA
    pTof = new TProfile("Tof","1/#beta vs runId;runId;1/#beta",totalNum,0,totalNum);
    pDedx = new TProfile("Dedx","dEdx vs runId;runId;dEdx",totalNum,0,totalNum);
    //pRMSDedx = new TProfile("RMSDedx","RMSdEdx vs runId;runId;RMSdEdx",totalNum,0,totalNum);
    pgDCA = new TProfile("gDCA","gDCA vs runId;runId;global DCA(cm)",totalNum,0,totalNum);
    pgPt = new TProfile("gPt","global Pt vs runId;runId;global p_{T}(GeV/c)",totalNum,0,totalNum);
    pgPhi = new TProfile("gPhi","global Phi vs runId;runId;gPhi",totalNum,0,totalNum);
    pgEta = new TProfile("gEta","global Eta vs runId;runId;Eta",totalNum,0,totalNum);
    pNFits = new TProfile("NFits","NHitsFit vs runId;runId;nHitsFit",totalNum,0,totalNum);
    ppPt = new TProfile("pPt","primary Pt vs runId;runId;primary p_{T}(GeV/c)",totalNum,0,totalNum);
    ppEta = new TProfile("pEta","primary Eta vs runId;runId;pEta",totalNum,0,totalNum);
    ppPhi = new TProfile("pPhi","primary Phi vs runId;runId;pPhi",totalNum,0,totalNum);
  }
   //event QA
    hVxVyVz = new TH3F("hVxVyVz","VxVyVz;Vx(cm);Vy(cm);Vz(cm)",100,-1,1,100,-1,1,240,-60,60);
    hVz = new TH1F("hVz","Vz;Vz(cm);Counts",240,-60,60);
    hVr = new TH1F("hVr","Vr;Vr(cm);Counts",100,-1,1);
    hVzVpdVz = new TH1F("hVzVpdVz","Vz-VpdVz(cm)",120,-6,6);
    // TH3F* hnEvsEtavsVz;
    hnEvsEtavsVz = new TH2F("hnEvsEtavsVz","nElectron;#eta;Vz(cm)",40,-2,2,240,-60,60);
    hnTofMulvsRef = new TH2F("hnTofMulvsRef","hnTofMul vs Ref;btofTrayMultiplicity;refMult",2000,0,2000,900,0,900); 
    hnTofMatvsRef= new TH2F("hnTofMatvsRef","nTofmatch VS Refmult;nTofMatch;refMult",900,0,900,900,0,900);
    hnTofHitvsRef= new TH2F("hnTofHitvsRef","hnTofHit vs Ref;nTofHits;refMult",900,0,900,900,0,900);

   //tracl level QA
    hnHitsFit = new TH1F("hnHitsFit","nHitsFit;nHitsFit",50,0,50);
    // hpDca = new TH1F("hpDca","pDca",50,0,5);
    hgDca = new TH1F("hgDca","gDca",50,0,5);
    hinvBetavsP = new TH2F("hinvBetavsP","#frac{1}{#beta} vs p;p(GeV/c);#frac{1}{#beta}",60,0,6,60,0.5,3.5);
    // hinvBetavsY;
    hdEdx = new TH2F("hdEdx","dEdx vs p*charge;p*charge(GeV/c);dEdx",40,-2,2,100,0,25);
    hpt = new TH1F("hpt","hpt;p_{T}(GeV/c)",120,0,12);
    hEta = new TH1F("hEta","Eta;#eta",60,-1.5,1.5);
    hPhi = new TH1F("hPhi","Phi;#phi",80,-4,4);
}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Finish()
{
  mFile->cd();
  //write the hists
 //event QA
    hVxVyVz->Write();
    hVz->Write();
    hVr->Write();
    hVzVpdVz->Write();
    hnEvsEtavsVz->Write();
    hnTofMulvsRef->Write(); 
    hnTofMatvsRef->Write();
    hnTofHitvsRef->Write();

    hevt->Write();
    hevtcut->Write();
    hevtbadcut->Write();
    
   //tracl level QA
    hnHitsFit->Write();
    // hpDca->Write();
    hgDca->Write();
    hinvBetavsP->Write();
    // hinvBetavsY->Write();
    hdEdx->Write();
    hpt->Write();
    hEta->Write();
    hPhi->Write();

  if (mRunbyRunQA) {
    pVpdVz->Write();
    pVzVpdVz->Write();
    pRefmult->Write();
    pVpdHitEast->Write();
    pVpdHitWest->Write();
    pVz->Write();
    pVx->Write();
    pVy->Write();
    pVr->Write();
    pTof->Write(); 
    pDedx->Write();
    pgDCA->Write();
    pgPt->Write();
    pgPhi->Write();
    pgEta->Write();
    pNFits->Write();
    ppPt->Write();
    ppEta->Write();
    ppPhi->Write();
  }
  mFile->Close();
  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Make()
{
  // StMemStat mem;
  if (!mPicoDstMaker)
  {
    LOG_WARN << " StPicoDstarMixedMaker - No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }
  StPicoDst const* picoDst = mPicoDstMaker->picoDst();
  if (!picoDst)
  {
    LOG_WARN << "StPicoDstarMixedMaker - No PicoDst! Skip! " << endm;
    return kStWarn;
  }
  // -------------- USER ANALYSIS -------------------------
  StPicoEvent const * picoEvent = picoDst->event();
  //trigger
  if (!isGoodTrigger(picoEvent)) return 0;    
  mRunId = picoEvent->runId();
  hevt->Fill(runnum[mRunId]);
  
  if (mRunbyRunQA && isGoodQaEvent(picoEvent) ){ 
    //primary vertex
    // TVector3 pVtx = picoEvent->primaryVertex();
    StThreeVectorF pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    mVr = sqrt(mVx*mVx+mVy*mVy);
    mVpdVz = picoEvent->vzVpd();
    mRefmult = picoEvent->refMult();
    mVpdHitEast = picoEvent->nVpdHitsEast();
    mVpdHitWest = picoEvent->nVpdHitsWest();
    if (DEBUG) cout<<"start filling "<<mRunId<<" "<<runnum[mRunId]<<endl; 
    // if (picoEvent->btofTrayMultiplicity()>700 && picoEvent->refMult()<20) hbadTofMult->Fill(runnum[mRunId]);   
    if (picoEvent->btofTrayMultiplicity()>700 && picoEvent->refMult()<20) cout<<"bad TOF mult " <<mRunId<<endl;   
    //fill event level profile
    pVpdVz->Fill(runnum[mRunId],mVpdVz);
    pVzVpdVz->Fill(runnum[mRunId],mVpdVz-mVz);
    pRefmult->Fill(runnum[mRunId],mRefmult);
    pVpdHitEast->Fill(runnum[mRunId],mVpdHitEast);
    pVpdHitWest->Fill(runnum[mRunId],mVpdHitWest);
    pVx->Fill(runnum[mRunId],mVx);
    pVy->Fill(runnum[mRunId],mVy);
    pVz->Fill(runnum[mRunId],mVz);
    pVr->Fill(runnum[mRunId],mVr);
    //track level 
    int nTracks = picoDst->numberOfTracks();
    //if (DEBUG)  cout << nTracks <<endl;
    //global
    for (int itrack=0;itrack<nTracks;itrack++){
      StPicoTrack* trk = picoDst->track(itrack);
      bool goodQAtrack = isGoodQaTrack(trk); 
      if (!goodQAtrack) continue;
      if (!(fabs(trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z()))<anaCuts::qaDca)) continue;
      bool isprimary = trk->isPrimary();
      double ptot = trk->gMom(pVtx, picoEvent->bField()).mag();
      float beta = getTofBeta(trk);
      bool tofmatch =( beta!=std::numeric_limits<float>::quiet_NaN() )&& beta>0;
      bool tofQaPion = false;
      if (tofmatch) {
        float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
        tofQaPion = fabs(1. / beta - 1. / beta_pi) < anaCuts::qaTofPion;
        if (1./beta<0.9 && ptot>0.7) cout<<"bad tof beta " <<mRunId<<endl; 
      }
      bool tpcQaPion = fabs(trk->nSigmaPion()) < anaCuts::qaTpcPion;

      //global
      //runbyrunQA
      //pid performance 
      if (tofQaPion && ptot<1) pTof->Fill(runnum[mRunId],(1./beta)); 
      if (tpcQaPion && ptot<0.5) {
        pDedx->Fill(runnum[mRunId],trk->dEdx());
      }
      //global tracking performance
      pgPt->Fill(runnum[mRunId],trk->gMom().perp());
      pgDCA->Fill(runnum[mRunId],trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z()));
      pgPhi->Fill(runnum[mRunId],trk->gMom().phi());
      pgEta->Fill(runnum[mRunId],trk->gMom().pseudoRapidity());
      pNFits->Fill(runnum[mRunId],trk->nHitsFit()); 

      //primary 
      if (isprimary){
        ppPt->Fill(runnum[mRunId],trk->pMom().perp());
        ppEta->Fill(runnum[mRunId],trk->pMom().pseudoRapidity());
        ppPhi->Fill(runnum[mRunId],trk->pMom().phi());
      }
    }
  } //runbyrun QA 
  return kStOK;
  //event and track level QA
  if (isBadrun(mRunId)) return 0;
  hevtbadcut->Fill(mRunId);
  if (isGoodEvent(picoEvent)){
    StThreeVectorF pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    mVpdVz = picoEvent->vzVpd();
    hevtcut->Fill(mRunId);
    hVz->Fill(mVz);
    hVxVyVz->Fill(mVx,mVy,mVz);
    hVr->Fill(sqrt(mVy*mVy+mVx*mVx));
    hVzVpdVz->Fill(mVpdVz-mVz);  
    hnTofMulvsRef->Fill(picoEvent->btofTrayMultiplicity(),picoEvent->refMult());  
    hnTofMatvsRef->Fill(picoEvent->nBTOFMatch(),picoEvent->refMult());  
    double ntofhits = 0; 
    int nTracks = picoDst->numberOfTracks();
    for (int itrack=0;itrack<nTracks;itrack++){
      StPicoTrack* trk = picoDst->track(itrack);
      bool goodtrack = isGoodTrack(trk);
      hgDca->Fill(trk->gDCA(mVx,mVy,mVz));
      bool isprimary = trk->isPrimary();
      if (!goodtrack) continue;
      if (!isprimary) continue;
      StThreeVectorF mom = trk->pMom(); 
      hpt->Fill(mom.perp());
      hPhi->Fill(mom.phi());
      hEta->Fill(mom.pseudoRapidity());
      // hpDca->Fill(trk->pDca(mVx,mVy,mVz));
      hnHitsFit->Fill(trk->nHitsFit());
      double beta = getTofBeta(trk);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //choose inclusive electron
      bool isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>0.75;
      bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false;
      if (isTOFElectron && isTPCElectron) hnEvsEtavsVz->Fill(mom.pseudoRapidity(),mVz); 
      if (tofmatch) {
        ntofhits++;
        hinvBetavsP->Fill(mom.mag(),1./beta);
        // hinvBetavsY->Fill(1./beta,);
      }
      hdEdx->Fill(mom.mag()*trk->charge(),trk->dEdx());
    }
    hnTofHitvsRef->Fill(ntofhits,picoEvent->refMult());
  } //Good Event
  return kStOK;
}
bool StPicoDstarMixedMaker::isGoodTrigger(StPicoEvent const* const picoEvent) const
{
  for (auto trg : anaCuts::triggers)
  {
    if (picoEvent->isTrigger(trg)) return true;
  }

  return false;
}
bool StPicoDstarMixedMaker::isGoodTrack(StPicoTrack const* const trk) const
{
  StThreeVectorF vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::GPt && fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::Eta &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx ;
}
bool StPicoDstarMixedMaker::isGoodQaTrack(StPicoTrack const* const trk) const
{
  StThreeVectorF vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::qaGPt && fabs(trk->nHitsFit()) >= anaCuts::qaNHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::qaEta &&
    // fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx && fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::qaDca;
    fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx ;
}
bool StPicoDstarMixedMaker::isGoodQaEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::qavz &&
    fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::qavzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::qaVerror && fabs(pVtx.y()) < anaCuts::qaVerror &&
        fabs(pVtx.z()) < anaCuts::qaVerror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::qaVr;
}
bool StPicoDstarMixedMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::vz &&
    fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
        fabs(pVtx.z()) < anaCuts::Verror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}
float StPicoDstarMixedMaker::getTofBeta(StPicoTrack const* const trk) const
{
  int index2tof = trk->bTofPidTraitsIndex();
  float beta = std::numeric_limits<float>::quiet_NaN();
  if (index2tof >= 0)
  {
    StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
    if (tofPid)
    {
      beta = tofPid->btofBeta();
      if (beta < 1e-4)
      {
        StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
        // StThreeVectorF vtx(vtx3.x(),vtx3.y(),vtx3.z());
        // TVector3 const btofHitPos3 = tofPid->btofHitPos();
        StThreeVectorF const btofHitPos = tofPid->btofHitPos();
        // StThreeVectorF btofHitPos(btofHitPos3.x(),btofHitPos3.y(),btofHitPos3.z());
        // StPicoPhysicalHelix helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        StPhysicalHelixD helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        float L = tofPathLength(&vtx, &btofHitPos, helix.curvature());
        float tof = tofPid->btof();
        if (tof > 0) beta = L / (tof * (C_C_LIGHT / 1.e9));
        else beta = std::numeric_limits<float>::quiet_NaN();
      }
    }
  } 
  return beta;
}

