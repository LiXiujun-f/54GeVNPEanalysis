#include "StBtoDAnalysis.h"

//#include "StRoot/StMyUtilities/StMyUtilities.h"
#include "StRoot/StRefMultCorr/StRefMultCorr.h"
#include "StRoot/StRefMultCorr/CentralityMaker.h"
#include "StEvent/StDcaGeometry.h"

#include "StRoot/StPicoEvent/StPicoArrays.h"
#include "StRoot/StPicoEvent/StPicoBTofHit.h"
#include "StRoot/StPicoEvent/StPicoBTofPidTraits.h"
#include "StRoot/StPicoEvent/StPicoBTowHit.h"
//#include "StRoot/StPicoDstMaker/StPicoConstants.h"
//#include "StRoot/StPicoDstMaker/StPicoCut.h"
#include "StRoot/StPicoEvent/StPicoDst.h"
#include "StRoot/StPicoDstMaker/StPicoDstMaker.h"
#include "StRoot/StPicoEvent/StPicoBEmcPidTraits.h"
#include "StRoot/StPicoEvent/StPicoEmcTrigger.h"
#include "StRoot/StPicoEvent/StPicoEvent.h"
#include "StRoot/StPicoEvent/StPicoMtdHit.h"
#include "StRoot/StPicoEvent/StPicoMtdPidTraits.h"
#include "StRoot/StPicoEvent/StPicoMtdTrigger.h"
#include "StRoot/StPicoEvent/StPicoTrack.h"
#include "StRoot/StPicoEvent/StPicoUtilities.h"

#include "PhysicalConstants.h"
#include "phys_constants.h"
#include "StLorentzVector.hh"
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH2I.h"
#include "TFile.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TProfile2D.h"
#include "TMath.h"
#include <algorithm>

#include "TTree.h"
#include "StBTofUtil/tofPathLength.hh"
#include "TRandom3.h"
#include "StPhysicalHelixD.hh"

#include "anaCuts.h"
#define DEBUG 0

ClassImp(StBtoDAnalysis)

StRefMultCorr* StBtoDAnalysis::mRefMultCorr = NULL;

//-----------------------------------------------------------------------------
StBtoDAnalysis::StBtoDAnalysis(const char* name, StPicoDstMaker *picoMaker, const char* outName)
   : StMaker(name)
{
   mPicoDstMaker = picoMaker;
   mPicoDst = 0;
   mOutName = outName;
   mUseHist = kTRUE;
   // mUseTree = kFALSE;
   mUseTree = kTRUE;
   cout << "M_PION_PLUS: " <<  M_PION_PLUS << endl;
}

//-----------------------------------------------------------------------------
StBtoDAnalysis::~StBtoDAnalysis()
{
   /*  */
}

//-----------------------------------------------------------------------------
Int_t StBtoDAnalysis::Init()
{
   PI = TMath::Pi();
   twoPI = PI * 2.;
   massPi = 0.13957;
   massK  = 0.493677;
   massP = 0.93827;
   massPhi = 1.019461;
   massK0S = 0.497614;
   massLambda0 = 1.11568;
   if (mUseTree) initTree();
   if (mUseHist) DeclareHistograms();
   if (!mRefMultCorr) mRefMultCorr = CentralityMaker::instance()->getgRefMultCorr_P16id() ;
   mRefMultCorr->setVzForWeight(6, -6.0, 6.0);
   mRefMultCorr->readScaleForWeight("StRoot/StRefMultCorr/macros/weight_grefmult_VpdnoVtx_Vpd5_Run16.txt");
   nEvents = 0;
   return kStOK;
}
//-----------------------------------------------------------------------------
void StBtoDAnalysis::initTree()
{
   mTree = new TTree("mTreeDst", "v0 tree");
   mTree->Branch("mCentrality", &v0Tree.mCentrality, "mCentrality/I");
   mTree->Branch("mRunId", &v0Tree.mRunId, "mRunId/I");
   mTree->Branch("mEvtId", &v0Tree.mEvtId, "mEvtId/I");
   mTree->Branch("mVx", &v0Tree.mVx, "mVx/F");
   mTree->Branch("mVy", &v0Tree.mVy, "mVy/F");
   mTree->Branch("mVz", &v0Tree.mVz, "mVz/F");
   //mTree->Branch("mRefMult",&v0Tree.mRefMult,"mRefMult/S");
   mTree->Branch("mGRefMult", &v0Tree.mGRefMult, "mGRefMult/S");
   mTree->Branch("mWeight", &v0Tree.mWeight, "mWeight/F");
   mTree->Branch("mNV0", &v0Tree.mNV0, "mNV0/I");
   mTree->Branch("mFlag", v0Tree.mFlag, "mFlag[mNV0]/b");
   mTree->Branch("mDca1", v0Tree.mDca1, "mDca1[mNV0]/F");
   mTree->Branch("mDca2", v0Tree.mDca2, "mDca2[mNV0]/F");
   mTree->Branch("mPt1", v0Tree.mPt1, "mPt1[mNV0]/F");
   mTree->Branch("mPt2", v0Tree.mPt2, "mPt2[mNV0]/F");
   mTree->Branch("mEta1", v0Tree.mEta1, "mEta1[mNV0]/F");
   mTree->Branch("mEta2", v0Tree.mEta2, "mEta2[mNV0]/F");
   mTree->Branch("mNSigma1", v0Tree.mNSigma1, "mNSigma1[mNV0]/F");
   mTree->Branch("mNSigma2", v0Tree.mNSigma2, "mNSigma2[mNV0]/F");
   //mTree->Branch("mTof1",v0Tree.mTof1,"mTof1[mNV0]/I");
   //mTree->Branch("mTof2",v0Tree.mTof2,"mTof2[mNV0]/I");
   mTree->Branch("mDiffInvBeta1", v0Tree.mDiffInvBeta1, "mDiffInvBeta1[mNV0]/F");
   mTree->Branch("mDiffInvBeta2", v0Tree.mDiffInvBeta2, "mDiffInvBeta2[mNV0]/F");
   //mTree->Branch("mBeta1",v0Tree.mBeta1,"mBeta1[mNV0]/F");
   //mTree->Branch("mBeta2",v0Tree.mBeta2,"mBeta2[mNV0]/F");
   mTree->Branch("mDcaDaughters", v0Tree.mDcaDaughters, "mDcaDaughters[mNV0]/F");
   mTree->Branch("mV0x", v0Tree.mV0x, "mV0x[mNV0]/F");
   mTree->Branch("mV0y", v0Tree.mV0y, "mV0y[mNV0]/F");
   mTree->Branch("mV0z", v0Tree.mV0z, "mV0z[mNV0]/F");
   mTree->Branch("mPt", v0Tree.mPt, "mPt[mNV0]/F");
   mTree->Branch("mEta", v0Tree.mEta, "mEta[mNV0]/F");
   mTree->Branch("mPhi", v0Tree.mPhi, "mPhi[mNV0]/F");
   mTree->Branch("mMass", v0Tree.mMass, "mMass[mNV0]/F");
   mTree->Branch("mCosTheta", v0Tree.mCosTheta, "mCosTheta[mNV0]/F");
   mTree->Branch("mDca2Vtx", v0Tree.mDca2Vtx, "mDca2Vtx[mNV0]/F");
   mTree->Branch("mDecayL", v0Tree.mDecayL, "mDecayL[mNV0]/F");
   mTree->Branch("mDcaXY", v0Tree.mDcaXY, "mDcaXY[mNV0]/F");
   mTree->Branch("mDcaZ", v0Tree.mDcaZ, "mDcaZ[mNV0]/F");
}
//----------------------------------------------------------------------------
void StBtoDAnalysis::writeTree()
{
   mTree->Write();
}
//----------------------------------------------------------------------------
void StBtoDAnalysis::clearTree()
{
   v0Tree.mCentrality = -1;
   v0Tree.mRunId = -1;
   v0Tree.mEvtId = -1;
   v0Tree.mVx = -100;
   v0Tree.mVy = -100;
   v0Tree.mVz = -100;
   v0Tree.mRefMult = -1;
   v0Tree.mGRefMult = -1;
   v0Tree.mNV0 = 0;
#if 0
   v0Tree.mFlag = -1;
   v0Tree.mDca1 = -10;
   v0Tree.mDca2 = -10;
   v0Tree.mPt1 = -1;
   v0Tree.mPt2 = -1;
   v0Tree.mNSigma1 = -100;
   v0Tree.mNSigma2 = -100;
   v0Tree.mNsigmaTof1 = -100;
   v0Tree.mNsigmaTof2 = -100;
   v0Tree.mBeta1 = -1;
   v0Tree.mBeta2 = -1;
   v0Tree.mDcaDaughters = -1;
   v0Tree.mV0x = -100;
   v0Tree.mV0y = -100;
   v0Tree.mV0z = -100;
   v0Tree.mPt = -1;
   v0Tree.mEta = -10;
   v0Tree.mPhi = -10;
   v0Tree.mMass = -1;
   v0Tree.mTheta = -1000;
   v0Tree.mDca2Vtx = -1;
   v0Tree.mDecayL = -1;
#endif
}
//-----------------------------------------------------------------------------
Int_t StBtoDAnalysis::Finish()
{
   if (mOutName != "")
   {
      TFile *fout = new TFile(mOutName.Data(), "RECREATE");
      fout->cd();
      if (mUseTree)
      {
         cout << "Write tree ..." << endl;
         writeTree();
         cout << "End write tree ..." << endl;
      }
      if (mUseHist)
      {
         cout << "Write histograph ..." << endl;
         WriteHistograms();
         cout << "End write histograph ..." << endl;
      }
      cout << "close root file ..." << endl;
      fout->Close();
      cout << "end close root file ..." << endl;
   }
   cout << "delete histograph ..." << endl;
   if (mUseHist) DeleteHistograms();
   cout << "end delete histograph ..." << endl;
   cout << "Total events processed in StBtoDAnalysis : " << nEvents << endl;
   return kStOK;
}

