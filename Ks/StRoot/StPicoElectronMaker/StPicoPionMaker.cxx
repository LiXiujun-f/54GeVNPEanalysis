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
#include "StPicoPionMaker.h"
#include "StAnaCuts.h"
#include "StMemStat.h"
#include "StLorentzVector.hh"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "../StRefMultCorr/CentralityMaker.h"
// #include "calmean.h"
// #include "TVector3.h"
#ifndef DEBUG
#define DEBUG 0
#endif
ClassImp(StPicoPionMaker)
  StPicoPionMaker::StPicoPionMaker(char const * name, TString const inputFilesList, TString const outFileBaseName, StPicoDstMaker* picoDstMaker):
    StMaker(name), mPicoDstMaker(picoDstMaker),
    mInputFilesList(inputFilesList), mOutFileBaseName(outFileBaseName)
{}

Int_t StPicoPionMaker::Init()
{
  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");
  // -------------- USER VARIABLES -------------------------
  mFile = new TFile(mOutFileBaseName+".Ks.root", "RECREATE");
  //initialize trees
  initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoPionMaker::~StPicoPionMaker()
{}
//-----------------------------------------------------------------------------
void StPicoPionMaker::initHists(){
  // hevt = new TH1D("hevt","hevt",9,-0.5,8.5);
  hevt = new TH1D("hevt","hevt",1,0.5,1.5);
  // hevtcut = new TH1D("hevtcut","hevtcut",9,-0.5,8.5);
  hevtcut = new TH1D("hevtcut","hevtcut",1,0.5,1.5);
  // hVxVyVz = new TH3F("hVxVyVz","VxVyVz;Vx(cm);Vy(cm);Vz(cm)",100,-0.5,0.5,100,-0.5,0.5,240,-60,60);
  // hnTofMulvsRef = new TH2F("hnTofMulvsRef","hnTofMul vs Ref;btofTrayMultiplicity;refMult",2000,0,2000,900,0,900); 
  // hnTofMatvsRef= new TH2F("hnTofMatvsRef","nTofmatch VS Refmult;nTofMatch;refMult",900,0,900,900,0,900);

  // electron
  // // hinvBetavsP = new TH2F("hinvBetavsP","#frac{1}{#beta} vs p;p(GeV/c);#frac{1}{#beta}",1200,0,3,640,0.7,1.5);
  // // hdEdx = new TH2F("hdEdx","dEdx vs p*charge;p*charge(GeV/c);dEdx",200,-2,2,400,0,25);
  // hnSigEvsP = new TH2F("hnSigEvsP","all tracks;p(GeV/c);nSigE", 480,0,6,1000,-10,15);  //all tracks
  // hnSigE_e = new TH2F("hnSigE_e","photonic elecron;p(GeV/c);nSigE", 480 ,0,6, 400,-5,5);  //select PE e
  // hnSigE_e_LS = new TH2F("hnSigE_e_ls","photonic elecron like sign;p(GeV/c);nSigE", 480, 0,6, 400,-5,5);
  // hnSigE_p = new TH2F("hnSigE_p","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 1000,-10,15);  //select tof p
  hnSigE_pi = new TH3F("hnSigE_pi","pion from Ks;p(GeV/c);nSigE;Centrality", 480,0,6,75,-10,5,10,-0.5,8.5);  //tof pi
  hnSigE_pi_LS = new TH3F("hnSigE_pi_ls","pion from Ks like sign;p(GeV/c);nSigE;Centrality", 480, 0,6, 75,-10,5,10,-0.5,8.5);
  // hnSigE_k = new TH2F("hnSigE_k","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6, 1000,-10,15);
  // hnSigE_Cut = new TH2F("hnSigE_Cut","elecron after pid cut;p(GeV/c);nSigE", 480 ,0,6, 400,-5,5);
  // hnSigE_tof = new TH2F("hnSigE_tof","elecron with tof only;p(GeV/c);nSigE", 480,0,6, 1000,-10,15);
  hnSigE_pi_tof = new TH3F("hnSigE_pi_tof","pion with tof only;p(GeV/c);nSigE;Centrality", 480,0,6, 40,-10,10,10,-0.5,8.5);
  // hincEptCent = new TH2F("hincEptCent","inclusive e;p_{T}(GeV/c);cent", 50,0,5,9,-0.5,8.5);
  // hePtvsP = new TH2F("hePtvsP","hePtvsP",200,0,10,200,0,10);
  hKs = new TH3F("hKs","M#pi#pi;M#pi#pi;partner #pi p_{T};Centrality",150,0.45,0.55,100,0.2,10,10,-0.5,8.5);
  hKs_LS = new TH3F("hKs_LS","Mpipi like sign;Mpipi;partner e p_{T};Centrality",150,0.45,0.55,100,0.2,10,10,-0.5,8.5);
  // hV0 = new TH3F("hV0","hV0;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  // hV0_LS = new TH3F("hV0_LS","hV0_LS;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
}
//-----------------------------------------------------------------------------
Int_t StPicoPionMaker::Finish()
{
  mFile->cd();
  //write the hists
  hevt->Write();
  hevtcut->Write();
  // hinvBetavsP->Write();
  // hnTofMulvsRef->Write();
  // hnTofMatvsRef->Write();
    //hists
  // hnSigEvsP->Write();  //all tracks
  // hnSigE_e->Write();  //select PE e
  // hnSigE_p->Write();  //select tof p
  hnSigE_pi->Write();  //Ks pi
  // hnSigE_k->Write();
  // hnSigE_Cut->Write();
  // hnSigE_tof->Write();
  hnSigE_pi_tof->Write();
  // hePtvsP->Write();
  hKs_LS->Write();
  hKs->Write();
  hnSigE_pi_LS->Write();
  // hV0->Write();
  // hV0_LS->Write();
  hcent->Write();
  hcentwg->Write();
  // hincEptCent->Write();
  mFile->Close();
  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoPionMaker::Make()
{
  // StMemStat mem;
  if (mPicoDstMaker)
  {
    mPicoDst = mPicoDstMaker->picoDst();
  }
  else {
    LOG_WARN << " StPicoPionMaker - No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }
  mRefMultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
  if (!ProcessPicoEvent()) return kStOK;
  return kStOK;
}
bool StPicoPionMaker::ProcessPicoEvent()
{
  if (!mPicoDst)
  {
    LOG_WARN << "StPicoPionMaker - No PicoDst! Skip! " << endm;
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
   mRefMultCorrUtil->init(mRunId);

   if (!mRefMultCorrUtil)
   {
      LOG_WARN << " No mRefMultCorrUtil! Skip! " << endl;
      return kStWarn;
   }
   if (isGoodEvent(picoEvent)){
    
   bool strangetof = picoEvent->btofTrayMultiplicity()>250&&picoEvent->refMult()<20;

   if (strangetof) return kStOK;
    StThreeVectorF pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    hevtcut->Fill(1);
    float bField=picoEvent->bField(); 
    // centrality definition
    float weight=1;
    mRefMultCorrUtil->initEvent(picoEvent->refMult(), pVtx.z(), picoEvent->ZDCx()) ;
    int mCent  = mRefMultCorrUtil->getCentralityBin9();
    weight = mRefMultCorrUtil->getWeight();
    // cout<<weight<<" ";
    // int mCent =  getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight);
    // cout<< mCent<<" "<< getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight)<<" "<<weight<<endl;
    if (mCent<0) return  kStOK;
    hcent->Fill(mCent);
    hcentwg->Fill(mCent,weight);
    int nTracks = mPicoDst->numberOfTracks();
    for (int itrack1=0;itrack1<nTracks;itrack1++){
      StPicoTrack* pe_1 = mPicoDst->track(itrack1);
      bool isprimary = pe_1->isPrimary();
      if (!isprimary) continue;
      bool goodtrack = isGoodTrack(pe_1,pe_1->gDCA(mVx,mVy,mVz));
      if (!goodtrack) continue;
      if (pe_1->gDCA(mVx,mVy,mVz)<anaCuts::minDca) continue;
      StThreeVectorF mom = pe_1->pMom(); //primary momentum
      // StThreeVectorF mom = pe_1->gMom(); //momentum
      float beta = getTofBeta(pe_1);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //fill pid
      // if (tofmatch) hinvBetavsP->Fill(mom.mag(),1./beta);
      // hnSigEvsP->Fill(mom.mag(),pe_1->nSigmaElectron());

      Float_t nSigPi1 = pe_1->nSigmaElectron();
      // bool istofpi = mom.mag()>1.5?true:(tofmatch?isTofPion(pe_1,beta):false);  //tof at pt<1.5 and no pid at high pt 
      bool istofpi = tofmatch?isTofPion(pe_1,beta):false;  
      bool isPi1 = isfirstPion(pe_1,tofmatch,beta);
      //Fill histograms 
      if (istofpi) hnSigE_pi_tof->Fill(mom.mag(),nSigPi1,mCent,weight); 

      if (isPi1){
        for (int itrack2=0;itrack2<nTracks;itrack2++){
          if (itrack1==itrack2) continue;
          StPicoTrack* pe_2 = mPicoDst->track(itrack2);
          if (!isGoodTrack(pe_2,pe_2->gDCA(mVx,mVy,mVz))) continue;
          //apply only tof pid cut
          float beta2 = getTofBeta(pe_2);
          bool tofmatch2 = (beta2!=std::numeric_limits<float>::quiet_NaN()) && beta2>0;
          if (!issecondPion(pe_2,tofmatch2,beta2)) continue; 
          if (pe_2->gDCA(mVx,mVy,mVz)<anaCuts::minDca) continue;
          // use straight lines approximation to get point of DCA of e+e- pair
          StThreeVectorF const p1Mom = pe_1->gMom();
          StThreeVectorF const p2Mom = pe_2->gMom(); //global mom is at the dca to pvtx of the helix
          StPhysicalHelixD p1Helix = pe_1->helix(bField);
          StPhysicalHelixD p2Helix = pe_2->helix(bField);
          StPhysicalHelixD const p1Line(p1Mom, pe_1->origin(), bField*kilogauss, pe_1->charge()); //B=0, the straight line
          StPhysicalHelixD const p2Line(p2Mom, pe_2->origin(), bField*kilogauss, pe_2->charge());

          pair<double, double> const ss = p1Line.pathLengths(p2Line);
          StThreeVectorF const p1AtDca2p2 = p1Line.at(ss.first);
          StThreeVectorF const p2AtDca2p1 = p2Line.at(ss.second);

          // calculate DCA of pion to kaon at their DCA
          float DcaDaughters = (p1AtDca2p2 - p2AtDca2p1).mag();

          // calculate Lorentz vector of e+e- pair
          StThreeVectorF const p1MomAtDca = p1Helix.momentumAt(ss.first, bField * kilogauss);
          StThreeVectorF const p2MomAtDca = p2Helix.momentumAt(ss.second, bField * kilogauss);

          StLorentzVector<float> p2FourMom(p2MomAtDca,p2MomAtDca.massHypothesis(M_PION_PLUS));
          StLorentzVector<float> p1FourMom(p1MomAtDca,p1MomAtDca.massHypothesis(M_PION_PLUS));
          StLorentzVector<float> mother = p2FourMom+p1FourMom;
          // calculate pointing angle and decay length
          StThreeVectorF const V0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5;
          StThreeVectorF const vtxToV0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5 - pVtx;
          float PointingAngle = vtxToV0.angle(mother.vect());  //theta
          float cosTheta = std::cos(PointingAngle);
          float DecayLength = vtxToV0.mag();
          float KsDca = DecayLength* std::sin(PointingAngle); 

          bool passKstopocut = fabs(DcaDaughters) < anaCuts::pairDca && fabs(DecayLength)<anaCuts::DecayL[1] 
                               && fabs(DecayLength)>anaCuts::DecayL[0] && fabs(cosTheta) >anaCuts::cosTheta 
                               && KsDca<anaCuts::KsDca;

          if (passKstopocut && fabs(mother.rapidity())<1 && mother.m()<0.6 && mother.m()>0.4) {
            //reconstruct the photon
            bool unlike=pe_2->charge()*pe_1->charge()<0;
            bool MassWindow = false;
            if (mother.m() > 0.492 && mother.m() < 0.503) MassWindow = kTRUE; 
            if (unlike && MassWindow){
                hnSigE_pi->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron(),mCent,weight);
              }
            else if ((!unlike) && MassWindow){
                hnSigE_pi_LS->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron(),mCent,weight);
              }
              if (unlike) hKs->Fill(mother.m(),pe_2->gMom().perp(),mCent,weight); 
              else hKs_LS->Fill(mother.m(), pe_2->gMom().perp(),mCent,weight); 
            }
        } //second pe loop
      } // if is PE first 
    } //loop first track
  } //Good Event
  return kStOK;
}
bool StPicoPionMaker::isGoodTrigger(StPicoEvent const* const picoEvent) const
{
  for (auto trg : anaCuts::triggers)
  {
    if (picoEvent->isTrigger(trg)) return true;
  }

  return false;
}
bool StPicoPionMaker::isGoodTrack(StPicoTrack const* trk, float dca) const
{
  // StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::GPt && fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::Eta &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && fabs(dca)<=anaCuts::Dca &&
    trk->nHitsFit()/trk->nHitsMax() >=anaCuts::NHitsFit2Poss;
  // fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx &&
  // fabs( trk->gDCA(vtx.x() , vtx.y(), vtx.z() )) <= anaCuts::Dca;
}
bool StPicoPionMaker::isGoodQaTrack(StPicoTrack const* const trk) const
{
  StThreeVectorF vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::qaGPt && fabs(trk->nHitsFit()) >= anaCuts::qaNHitsFit && 
    fabs(trk->gMom().pseudoRapidity())<anaCuts::qaEta &&
    // fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx && fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::qaDca;
    fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx ;
}
bool StPicoPionMaker::isGoodQaEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::qavz &&
    fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::qavzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::qaVerror && fabs(pVtx.y()) < anaCuts::qaVerror &&
        fabs(pVtx.z()) < anaCuts::qaVerror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::qaVr;
}
bool StPicoPionMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  // TVector3 pVtx = picoEvent->primaryVertex();
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  double refmult = picoEvent->refMult();
  double Tofmult = picoEvent->btofTrayMultiplicity();
  bool pileup = (Tofmult*0.3405+48) < refmult;
  return (!pileup) && fabs(pVtx.z()) < anaCuts::vz &&  fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
        fabs(pVtx.z()) < anaCuts::Verror) && sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}
