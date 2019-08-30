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
#include "TLorentzVector.h"
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
  bookHists(); //if already have the recenter and shift hists
  mWriteTree=true;
  //initialize trees
  initTree();
  // initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoElectronMaker::~StPicoElectronMaker()
{}
//-----------------------------------------------------------------------------
bool StPicoElectronMaker::bookHists()
{
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
  file = TFile::Open(mShiftFile.c_str());
  if (!file->IsOpen()){
    cout<<"Error: Cannot read shift file!!!"<<endl;
    return kStWarn;
  }
  // for (int i=0;i<20;i++)
  // {
    for (int ic=0;ic<9;ic++)
    {
      pSinEtaM[ic] = (TProfile*)file->Get(Form("pSinEtaM_Cent%d",ic));
      pCosEtaM[ic] = (TProfile*)file->Get(Form("pCosEtaM_Cent%d",ic));
      pSinEtaP[ic] = (TProfile*)file->Get(Form("pSinEtaP_Cent%d",ic));
      pCosEtaP[ic] = (TProfile*)file->Get(Form("pCosEtaP_Cent%d",ic));
      pSinEtaM[ic]->SetDirectory(0);
      pCosEtaM[ic]->SetDirectory(0);
      pSinEtaP[ic]->SetDirectory(0);
      pCosEtaP[ic]->SetDirectory(0);
    }
  // }
  file->Close();
  return kStOK;
}
//-----------------------------------------------------------------------------
bool StPicoElectronMaker::initHists(){
  mFile = new TFile(mOutFileBaseName+".hist.root", "RECREATE");
  if (DEBUG)cout<<"ok1"<<endl;

  // mFile->cd();
  // if (DEBUG)cout<<"ok2"<<endl;
  hevt = new TH1D("hevt","hevt",1,0.5,1.5);
  hevtcut = new TH1D("hevtcut","hevtcut",1,0.5,1.5);
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  hrefmult = new TH1F("hrefmult","hrefmult",700,0,700);
  hrefmultcor = new TH1F("hrefmultcor","hrefmultcor",700,0,700);
  float const PI = TMath::Pi();
  hEventPlaneCent_M = new TH2F("hEventPlaneCent_M","hEventPlaneCent;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_M_Re = new TH2F("hEventPlaneCent_M_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_M_Sh = new TH2F("hEventPlaneCent_M_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_P = new TH2F("hEventPlaneCent_P","hEventPlaneCent;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_P_Re = new TH2F("hEventPlaneCent_P_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  hEventPlaneCent_P_Sh = new TH2F("hEventPlaneCent_P_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI); 
  EPconsRes = new TProfile("EPRes","Event Plane resolution;Cent;Res",9,-0.5,8.5);
  hIncEv2vsPtvsCent = new TH3F("hIncEv2vsPtvsCent","hIncEv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  hIncHadronv2vsPtvsCent = new TH3F("hIncHadronv2vsPtvsCent","hIncHadronv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  pIncEv2 = new TProfile2D("pIncEv2","pIncEv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pIncEv2_hitcut = new TProfile2D("pIncEv2_hitcut","pIncEv2_hitcut;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pIncHadronv2 = new TProfile2D("pIncHadronv2","pIncHadronv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pPionMinusv2 = new TProfile2D("pPionMinusv2","pPionMinusv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pPionPlusv2 = new TProfile2D("pPionPlusv2","pPionPlusv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  // hPionv2vsPtvsCent = new TH3F("hPionv2vsPtvsCent","hPionv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,200,0,4,9,-0.5,8.5);
  // hKaonv2vsPtvsCent = new TH3F("hKaonv2vsPtvsCent","hKaonv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,200,0,4,9,-0.5,8.5);
  hPhEv2vsPtvsCent = new TH3F("hPhEv2vsPtvsCent","hPhEv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  hPhEv2vsPtvsCentLS = new TH3F("hPhEv2vsPtvsCentLS","hPhEv2vsPtvsCentLS;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  pTagEv2 = new TProfile2D("pTagEv2","pTagEv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pTagEv2_LS = new TProfile2D("pTagEv2_LS","pTagEv2_LS;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  hphoto = new TH3F("hphoto","Mee;Mee;partner e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphoto_LS = new TH3F("hphoto_LS","Mee like sign vs e pt vs cent;Mee;Tag e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphotoVsPt_LS = new TH3F("hphotoVsPt_LS","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);
  hphotoVsPt = new TH3F("hphotoVsPt","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);

  // prfQxCent = new TProfile("prfQxCent","prfQxCent", 9, -0.5, 8.5);
  // prfQyCent = new TProfile("prfQyCent","prfQyCent", 9, -0.5, 8.5);
  // hQyQxCent = new TH3F("hQyQxCent", "hQyQxCent;Cent;Qx;Qy", 9,-0.5,8.5,1000,-50,50,1000,-50,50);
  return kStOK;
}
//----------------------------------------------------------------------------
bool StPicoElectronMaker::initTree()
{
  mTreeFile = new TFile(mOutFileBaseName+".tree.root", "RECREATE");
  int const BufSize = (int)pow(2.,16);
  int const Split = 1;
  mTreeFile->SetCompressionLevel(1);
  mTree = new TTree("Tree","Tree",BufSize);
  mTree->SetAutoSave(1.e6);
  
  mTree->Branch("runId",&m_runId,"runId/I");
  mTree->Branch("eventId",&m_eventId,"eventId/I");
  mTree->Branch("bfield",&m_bfield,"bfield/F");
  mTree->Branch("centrality",&m_cent,"centrality/I");
  mTree->Branch("weight",&m_weight,"weight/F");
  mTree->Branch("refMult",&m_refMult,"refMult/F");
  mTree->Branch("pVtx_x",&m_pVtx_x,"pVtx_x/F");
  mTree->Branch("pVtx_y",&m_pVtx_y,"pVtx_y/F");
  mTree->Branch("pVtx_z",&m_pVtx_z,"pVtx_z/F");
  mTree->Branch("vzVpdVz",&m_vzVpdVz,"vzVpdVz/F");
  mTree->Branch("BBCx",&m_BBCx,"BBCx/F");
  mTree->Branch("ZDCx",&m_ZDCx,"ZDCx/F");
  mTree->Branch("nTofMult",&m_nTofMult,"nTofMult/F"); //refuse pile up, incase offical centrality
  mTree->Branch("nTofmatch",&m_nTofMat,"nTofmatch/F");
// event plane
  mTree->Branch("nQvecP",&m_nQvec_P,"nQvecP/F");
  mTree->Branch("nQvecM",&m_nQvec_M,"nQvecM/F");
  mTree->Branch("Qx_M",&m_Qx_M,"Qx_M/D");
  mTree->Branch("Qy_M",&m_Qy_M,"Qy_M/D");
  mTree->Branch("Qx_P",&m_Qx_P,"Qx_P/D");
  mTree->Branch("Qy_P",&m_Qy_P,"Qy_P/D");
  mTree->Branch("EP_M_raw",&mEP_M_raw,"EP_M_raw/F");
  mTree->Branch("EP_M_re",&mEP_M_re,"EP_M_re/F");
  mTree->Branch("EP_M_sh",&mEP_M_sh,"EP_M_sh/F");
  mTree->Branch("EP_P_raw",&mEP_P_raw,"EP_P_raw/F");
  mTree->Branch("EP_P_re",&mEP_P_re,"EP_P_re/F");
  mTree->Branch("EP_P_sh",&mEP_P_sh,"EP_P_sh/F");
  mTree->Branch("resolution",&mRes,"resolution/F");
  
  mTree->Branch("nIncE",&m_nIncE,"nIncE/I");
  mTree->Branch("idx_inc",&m_idx_inc,"idx_inc[nIncE]/I");
  mTree->Branch("nSigE_inc",m_nSigmaE,"nSigE_inc[nIncE]/F");
  mTree->Branch("beta_inc",m_beta_inc,"beta_inc[nIncE]/F");
  mTree->Branch("pt_inc",m_pt_inc,"pt_inc[nIncE]/F");
  mTree->Branch("eta_inc",m_eta_inc,"eta_inc[nIncE]/F");
  mTree->Branch("phi_inc",m_phi_inc,"phi_inc[nIncE]/F");
  mTree->Branch("deltaphi_inc",m_deltaphi_inc,"deltaphi_inc[nIncE]/F");
  mTree->Branch("cos2phi_inc",m_cos2phi_inc,"cos2phi_inc[nIncE]/F");
  mTree->Branch("gDca_inc",m_gDca_inc,"gDca_inc[nIncE]/F");
  mTree->Branch("charge_inc",m_charge_inc,"charge_inc[nIncE]/F");
  mTree->Branch("nFit_inc",m_nFits_inc,"nFit_inc[nIncE]/F"); 
  mTree->Branch("ndEdx_inc",m_ndEdx_inc,"ndEdx_inc[nIncE]/F"); 
  mTree->Branch("topomap0_inc",m_topomap0_inc,"topomap0_inc[nIncE]/i"); 
  mTree->Branch("has1hit_inc",m_has1hit_inc,"has1hit_inc[nIncE]/O"); 
  mTree->Branch("has2hit_inc",m_has2hit_inc,"has2hit_inc[nIncE]/O"); 
  mTree->Branch("has3hit_inc",m_has3hit_inc,"has3hit_inc[nIncE]/O"); 

  mTree->Branch("nPhoE",&m_nPhoE,"nPhoE/I");
  mTree->Branch("idx_phe",&m_idx_phe,"idx_phe[nPhoE]/I");
  mTree->Branch("nSigE_phe",m_nSigE_phe,"nSigE_phe[nPhoE]/F");
  mTree->Branch("beta_phe",m_beta_phe,"beta_phe[nPhoE]/F");
  mTree->Branch("pt_phe",m_pt_phe,"pt_phe[nPhoE]/F");
  mTree->Branch("eta_phe",m_eta_phe,"eta_phe[nPhoE]/F");
  mTree->Branch("phi_phe",m_phi_phe,"phi_phe[nPhoE]/F");
  mTree->Branch("gDca_phe",m_gDca_phe,"gDca_phe[nPhoE]/F");
  mTree->Branch("charge_phe",m_charge_phe,"charge_phe[nPhoE]/F");
  mTree->Branch("nFit_phe",m_nFit_phe,"nFit_phe[nPhoE]/F");
  mTree->Branch("ndEdx_phe",m_ndEdx_phe,"ndEdx_phe[nPhoE]/F");
  mTree->Branch("deltaphi_phe",m_deltaphi_phe,"deltaphi_phe[nPhoE]/F");
  mTree->Branch("cos2phi_phe",m_cos2phi_phe,"cos2phi_phe[nPhoE]/F");

  mTree->Branch("topomap0_phe",m_topomap0_phe,"topomap0_phe[nPhoE]/i"); 
  mTree->Branch("idx_parte",&m_idx_parte,"idx_parte[nPhoE]/I");
  mTree->Branch("nSigE_parte",m_nSigE_parte,"nSigE_parte[nPhoE]/F");
  mTree->Branch("beta_parte",m_beta_parte,"beta_parte[nPhoE]/F");
  mTree->Branch("gpt_parte",m_pt_parte,"gpt_parte[nPhoE]/F");
  mTree->Branch("geta_parte",m_eta_parte,"geta_parte[nPhoE]/F");
  mTree->Branch("gphi_parte",m_phi_parte,"gphi_parte[nPhoE]/F");
  mTree->Branch("gDca_parte",m_gDca_parte,"gDca_parte[nPhoE]/F");
  mTree->Branch("charge_parte",m_charge_parte,"charge_parte[nPhoE]/F");
  mTree->Branch("nFit_parte",m_nFit_parte,"nFit_parte[nPhoE]/F");
  mTree->Branch("ndEdx_parte",m_ndEdx_parte,"ndEdx_parte[nPhoE]/F");
  mTree->Branch("topomap0_parte",m_topomap0_parte,"topomap0_parte[nPhoE]/i"); 
  mTree->Branch("DCA_pair",m_DCA_pair,"DCA_pair[nPhoE]/F");
  mTree->Branch("decayL_pair",m_decayL_pair,"decayL_pair[nPhoE]/F");
  mTree->Branch("V0x_pair",m_V0x_pair,"V0x_pair[nPhoE]/F");
  mTree->Branch("V0y_pair",m_V0y_pair,"V0y_pair[nPhoE]/F");
  mTree->Branch("V0z_pair",m_V0z_pair,"V0z_pair[nPhoE]/F");
  mTree->Branch("vtx2V0x_pair",m_vtx2V0x_pair,"vtx2V0x_pair[nPhoE]/F");
  mTree->Branch("vtx2V0y_pair",m_vtx2V0y_pair,"vtx2V0y_pair[nPhoE]/F");
  mTree->Branch("vtx2V0z_pair",m_vtx2V0z_pair,"vtx2V0z_pair[nPhoE]/F");
  mTree->Branch("M_pair",m_M_pair,"M_pair[nPhoE]/F");
  mTree->Branch("px_pair",m_px_pair,"px_pair[nPhoE]/F");
  mTree->Branch("py_pair",m_py_pair,"py_pair[nPhoE]/F");
  mTree->Branch("pz_pair",m_pz_pair,"pz_pair[nPhoE]/F");
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoElectronMaker::Finish()
{
  // if (mWriteHist) WriteHists();
  if (mWriteTree) WriteTree();
  //if book these hists
  // for (int ic=0;ic<9;ic++)
  // {
  //   delete pQxRecenterM[ic];
  //   delete pQyRecenterM[ic];
  //   delete pQxRecenterP[ic]; 
  //   delete pQyRecenterP[ic]; 
  // }
  // for (int ic=0;ic<9;ic++)
  // {
  //   delete pSinEtaM[ic];
  //   delete pCosEtaM[ic];
  //   delete pSinEtaP[ic];
  //   delete pCosEtaP[ic];
  // }
  return kStOK;
}
bool StPicoElectronMaker::WriteHists()
{
  mFile->cd();
  //write the hists
  hevt->Write();
  hevtcut->Write();

  hEventPlaneCent_M->Write();
  hEventPlaneCent_M_Re->Write();
  hEventPlaneCent_M_Sh->Write();
  hEventPlaneCent_P->Write();
  hEventPlaneCent_P_Re->Write();
  hEventPlaneCent_P_Sh->Write();
  EPconsRes->Write();
  // prfQxCent->Write();
  // prfQyCent->Write();
  // hQyQxCent->Write();
  hIncEv2vsPtvsCent->Write();
  hIncHadronv2vsPtvsCent->Write();
  // hPionv2vsPtvsCent->Write();
  // hKaonv2vsPtvsCent->Write();
  hPhEv2vsPtvsCent->Write();
  hPhEv2vsPtvsCentLS->Write();
  pIncHadronv2->Write();
  pPionMinusv2->Write();
  pPionPlusv2->Write();
  pIncEv2->Write();
  pIncEv2_hitcut->Write();
  pTagEv2_LS->Write();
  pTagEv2->Write();
  hcent->Write();
  hcentwg->Write();
  hrefmult->Write();
  hrefmultcor->Write();
  hphotoVsPt->Write();
  hphotoVsPt_LS->Write();
  hphoto->Write();
  hphoto_LS->Write();
  mFile->Close();
  return kStOK;
}
//-----------------------------
bool StPicoElectronMaker::WriteTree()
{
   mTreeFile->cd();
   mTree->Write();
   mTreeFile->Close();
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
  if (DEBUG)cout<<"runid is "<<mRunId<<" "<<runnum[mRunId]<<endl;
  // hevt->Fill(1);
  //event and track level QA
  if (isBadrun(mRunId)) return kStOK;
  
  if (isGoodEvent(picoEvent)){
   bool strangetof = picoEvent->btofTrayMultiplicity()>250&&picoEvent->refMult()<20;
   // hnTofMulvsRef->Fill(picoEvent->btofTrayMultiplicity(),picoEvent->refMult());
   // hnTofMatvsRef->Fill(picoEvent->nBTOFMatch(),picoEvent->refMult());  
   if (strangetof) return kStOK;
   StThreeVectorF pVtx = picoEvent->primaryVertex();
   // TVector3 pVtx = picoEvent->primaryVertex();
   mVx = pVtx.x();
   mVy = pVtx.y();
   mVz = pVtx.z();
   // hevtcut->Fill(1);
   float bField=picoEvent->bField(); 
   // centrality definition
   mWeight=1;

   if (DEBUG)cout<<"ok3"<<endl;
   double refmultcor;
   mCent = getCentralityBin(mVz,mRunId,picoEvent->refMult(),mWeight,refmultcor);
   if (mCent<0) return  kStOK;
   // hcent->Fill(mCent);
   // hcentwg->Fill(mCent,mWeight);
   // hrefmultcor->Fill(refmultcor,mWeight);
   // hrefmult->Fill(picoEvent->refMult(),mWeight);

   //store tree parameter
   m_runId=mRunId;
   m_eventId=picoEvent->eventId();
   m_bfield=bField;
   m_cent=mCent;
   m_weight=mWeight;
   m_refMult=picoEvent->refMult();
   m_pVtx_x=mVx;
   m_pVtx_y=mVy;
   m_pVtx_z=mVz;
   m_vzVpdVz=mVz - picoEvent->vzVpd();
   m_BBCx=picoEvent->BBCx();
   m_ZDCx=picoEvent->ZDCx();
   m_nTofMult=picoEvent->btofTrayMultiplicity(); 
   m_nTofMat=picoEvent->nBTOFMatch();

   // initParameters();
   if (!recenterEventPlane()) return kStOK;  // fill eventplane  info
   getIncElectronv2(bField, pVtx);
   mTree->Fill();
  } //Good Event
  return kStOK;
}
// bool StPicoElectronMaker::fillTreeEventInfo(StPicoEvent* picoEvent)
// {
// }
bool StPicoElectronMaker::recenterEventPlane()
{
  if (DEBUG)cout<<"ok4"<<endl;
  int nTracks = mPicoDst->numberOfTracks();
  double QxEtaM=0., QxEtaP=0.,QyEtaM=0.,QyEtaP=0.;
  double QxEtaM_ori=0., QxEtaP_ori=0.,QyEtaM_ori=0.,QyEtaP_ori=0.;
  int numOfTrksM=0,numOfTrksP=0;
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
    double phi = mom.phi();
    float eta = mom.pseudoRapidity();
    float pt = mom.perp();
    if (pt>anaCuts::ptMax_Re || pt<anaCuts::ptMin_Re) continue; 
    double qx = cos(2*phi)*pt;
    double qy = sin(2*phi)*pt;
    if (eta<-0.05) //eta gap = 0.05 
    {
      QyEtaM_ori+=qy;
      QxEtaM_ori+=qx;
      qx-=pQxRecenterM[mCent]->GetBinContent(runnum[mRunId]+1);
      qy-=pQyRecenterM[mCent]->GetBinContent(runnum[mRunId]+1);
      QxEtaM+=qx;
      QyEtaM+=qy;
      numOfTrksM++;
    }
    else if (eta>0.05)
    {
      QxEtaP_ori+=qx;
      QyEtaP_ori+=qy;
      qx-=pQxRecenterP[mCent]->GetBinContent(runnum[mRunId]+1);
      qy-=pQyRecenterP[mCent]->GetBinContent(runnum[mRunId]+1);
      QxEtaP+=qx;
      QyEtaP+=qy;
      numOfTrksP++;
    } 
  } //loop  track
  if (DEBUG)cout<<"ok5"<<endl;
  if (numOfTrksP==0 && numOfTrksM==0) return 0;
  TVector2 Q_M_ori(QxEtaM_ori,QyEtaM_ori);
  TVector2 Q_P_ori(QxEtaP_ori,QyEtaP_ori);
  TVector2 QEta_M(QxEtaM, QyEtaM);  //minus eta recentered vector
  double  EP_M= QEta_M.Phi()*0.5;
  double EP_M_Sh = getEventPlaneShift(EP_M,-1,mCent,runnum[mRunId]); 
  if (EP_M_Sh<0) EP_M_Sh+=TMath::Pi();
  else if (EP_M_Sh>TMath::Pi()) EP_M_Sh-=TMath::Pi();
  // if(QEta_M.Mod()>0) 
  // {
  //   hEventPlaneCent_M_Re->Fill(mCent, EP_M,mWeight);
  //   hEventPlaneCent_M_Sh->Fill(mCent, EP_M_Sh,mWeight);
  //   hEventPlaneCent_M->Fill(mCent,Q_M_ori.Phi()*0.5,mWeight);
  // }

  TVector2 QEta_P(QxEtaP, QyEtaP);  //plus eta recentered vector
  double  EP_P= QEta_P.Phi()*0.5;
  double EP_P_Sh = getEventPlaneShift(EP_P,1,mCent,runnum[mRunId]); 
  if (DEBUG) cout <<"finish shift" <<endl;
  if (EP_P_Sh<0) EP_P_Sh+=TMath::Pi();
  else if (EP_P_Sh>TMath::Pi()) EP_P_Sh-=TMath::Pi();
  // if(QEta_P.Mod()>0)
  // {
  //   hEventPlaneCent_P_Re->Fill(mCent, EP_P);
  //   hEventPlaneCent_P_Sh->Fill(mCent, EP_P_Sh);
  //   hEventPlaneCent_P->Fill(mCent,Q_P_ori.Phi()*0.5);
  // }
  //event plane resolution
  double resolution = cos(2.*EP_P_Sh-2.*EP_M_Sh);
  // EPconsRes->Fill(mCent,resolution);
  // prfQxCent->Fill(mCent,QxEtaP);
  // prfQyCent->Fill(mCent,QyEtaP);
  // hQyQxCent->Fill(mCent,QxEtaM,QyEtaM);
  mEP_M_Sh=EP_M_Sh;
  mEP_M_Re=EP_M;
  mEP_P_Sh=EP_P_Sh;
  mEP_P_Re=EP_P;
 
  //fill tree
   m_nQvec_M=numOfTrksM;
   m_nQvec_P=numOfTrksP;
   m_Qx_M=QxEtaM_ori;
   m_Qy_M=QyEtaM_ori;
   m_Qx_P=QxEtaP_ori;
   m_Qy_P=QyEtaP_ori;
   mEP_M_raw=Q_M_ori.Phi()*0.5;
   mEP_M_re=EP_M;
   mEP_M_sh=EP_M_Sh;
   mEP_P_raw=Q_P_ori.Phi()*0.5;
   mEP_P_re=EP_P;
   mEP_P_sh=EP_P_Sh;
   mRes = resolution;

  if (DEBUG) cout <<"finish reconstruct EP" <<endl;
  // return kStOK;
  return 1;
}
bool StPicoElectronMaker::getIncElectronv2(float bField,StThreeVectorF pVtx)
{
  //to be added in later on analysis 
  if (DEBUG) cout << "start calculate inc E v2 " <<endl;
  int nTracks = mPicoDst->numberOfTracks();

  m_nIncE=0;
  m_nPhoE=0;
  
  for (int itrack1=0;itrack1<nTracks;itrack1++)
  {
      StPicoTrack* pe_1 = mPicoDst->track(itrack1);
      bool isprimary = pe_1->isPrimary();
      if (!isprimary) continue;
      bool goodtrack = isGoodTrack(pe_1,pe_1->gDCA(mVx,mVy,mVz));
      if (!goodtrack) continue;
      StThreeVectorF mom = pe_1->pMom(); //primary momentum
      // TVector3 mom = pe_1->pMom(); //primary momentum
      // if (mom.Perp()<0.15) continue; 
      // StThreeVectorF mom = pe_1->gMom(); //momentum
      float beta = getTofBeta(pe_1);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //fill pid
      bool isPE1 = isPEfirstElectron(pe_1,tofmatch,beta);
      
      // bool isPE1 = isElectron(pe_1,tofmatch,beta);
      bool iselectron = isElectron(pe_1,tofmatch,beta);  //after pid cut, same as first e
      
      double EP;
      if (pe_1->pMom().pseudoRapidity()>0.0) 
        EP = mEP_M_Sh;
      else if (pe_1->pMom().pseudoRapidity()<-0.0)    
        EP = mEP_P_Sh;
      else continue;
      double phi = pe_1->pMom().phi();
      if (phi<0.) phi+=2*TMath::Pi();
      double cos2deltaPhi=std::cos(2*phi-2*EP);
      double deltaPhi = pe_1->pMom().phi()-EP;
      if (deltaPhi<0) deltaPhi+=2*TMath::Pi();
      if (deltaPhi>TMath::Pi()) deltaPhi-=TMath::Pi();
      bool hasFirst1hit = pe_1->topologyMap(0) >> 8 & 0x1;
      bool hasFirst2hit = pe_1->topologyMap(0) >> 8 & 0x3;
      bool hasFirst3hit = pe_1->topologyMap(0) >> 8 & 0x7;
      bool hasFirst4hit = pe_1->topologyMap(0) >> 8 & 0xF;
      bool passTPChit = hasFirst3hit;
      if (iselectron) {
        // hIncEv2vsPtvsCent->Fill(deltaPhi/anaCuts::resolution[mCent],pe_1->pMom().Perp(),mCent,mWeight); 
        // pIncEv2->Fill(pe_1->pMom().Perp(),mCent,cos2deltaPhi/anaCuts::resolution[mCent],mWeight); 
        // if (passTPChit) 
        //   pIncEv2_hitcut->Fill(pe_1->pMom().Perp(),mCent,cos2deltaPhi/anaCuts::resolution[mCent],mWeight); 
      
        //fill tree
       m_idx_inc[m_nIncE]=itrack1;
       m_nSigmaE[m_nIncE]=pe_1->nSigmaElectron();
       m_beta_inc[m_nIncE]=beta;
       m_pt_inc[m_nIncE]=mom.perp();
       m_eta_inc[m_nIncE]=mom.pseudoRapidity();
       m_phi_inc[m_nIncE]=mom.phi();
       m_deltaphi_inc[m_nIncE]=deltaPhi;
       m_cos2phi_inc[m_nIncE]=cos2deltaPhi;
       m_gDca_inc[m_nIncE]=pe_1->gDCA(mVx,mVy,mVz);
       m_charge_inc[m_nIncE]=pe_1->charge();
       m_nFits_inc[m_nIncE]=pe_1->nHitsFit(); 
       m_ndEdx_inc[m_nIncE]=pe_1->nHitsDedx(); 
       m_topomap0_inc[m_nIncE]=pe_1->topologyMap(0); 

       m_has1hit_inc[m_nIncE]=hasFirst1hit; 
       m_has2hit_inc[m_nIncE]=hasFirst2hit; 
       m_has3hit_inc[m_nIncE]=hasFirst3hit; 
       m_has4hit_inc[m_nIncE]=hasFirst4hit; 
          
       m_nIncE++;
      }
      
      if (DEBUG) cout << "start loop second pe" <<endl;
      //inclusive charged particle, just for check
      // pIncHadronv2->Fill(pe_1->pMom().Perp(),mCent,cos2deltaPhi/anaCuts::resolution[mCent],mWeight);
      // hIncHadronv2vsPtvsCent->Fill(deltaPhi/anaCuts::resolution[mCent],pe_1->pMom().Perp(),mCent,mWeight); 

      // pion, just for check
      // if (isPion(pe_1,beta) && pe_1->charge()>0) pPionPlusv2->Fill(pe_1->pMom().Perp(),mCent,cos2deltaPhi/anaCuts::resolution[mCent],mWeight);
      // if (isPion(pe_1,beta) && pe_1->charge()<0) pPionMinusv2->Fill(pe_1->pMom().Perp(),mCent,cos2deltaPhi/anaCuts::resolution[mCent],mWeight);


      if (isPE1){
        for (int itrack2=0;itrack2<nTracks;itrack2++){
          if (itrack1==itrack2) continue;
          StPicoTrack* pe_2 = mPicoDst->track(itrack2);
          if (!isGoodTrackGlobal(pe_2,pe_2->gDCA(mVx,mVy,mVz))) continue;
          //apply only tof pid cut
          float beta2 = getTofBeta(pe_2);
          bool tofmatch2 = (beta2!=std::numeric_limits<float>::quiet_NaN()) && beta2>0;
          if (!isPEsecondElectron(pe_2,tofmatch2,beta2)) continue; 
          // if (fabs(pe_2->gMom().pseudoRapidity())>=0.8) continue;
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

          if (passPEtopocut) {
            //reconstruct the photon
            // bool unlike=pe_2->charge()*pe_1->charge()<0;
            if (mother.m()<0.28){
              m_idx_phe[m_nPhoE]=itrack1;
              m_nSigE_phe[m_nPhoE]=pe_1->nSigmaElectron();
              m_topomap0_phe[m_nPhoE]=pe_1->topologyMap(0); 
              m_beta_phe[m_nPhoE]=beta;
              m_pt_phe[m_nPhoE]=pe_1->pMom().perp();
              m_eta_phe[m_nPhoE]=pe_1->pMom().pseudoRapidity();
              m_phi_phe[m_nPhoE]=pe_1->pMom().phi();
              m_gDca_phe[m_nPhoE]=pe_1->gDCA(mVx,mVy,mVz);
              m_charge_phe[m_nPhoE]=pe_1->charge();
              m_nFit_phe[m_nPhoE]=pe_1->nHitsFit();
              m_ndEdx_phe[m_nPhoE]=pe_1->nHitsDedx();
              m_deltaphi_phe[m_nPhoE]=deltaPhi;
              m_cos2phi_phe[m_nPhoE]=cos2deltaPhi;

              m_idx_parte[m_nPhoE]=itrack2;
              m_topomap0_parte[m_nPhoE]=pe_2->topologyMap(0); 
              m_nSigE_parte[m_nPhoE]=pe_2->nSigmaElectron();
              m_beta_parte[m_nPhoE]=beta2;
              m_pt_parte[m_nPhoE]=pe_2->gMom().perp();
              m_eta_parte[m_nPhoE]=pe_2->gMom().pseudoRapidity();
              m_phi_parte[m_nPhoE]=pe_2->gMom().phi();
              m_gDca_parte[m_nPhoE]=pe_2->gDCA(mVx,mVy,mVz);
              m_charge_parte[m_nPhoE]=pe_2->charge();
              m_nFit_parte[m_nPhoE]=pe_2->nHitsFit();
              m_ndEdx_parte[m_nPhoE]=pe_2->nHitsDedx();
              m_DCA_pair[m_nPhoE]=fabs(DcaDaughters);
              m_decayL_pair[m_nPhoE]=vtxToV0.mag();
              m_V0x_pair[m_nPhoE]=V0.x();
              m_V0y_pair[m_nPhoE]=V0.y();
              m_V0z_pair[m_nPhoE]=V0.z();
              m_vtx2V0x_pair[m_nPhoE]=vtxToV0.x();
              m_vtx2V0y_pair[m_nPhoE]=vtxToV0.y();
              m_vtx2V0z_pair[m_nPhoE]=vtxToV0.z();
              m_M_pair[m_nPhoE]=mother.m();
              m_px_pair[m_nPhoE]=mother.px();
              m_py_pair[m_nPhoE]=mother.py();
              m_pz_pair[m_nPhoE]=mother.pz();
              m_nPhoE++;
            } //mass <0.3
          } // pass topo cut
        } //second pe loop
      } // if is PE first 
  }  //first track loop  
  return kStOK;
}
double StPicoElectronMaker::getEventPlaneShift(double EP_Re,int side,int cent,int runnum) const
{
  //to be added afer check recenter
  //side =-1 minus eta, +1 plus eta
  double EP_Sh = EP_Re;
  for (int i=1;i<21;i++){
    double meansin=0, meancos=0;
    if (side<0){
      meansin = pSinEtaM[cent]->GetBinContent(i);  //map is start from 0
      meancos = pCosEtaM[cent]->GetBinContent(i);  //map is start from 0 
    }
    else if (side>0){
      meansin = pSinEtaP[cent]->GetBinContent(i);  //map is start from 0
      meancos = pCosEtaP[cent]->GetBinContent(i);  //map is start from 0 
    }
    EP_Sh += (1./(double)i)*(-1.*meansin*std::cos(i*2.*EP_Re)+meancos*std::sin(i*2.*EP_Re));
  }
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
bool StPicoElectronMaker::isGoodTrackGlobal(StPicoTrack const* trk, float dca) const
{
  //use a very loose cut for partnere electron to enhance the reco eff.
  return 
         trk->gMom().perp() > anaCuts::GPt &&
         fabs(trk->nHitsFit()) >= anaCuts::NHitsFit_loose; 
         // fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
         // fabs(trk->gMom().pseudoRapidity())<anaCuts::Eta &&
         // fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx &&
         // fabs(dca)<=anaCuts::Dca &&
         // fabs(trk->nHitsFit()*1.0/trk->nHitsMax()) >=anaCuts::NHitsFit2Poss;
}
//-----------------------------------------------------
bool StPicoElectronMaker::isGoodTrack(StPicoTrack const* trk, float dca) const
{
  // StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->pMom().perp() > anaCuts::GPt && fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->pMom().pseudoRapidity())<anaCuts::Eta &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && fabs(dca)<=anaCuts::Dca &&
    fabs(trk->nHitsFit()*1.0/trk->nHitsMax()) >=anaCuts::NHitsFit2Poss;
  // fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx &&
  // fabs( trk->gDCA(vtx.x() , vtx.y(), vtx.z() )) <= anaCuts::Dca;
}
bool StPicoElectronMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  StThreeVectorF pVtx = picoEvent->primaryVertex();
  // TVector3 pVtx = picoEvent->primaryVertex();
  double refmult = picoEvent->refMult();
  double Tofmult = picoEvent->btofTrayMultiplicity();
  // bool pileup = Tofmult*0.37+50 < refmult;
  bool pileup = Tofmult < refmult*2.88-155;
  // bool pileup = false;
  return (!pileup)  && fabs(pVtx.z()) < anaCuts::vz &&
         fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
         !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror && fabs(pVtx.z()) < anaCuts::Verror) 
         && sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}
float StPicoElectronMaker::getTofBeta(StPicoTrack const* const trk) const
{
  int index2tof = trk->bTofPidTraitsIndex();
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
      }
    }
  } 
  return beta;
}
bool StPicoElectronMaker::isPEfirstElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  // first photonic electron
  // bool isTPCElectron = fabs(trk->nSigmaElectron())<anaCuts::nESigma_Tag;
  bool isTPCElectron = -1*anaCuts::nESigma_Tag<trk->nSigmaElectron() && trk->nSigmaElectron()<2.5;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<anaCuts::tofe:false; 
  // if (trk->gMom().mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-1.2;
  // else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(trk->gMom().mag()*2-2.8);
  return isTPCElectron && isTOFElectron;
}

bool StPicoElectronMaker::isPEsecondElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  //same as tof electron 
  // choose electron from tof, used for purity  analysis
  // require MUST tof match at pt<1.5, and hybrid PID at >1.5
  // TVector3 mom = trk->pMom(); 
  bool isTPCElectron = fabs(trk->nSigmaElectron())<anaCuts::nESigma_partner;  //attention this cut is different from purity fit sample
  if (trk->gMom().mag()<0.3)  isTPCElectron = fabs(trk->nSigmaElectron())<anaCuts::nESigma_partner+0.3;  
  // bool isTOFElectron = tofmatch?fabs(1./beta-1.)<anaCuts::tofe:false; 
  // bool isTOFElectron = tofmatch?fabs(1./beta-1.)<anaCuts::tofe:true;  //very loose cut 
  bool isTOFElectron = true; 
  return  isTPCElectron&&isTOFElectron; //donot require electron
}
bool StPicoElectronMaker::isElectron(StPicoTrack const * const trk, bool tofmatch, float beta) const
{ 
  // first photonic electron
  bool isTPCElectron = false;
  bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false; 
  isTPCElectron = trk->nSigmaElectron()<2.5 && trk->nSigmaElectron()>-2.5;
  // if (trk->pMom().Mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>0;
  // else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(trk->pMom().Mag()*3.5-2.8);
  return isTPCElectron && isTOFElectron;
}
bool StPicoElectronMaker::isKaon(StPicoTrack const * const trk, float beta) const
{
  //choose kaon
  // TVector3 mom = trk->gMom(); 
  StThreeVectorF mom = trk->gMom(); 
  // Float_t p2 = mom.mag()*mom.mag();
  // Float_t m2 = p2/beta/beta - p2;
  // return m2<0.243+0.005 && m2>0.243+0.005;
  double ptot = mom.mag();
  float beta_k = ptot / sqrt(ptot * ptot + M_KAON_PLUS * M_KAON_PLUS);
  bool TofKaon = fabs(1 / beta - 1 / beta_k) < anaCuts::kTof ? true : false;
  bool TpcKaon = fabs(trk->nSigmaKaon())<anaCuts::kTpc;
  return TofKaon && TpcKaon;
}
bool StPicoElectronMaker::isPion(StPicoTrack const * const trk, float beta) const
{
  //choose pion sample from tof
  // TVector3 mom = trk->gMom(); 
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
  bool TofPion = fabs(1 / beta - 1 / beta_pi) < anaCuts::piTof ? true : false;
  bool TpcPion = fabs(trk->nSigmaPion())<anaCuts::piTpc;
  return TpcPion && TofPion;
  // Float_t p2 = mom.mag()*mom.mag();
  // Float_t m2 = p2/beta/beta - p2;
  // return m2<0.019+0.003  &&  m2>0.019-0.003; 
}
bool StPicoElectronMaker::isProton(StPicoTrack const * const trk,  float beta) const
{
  //choose proton sample from tof
  // TVector3 mom = trk->gMom(); 
  StThreeVectorF mom = trk->gMom(); 
  double ptot = mom.mag();
  float beta_p = ptot / sqrt(ptot * ptot + M_PROTON * M_PROTON);
  bool TofProton = fabs(1 / beta - 1 / beta_p) < anaCuts::pTof ? true : false;
  bool TpcProton = fabs(trk->nSigmaProton())<anaCuts::pTpc;
  return TpcProton && TofProton;
  // StThreeVectorF mom = trk->gMom(); 
  // Float_t p2 = mom.mag()*mom.mag();
  // Float_t m2 = p2/beta/beta - p2;
  // return m2<0.879+0.02 && m2>0.879+0.02;
}
int StPicoElectronMaker::getCentralityBin(float vz,int runId,float mult,float &weight,double &refmultcor) const
{
  float mult_corr = mult;
  //if trigger 580001, correct the refmult
  mult+=gRandom->Rndm();
  if (runId<18156031) {
    float fvz = 0;
    for (int i=0;i<anaCuts::nparVz_mult;i++){
      fvz +=anaCuts::parVz_mult[i]*std::pow(vz,i);
    }
    mult=mult*1.0*anaCuts::parVz_mult[0]/fvz;
  }
  refmultcor=mult;
  weight = reweight(mult);
  for (int cent=0;cent<anaCuts::nCent;cent++)
  {
    if (mult<anaCuts::Refmult_cent[cent]) return cent-1;
  }
  return anaCuts::nCent-1;
}
float StPicoElectronMaker::reweight(float x) const
{
  x+=gRandom->Rndm();
  // float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  float p[5] = {0.811,238.9,24.31,-25,6.325e-5};
  // return 1;
  if (x>70) return 1;
  // else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]);
}
