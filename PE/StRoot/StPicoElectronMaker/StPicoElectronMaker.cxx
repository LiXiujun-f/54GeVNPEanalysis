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
#include "TRandom.h"
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
  hnSigEvsP = new TH3F("hnSigEvsP","all tracks;p(GeV/c);nSigE;Centrality", 480,0,6,500,-10,15,9,-0.5,8.5);  //all tracks  //avoid the precision
  hnSigE_e = new TH3F("hnSigE_e","photonic elecron;p(GeV/c);nSigE;Centrality", 480,0,6,100 ,-5.01,5,9,-0.5,8.5);  //select PE e
  // hnSigE_e = new TH2F("hnSigE_e","photonic elecron;p(GeV/c);nSigE", 480,0,6,100 ,-5.01,5);  //select PE e
  hndedx_e = new TH2F("hndedx_e","e with tof pid;nSigE;ndEdx", 100,-5,5,35,15,50);  //select tof p
  // hnSigE_e_Dz = new TH2F("hnSigE_e_Dz","Dalitz elecron;p(GeV/c);nSigE", 480,0,6,100,-5.01,5);  //select PE e
  // hnSigE_e_LS = new TH2F("hnSigE_e_ls","photonic elecron like sign;p(GeV/c);nSigE", 480, 0,6, 100,-5.01,5);
  hnSigE_e_LS = new TH3F("hnSigE_e_ls","photonic elecron like sign;p(GeV/c);nSigE;Centrality", 480, 0,6, 100,-5.01,5,9,-0.5,8.5);
  // hnSigE_e_LS_Dz = new TH2F("hnSigE_e_ls_Dz","Dalitz elecron like sign;p(GeV/c);nSigE", 480, 0,6, 100,-5.01,5);
  hinvBeta_e = new TH2F("hinvBeta_e","photonic elecron;p(GeV/c);nSigE", 480 ,0,6, 100,0.9,1.1);  //select PE e
  hinvBeta_e_LS = new TH2F("hinvBeta_e_ls","photonic elecron;p(GeV/c);nSigE", 480 ,0,6, 100,-0.9,1.1);  //select PE e
  // hnSigE_p = new TH2F("hnSigE_p","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 180,-10,35);  //select tof p
  hnSigE_p = new TH3F("hnSigE_p","proton with tof pid;p(GeV/c);nSigE;Centraity", 480 ,0,6, 180,-10,35,9,-0.5,8.5);  //select tof p
  // hndedx_p = new TH2F("hndedx_p","proton with tof pid;nSigE;ndEdx", 180,-10,35,35,15,50);  //select tof p
  // hnSigE_p_wd = new TH2F("hnSigE_p_wd","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 180,-10,35);  //select tof p
  // hnSigE_p_tof = new TH2F("hnSigE_p_tof","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 180,-10,35);  //select tof p
  // hnSigE_p_sb = new TH2F("hnSigE_p_sb","proton with tof pid;p(GeV/c);nSigE", 480 ,0,6, 180,-10,35);  //select tof p
  // hnSigE_pi = new TH2F("hnSigE_pi","pion with tof pid;p(GeV/c);nSigE", 480 ,0,6, 150,-10,5);  //tof pi
  hnSigE_pi = new TH3F("hnSigE_pi","pion with tof pid;p(GeV/c);nSigE;Centraity", 480 ,0,6, 150,-10,5,9,-0.5,8.5);  //tof pi
  // hndedx_pi = new TH2F("hndedx_pi","pion with tof pid;nSigE;ndEdx", 150,-10,5,35,15,50);  //select tof p
  // hnSigE_piMg = new TH2F("hnSigE_piMg","merged pion with tof pid;p(GeV/c);nSigE", 480 ,0,6,180,-10,35);  //select tof p
  hnSigE_piMg = new TH3F("hnSigE_piMg","merged pion with tof pid;p(GeV/c);nSigE;Centrality", 480 ,0,6,180,-10,35,9,-0.5,8.5);  //select tof p
  // hndedx_piMg = new TH2F("hndedx_piMg","merged pion with tof pid;nSigE;ndEdx", 180,-10,35,35,15,50);  //select tof p
  // hnSigE_k = new TH2F("hnSigE_k","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6,70 ,-10,25);
  hnSigE_k = new TH3F("hnSigE_k","kaon with tof pid;p(GeV/c);nSigE;Centrality", 480 ,0,6,70 ,-10,25,9,-0.5,8.5);
  // hndedx_k = new TH2F("hndedx_k","kaon with tof pid;nSigE;ndEdx", 70,-10,25,35,15,50);  //select tof p
  // hnSigE_k_sb = new TH2F("hnSigE_k_sb","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6,70 ,-10,25);
  // hnSigE_k_tof = new TH2F("hnSigE_k_tof","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6,70,-10,25);
  // hnSigE_k_wd = new TH2F("hnSigE_k_wd","kaon with tof pid;p(GeV/c);nSigE", 480 ,0,6,70,-10,25);
  // hnSigE_Cut = new TH2F("hnSigE_Cut","elecron after pid cut;p(GeV/c);nSigE", 480 ,0,6, 100,-5,5);
  hnSigE_Cut = new TH3F("hnSigE_Cut","elecron after pid cut;p(GeV/c);nSigE;Centrality", 480 ,0,6, 100,-5,5,9,-0.5,8.5);
  // hnSigE_tof = new TH2F("hnSigE_tof","elecron with tof only;p(GeV/c);nSigE", 480,0,6, 100,-10,15);
  hnSigE_tof = new TH3F("hnSigE_tof","elecron with tof only;p(GeV/c);nSigE;Centraity", 480,0,6, 100,-10,15,9,-0.5,8.5);
  // hndedx_tof = new TH2F("hndedx_tof","only with tof pid;nSigE;ndEdx", 100,-10,15,35,15,50);  //select tof p
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
  // hndedx_p->Write();
  // hnSigE_p_sb->Write();
  // hnSigE_p_tof->Write();
  // hnSigE_p_wd->Write();
  hnSigE_pi->Write();  //tof pi
  hnSigE_piMg->Write();
  // hndedx_piMg->Write();
  hnSigE_k->Write();
  // hndedx_k->Write();
  // hnSigE_k_sb->Write();
  // hnSigE_k_wd->Write();
  // hnSigE_k_tof->Write();
  hnSigE_Cut->Write();
  hnSigE_tof->Write();
  // hndedx_tof->Write();
  hePtvsP->Write();
  hphoto_LS->Write();
  hphoto->Write();
  hphotoVsPt->Write();
  hphotoVsPt_LS->Write();
  hinvBeta_e->Write();
  hinvBeta_e_LS->Write();
  hnSigE_e->Write();  //select PE e
  // hnSigE_e_Dz->Write();  //select PE e
  hnSigE_e_LS->Write();
  // hnSigE_e_LS_Dz->Write();
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

    int mCent = getCentralityBin(mVz,mRunId,picoEvent->refMult(),weight);
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
      StThreeVectorF mom = pe_1->pMom(); //primary momentum
      // StThreeVectorF mom = pe_1->gMom(); //momentum
      float beta = getTofBeta(pe_1);
      float beta_k = mom.mag() / sqrt(mom.mag() * mom.mag() + M_KAON_PLUS * M_KAON_PLUS);
      float beta_p = mom.mag() / sqrt(mom.mag() * mom.mag() + M_PROTON* M_PROTON);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //fill pid
      if (tofmatch) hinvBetavsP->Fill(mom.mag(),1./beta);
      hnSigEvsP->Fill(mom.mag(),pe_1->nSigmaElectron(),mCent);

      // Float_t m2 = mom.mag()*mom.mag()/ beta/beta;
      Float_t nSigPE1 = pe_1->nSigmaElectron();
      // bool istofelectron = isTOFElectron(pe_1,tofmatch,beta);  //tof at pt<1.5 and no pid at high pt 
      bool istofelectron = tofmatch?fabs(1./beta-1.)<0.025:false;  
      bool istofkaon = tofmatch?isTofKaon(pe_1,beta):false;
      // bool istofkaonWD = tofmatch?(1./beta < 1./beta_k+0.005 &&1./beta>1./beta_k-0.005):false;
      // bool istofkaonSB1 = tofmatch?(1./beta > 1./beta_k+0.004 &&1./beta<1./beta_k+0.005):false;
      // bool istofkaonSB2 = tofmatch?(1./beta < 1./beta_k-0.004 &&1./beta>1./beta_k-0.005):false;
      bool istofproton = tofmatch? isTofProton(pe_1,beta):false;
      // bool istofprotonWD = tofmatch?(1./beta > 1./beta_p-0.005 &&1./beta<1./beta_p+0.005):false;
      // bool istofprotonSB1 = tofmatch?(1./beta > 1./beta_p+0.004 &&1./beta<1./beta_p+0.005):false;
      // bool istofprotonSB2 = tofmatch?(1./beta < 1./beta_p-0.004 &&1./beta>1./beta_p-0.005):false;
      bool istofpi = tofmatch? isTofPion(pe_1,beta):false;
      bool isPE1 = isPEfirstElectron(pe_1,tofmatch,beta);
      bool iselectron = isElectron(pe_1,tofmatch,beta);  //after pid cut

      //Fill histograms 
      if (istofpi &&  fabs(pe_1->nSigmaPion())<anaCuts::nSigmaPi_tofsmp) hnSigE_pi->Fill(mom.mag(),nSigPE1,mCent); 
      if (istofkaon &&  fabs(pe_1->nSigmaKaon())<anaCuts::nSigmaK_tofsmp) {
        hnSigE_k->Fill(mom.mag(),nSigPE1,mCent); 
        // hndedx_k->Fill(mom.mag(),pe_1->nHitsDedx());
      }
      if (istofproton && fabs(pe_1->nSigmaProton())< anaCuts::nSigmaP_tofsmp) {
        hnSigE_p->Fill(mom.mag(),nSigPE1,mCent);
        // hndedx_p->Fill(mom.mag(),pe_1->nHitsDedx());
      }
      // if (istofkaon ) hnSigE_k_tof->Fill(mom.mag(),nSigPE1); 
      // if (istofkaonWD ) hnSigE_k_wd->Fill(mom.mag(),nSigPE1); 
      // if (istofproton ) hnSigE_p_tof->Fill(mom.mag(),nSigPE1);
      // if (istofprotonWD ) hnSigE_p_wd->Fill(mom.mag(),nSigPE1);
      // if (istofprotonSB1 || istofprotonSB2 ) hnSigE_p_sb->Fill(mom.mag(),nSigPE1);
      // if (istofkaonSB1 || istofkaonSB2 ) hnSigE_k_sb->Fill(mom.mag(),nSigPE1);
      if (iselectron) {
        hnSigE_Cut->Fill(mom.mag(),nSigPE1,mCent); 
        hePtvsP->Fill(mom.perp(),pe_1->gMom().mag());
        hincEptCent->Fill(pe_1->gMom().perp(),mCent);
      }
      if (istofelectron) {
        hnSigE_tof->Fill(mom.mag(),nSigPE1,mCent);
        // hndedx_tof->Fill(mom.mag(),pe_1->nHitsDedx());
      }
      if (istofpi && pe_1->nSigmaPion()>5 ) {
        hnSigE_piMg->Fill(mom.mag(),nSigPE1,mCent);
        // hndedx_piMg->Fill(mom.mag(),pe_1->nHitsDedx());
      }
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
          StPhysicalHelixD p1Helix = pe_1->helix(bField);
          StPhysicalHelixD p2Helix = pe_2->helix(bField);
          StThreeVectorF const p1Mom = pe_1->gMom();
          StThreeVectorF const p2Mom = pe_2->gMom(); //global mom is at the dca to pvtx of the helix
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
          StThreeVectorF const vtxToV0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5 - pVtx;
          // float PointingAngle = vtxToV0.angle(mother.vect());  //theta
          // float DecayLength = vtxToV0.mag();

          bool passPEtopocut = fabs(DcaDaughters) < anaCuts::EEdcaDaughter;

          if (passPEtopocut && fabs(mother.rapidity())<1) {
            //reconstruct the photon
            bool unlike=pe_2->charge()*pe_1->charge()<0;
            if (mother.m()<0.3){
              if (mother.m()<0.1 && unlike) {
                hnSigE_e->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron(),mCent);
                // hnSigE_e->Fill(pe_1->pMom().mag(),pe_1->nSigmaElectron());
                if (vtxToV0.perp()<2.5)
                {
                  // hnSigE_e_Dz->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron());
                  // hnSigE_e_Dz->Fill(pe_1->pMom().mag(),pe_1->nSigmaElectron());
                }
                hinvBeta_e->Fill(pe_2->gMom().mag(),1./beta2);
                hV0->Fill(V0.x(),V0.y(),V0.z());
              }
              if ( mother.m()<0.1 && (!unlike)){
                hnSigE_e_LS->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron(),mCent);
                // hnSigE_e_LS->Fill(pe_1->pMom().mag(),pe_1->nSigmaElectron());
                if (vtxToV0.perp()<2.5) {
                  // hnSigE_e_LS_Dz->Fill(pe_2->gMom().mag(),pe_2->nSigmaElectron());
                  // hnSigE_e_LS_Dz->Fill(pe_1->pMom().mag(),pe_1->nSigmaElectron());
                }
                hinvBeta_e_LS->Fill(pe_2->gMom().mag(),1./beta2);
                hV0_LS->Fill(V0.x(),V0.y(),V0.z());
              }
              if (unlike) {
                // hphoto->Fill(mother.m(),pe_2->gMom().perp(), mCent); 
                hphoto->Fill(mother.m(),pe_1->pMom().perp(), mCent); 
                hphotoVsPt->Fill(mother.m(),mother.perp(),mCent);
              }
              else {
                // hphoto_LS->Fill(mother.m(), pe_2->gMom().perp(),mCent); 
                hphoto_LS->Fill(mother.m(), pe_1->pMom().perp(),mCent); 
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
bool StPicoElectronMaker::isPEfirstElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  // first photonic electron
  // bool isTPCElectron=0;
  bool isTPCElectron= fabs(trk->nSigmaElectron())<anaCuts::nESigma_partner;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
  // if (trk->gMom().mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-1.2;
  // else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(trk->gMom().mag()*2-2.8);
  return isTPCElectron && isTOFElectron;
  // if (trk->gMom().mag()<1.5) return isTPCElectron && isTOFElectron; 
  // else {
  //   if (tofmatch) return isTPCElectron && isTOFElectron ;
  //   else return isTPCElectron;
  // }
}