//-----------------------------------------------------------------------------
void StBtoDAnalysis::DeclareHistograms()
{
   TH1::SetDefaultSumw2();
   hCentrality = new TH1F("hCentrality", "hCentrality", 10, 0, 10);
   hCentrality->SetDirectory(0);
   hNPar1VsNPar2 = new TH2I("hNPar1VsNPar2", "hNPar1VsNPar2", 150, 0, 150, 150, 0, 150);
   hNPar1VsNPar2->SetDirectory(0);
   hgRefMult = new TH1F("hgRefMult", "hgRefMult", 650, 0, 650);
   hgRefMult->SetDirectory(0);
   hgRefMultWg = new TH1F("hgRefMultWg", "hgRefMultWg", 650, 0, 650);
   hgRefMultWg->SetDirectory(0);

   h2pTCent_tpcPi = new TH2F("h2pTCent_tpcPi", "h2pTCent_tpcPi", 200, 0, 10, 10, 0, 10);
   h2pTCent_tpcPi->SetDirectory(0);
   h2pTCent_tofPi = new TH2F("h2pTCent_tofPi", "h2pTCent_tofPi", 200, 0, 10, 10, 0, 10);
   h2pTCent_tofPi->SetDirectory(0);
   h2pTCent_tpcPi2 = new TH2F("h2pTCent_tpcPi2", "h2pTCent_tpcPi2", 200, 0, 10, 10, 0, 10);
   h2pTCent_tpcPi2->SetDirectory(0);
   h2pTCent_tofPi2 = new TH2F("h2pTCent_tofPi2", "h2pTCent_tofPi2", 200, 0, 10, 10, 0, 10);
   h2pTCent_tofPi2->SetDirectory(0);

   hmassUL = new TH1F("hmassUL", "hmassUL", 200, 0.4, 0.6);
   hmassUL->SetDirectory(0);
   hmassLS = new TH1F("hmassLS", "hmassLS", 200, 0.4, 0.6);
   hmassLS->SetDirectory(0);
   hmass = new TH1F("hmass", "hmass", 200, 0.4, 0.6);
   hmass->SetDirectory(0);

   h2massUL = new TH2F("h2massUL", "h2massUL", 200, 0.4, 0.6, 100, 0, 10);
   h2massUL->SetDirectory(0);
   h2massLS = new TH2F("h2massLS", "h2massLS", 200, 0.4, 0.6, 100, 0, 10);
   h2massLS->SetDirectory(0);
   h2mass = new TH2F("h2mass", "h2mass", 200, 0.4, 0.6, 100, 0, 10);
   h2mass->SetDirectory(0);

   h2NsigmaVsP_clean = new TH2F("h2NsigmaVsP_clean", "h2NsigmaVsP_clean", 200, 0, 10, 100, -10, 10);
   h2NsigmaVsP_clean->SetDirectory(0);
   h2NsigmaVsP_cleanUL = new TH2F("h2NsigmaVsP_cleanUL", "h2NsigmaVsP_cleanUL", 200, 0, 10, 100, -10, 10);
   h2NsigmaVsP_cleanUL->SetDirectory(0);
   h2NsigmaVsP_cleanLS = new TH2F("h2NsigmaVsP_cleanLS", "h2NsigmaVsP_cleanLS", 200, 0, 10, 100, -10, 10);
   h2NsigmaVsP_cleanLS->SetDirectory(0);
   h2NsigmaVsP_cleanSB = new TH2F("h2NsigmaVsP_cleanSB", "h2NsigmaVsP_cleanSB", 200, 0, 10, 100, -10, 10);
   h2NsigmaVsP_cleanSB->SetDirectory(0);

   h2InvBetaVsP_clean = new TH2F("h2InvBetaVsP_clean", "h2InvBetaVsP_clean", 200, 0, 10, 300, -0.1, 0.2);
   h2InvBetaVsP_clean->SetDirectory(0);
   h2InvBetaVsP_cleanUL = new TH2F("h2InvBetaVsP_cleanUL", "h2InvBetaVsP_cleanUL", 200, 0, 10, 300, -0.1, 0.2);
   h2InvBetaVsP_cleanUL->SetDirectory(0);
   h2InvBetaVsP_cleanLS = new TH2F("h2InvBetaVsP_cleanLS", "h2InvBetaVsP_cleanLS", 200, 0, 10, 300, -0.1, 0.2);
   h2InvBetaVsP_cleanLS->SetDirectory(0);
   h2InvBetaVsP_cleanSB = new TH2F("h2InvBetaVsP_cleanSB", "h2InvBetaVsP_cleanSB", 200, 0, 10, 300, -0.1, 0.2);
   h2InvBetaVsP_cleanSB->SetDirectory(0);

   hNsigmaVsPCentUL = new TH3F("hNsigmaVsPCentUL", "hNsigmaVsPCentUL", 400, 0, 4, 10, 0, 10, 50, -5, 5);
   hNsigmaVsPCentUL->SetDirectory(0);
   hNsigmaVsPCentLS = new TH3F("hNsigmaVsPCentLS", "hNsigmaVsPCentLS", 400, 0, 4, 10, 0, 10, 50, -5, 5);
   hNsigmaVsPCentLS->SetDirectory(0);
   hNsigmaVsPCentSB = new TH3F("hNsigmaVsPCentSB", "hNsigmaVsPCentSB", 400, 0, 4, 10, 0, 10, 50, -5, 5);
   hNsigmaVsPCentSB->SetDirectory(0);
   hNsigmaVsPCent = new TH3F("hNsigmaVsPCent", "hNsigmaVsPCent", 400, 0, 4, 10, 0, 10, 50, -5, 5);
   hNsigmaVsPCent->SetDirectory(0);

   hpT_tofMatchUL = new TH1F("hpT_tofMatchUL", "hpT_tofMatchUL", 200, 0, 10);
   hpT_tofMatchUL->SetDirectory(0);
   hpT_tofMatchLS = new TH1F("hpT_tofMatchLS", "hpT_tofMatchLS", 200, 0, 10);
   hpT_tofMatchLS->SetDirectory(0);
   hpT_tofMatch = new TH1F("hpT_tofMatch", "hpT_tofMatch", 200, 0, 10);
   hpT_tofMatch->SetDirectory(0);
   hpT_tofMatchCutUL = new TH1F("hpT_tofMatchCutUL", "hpT_tofMatchCutUL", 200, 0, 10);
   hpT_tofMatchCutUL->SetDirectory(0);
   hpT_tofMatchCutLS = new TH1F("hpT_tofMatchCutLS", "hpT_tofMatchCutLS", 200, 0, 10);
   hpT_tofMatchCutLS->SetDirectory(0);
   hpT_tofMatchCutSB = new TH1F("hpT_tofMatchCutSB", "hpT_tofMatchCutSB", 200, 0, 10);
   hpT_tofMatchCutSB->SetDirectory(0);
   hpT_tofMatchCut = new TH1F("hpT_tofMatchCut", "hpT_tofMatchCut", 200, 0, 10);
   hpT_tofMatchCut->SetDirectory(0);
   hpTCent_tofMatchUL = new TH2F("hpTCent_tofMatchUL", "hpTCent_tofMatchUL", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchUL->SetDirectory(0);
   hpTCent_tofMatchLS = new TH2F("hpTCent_tofMatchLS", "hpTCent_tofMatchLS", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchLS->SetDirectory(0);
   hpTCent_tofMatch = new TH2F("hpTCent_tofMatch", "hpTCent_tofMatch", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatch->SetDirectory(0);
   hpTCent_tofMatchCutUL = new TH2F("hpTCent_tofMatchCutUL", "hpTCent_tofMatchCutUL", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchCutUL->SetDirectory(0);
   hpTCent_tofMatchCutLS = new TH2F("hpTCent_tofMatchCutLS", "hpTCent_tofMatchCutLS", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchCutLS->SetDirectory(0);
   hpTCent_tofMatchCutSB = new TH2F("hpTCent_tofMatchCutSB", "hpTCent_tofMatchCutSB", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchCutSB->SetDirectory(0);
   hpTCent_tofMatchCut = new TH2F("hpTCent_tofMatchCut", "hpTCent_tofMatchCut", 200, 0, 10, 10, 0, 10);
   hpTCent_tofMatchCut->SetDirectory(0);

   hInvBetaVsPCentUL = new TH3F("hInvBetaVsPCentUL", "hInvBetaVsPCentUL", 400, 0, 4, 10, 0, 10, 300, -0.1, 0.2);
   hInvBetaVsPCentUL->SetDirectory(0);
   hInvBetaVsPCentLS = new TH3F("hInvBetaVsPCentLS", "hInvBetaVsPCentLS", 400, 0, 4, 10, 0, 10, 300, -0.1, 0.2);
   hInvBetaVsPCentLS->SetDirectory(0);
   hInvBetaVsPCentSB = new TH3F("hInvBetaVsPCentSB", "hInvBetaVsPCentSB", 400, 0, 4, 10, 0, 10, 300, -0.1, 0.2);
   hInvBetaVsPCentSB->SetDirectory(0);
   hInvBetaVsPCent = new TH3F("hInvBetaVsPCent", "hInvBetaVsPCent", 400, 0, 4, 10, 0, 10, 300, -0.1, 0.2);
   hInvBetaVsPCent->SetDirectory(0);
}

//-----------------------------------------------------------------------------
void StBtoDAnalysis::WriteHistograms()
{
   hCentrality->Write();
   hNPar1VsNPar2->Write();
   hgRefMult->Write();
   hgRefMultWg->Write();

   h2pTCent_tpcPi->Write();
   h2pTCent_tofPi->Write();
   h2pTCent_tpcPi2->Write();
   h2pTCent_tofPi2->Write();

   hmassUL->Write();
   hmassLS->Write();
   hmass->Add(hmassUL, hmassLS, 1, -1);
   hmass->Write();

   h2massUL->Write();
   h2massLS->Write();
   h2mass->Add(h2massUL, h2massLS, 1, -1);
   h2mass->Write();

   h2NsigmaVsP_clean->Sumw2();
   // h2NsigmaVsP_clean->Add(h2NsigmaVsP_cleanUL, h2NsigmaVsP_cleanLS, 1, -1);
   h2NsigmaVsP_clean->Add(h2NsigmaVsP_cleanUL, h2NsigmaVsP_cleanSB, 1, -1);
   h2NsigmaVsP_clean->Write();
   h2NsigmaVsP_cleanUL->Write();
   h2NsigmaVsP_cleanLS->Write();

   h2InvBetaVsP_clean->Sumw2();
   // h2InvBetaVsP_clean->Add(h2InvBetaVsP_cleanUL, h2InvBetaVsP_cleanLS, 1, -1);
   h2InvBetaVsP_clean->Add(h2InvBetaVsP_cleanUL, h2InvBetaVsP_cleanSB, 1, -1);
   h2InvBetaVsP_clean->Write();
   h2InvBetaVsP_cleanUL->Write();
   h2InvBetaVsP_cleanLS->Write();
   h2InvBetaVsP_cleanSB->Write();

   hNsigmaVsPCentUL->Write();
   hNsigmaVsPCentLS->Write();
   hNsigmaVsPCentSB->Write();
   // hNsigmaVsPCent->Add(hNsigmaVsPCentUL, hNsigmaVsPCentLS, 1, -1);
   hNsigmaVsPCent->Add(hNsigmaVsPCentUL, hNsigmaVsPCentSB, 1, -1);
   hNsigmaVsPCent->Write();

   hpT_tofMatchUL->Write();
   hpT_tofMatchLS->Write();
   hpT_tofMatch->Add(hpT_tofMatchUL, hpT_tofMatchLS, 1, -1);
   hpT_tofMatch->Write();
   hpT_tofMatchCutUL->Write();
   hpT_tofMatchCutLS->Write();
   hpT_tofMatchCutSB->Write();
   // hpT_tofMatchCut->Add(hpT_tofMatchCutUL, hpT_tofMatchCutLS, 1, -1);
   hpT_tofMatchCut->Add(hpT_tofMatchCutUL, hpT_tofMatchCutSB, 1, -1);
   hpT_tofMatchCut->Write();
   hpTCent_tofMatchUL->Write();
   hpTCent_tofMatchLS->Write();
   hpTCent_tofMatch->Add(hpTCent_tofMatchUL, hpTCent_tofMatchLS, 1, -1);
   hpTCent_tofMatch->Write();
   hpTCent_tofMatchCutUL->Write();
   hpTCent_tofMatchCutLS->Write();
   hpTCent_tofMatchCutSB->Write();
   // hpTCent_tofMatchCut->Add(hpTCent_tofMatchCutUL, hpTCent_tofMatchCutLS, 1, -1);
   hpTCent_tofMatchCut->Add(hpTCent_tofMatchCutUL, hpTCent_tofMatchCutSB, 1, -1);
   hpTCent_tofMatchCut->Write();

   hInvBetaVsPCentUL->Write();
   hInvBetaVsPCentLS->Write();
   hInvBetaVsPCentSB->Write();
   // hInvBetaVsPCent->Add(hInvBetaVsPCentUL, hInvBetaVsPCentLS, 1, -1);
   hInvBetaVsPCent->Add(hInvBetaVsPCentUL, hInvBetaVsPCentSB, 1, -1);
   hInvBetaVsPCent->Write();
   //hInvBetaVsP->Sumw2();
   hInvBetaVsPCent->GetYaxis()->SetRangeUser(0 + 1e-6, 10 - 1e-6);
   hInvBetaVsP = (TH2F*)hInvBetaVsPCent->Project3D("zx");
   hInvBetaVsP->SetDirectory(0);
   hInvBetaVsP->Write("hInvBetaVsP");
}
//----------------------------------------------------------------------------
void StBtoDAnalysis::DeleteHistograms()
{
   if (hCentrality)
   {
      delete hCentrality;
      hCentrality = NULL;
   }
   if (hNPar1VsNPar2)
   {
      delete hNPar1VsNPar2;
      hNPar1VsNPar2 = NULL;
   }
   if (hgRefMult)
   {
      delete hgRefMult;
      hgRefMult = NULL;
   }
   if (hgRefMultWg)
   {
      delete hgRefMultWg;
      hgRefMultWg = NULL;
   }

   if (h2pTCent_tpcPi)
   {
      delete h2pTCent_tpcPi;
      h2pTCent_tpcPi = NULL;
   }
   if (h2pTCent_tofPi)
   {
      delete h2pTCent_tofPi;
      h2pTCent_tofPi = NULL;
   }
   if (h2pTCent_tpcPi2)
   {
      delete h2pTCent_tpcPi2;
      h2pTCent_tpcPi2 = NULL;
   }
   if (h2pTCent_tofPi2)
   {
      delete h2pTCent_tofPi2;
      h2pTCent_tofPi2 = NULL;
   }

   if (hmassUL)
   {
      delete hmassUL;
      hmassUL = NULL;
   }
   if (hmassLS)
   {
      delete hmassLS;
      hmassLS = NULL;
   }

   if (h2massUL)
   {
      delete h2massUL;
      h2massUL = NULL;
   }
   if (h2massLS)
   {
      delete h2massLS;
      h2massLS = NULL;
   }
   if (h2NsigmaVsP_clean)
   {
      delete h2NsigmaVsP_clean;
      h2NsigmaVsP_clean = NULL;
   }
   if (h2NsigmaVsP_cleanUL)
   {
      delete h2NsigmaVsP_cleanUL;
      h2NsigmaVsP_cleanUL = NULL;
   }
   if (h2NsigmaVsP_cleanLS)
   {
      delete h2NsigmaVsP_cleanLS;
      h2NsigmaVsP_cleanLS = NULL;
   }
   if (h2NsigmaVsP_cleanSB)
   {
      delete h2NsigmaVsP_cleanSB;
      h2NsigmaVsP_cleanSB = NULL;
   }

   if (h2InvBetaVsP_clean)
   {
      delete h2InvBetaVsP_clean;
      h2InvBetaVsP_clean = NULL;
   }
   if (h2InvBetaVsP_cleanUL)
   {
      delete h2InvBetaVsP_cleanUL;
      h2InvBetaVsP_cleanUL = NULL;
   }
   if (h2InvBetaVsP_cleanLS)
   {
      delete h2InvBetaVsP_cleanLS;
      h2InvBetaVsP_cleanLS = NULL;
   }
   if (h2InvBetaVsP_cleanSB)
   {
      delete h2InvBetaVsP_cleanSB;
      h2InvBetaVsP_cleanSB = NULL;
   }

   if (hNsigmaVsPCentUL)
   {
      delete hNsigmaVsPCentUL;
      hNsigmaVsPCentUL = NULL;
   }
   if (hNsigmaVsPCentLS)
   {
      delete hNsigmaVsPCentLS;
      hNsigmaVsPCentLS = NULL;
   }
   if (hNsigmaVsPCentSB)
   {
      delete hNsigmaVsPCentSB;
      hNsigmaVsPCentSB = NULL;
   }
   if (hNsigmaVsPCent)
   {
      delete hNsigmaVsPCent;
      hNsigmaVsPCent = NULL;
   }

   if (hpT_tofMatchUL)
   {
      delete hpT_tofMatchUL;
      hpT_tofMatchUL = NULL;
   }
   if (hpT_tofMatchLS)
   {
      delete hpT_tofMatchLS;
      hpT_tofMatchLS = NULL;
   }
   if (hpT_tofMatch)
   {
      delete hpT_tofMatch;
      hpT_tofMatch = NULL;
   }
   if (hpT_tofMatchCutUL)
   {
      delete hpT_tofMatchCutUL;
      hpT_tofMatchCutUL = NULL;
   }
   if (hpT_tofMatchCutLS)
   {
      delete hpT_tofMatchCutLS;
      hpT_tofMatchCutLS = NULL;
   }
   if (hpT_tofMatchCutSB)
   {
      delete hpT_tofMatchCutSB;
      hpT_tofMatchCutSB = NULL;
   }
   if (hpT_tofMatchCut)
   {
      delete hpT_tofMatchCut;
      hpT_tofMatchCut = NULL;
   }
   if (hpTCent_tofMatchUL)
   {
      delete hpTCent_tofMatchUL;
      hpTCent_tofMatchUL = NULL;
   }
   if (hpTCent_tofMatchLS)
   {
      delete hpTCent_tofMatchLS;
      hpTCent_tofMatchLS = NULL;
   }
   if (hpTCent_tofMatch)
   {
      delete hpTCent_tofMatch;
      hpTCent_tofMatch = NULL;
   }
   if (hpTCent_tofMatchCutUL)
   {
      delete hpTCent_tofMatchCutUL;
      hpTCent_tofMatchCutUL = NULL;
   }
   if (hpTCent_tofMatchCutLS)
   {
      delete hpTCent_tofMatchCutLS;
      hpTCent_tofMatchCutLS = NULL;
   }
   if (hpTCent_tofMatchCutSB)
   {
      delete hpTCent_tofMatchCutSB;
      hpTCent_tofMatchCutSB = NULL;
   }
   if (hpTCent_tofMatchCut)
   {
      delete hpTCent_tofMatchCut;
      hpTCent_tofMatchCut = NULL;
   }

   if (hInvBetaVsPCentUL)
   {
      delete hInvBetaVsPCentUL;
      hInvBetaVsPCentUL = NULL;
   }
   if (hInvBetaVsPCentLS)
   {
      delete hInvBetaVsPCentLS;
      hInvBetaVsPCentLS = NULL;
   }
   if (hInvBetaVsPCentSB)
   {
      delete hInvBetaVsPCentSB;
      hInvBetaVsPCentSB = NULL;
   }
   if (hInvBetaVsPCent)
   {
      delete hInvBetaVsPCent;
      hInvBetaVsPCent = NULL;
   }
   if (hInvBetaVsP)
   {
      delete hInvBetaVsP;
      hInvBetaVsP = NULL;
   }
}
//-----------------------------------------------------------------------------
void StBtoDAnalysis::Clear(Option_t *opt)
{
   runnumberPointer = -999;
   centrality       = -999;
   mPicoDst         = NULL;
   mEvent           = NULL;
   mTrack           = NULL;
   mTrack1          = NULL;
}
//-----------------------------------------------------------------------------
Int_t StBtoDAnalysis::passEvent()
{
   mEvent = (StPicoEvent*) mPicoDst->event();
   if (!mEvent) {
	   if (DEBUG) cout << "cannot read event"<<endl;
	   return  -1;
   }
   B = mEvent->bField();
   int runId = mEvent->runId();
   //mUtilities->setInfo(mEvent->runId(), mEvent->triggerIds()); //
   
 //  runnumberPointer = mUtilities->CheckrunNumber();

   nEvents++;
   //trigger
   bool isTrigger = false;
   for (auto trg : anaCuts::triggers)
   {
      if (mEvent->isTrigger(trg)) {
		  isTrigger = true;
		  break;
	  }
   }
   if (!isTrigger){
	  if (DEBUG)  {
		cout<<"it is not my trigger, id: ";
		for (int i=0;i<mEvent->triggerIds().size();i++){
		  cout<<mEvent->triggerIds()[i]<<" ";}
		cout<<endl;
	  }
	  return -1;
   }
   else {
	  if (DEBUG) cout<<"find trigger"<<endl;
   }
//  if (runnumberPointer < 0) return -1;
//   if (!mUtilities->isMyEvent())
//   {
//      if (DEBUG) cout << "Not my event" << endl;
//      return -1;
//   }

   if (mRefMultCorr->isBadRun((Int_t)mEvent->runId()))
   {
      if (DEBUG) cout << "Not my event rejected by RefMultCorr" << endl;
      return -1;
   }
//-------------bad run----------------------------------------//
   if (mEvent->runId()<17062047) return -1;
   if (mEvent->runId()>=17063041 && mEvent->runId()<=17063043) return -1;
   if (mEvent->runId()>=17065002 && mEvent->runId()<=17068053) return -1;
   
   if (DEBUG) cout << "Find my event" << endl;
   mRefMultCorr->init((Int_t)mEvent->runId());
   //float mEnergy = mUtilities->getEnergy();
   Vertex = mEvent->primaryVertex();
   float VertexXPos = Vertex.x();
   float VertexYPos = Vertex.y();
   float VertexZPos = Vertex.z();
   float vpdVz = mEvent->vzVpd();
   float Vr = sqrt(pow(VertexXPos, 2.) + pow(VertexYPos, 2.));
   float ZDCx = mEvent->ZDCx();
   if (fabs(VertexXPos) < 1e-5 && fabs(VertexYPos) < 1e-5 && fabs(VertexZPos) < 1e-5) return -1;
   if (fabs(VertexZPos) > 6)   return -1;
   if (Vr > 2) return -1;
   if (fabs(VertexZPos - vpdVz) > 3) return -1;

   // grefMult is used for Run16 AuAu 200GeV centrality definition
   int refMult = mEvent->grefMult();
   mRefMultCorr->initEvent(refMult, VertexZPos, mEvent->ZDCx());
   float refMultc = mRefMultCorr->getRefMultCorr();
   gweight  = mRefMultCorr->getWeight();
   centrality = mRefMultCorr->getCentralityBin9() + 1; // StRefMultCorr return cent 0~8
   if (centrality <= 0 || centrality > 9) {
	if (DEBUG) cout<<centrality<<endl;
	return -1;
   }
   
   if (mUseHist)
   {
      hCentrality->Fill(centrality, gweight);
      hgRefMult->Fill(refMultc);
      hgRefMultWg->Fill(refMultc, gweight);
   }
   if (mUseTree)
   {
      v0Tree.mCentrality = centrality;
      v0Tree.mRunId = runId;
      v0Tree.mEvtId = mEvent->eventId();
      v0Tree.mVx = VertexXPos;
      v0Tree.mVy = VertexYPos;
      v0Tree.mVz = VertexZPos;
      v0Tree.mRefMult = mEvent->refMult();
      v0Tree.mGRefMult = mEvent->grefMult();
      v0Tree.mWeight = gweight;
      v0Tree.mVzVpd = vpdVz;
   }
   return 0;
}
//-----------------------------------------------------------------------------
float StBtoDAnalysis::getDca(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex)
{
   StThreeVectorF posDiff = pos - vertex;
   return fabs(p.cross(posDiff.cross(p)).unit().dot(posDiff));
}
//-----------------------------------------------------------------------------
float StBtoDAnalysis::getDcaXY(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex)
{
   StThreeVectorF newPos(pos);
   newPos.setZ(0);

   StThreeVectorF newP(p);
   newP.setZ(0);

   StThreeVectorF newVertex(vertex);
   newVertex.setZ(0);

   StThreeVectorF posDiff = newPos - newVertex;
   float sign = posDiff.x() * p.y() - posDiff.y() * p.x() > 0 ? +1 : -1;
   return sign * newP.cross(posDiff.cross(newP)).unit().dot(posDiff);
}

//-----------------------------------------------------------------------------
float StBtoDAnalysis::getDcaZ(StThreeVectorF const& p, StThreeVectorF const& pos, StThreeVectorF const& vertex)
{
   StThreeVectorF posDiff = pos - vertex;
   if (sin(p.theta()) == 0) return 0;
   else return (-posDiff.x() * cos(p.phi()) - posDiff.y() * sin(p.phi())) * cos(p.theta()) / sin(p.theta()) + posDiff.z();
}
//-----------------------------------------------------------------------------
int StBtoDAnalysis::getD0PtIndex(float const pt)
{
   int bin = -1;
   for (int i = 0; i < anaCuts::nPtBins; i++)
   {
      if ((pt >= anaCuts::PtEdge[i]) && (pt < anaCuts::PtEdge[i + 1]))
         bin = i;
   }
   return bin;
}
//-----------------------------------------------------------------------------
int StBtoDAnalysis::getMeaSigPtIndex(float const pt)
{
   int bin = -1;
   for (int i = 0; i < anaCuts::nPtBinsSig; i++)
   {
      if ((pt >= anaCuts::PtEdgeSig[i]) && (pt < anaCuts::PtEdgeSig[i + 1]))
         bin = i;
   }
   return bin;
}
//-----------------------------------------------------------------------------
Int_t StBtoDAnalysis::loopTrack()
{
   nPar1 = 0;
   nPar2 = 0;
   int nTr = mPicoDst->numberOfTracks();
   for (int i = 0; i < nTr; i++)
   {
      mTrack = (StPicoTrack*) mPicoDst->track(i);
      if (!mTrack) continue;
      int q = mTrack->charge();
      if (abs(q) != 1)continue;
      bool isHFT = mTrack->isHFTTrack();
      if (!isHFT) continue;

      // gDca and Global mom
//      StDcaGeometry *dcaG = new StDcaGeometry();
//      dcaG->set(mTrack->params(), mTrack->errMatrix());
//      THelixTrack     thelix =  dcaG->thelix();
//      Double_t vtx[3] = {Vertex.x(), Vertex.y(), Vertex.z()};
      //use Path to move to the nearest point 
//      thelix.Move(thelix.Path(vtx)); // Move(); move along helix, to the DCA point

      //const Double_t *pos = thelix.Pos(); // position
      //const Double_t *dir = thelix.Dir(); // direction
    //  StThreeVectorF dcaGlobal = StThreeVectorF(pos[0], pos[1], pos[2]) - Vertex;
    const Double_t pos[3] = {mTrack->origin().x(), mTrack->origin().y(), mTrack->origin().z()};
    StThreeVectorD dcaGlobal = mTrack->dca();

    //  StThreeVectorF gmom = StThreeVectorF(dir[0], dir[1], dir[2]);
   //   double ptot = dcaG->momentum().mag();
     // gmom *= ptot; //get momentum vector;-->momentum at DCA point --> Global momentum
      StThreeVectorD gmom = mTrack->gMom();
      StPhysicalHelixD helix = mTrack->helix( mEvent->bField());
      double gdca = helix.geometricSignedDistance(Vertex); // helix to vtx DCA
      if (gdca < 0) gdca = fabs(gdca);
      double dcaXy = helix.geometricSignedDistance(Vertex.x(), Vertex.y());
      double dcaZ = dcaGlobal.z();
 //     delete dcaG;
      StThreeVectorD pmom = mTrack->pMom();

      float p = gmom.mag();
      float pt = gmom.perp();
      float eta = gmom.pseudoRapidity();
      double phi = gmom.phi();
      double phi0 = phi; // range 0~2PI
      if (phi0 < 0) phi0 += twoPI;
      int nHitsFit = (int) fabs(mTrack->nHitsFit());
      int nHitsMax = (int) fabs(mTrack->nHitsMax());
      if (nHitsMax == 0) continue;
      float nratio = 0;
      nratio = (1.0 * nHitsFit) / (1.0 * nHitsMax);
      int nHitsdEdx = (int) fabs(mTrack->nHitsDedx());
      float dedx = mTrack->dEdx();
      float nsigmae = mTrack->nSigmaElectron();
      float nsigmapi = mTrack->nSigmaPion();
      float nsigmak = mTrack->nSigmaKaon();
      float nsigmap = mTrack->nSigmaProton();
//----------------------------------------------//
      bool isGoodTrack = (pt > 0.1
                          && fabs(eta) < 1
                          && nHitsFit >= 20
                          && gdca < 3.5
                          && gdca > 0.1
                         );
      if (!isGoodTrack) continue;
//----------------------------------------------//
      bool isGoodTrack1 = (pt > 0.2
            && fabs(eta) < 1
            && nHitsFit >= 20
            && gdca < 1.5
            && gdca > 0.1
           );
//---------------------------------------------//
      // re-calculate tof beta
      Int_t   btofMatchFlag =  0;
      Float_t btofYLocal    =  -999;
      int index2tof = mTrack->bTofPidTraitsIndex();
      float beta = -1.;
      if (index2tof >= 0)
      { 
         StPicoBTofPidTraits *tofPid = mPicoDst->btofPidTraits(index2tof);
         btofMatchFlag = tofPid->btofMatchFlag();
         btofYLocal    = tofPid->btofYLocal();
         if (tofPid)
         {
            beta = tofPid->btofBeta();
            if (beta < 1e-4)
            {
               StThreeVectorF btofHitPos = tofPid->btofHitPos();

               float L = tofPathLength(&Vertex, &btofHitPos, helix.curvature());
               float tof = tofPid->btof();
               if (tof > 0) beta = L / (tof * (C_C_LIGHT / 1.e9));
               else beta = -1;
            }
         }
      }

      bool isGoodTof = btofMatchFlag > 0 && beta > 0 ;
      // bool isGoodTof = btofMatchFlag > 0 && beta > 0 && fabs(btofYLocal) < 1.8;

      float diffBetaPi = -1;
      float diffBetaPi2 = -1;
      float IvaBetaTpcpi = -999;
      float IvaBetaTpcpi2 = -999;
      if (isGoodTof)
      {
         IvaBetaTpcpi = sqrt(massPi * massPi / p / p + 1);
         IvaBetaTpcpi2 = sqrt(massPi * massPi / p / p + 1);
         float IvaBeta = 1. / beta;
         diffBetaPi = IvaBeta - IvaBetaTpcpi;
         diffBetaPi2 = IvaBeta - IvaBetaTpcpi2;
      }

      // bool isTpcPion = fabs(nsigmapi) < 2.0;
      bool isTpcPion = fabs(nsigmapi) < 1.0;
      bool isTpcPion2 = fabs(nsigmapi) < 4.0;
      bool isTofPion = isGoodTof && fabs(diffBetaPi) < 0.02;
      // bool isTofPion = isGoodTof && fabs(diffBetaPi) < 0.03 * IvaBetaTpcpi;
      // bool isTofPion2= isGoodTof && fabs(diffBetaPi2) < 0.03 * IvaBetaTpcpi2;
      // bool isTofPion = isGoodTof && fabs(diffBetaPi) < 0.1 * IvaBetaTpcpi;
      bool isTofPion2 = isGoodTof && fabs(diffBetaPi2) < 0.1;

      bool isPionCandidate = false;
      if (isTpcPion)
      {
         // if(!isGoodTof) {
         //     isPionCandidate = true;
         // }
         // else {
         if (isTofPion)  isPionCandidate = true; 
         // }
      }

      bool isPion2Candidate = false;
      if (isTpcPion2)
      {
         if (!isGoodTof)
         {
            isPion2Candidate = true;
         }
         else
         {
            if (isTofPion2) isPion2Candidate = true;
         }
      }

      bool isClean = pt < 1.6 ? isGoodTof : true;

      //get momentum at dca to vertex
      //StThreeVectorF gmom_atOrigin = mTrack->gMom();
      //StThreeVectorF origin  = mTrack->origin();
      //StPhysicalHelix helix(gmom_atOrigin*GeV,origin*centimeter,B*kilogauss,q);
      //double length = helix.pathLength(Vertex);
      //StThreeVectorF pos = helix.at(length);  // dca position
      //StThreeVectorF gmom = helix.momentumAt(length, B*kilogauss); // momentum at dca
      //================test
      //float openAngle = (pos - Vertex).angle(gmom);
      //float test_dca1 = (pos - Vertex).mag()*TMath::Sin(openAngle);
      //float test_dca2 = helix.geometricSignedDistance(Vertex); // helix to vtx DCA
      //cout << "Track id = " << i << ", dca = " << gdca << "\t" << test_dca1 << "\t" << test_dca2 << endl;
      //================end test

      //target particle
      if (isPionCandidate)
      {
         mcharge1[nPar1] = q;
         gmomx1[nPar1] = gmom.x();
         gmomy1[nPar1] = gmom.y();
         gmomz1[nPar1] = gmom.z();
         moriginx1[nPar1] = pos[0];
         moriginy1[nPar1] = pos[1];
         moriginz1[nPar1] = pos[2];
         mDiffInvBeta1[nPar1] = diffBetaPi;
         mNsigmaX1[nPar1] = nsigmapi;
         //mNdEdxCut1[nPar1] = (nHitsdEdx>=16);
         mTofMatch1[nPar1] = isGoodTof;
         mtrackID1[nPar1] = i;
         mdca1[nPar1] = gdca;
         mClean1[nPar1] = isClean;
         mEta1[nPar1] = eta;
         nPar1++;
      }
      //partner particle
      if (isPion2Candidate)
      {
         // if(pt<0.5) cout<< "check **** " << pt <<endl;
         mcharge2[nPar2] = q;
         gmomx2[nPar2] = gmom.x();
         gmomy2[nPar2] = gmom.y();
         gmomz2[nPar2] = gmom.z();
         moriginx2[nPar2] = pos[0];
         moriginy2[nPar2] = pos[1];
         moriginz2[nPar2] = pos[2];
         mDiffInvBeta2[nPar2] = diffBetaPi2;
         mNsigmaX2[nPar2] = nsigmapi;
         //mNdEdxCut2[nPar2] = (nHitsdEdx>=16);
         mTofMatch2[nPar2] = isGoodTof;
         mtrackID2[nPar2] = i;
         mdca2[nPar2] = gdca;
         mClean2[nPar2] = isClean;
         mEta2[nPar2] = eta;
         nPar2++;
      }
      if (mUseHist)
      {
         //tof match efficiency
         if (fabs(nsigmapi) < 0.3)
         {
            h2pTCent_tpcPi2->Fill(pt, centrality, gweight);
            if (isGoodTof) h2pTCent_tofPi2->Fill(pt, centrality, gweight);
         }
         if (fabs(nsigmapi) < 0.3)
         {
            h2pTCent_tpcPi->Fill(pt, centrality, gweight);
            if (isGoodTof) h2pTCent_tofPi->Fill(pt, centrality, gweight);
         }
         //end tof match efficiency
      }
   }
   return 0;
}
//-----------------------------------------------------------------------------
void StBtoDAnalysis::MakeRealEvent()
{
   //cout << "number of paticle 1: " << nPar1 << endl;
   //cout << "number of paticle 2: " << nPar2 << endl;
   int iV0 = 0;
   for (int i = 0; i < nPar1; i++)
   {
      int q1 = mcharge1[i];
      StThreeVectorD pos1_ori(moriginx1[i], moriginy1[i], moriginz1[i]);
      StThreeVectorD mom1_ori(gmomx1[i], gmomy1[i], gmomz1[i]);
      StPhysicalHelix helix1(mom1_ori * GeV, pos1_ori * centimeter, B * kilogauss, q1);
      float pt1 = mom1_ori.perp();
      float p1 = mom1_ori.mag();
      for (int j = 0; j < nPar2; j++)
      {
         int q2 = mcharge2[j];
         bool mUL = kFALSE;
         if (q1 + q2 == 0) mUL = kTRUE;
         else if (q1 == q2) mUL = kFALSE;
         else continue;
         if (mtrackID1[i] == mtrackID2[j]) continue;
         StThreeVectorD pos2_ori(moriginx2[j], moriginy2[j], moriginz2[j]);
         StThreeVectorD mom2_ori(gmomx2[j], gmomy2[j], gmomz2[j]);
         StPhysicalHelix helix2(mom2_ori * GeV, pos2_ori * centimeter, B * kilogauss, q2);
         float pt2 = mom2_ori.perp();
         float p2 = mom2_ori.mag();
         // if(pt2<0.5) cout<< "check **** " << pt2 <<endl;

         StPhysicalHelix line1(mom1_ori * GeV, pos1_ori * centimeter, 0, q1); //B=0, the momtum of this helix equal to 0
         StPhysicalHelix line2(mom2_ori * GeV, pos2_ori * centimeter, 0, q2); //if momemtum is not at the dca, can't use straight line to aproach the dca betweent two helix

         pairD pair = line1.pathLengths(line2);
         StThreeVectorF pos1 = line1.at(pair.first);
         StThreeVectorF pos2 = line2.at(pair.second);
         float pairDCA = (pos1 - pos2).mag();  /// pair DCA
         //if(pairDCA>=0.01) continue;

         StThreeVectorF V0 = (pos1 + pos2) * 0.5; // secendary vertex
         StThreeVectorF V0ToPv  = V0 - Vertex;
         float decayLength = V0ToPv.mag();  // decay length
         //if(decayLength<=0.008) continue;

         StThreeVectorF mom1 = helix1.momentumAt(pair.first, B * kilogauss);
         StThreeVectorF mom2 = helix2.momentumAt(pair.second, B * kilogauss);
         float E1 = TMath::Sqrt(mom1.mag2() + massPi * massPi);
         float E2 = TMath::Sqrt(mom2.mag2() + massPi * massPi);
         StLorentzVector<Float_t> FourMom1(mom1, E1);
         StLorentzVector<Float_t> FourMom2(mom2, E2);
         StLorentzVector<Float_t> FourMom = FourMom1 + FourMom2;
         StThreeVectorF Mom = mom1 + mom2;
         float mass = FourMom.m();
         float pt = FourMom.perp();
         float y = FourMom.rapidity(); 
         float eta = FourMom.pseudoRapidity();
         float phi = FourMom.phi();
         float openAngle = V0ToPv.angle(Mom);
         float cosTheta = TMath::Cos(openAngle);   //cosTheta
         float DCA = V0ToPv.mag() * TMath::Sin(openAngle); //Dca

         //phi
         if (mUseTree)
         {
            // bool isReco =  1 ;
            // bool isReco = mass > 1.1 && mass < 1.14  ;
            // bool isReco = mass > 1.1 && mass < 1.14 && fabs(cosTheta) > 0.995;
            // bool isReco =  decayLength > 0.5 && decayLength < 25 && DCA < 0.4 && pairDCA < 0.06 && fabs(cosTheta) > 0.999 && mass > 0.4 && mass < 0.6;
            bool isReco =  decayLength > 0.5 && decayLength < 3.5 && DCA < 0.4 && pairDCA < 0.01 && fabs(cosTheta) > 0.995 && mass > 0.4 && mass < 0.6;
            if (isReco)
            {
               if (iV0 >= 999) continue;
               v0Tree.mFlag[iV0] = (UChar_t)mUL;
               v0Tree.mDca1[iV0] = mdca1[i];
               v0Tree.mDca2[iV0] = mdca2[j];
               v0Tree.mPt1[iV0] = pt1;
               v0Tree.mPt2[iV0] = pt2;
               v0Tree.mEta1[iV0] = mEta1[i];
               v0Tree.mEta2[iV0] = mEta2[j];
               v0Tree.mNSigma1[iV0] = mNsigmaX1[i];
               v0Tree.mNSigma2[iV0] = mNsigmaX2[j];
               v0Tree.mDiffInvBeta1[iV0] = mDiffInvBeta1[i];
               v0Tree.mDiffInvBeta2[iV0] = mDiffInvBeta2[j];
               v0Tree.mDcaDaughters[iV0] = pairDCA;
               v0Tree.mV0x[iV0] = V0.x();
               v0Tree.mV0y[iV0] = V0.y();
               v0Tree.mV0z[iV0] = V0.z();
               v0Tree.mPt[iV0] = pt;
               v0Tree.mEta[iV0] = eta;
               v0Tree.mPhi[iV0] = phi;
               v0Tree.mMass[iV0] = mass;
               v0Tree.mCosTheta[iV0] = cosTheta;
               v0Tree.mDca2Vtx[iV0] = DCA;
               v0Tree.mDecayL[iV0] = decayLength;
               v0Tree.mDcaXY[iV0] = getDcaXY(Mom, V0, Vertex);
               v0Tree.mDcaZ[iV0] = getDcaZ(Mom, V0, Vertex);
               iV0++;
            }
         }

         //Lambda0
         if (mUseHist)
         {
            int ptIndex = getD0PtIndex(pt);
            if (ptIndex < 0) continue;

            // bool isReco =  decayLength > 0.5 && decayLength < 25 && DCA < 0.4 && pairDCA < 0.06 && fabs(cosTheta) > 0.999 && mass > 0.4 && mass < 0.6;
            // bool isReco =  decayLength > 2.5 && decayLength < 25 && DCA < 0.4 && pairDCA < 0.01 && fabs(cosTheta) > 0.999 && mass > 0.4 && mass < 0.6;
            bool isReco =  decayLength > 0.5 && decayLength < 3.5 && DCA < 0.4 && pairDCA < 0.01 && fabs(cosTheta) > 0.995 && mass > 0.4 && mass < 0.6;
            if (!isReco) continue;
            if (mUL) hmassUL->Fill(mass);
            else hmassLS->Fill(mass);
            if (mUL) h2massUL->Fill(mass, pt2);
            else h2massLS->Fill(mass, pt2);
            // if (mass < 0.487 && mass > 0.507) continue; // mass cut for Lambda // Wrong
            // if (mass < 0.492 && mass > 0.502) continue; // mass cut for Lambda // Wrong

            bool sideBand = kFALSE;
            if ((mass > 0.477 && mass < 0.482) || (mass > 0.512 && mass < 0.517))   //side band
               sideBand = kTRUE;

            // if (mass < 0.492 || mass > 0.502) continue; // mass cut for Lambda selcection
            bool MassWindow = kFALSE;
            if (mass > 0.492 && mass < 0.502) MassWindow = kTRUE; // mass cut for Lambda selcection

            if (mUL)
            {
               if (MassWindow)
               {
                  h2NsigmaVsP_cleanUL->Fill(pt2, mNsigmaX2[j]);
                  if (mTofMatch2[j]) h2InvBetaVsP_cleanUL->Fill(pt2, mDiffInvBeta2[j]);
               }
               if (sideBand)
               {
                  h2NsigmaVsP_cleanSB->Fill(pt2, mNsigmaX2[j]);
                  if (mTofMatch2[j]) h2InvBetaVsP_cleanSB->Fill(pt2, mDiffInvBeta2[j]);
               }
            }
            else
            {
               if (MassWindow)
               {
                  h2NsigmaVsP_cleanLS->Fill(pt2, mNsigmaX2[j]);
                  if (mTofMatch2[j]) h2InvBetaVsP_cleanLS->Fill(pt2, mDiffInvBeta2[j]);
               }
            }

            //nsigma cut efficiency -- dE/dx
            if (mUL)
            {
               if (MassWindow)
               {
                  hNsigmaVsPCentUL->Fill(pt2, centrality, mNsigmaX2[j], gweight);
               }
               if (sideBand) hNsigmaVsPCentSB->Fill(pt2, centrality, mNsigmaX2[j], gweight);
            }
            else
            {
               if (MassWindow)
               {
                  hNsigmaVsPCentLS->Fill(pt2, centrality, mNsigmaX2[j], gweight);
               }
            }
            if (fabs(mNsigmaX2[j]) > 2.0) continue;

            //TOF matching efficiency
            if (mUL)
            {
               if (MassWindow)
               {
                  hpT_tofMatchUL->Fill(pt2, gweight);
                  hpTCent_tofMatchUL->Fill(pt2, centrality, gweight);
               }
            }
            else
            {
               if (MassWindow)
               {
                  hpT_tofMatchLS->Fill(pt2, gweight);
                  hpTCent_tofMatchLS->Fill(pt2, centrality, gweight);
               }
            }
            if (mTofMatch2[j])
            {
               if (mUL)
               {
                  if (MassWindow)
                  {
                     hpT_tofMatchCutUL->Fill(pt2, gweight);
                     hpTCent_tofMatchCutUL->Fill(pt2, centrality, gweight);
                  }
                  if (sideBand)
                  {
                     hpT_tofMatchCutSB->Fill(pt2, gweight);
                     hpTCent_tofMatchCutSB->Fill(pt2, centrality, gweight);
                  }
               }
               else
               {
                  if (MassWindow)
                  {
                     hpT_tofMatchCutLS->Fill(pt2, gweight);
                     hpTCent_tofMatchCutLS->Fill(pt2, centrality, gweight);
                  }
               }
            }
            if (!mTofMatch2[j]) continue;

            //TOF pid cut efficiency
            if (mUL)
            {
               if (MassWindow)
               {
                  hInvBetaVsPCentUL->Fill(pt2, centrality, mDiffInvBeta2[j], gweight);
               }
               if (sideBand)  hInvBetaVsPCentSB->Fill(pt2, centrality, mDiffInvBeta2[j], gweight);
            }
            else
            {
               if (MassWindow)
               {
                  hInvBetaVsPCentLS->Fill(pt2, centrality, mDiffInvBeta2[j], gweight);
               }
            }

            //default cuts

         }// end mUseHist
      } // end daughter 2
   } // end daughter 1
   //end of reconstruction
   if (mUseTree)
   {
      v0Tree.mNV0 = iV0;
      mTree->Fill();
   }
}
//-----------------------------------------------------------------------------
Int_t StBtoDAnalysis::Make()
{
   if (!mPicoDstMaker)
   {
      LOG_WARN << " No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   mPicoDst = mPicoDstMaker->picoDst();
   if (!mPicoDst)
   {
      LOG_WARN << " No PicoDst! Skip! " << endm;
      return kStWarn;
   }
   if (mUseTree) clearTree();

   if (passEvent()) return kStOK;
   loopTrack();
   if (mUseHist) hNPar1VsNPar2->Fill(nPar1, nPar2);
   MakeRealEvent();

   return kStOK;
}