float StPicoPionMaker::getTofBeta(StPicoTrack const* const trk) const
{
  int index2tof = trk->bTofPidTraitsIndex();
  float beta = std::numeric_limits<float>::quiet_NaN();
  if (index2tof >= 0)
  {
    StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
    if (tofPid){
      bool btofMatchFlag = tofPid->btofMatchFlag();
      float btofYLocal = tofPid->btofYLocal();

      if (fabs(btofYLocal)<anaCuts::tofYlocal && btofMatchFlag)
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
bool StPicoPionMaker::isfirstPion(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  // first 
  bool isTOFPion = tofmatch?isTofPion(trk,beta):false; 
  bool isTPCPion = fabs(trk->nSigmaPion())<0.5;
  // return trk->gMom().mag()>1.5?isTPCPion:(isTPCPion && isTOFPion);
  return isTOFPion &&isTOFPion;
}

bool StPicoPionMaker::issecondPion(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  //same as tof electron 
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  StThreeVectorF mom = trk->gMom(); 
  float ptot = mom.mag();
  // bool isTPCPion= trk->nSigmaElectron()<4 && trk->nSigmaElectron()>-10; //almost have no cut
  bool isTPCPion= fabs(trk->nSigmaPion())<5; //almost have no cut
  float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
  bool isTOFPion = tofmatch?fabs(1./beta-1./beta_pi)<anaCuts::piTof2:false; //very loose tof cut 
  return isTOFPion && isTPCPion;
  // if (mom.mag()<1.5) return isTOFPion&&isTPCPion;
  // else {
  //   if (tofmatch) return isTOFPion&&isTPCPion;
  //   else return isTPCPion;
  // }
}
bool StPicoPionMaker::isElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{
  //Add TPC PID cut
  //tof PID is just used to supress the hadron contamination
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron=0;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
 if (mom.mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-1.2;
  else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(mom.mag()*2-2.8);
  return isTPCElectron;
  // if (mom.mag()<1.5) {
  //   if (isTOFElectron && isTPCElectron) return true;
  //   else return false;
  // }
  // else {
  //   if (tofmatch) return isTPCElectron && isTOFElectron;
  //   else return isTPCElectron;
  // }
}
bool StPicoPionMaker::isTofElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron=0;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
  if (mom.perp()<1.5) return isTOFElectron;
  else {
    if (tofmatch) return isTOFElectron;
    else return true;
  }
}

bool StPicoPionMaker::isTofKaon(StPicoTrack const * const trk, float beta) const
{
  //choose kaon sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_k = ptot / sqrt(ptot * ptot + M_KAON_PLUS * M_KAON_PLUS);
  return fabs(1 / beta - 1 / beta_k) < anaCuts::kTof ? true : false;
}
bool StPicoPionMaker::isTofPion(StPicoTrack const * const trk, float beta) const
{
  //choose pion sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
  return fabs(1. / beta - 1. / beta_pi) < anaCuts::piTof ;
}
bool StPicoPionMaker::isTofProton(StPicoTrack const * const trk,  float beta) const
{
  //choose proton sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_p = ptot / sqrt(ptot * ptot + M_PROTON * M_PROTON);
  return fabs(1 / beta - 1 / beta_p) < anaCuts::pTof ? true : false;
}
int StPicoPionMaker::getCentralityBin(float vz,int runId,float mult,float &weight) const
{
  // float mult_corr = mult;
  //if trigger 580001, correct the refmult
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
float StPicoPionMaker::reweight(float x) const
{
  float p[7] = {3.905,-204.4,1.851,24.32,-0.01746,6405,3.742e-5};
  if (x>70) return 1;
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
}