bool StPicoElectronMaker::isPEsecondElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  //same as tof electron 
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron= fabs(trk->nSigmaElectron())<anaCuts::nESigma_partner;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
  return  isTPCElectron; //donot require electron
  // if (mom.mag()<1.5) return isTOFElectron&&isTPCElectron;
  // else {
  //   if (tofmatch) return isTOFElectron&&isTPCElectron;
  //   else return isTPCElectron;
  // }
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
  return isTPCElectron && isTOFElectron;
}
bool StPicoElectronMaker::isBemcElectron(StPicoTrack const * const trk) const 
{
  int Id = trk->bemcPidTraitsIndex();
  bool isbemcE = false;
  // if (Id>=0){
  //   StPicoBEmcPidTraits* bemctrait = mPicoDst->bemcPidTraits(Id);
  //   if (bemctrait){
  //     float E0 = bemctrait->bemcE0();
  //     float p = trk->gMom().mag();
  //     float bemcDphi = bemctrait->bemcPhiDist(); 
  //     float bemcdz = bemctrait->bemcZdist(); 
  //     // float bsmdEtaDphi = 
  //     float btowEtaDz = bemctrait->btowEtaDist(); 
  //     // float bsmdPhiDphi = 
  //     float btowPhiDz = bemctrait->btowPhiDist();
  //     float nSMDeta = bemctrait->bemcSmdNEta();
  //     float nSMDphi = bemctrait->bemcSmdNPhi();
  //     isbemcE = nSMDphi>1 && nSMDeta>1 && btowPhiDz<16 && btowEtaDz<10 
  //               && bemcdz<10 && bemcDphi<0.05
  //               && p/E0>0.3 &&p/E0<2;
  //   }
  // }
  return isbemcE;
}
bool StPicoElectronMaker::isTofElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  StThreeVectorF mom = trk->gMom(); 
  bool isTPCElectron=0;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
  return isTOFElectron;
  // if (mom.perp()<1.5) return isTOFElectron;
  // else {
  //   if (tofmatch) return isTOFElectron;
  //   else return true;
  // }
}

