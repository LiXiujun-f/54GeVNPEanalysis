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
// #include "TVector3.h"
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
  mFile = new TFile(mOutFileBaseName+".PE.root", "RECREATE");
  //initialize trees
  initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoElectronMaker::~StPicoElectronMaker()
{}
//-----------------------------------------------------------------------------
void StPicoElectronMaker::initHists(){
  // hevt = new TH1D("hevt","hevt",9,-0.5,8.5);
  hevt = new TH1D("hevt","hevt",1,0.5,1.5);
  // hevtcut = new TH1D("hevtcut","hevtcut",9,-0.5,8.5);
  hevtcut = new TH1D("hevtcut","hevtcut",1,0.5,1.5);
  // hVxVyVz = new TH3F("hVxVyVz","VxVyVz;Vx(cm);Vy(cm);Vz(cm)",100,-0.5,0.5,100,-0.5,0.5,240,-60,60);
  hnTofMulvsRef = new TH2F("hnTofMulvsRef","hnTofMul vs Ref;btofTrayMultiplicity;refMult",2000,0,2000,900,0,900); 
  hnTofMatvsRef= new TH2F("hnTofMatvsRef","nTofmatch VS Refmult;nTofMatch;refMult",900,0,900,900,0,900);

  // electron
  hinvBetavsP = new TH2F("hinvBetavsP","#frac{1}{#beta} vs p;p(GeV/c);#frac{1}{#beta}",1200,0,3,640,0.7,1.5);
  // hdEdx = new TH2F("hdEdx","dEdx vs p*charge;p*charge(GeV/c);dEdx",200,-2,2,400,0,25);
  hnSigEvsP = new TH2F("hnSigEvsP","all tracks;p(GeV/c);nSigE", 480,0,6,500,-10.01,15);  //all tracks  //avoid the precision
  hnSigE_e = new TH2F("hnSigE_e","photonic elecron;p(GeV/c);nSigE", 480 ,0,6,200 ,-5.01,5);  //select PE e
  hnSigE_e_LS = new TH2F("hnSigE_e_ls","photonic elecron like sign;p(GeV/c);nSigE", 480, 0,6, 200,-5.01,5);
  hinvBeta_e = new TH2F("hinvBeta_e","photonic elecron;p(GeV/c);nSigE", 480 ,0,6, 100,0.9,1.1);  //select PE e
  hinvBeta_e_LS = new TH2F("hinvBeta_e_ls","photonic elecron;p(GeV/c);nSigE", 480 ,0,6, 100,-0.9,1.1);  //select PE e
  hnSigE_p = new TH2F("hnSigE_p","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 500,-10.01,15);  //select tof p
  hnSigE_pi = new TH2F("hnSigE_pi","pion with tof pid;p(GeV/c);nSigE", 480 ,0,6, 300,-10.01,5);  //tof pi
  hnSigE_k = new TH2F("hnSigE_k","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6,500 ,-10.01,15);
  hnSigE_Cut = new TH2F("hnSigE_Cut","elecron after pid cut;p(GeV/c);nSigE", 480 ,0,6, 200,-5.01,5);
  hnSigE_tof = new TH2F("hnSigE_tof","elecron with tof only;p(GeV/c);nSigE", 480,0,6, 500,-10.01,15);
  hincEptCent = new TH2F("hincEptCent","inclusive e;p_{T}(GeV/c);cent", 100,0,5,9,-0.5,8.5);
  hePtvsP = new TH2F("hePtvsP","hePtvsP",200,0,10,200,0,10);
  hphoto = new TH3F("hphoto","Mee;Mee;partner e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphoto_LS = new TH3F("hphoto_LS","Mee like sign vs e pt vs cent;Mee;partner e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphotoVsPt_LS = new TH3F("hphotoVsPt_LS","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);
  hphotoVsPt = new TH3F("hphotoVsPt","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);
  hV0 = new TH3F("hV0","hV0;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  hV0_LS = new TH3F("hV0_LS","hV0_LS;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
}
//-----------------------------------------------------------------------------
Int_t StPicoElectronMaker::Finish()
{
  mFile->cd();
  //write the hists
  hevt->Write();
  hevtcut->Write();
  hinvBetavsP->Write();
  hnTofMulvsRef->Write();
  hnTofMatvsRef->Write();
  //hists
  hnSigEvsP->Write();  //all tracks
  hnSigE_p->Write();  //select tof p
  hnSigE_pi->Write();  //tof pi
  hnSigE_k->Write();
  hnSigE_Cut->Write();
  hnSigE_tof->Write();
  hePtvsP->Write();
  hphoto_LS->Write();
  hphoto->Write();
  hphotoVsPt->Write();
  hphotoVsPt_LS->Write();
  hinvBeta_e->Write();
  hinvBeta_e_LS->Write();
  hnSigE_e->Write();  //select PE e
  hnSigE_e_LS->Write();
  hV0->Write();
  hV0_LS->Write();
  hcent->Write();
  hcentwg->Write();
  hincEptCent->Write();
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
    
   bool strangetof = picoEvent->btofTrayMultiplicity()>250&&picoEvent->refMult()<20;

  hnTofMulvsRef->Fill(picoEvent->btofTrayMultiplicity(),picoEvent->refMult());
  hnTofMatvsRef->Fill(picoEvent->nBTOFMatch(),picoEvent->refMult());  
   if (strangetof) return kStOK;
    StThreeVectorF pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    hevtcut->Fill(1);
    float bField=picoEvent->bField(); 
    // centrality definition
    float weight=1;

    int mCent =  getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight);
    if (mCent<0) return  kStOK;
    hcent->Fill(mCent);
    hcentwg->Fill(mCent,weight);
    int nTracks = mPicoDst->numberOfTracks();
    for (int itrack1=0;itrack1<nTracks;itrack1++){
      StPicoTrack* pe_1 = mPicoDst->track(itrack1);
      bool isprimary = pe_1->isPrimary();
      // if (!isprimary) continue;
      bool goodtrack = isGoodTrack(pe_1,pe_1->gDCA(mVx,mVy,mVz));
      if (!goodtrack) continue;
      // StThreeVectorF mom = pe_1->pMom(); //primary momentum
      StThreeVectorF mom = pe_1->gMom(); //momentum
      float beta = getTofBeta(pe_1);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //fill pid
      if (tofmatch) hinvBetavsP->Fill(mom.mag(),1./beta);
      hnSigEvsP->Fill(mom.mag(),pe_1->nSigmaElectron());

      float nSigPE1 = pe_1->nSigmaElectron();
      // bool istofelectron = isTOFElectron(pe_1,tofmatch,beta);  //tof at pt<1.5 and no pid at high pt 
      bool istofelectron = tofmatch?fabs(1./beta-1.)<0.025:false;  //tof at pt<1.5 and no pid at high pt 
      bool istofkaon = tofmatch? isTofKaon(pe_1,beta):false;
      bool istofproton = tofmatch? isTofProton(pe_1,beta):false;
      bool istofpi = tofmatch? isTofPion(pe_1,beta):false;
      bool isPE1 = isPEfirstElectron(pe_1,tofmatch,beta);
      bool iselectron = isElectron(pe_1,tofmatch,beta);  //after pid cut

      //Fill histograms 
      if (istofpi &&  fabs(pe_1->nSigmaPion())<anaCuts::nSigmaPi_tofsmp) hnSigE_pi->Fill(mom.mag(),nSigPE1); 
      if (istofkaon &&  fabs(pe_1->nSigmaKaon())<anaCuts::nSigmaK_tofsmp) hnSigE_k->Fill(mom.mag(),nSigPE1); 
      if (istofproton && fabs(pe_1->nSigmaProton())< anaCuts::nSigmaP_tofsmp) hnSigE_p->Fill(mom.mag(),nSigPE1);
      if (iselectron) {
        hnSigE_Cut->Fill(mom.mag(),nSigPE1); 
        hePtvsP->Fill(mom.perp(),pe_1->gMom().mag());
        hincEptCent->Fill(pe_1->gMom().perp(),mCent);
      }
      if (istofelectron) hnSigE_tof->Fill(mom.mag(),nSigPE1);

      if (isPE1){
        for (int itrack2=0;itrack2<nTracks;itrack2++){
          if (itrack1==itrack2) continue;
          StPicoTrack* pe_2 = mPicoDst->track(itrack2);
          if (!isGoodTrack(pe_2,pe_2->gDCA(mVx,mVy,mVz))) continue;
          //apply only tof pid cut
          float beta2 = getTofBeta(pe_2);
          bool tofmatch2 = (beta2!=std::numeric_limits<float>::quiet_NaN()) && beta2>0;
          if (!isPEsecondElectron(pe_2,tofmatch2,beta2)) continue; 

          // use straight lines approximation to get point of DCA of e+e- pair
          StThreeVectorF const p1Mom = pe_1->gMom();
          StThreeVectorF const p2Mom = pe_2->gMom(); //global mom is at the dca to pvtx of the helix
          StPhysicalHelixD p1Helix = pe_1->helix(bField);
          StPhysicalHelixD p2Helix = pe_2->helix(bField);
          StPhysicalHelixD const p1Line(p1Mom, pe_1->origin(),bField*kilogauss , pe_1->charge()); //B
          StPhysicalHelixD const p2Line(p2Mom, pe_2->origin(),bField*kilogauss , pe_2->charge());

          pair<double, double> const ss = p1Line.pathLengths(p2Line);
          StThreeVectorF const p1AtDca2p2 = p1Line.at(ss.first);
          StThreeVectorF const p2AtDca2p1 = p2Line.at(ss.second);

          // calculate DCA of pion to kaon at their DCA
          float DcaDaughters = (p1AtDca2p2 - p2AtDca2p1).mag();

          // calculate Lorentz vector of e+e- pair
          StThreeVectorF const p1MomAtDca = p1Helix.momentumAt(ss.first, bField * kilogauss);
          StThreeVectorF const p2MomAtDca = p2Helix.momentumAt(ss.second, bField * kilogauss);

          StLorentzVector<float> p2FourMom(p2MomAtDca,p2MomAtDca.massHypothesis(M_ELECTRON));
          StLorentzVector<float> p1FourMom(p1MomAtDca,p1MomAtDca.massHypothesis(M_ELECTRON));
          StLorentzVector<float> mother = p2FourMom+p1FourMom;
          // calculate pointing angle and decay length
          StThreeVectorF const V0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5;
          // StThreeVectorF const vtxToV0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5 - pVtx;
          // float PointingAngle = vtxToV0.angle(mother.vect());  //theta
          // float DecayLength = vtxToV0.mag();

          bool passPEtopocut = fabs(DcaDaughters) < anaCuts::EEdcaDaughter;

          if (passPEtopocut && fabs(mother.rapidity())<1) {
            //reconstruct the photon
            bool unlike=pe_2->charge()*pe_1->charge()<0;
            if (mother.m()<0.3){
              if (mother.m()<0.01 && unlike) {
                hnSigE_e->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron());
                hinvBeta_e->Fill(pe_2->gMom().mag(),1./beta2);
                hV0->Fill(V0.x(),V0.y(),V0.z());
              }
              if ( mother.m()<0.01&& (!unlike)){
                hnSigE_e_LS->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron());
                hinvBeta_e_LS->Fill(pe_2->gMom().mag(),1./beta2);
                hV0_LS->Fill(V0.x(),V0.y(),V0.z());
              }
              if (unlike) {
                hphoto->Fill(mother.m(),pe_2->gMom().perp(), mCent); 
                hphotoVsPt->Fill(mother.m(),mother.perp(),mCent);
              }
              else {
                hphoto_LS->Fill(mother.m(), pe_2->gMom().perp(),mCent); 
                hphotoVsPt_LS->Fill(mother.m(), mother.perp(),mCent);
              }
            }
          } // pass topo cut
        } //second pe loop
      } // if is PE first 
    } //loop first track
  } //Good Event
  return kStOK;
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
  bool pileup = (Tofmult*0.3405+48) < refmult;
  return (!pileup) && fabs(pVtx.z()) < anaCuts::vz &&  fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
        fabs(pVtx.z()) < anaCuts::Verror) && sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}
float StPicoElectronMaker::getTofBeta(StPicoTrack const* const trk) const
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
bool StPicoElectronMaker::isPEfirstElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  // first photonic electron
  bool isTPCElectron=0;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
 if (trk->gMom().mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-1.2;
  else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(trk->gMom().mag()*2-2.8);
  if (trk->gMom().mag()<1.5) return isTPCElectron && isTOFElectron; 
  else {
    if (tofmatch) return isTPCElectron && isTOFElectron ;
    else return isTPCElectron;
  }
}

bool StPicoElectronMaker::isPEsecondElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  //same as tof electron 
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron= fabs(trk->nSigmaElectron())<anaCuts::nESigma_partner;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.04:false; 
  if (mom.mag()<1.5) return isTOFElectron&&isTPCElectron;
  else {
    if (tofmatch) return isTOFElectron&&isTPCElectron;
    else return isTPCElectron;
  }
}
bool StPicoElectronMaker::isElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{
  //Add TPC PID cut
  //tof PID is just used to supress the hadron contamination
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron=0;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
 if (mom.mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-1.2;
  else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(mom.mag()*2-2.8);
  if (mom.mag()<1.5) {
    if (isTOFElectron && isTPCElectron) return true;
    else return false;
  }
  else {
    if (tofmatch) return isTPCElectron && isTOFElectron;
    else return isTPCElectron;
  }
}
bool StPicoElectronMaker::isTofElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
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

bool StPicoElectronMaker::isTofKaon(StPicoTrack const * const trk, float beta) const
{
  //choose kaon sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_k = ptot / sqrt(ptot * ptot + M_KAON_PLUS * M_KAON_PLUS);
  return fabs(1 / beta - 1 / beta_k) < anaCuts::kTof ? true : false;
}
bool StPicoElectronMaker::isTofPion(StPicoTrack const * const trk, float beta) const
{
  //choose pion sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
  return fabs(1 / beta - 1 / beta_pi) < anaCuts::piTof ? true : false;
}
bool StPicoElectronMaker::isTofProton(StPicoTrack const * const trk,  float beta) const
{
  //choose proton sample from tof
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_p = ptot / sqrt(ptot * ptot + M_PROTON * M_PROTON);
  return fabs(1 / beta - 1 / beta_p) < anaCuts::pTof ? true : false;
}
int StPicoElectronMaker::getCentralityBin(float vz,int runId,float mult,float &weight) const
{
  // float mult_corr = mult;
  //if trigger 580001, correct the refmult
  if (runId<18155050) {
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
  float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  if (x>89) return 1;
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
}