bool StPicoElectronMaker::isTofKaon(StPicoTrack const * const trk, float beta) const
{
  //choose kaon sample from tof
  StThreeVectorF mom = trk->gMom(); 
  Float_t p2 = mom.mag()*mom.mag();
  Float_t m2 = p2/beta/beta - p2;
  return m2<0.243+0.005 && m2>0.243-0.005;
  // double ptot = mom.mag();
  // float beta_k = ptot / sqrt(ptot * ptot + M_KAON_PLUS * M_KAON_PLUS);
  // return fabs(1 / beta - 1 / beta_k) < anaCuts::kTof ? true : false;
}
bool StPicoElectronMaker::isTofPion(StPicoTrack const * const trk, float beta) const
{
  //choose pion sample from tof
  StThreeVectorF mom = trk->gMom(); 
  // double ptot = mom.mag();
  // float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
  // return fabs(1 / beta - 1 / beta_pi) < anaCuts::piTof ? true : false;
  Float_t p2 = mom.mag()*mom.mag();
  Float_t m2 = p2/beta/beta - p2;
  return m2<0.019+0.003  &&  m2>0.019-0.003; 
}
bool StPicoElectronMaker::isTofProton(StPicoTrack const * const trk,  float beta) const
{
  //choose proton sample from tof
  StThreeVectorF mom = trk->gMom(); 
  // double ptot = mom.mag();
  // float beta_p = ptot / sqrt(ptot * ptot + M_PROTON * M_PROTON);
  // return fabs(1 / beta - 1 / beta_p) < anaCuts::pTof ? true : false;
  // StThreeVectorF mom = trk->gMom(); 
  Float_t p2 = mom.mag()*mom.mag();
  Float_t m2 = p2/beta/beta - p2;
  return m2<0.879+0.02 && m2>0.879-0.02;

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
  weight = 1./reweight(mult);
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
