#include "TFile.h"
// #include "TH3F.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TF1.h"
#include "StarClassLibrary/StParticleDefinition.hh"
#include "StarClassLibrary/SystemOfUnits.h"
#include "StarClassLibrary/StParticleTypes.hh"
#include "StBFChain/StBFChain.h"

#include "StEvent/StEventTypes.h"
#include "StEvent/StTrack.h"
#include "StEvent/StTrackGeometry.h"
#include "StEvent/StTrackNode.h"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StTrackTopologyMap.h"
#include "StEvent/StEventSummary.h"
#include "StEvent/StBTofCollection.h"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTpcDedxPidAlgorithm.h"
#include "StEventUtilities/StuRefMult.hh"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "StMcEvent/StMcEventTypes.hh"
#include "StMcEvent/StMcContainers.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"

#include "StAssociationMaker/StAssociationMaker.h"
#include "StAssociationMaker/StTrackPairInfo.hh"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "../StRefMultCorr/CentralityMaker.h"
#include "StMcAnaCuts.h"
#include "StMcAnalysisMaker.h"
#include "phys_constants.h"
#include "TRandom.h"

ClassImp(StMcAnalysisMaker);

StMcAnalysisMaker::StMcAnalysisMaker(const char *name, const char *title): StMaker(name),
  mGRefMultCorrUtil(NULL), mMuDst(NULL), mField(-999), mCentrality(-999), 
  mFile(NULL),  mMcEvent(NULL), mEvent(NULL), mAssoc(NULL)
{
  LOG_INFO << "StMcAnalysisMaker() - DONE" << endm;
}
//__________________________________
int StMcAnalysisMaker::Init()
{
  // mGRefMultCorrUtil = new StRefMultCorr("refMult");
  // mGRefMultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
  mOutfileName = mOutfileName.ReplaceAll(".root", "");
  mFile = new TFile(Form("%s.McAna.root", mOutfileName.Data()), "recreate");
  assert(mFile && !mFile->IsZombie());

  for (int ii = 0; ii < McAnaCuts::maxNumberOfTriggers; ++ii)
  {
    firedTriggersIndices.push_back(-1);
  };
  mOutfileName.Remove(0,33); 
  gRandom->SetSeed(mOutfileName.Atoi()*897);
  InitHists();  
  LOG_INFO << "Init() - DONE" << endm;
  return kStOk;
}

bool StMcAnalysisMaker::InitHists()
{
  hnPi0 = new TH1F("hnPi0","hnPi0",100,0,100); 
  hRefmult = new TH1F("hRefmult","hRefmult",500,0,500);
  hEvent = new TH1F("hEvent","hEvent;Cent",9,-0.5,8.5);
  hEP = new TH1F("hEP","hEP;#Phi",360,0,TMath::Pi()); 
  //check the pi pt distribution
  hPi2tot = new TH1F("hPi2tot","hPi2tot",30,0,0.3);
  hPi0Pt = new TH2F("hPi0Pt","hPi0Pt;p_{T}[GeV];Cent",120,0,15,9,-0.5,8.5);

  //check the global tracks (electron) efficiency and QA
  hnHitsvsPt = new TH2F("hnHits","hnHits;nHits;p_{T}",55,0,55,10,0,5); 
  hDCAvsPt = new TH2F("hDCA","hDCA;DCA;p_{T}",100,0,3,10,0,5);

  hMcElectronPtvsCent = new TH2F("hMcElectronPtvsCent","hMcElectron;p_{T};Cent",80,0,4,9,-0.5,8.5);
  hMcElectronPtvsCent->Sumw2();
  // hMcElectronPtvsCent_test = new TH2F("hMcElectronPtvsCent_test","hMcElectron;p_{T};Cent",80,0,4,9,-0.5,8.5);
  hRcElectronPtvsCent = new TH2F("hRcElectronPtvsCent","hRcElectron;p_{T};Cent",80,0,4,9,-0.5,8.5);
  hRcElectronPtvsCent->Sumw2();
  hMomResolution = new TH2F("hMomResolution","hMomResolution;mc p_{T};#frac{rc p_{T}-mc p_{T}}{mc p_{T}}",80,0,4,100,-0.1,0.1);

  //check the duplicated tracks
  hDuplicated = new TH3F("hDuplicated","hDuplicated;MC p_{T};Mc tpcHits;Dup Rc tpcFits",160,0,4,45,10,50,45,10,50);
  hDupTracksDpt = new TH2F("hDupTracksDpt","hDupTracksDpt;rc1 pt;rc2 pt", 160,0,4,160,0,4);
  hDupTracksDhits = new TH2F("hDupTracksDhits","hDupTracksDhits;rc1 nFits;rc2 nFits",40,10,50,40,10,50);
  hDupTracksCharge = new TH2F("hDupTracksCharge","hDupTracksCharge;rc1 charge;rc2 charge",2,-1.5,1.5,2,-1.5,1.5);

  //pho eff
  if (McAnaCuts::parentId>0)
  {
    hElectron = new TH2F("hTagElectron","hTagElectron;p_{T};mCent",100,0,4,9,-0.5,8.5);
    hElectron->Sumw2();
    hElectronPassCut = new TH2F("hTagElectronPassCut","hTagElectronPassCut;p_[T};mCent",100,0,4,9,-0.5,8.5); 
    hElectronPassCut->Sumw2();

    hMassDiE = new TH3F("hMassDiE","hMassDiE;mass;pt;Cent",160,0,0.4,100,0,4,9,-0.5,8.5);
    hMassDiEMC = new TH3F("hMassDiEMC","hMassDiEMC;mass;pt;Cent",160,0,0.4,100,0,4,9,-0.5,8.5);
    hDcaPair = new TH3F("hDcaPair","hDcaPair;DCApair;pt;Cent",40,0,2,100,0,4,9,-0.5,8.5);

    //check the partner electron QA 
    hPartEnFits = new TH2F("hPartEnFits","hPartEnFits;p_{T};nHitsFit",100,0,4,45,10,55);
    hPartEdca = new TH2F("hPartEdca","hPartEdca;p_{T};DCA",100,0,4,50,0,3); 
    hPartEptetaphi = new TH3F("hPartEptetaphi","hPartEptetaphi;pt;Eta;Phi",100,0,4,50,-1,1,180,-3.14,3.14);
    hPairDecayL = new TH2F("hPairDecayL","hPairDecayL;DecayL;p_{T}",150,0,30,60,0,4);

    //pho v2
    pPi0Ev2 = new TProfile2D("pPi0Ev2","pPi0Ev2;pt;cent",100,0,4,9,-0.5,8.5);
    pPhoEv2 = new TProfile2D("pPhoEv2","pPhoEv2;pt;cent",100,0,4,9,-0.5,8.5);
    hPhoEPtPhiCent = new TH3F("hPhoEPtPhiCent","hPhoEPtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hPhoEPtPhiCent->Sumw2();
    hPhoEPtPhiCentRaw = new TH3F("hPhoEPtPhiCentRaw","hPhoEPtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hPhoEPtPhiCentRaw->Sumw2();

    pRecoEv2 = new TProfile2D("pRecoEv2","pRecoEv2;pt;cent",100,0,4,9,-0.5,8.5);
    hRecoEPtPhiCent = new TH3F("hRecoEPtPhiCent","hRecEPtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hRecoEPtPhiCent->Sumw2();
    hRecoEPtPhiCentRaw = new TH3F("hRecoEPtPhiCentRaw","hRecoEPtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hRecoEPtPhiCentRaw->Sumw2();

    hPi0PtPhiCent = new TH3F("hPi0PtPhiCent","hPi0PtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hPi0PtPhiCent->Sumw2();
    hPi0PtPhiCentRaw = new TH3F("hPi0PtPhiCentRaw","hPi0PtPhiCent",100,0,4, 180, 0, TMath::Pi() ,9,-0.5,8.5);
    hPi0PtPhiCentRaw->Sumw2();
  }
  return true;
}

void StMcAnalysisMaker::bookSpectra(int centrality)
{
  mPi0Spectra = new TF1("pi0spectra","2*TMath::Pi()*x*[0]*pow(TMath::Exp(-1*[1]*x-[2]*x*x)+x/[3], -[4])",0,15);
  mPi0Spectra->SetParameters(McAnaCuts::SpectraParPi0[McAnaCuts::SpectraParPi0_centbin[mCentrality]]);
  TFile* file = TFile::Open("StRoot/macros/fitpionv2.root");
  TString namev2[9]={"50_60","50_60","50_60","40_50","30_40","20_30","10_20","0_10","0_10"};
  fPi0v2 = (TF1*)file->Get(Form("fit_%s", namev2[centrality].Data())); 
  // fPi0v2 = (TF1*)file->Get("fit_20_30"); 
  file->Close();
}
//__________________________________
int StMcAnalysisMaker::Make()
{
  // cout<< "start new event.."<<endl;
  StMuDstMaker* muDstMaker = (StMuDstMaker*)GetMaker("MuDst");

  if (!muDstMaker)
  {
    LOG_WARN << " No MuDstMaker, will try to take all event information from StEvent" << endm;
    mMuDst = NULL;
  }
  else
  {
    mMuDst = (StMuDst*)muDstMaker->muDst();
  }

  if(!mMuDst || !mMuDst->event())
  {
    LOG_WARN << "MuDst or mMuDst->event() is missing, will try to take all event information from StEvent" << endm;
    mMuDst = NULL;
  }
  mMcEvent = (StMcEvent*)GetDataSet("StMcEvent");

  if (!mMcEvent)
  {
    LOG_WARN << "No StMcEvent" << endm;
    return kStWarn;
  }
  mEvent = (StEvent*)GetDataSet("StEvent");
  if (!mEvent)
  {
    LOG_WARN << "No StEvent" << endm;
    return kStWarn;
  }
  // cout<< "load event Mc event finish..."<<endl;
  // cout<<"Get StAssociationMaker" <<endl;
  mAssoc = (StAssociationMaker*)GetMaker("StAssociationMaker");
  if (!mAssoc)
  {
    cout << "Could not get StAssociationMaker" << endl;
    return kStErr;
  }
  // else if (mAssoc) {
  //      if (!(mAssoc->rcTpcHitMap()&&
  //            mAssoc->rcTrackMap()&&
  //            mAssoc->mcTrackMap()))
  //        {
  //          cout<<"Could not get StAssociationMaker maps" << endl;
  //          return kStErr;
  //        }
  // }

  mField = (float)mEvent->summary()->magneticField();
  mpVtx=mMuDst->event()->primaryVertexPosition();
  // cout<<"start refmult"<<endl;
  // bool goodevent = fabs(mpVtx.z())<35 && fabs(mpVtx.perp()) < McAnaCuts::vr && fabs(mpVtx.z()-mMuDst->event()->vpdVz())<McAnaCuts::VzVpdvz;
  bool goodevent = fabs(mpVtx.z())<35;
  if (!goodevent) 
  {
    cout <<"is not good event  "<<endl;
    // cout << mpVtx.z()<<" "<< mpVtx.perp()<<" "<< mpVtx.z()-mMuDst->event()->vpdVz()<<endl;
    return kStSkip;    
  }
  // if(mGRefMultCorrUtil && mMuDst)
  // {
  //   mGRefMultCorrUtil->init(mEvent->runId());
  //   mGRefMultCorrUtil->initEvent(mMuDst->event()->refMult(),
  //                                mEvent->primaryVertex()->position().z()); 
  //                                // mEvent->primaryVertex()->position().z(), 
  //                                // mEvent->runInfo()->zdcCoincidenceRate() );
  //
  //   if (mGRefMultCorrUtil->isBadRun(mEvent->runId()))
  //   {
  //     LOG_INFO << "This is a bad run from mGRefMultCorrUtil! Skip! " << endm;
  //     return kStSkip;
  //   }
  //   mCentrality  = mGRefMultCorrUtil->getCentralityBin9();
  //   cout << " centrality "<< mCentrality << " refMult "<<  mMuDst->event()->refMult()<<endl;
  // }
  // else
  // {
  //   mCentrality = -999;
  // }

  //get centrality
  mCentWeight = 1.;
  double refmultcor=0;
  // mCentrality = getCentralityBin(mpVtx.z(),mEvent->runId(),mMuDst->event()->refMult(),mCentWeight,refmultcor); 
  mCentrality = getCentralityBin(mpVtx.z(),mEvent->runId(),mMuDst->event()->refMult()/(1.+0.05),mCentWeight,refmultcor); 
  if (mCentrality<0) return kStSkip;

  bookSpectra(mCentrality); 
  mEP = gRandom->Rndm()*TMath::Pi();
  // mEP = 0;
  hEP->Fill(mEP);
  // cout<<"refmult finish..." <<endl;
  hRefmult->Fill(mMuDst->event()->refMult()); 
  hEvent->Fill(mCentrality,mCentWeight);
  // Fill
  int nRTracks = -1;
  int nMcTracks = -1;

  // cout<< "check trigger and fill track..."<<endl;
  int fillTracksStatus = kStOk;
  if (passTrigger())
  {
    fillTracksStatus = fillTracks(nRTracks, nMcTracks);
  }
  else
  {
    LOG_INFO << "No interesting triggers. Counting event then skipping." << endm;
  }

  // int fillEventCountStatus = fillEventCounts((float)nRTracks, (float)nMcTracks);

  // return fillTracksStatus && fillEventCountStatus;
  return fillTracksStatus;
}

int StMcAnalysisMaker::fillTracks(int& nRTracks, int& nMcTracks)
{
  nRTracks = 0;
  nMcTracks = 0;

  LOG_INFO << "START Filling " << mMcEvent->tracks().size() << " mcTracks..." << "\n";

  nPi0 = 0;

  // key -1 is the index and idTruth is equal to key if this rctrack is a MC track 
  std::map<int,int> mc_rcpair; 
  // cout <<"fillMcTrack" <<endl;
  for (unsigned int iTrk = 0;  iTrk < mMcEvent->tracks().size(); ++iTrk)
  {
    StMcTrack* const mcTrack = mMcEvent->tracks()[iTrk];
    if (!mcTrack)
    {
      LOG_WARN << "Empty mcTrack container" << endm;
      continue;
    }
    if (!isGoodMcTrack(mcTrack)) continue;
    ++nMcTracks;
    // cout <<"mcTrack key: "<< mcTrack->key() << " "<< mcTrack->geantId() << endl;
    fillMcTrack(mcTrack);
    int nCom = -999;
    StTrack const * rcTrack = findPartner(mcTrack,nCom); 
    if (!rcTrack || nCom<=10) {   
      mc_rcpair.insert(std::pair<int,int>(mcTrack->key(),-999));
    }
    else {
      // ++nRTracks;
      mc_rcpair.insert(std::pair<int,int>(mcTrack->key(),rcTrack->key()));
      // cout <<"mcTrack key: "<< mcTrack->key() << " "<< mcTrack->geantId()<<" ";
      // cout<< "paired rc: "<<rcTrack->key()<< " idtruth "<< rcTrack->idTruth()<<endl;
    }
  }
  hnPi0->Fill(nPi0);
  hPi2tot->Fill(1.*nPi0/mMuDst->event()->refMult()); 

  //more than one track 
  // cout <<"fillRcTrack... total "<<  mMuDst->globalTracks()->GetEntries()<<endl;
  // cout << "StEvent: "<< (mEvent->trackNodes()).size()<<endl;

  for (int ircTrk = 0;ircTrk<mMuDst->globalTracks()->GetEntries();ircTrk++)
  {
    // StMuTrack* rcTrack = mMuDst->primaryTracks(ircTrk);
    StMuTrack* rcTrack = mMuDst->globalTracks(ircTrk);
    if (!rcTrack) {
      LOG_WARN<<" no rcTrack input! "<< endm;
      continue;
    }
    int idTruth = rcTrack->idTruth();
    // bool isfromMcTrack =  (idTruth>nPi0 && idTruth<=mMcEvent->tracks().size()); 
    bool isfromMcTrack =  (idTruth>0 && idTruth<=(mMcEvent->tracks().size())); 
    if (!isfromMcTrack) continue;
    if (!isGoodRcTrack(rcTrack)) continue; 
    ++nRTracks;

    // cout<< "idTruth " << idTruth<< endl;
    StMcTrack* const mcTrack = mMcEvent->tracks()[idTruth-1];
    if (!mcTrack) {
      // cout<<"idTruth is not correct!" << endl;
      continue;
    }
    // cout << ircTrk << " "<<rcTrack->id()<<endl;
    //StTrack key() == StMuTrack id()
    if (mc_rcpair[idTruth]!=rcTrack->id()) {
      // cout<<"duplicated rctrack ! Mc pt: "<< mcTrack->pt() << " rc pt: "<<rcTrack->pt()<<" rcTrack Id: "<<rcTrack->id()<<" ";
      // int nCom=-999;
      // cout<<" find MC partner id: "<<findPartner(mcTrack,nCom)?findPartner(mcTrack,nCom)->key():0;
      // cout<<" nCom: "<<nCom<<endl;
      hDuplicated->Fill(mcTrack->pt(),mcTrack->tpcHits().size() ,rcTrack->nHitsFit(kTpcId));
      if (mc_rcpair[idTruth]>0 )
      { 
        StTrack* duplicatedTrk = mEvent->trackNodes()[mc_rcpair[idTruth]-1]->track(global);
        // cout <<duplicatedTrk->key()<<" "<<mc_rcpair[idTruth] << endl;
        hDupTracksDpt->Fill(duplicatedTrk->geometry()->momentum().perp(),rcTrack->pt());
        hDupTracksDhits->Fill(duplicatedTrk->fitTraits().numberOfFitPoints(),rcTrack->nHitsFit());
        hDupTracksCharge->Fill(duplicatedTrk->geometry()->helix().charge(mField),rcTrack->charge());
      }
      continue;
    }

    // fillRcTrack(rcTrack,mcTrack);
    // cout <<"global track" << endl;
    if (rcTrack->primaryTrack() ) 
    {
      bool isgoodTagE = isGoodTagE(rcTrack->primaryTrack());
      if (!isgoodTagE) continue;
      // fillRcTrack(rcTrack->primaryTrack(),mcTrack);
      fillRcTrack(rcTrack,mcTrack);

      // cout <<"primary track" << endl;
      // if have parent particle then pair the partner electron
      if ( McAnaCuts::parentId>0) pairPartnerElectron(rcTrack->primaryTrack(),mcTrack, mc_rcpair);   
    }
  }
  mc_rcpair.clear(); 
  //  StSPtrVecTrackNode& trackNode = mEvent->trackNodes();
  // it nTracks = trackNode.size();
  // for (int i=0; i < nTracks; i++) {
  //   StTrackNode *node = trackNode[i];
  //   if (!node) continue;
  //   StTrack *glTrack = node->track(global);
  //   if (! glTrack) continue;
  //   ++nRTracks;
  // }
  // cout<<"MC Tracks:  " <<nMcTracks << " RC tracks: "<< nRTracks <<endl;
  return kStOk;
}

void StMcAnalysisMaker::fillMcTrack(StMcTrack const* const mcTrk)
{
  // cout << " parent  "<< mcTrk->parent()->geantId() << endl;
  if (McAnaCuts::parentId>0&&mcTrk->geantId()==McAnaCuts::parentId) 
  {
    // cout << " test if the key is the same "<< mMcEvent->tracks()[mcTrk->key()-1]->key() << " " << mcTrk->key()<<endl; 
    nPi0++;    
    hPi0Pt->Fill(mcTrk->pt(),mCentrality ,mCentWeight);
    fillPi0v2(mcTrk);
    // cout <<" mcTrk pi0 parent: "<< (mcTrk->parent()) << endl;
  } 
  else 
  {
    if ( mcTrk->geantId() == McAnaCuts::dauId1 ||  mcTrk->geantId() == McAnaCuts::dauId2)
    {
      // cout <<" test the mother key "<<mcTrk->key() <<" " << mcTrk->parent()->key()<<endl;; 
      // only check the dalitz decay particles
      // if (mcTrk->parent()->geantId() == 10007 && mcTrk->pt()>McAnaCuts::minPt && fabs(mcTrk->pseudoRapidity())<McAnaCuts::partnerEta)
      if ( mcTrk->pt()>McAnaCuts::minPt && fabs(mcTrk->pseudoRapidity())<McAnaCuts::eta)
      {
        hMcElectronPtvsCent->Fill(mcTrk->pt(),mCentrality,mCentWeight);
        // hMcElectronPtvsCent_test->Fill(mcTrk->momentum().perp(),mCentrality,mCentWeight);
      }
    }
  }
}
void StMcAnalysisMaker::fillRcTrack( StMuTrack const* const rcTrack,StMcTrack const* const mcTrack)
{
  if (mcTrack->geantId()!=McAnaCuts::dauId1 &&  mcTrack->geantId()!=McAnaCuts::dauId2) return;
  // if (mcTrack->parent()->geantId()!=10007) return;

  float dca = rcTrack->dcaGlobal().mag();
  // cout << "global: "<< rcTrack->globalTrack()->nHitsFit() << " primary "<<  rcTrack->nHitsFit() <<endl ;
  int nHitsFit = rcTrack->nHitsFit();
  // cout << (nHitsFit >20)<<" "<< (nHitsFit/(1.*nHitsMax) > 0.52)  <<endl;
  // cout <<" nSigmaE "<<  rcTrack->nSigmaElectron()  <<endl;
  double pt = rcTrack->pt();
  double eta = rcTrack->eta();
  if (rcTrack->primaryTrack())
  {
    pt = rcTrack->primaryTrack()->pt();
    eta = rcTrack->primaryTrack()->eta();
  }
  hDCAvsPt->Fill(dca,pt,mCentWeight);  
  bool goodRcTrack = pt>McAnaCuts::minPt && fabs(eta)<McAnaCuts::eta;
  if (!goodRcTrack) return; 
  hnHitsvsPt->Fill(nHitsFit,pt,mCentWeight); 
  hRcElectronPtvsCent->Fill(pt,mCentrality,mCentWeight);
  hMomResolution->Fill(mcTrack->pt(),(pt-mcTrack->pt())/mcTrack->pt(),mCentWeight);
}

void StMcAnalysisMaker::pairPartnerElectron(StMuTrack const* const TagE,StMcTrack const * const McTagE, std::map<int,int>& mc_rcpair)
{
  StMcTrack const * const  mother = McTagE->parent(); 
  if (!mother) {
    LOG_WARN<<" This primary e do not have mother??" <<endm;
    return;
  }
  // float motherPt = mother->pt();
  float weight=1.;
  if (mother->geantId() == McAnaCuts::parentId)
  {
    weight = 1./nPi0*mPi0Spectra->Eval(mother->pt())*mCentWeight;  // set a scale factor 
    fillPhoEv2(TagE,mother,weight,0);
  }
  else if (mother->geantId() == McAnaCuts::gammaId)  //gamma
  {
    StMcTrack const * const  grandparent = McTagE->parent()->parent();
    if ((!grandparent) || grandparent->geantId()!= McAnaCuts::parentId) {
      cout <<"This electron does not come from pi0->e or pi0->gamma->e!" << endl;
      return;
    }
    weight = 1./nPi0*mPi0Spectra->Eval(grandparent->pt())*mCentWeight;  // set a scale factor 
    fillPhoEv2(TagE,grandparent,weight,0);
  }
  hElectron->Fill(TagE->pt(), mCentrality,weight);

  //partner electron are global tracks
  for (unsigned int ipart =0;ipart<mMuDst->globalTracks()->GetEntries();ipart++)
  {
    StMuTrack* PartE = mMuDst->globalTracks(ipart);
    if (!PartE) {
      LOG_WARN<<" no Partner Electron input! "<< endm;
      continue;
    }

    if (PartE->id() == TagE->id()) continue;
    if (!isGoodPartE(PartE)) continue; 

    if (PartE->charge()*TagE->charge()>0) continue;
    int idTruth = PartE->idTruth();
    bool isfromMcTrack =  (idTruth>0 && idTruth<=int(mMcEvent->tracks().size())); 
    if (!isfromMcTrack) continue;
    StMcTrack const * const McPartE = mMcEvent->tracks()[idTruth-1]; 
    if (!McPartE)
    { 
      continue; 
      cout<<"cannot find partner electron MC track"<<endl;
    }
    if (mc_rcpair[idTruth]!=PartE->id())  continue; 
    int motherId = -999;
    motherId = mother->key();
    //check if partner e come from the same mother as tag e
    if (!McPartE->parent()) continue;
    else if (McPartE->parent()->key()!=motherId) continue;

    StLorentzVector<float> McPartE4Mom(McPartE->momentum(),McPartE->momentum().massHypothesis(M_ELECTRON));
    StLorentzVector<float> McTagE4Mom(McTagE->momentum(),McTagE->momentum().massHypothesis(M_ELECTRON));  
    StLorentzVector<float> McMother = McTagE4Mom+McPartE4Mom;
    hMassDiEMC->Fill(McMother.m(),McTagE->pt(),mCentrality , weight);
    bool passRecoCut = passReconstructionCut(TagE->globalTrack(), PartE,weight);
    if (passRecoCut)
    {
      //fill electron histrogram
      hElectronPassCut->Fill(TagE->pt(),mCentrality,weight); 
      hPartEnFits->Fill(PartE->pt(),PartE->nHitsFit(),weight);
      hPartEdca->Fill(PartE->pt(),PartE->dcaGlobal().mag(),weight);
      hPartEptetaphi->Fill(PartE->pt(),PartE->momentum().pseudoRapidity(),PartE->momentum().phi(),weight);
      if (mother->geantId() == McAnaCuts::parentId)
      {
        fillPhoEv2(TagE,mother,weight,1);
      }
      else if (mother->geantId() == McAnaCuts::gammaId)  //gamma
      {
        StMcTrack const * const  grandparent = McTagE->parent()->parent();
        if ((!grandparent) || grandparent->geantId()!= McAnaCuts::parentId) {
          cout <<"This electron does not come from pi0->e or pi0->gamma->e!" << endl;
          return;
        }
        fillPhoEv2(TagE,grandparent,weight,1);
      }
    }
  }
}

void StMcAnalysisMaker::fillPhoEv2(StMuTrack const* const TagE,StMcTrack const * const mother,float weight, bool isReco)
{
  //will add it later 
  float mother_phi = mother->momentum().phi();
  float mother_pt = mother->momentum().perp();
  float mother_v2 = fPi0v2->Eval(mother_pt); // it should be added after collected the v2 inputs
  float phiweight = 1+ 2*mother_v2*std::cos(2*( mother_phi-mEP));
  float TagE_phi = TagE->momentum().phi();
  if (TagE_phi<0) TagE_phi+=2*TMath::Pi();
  float deltaPhi = TagE_phi-mEP;
  if (deltaPhi<0) deltaPhi+=2*TMath::Pi();
  if (deltaPhi>TMath::Pi()) deltaPhi-=TMath::Pi();

  if (!isReco){ 
    pPhoEv2->Fill( TagE->pt(),mCentrality  ,std::cos( 2*(TagE_phi-mEP)),weight*phiweight); 
    hPhoEPtPhiCent->Fill(TagE->pt(),deltaPhi, mCentrality ,weight*phiweight);
    hPhoEPtPhiCentRaw->Fill(TagE->pt(),deltaPhi, mCentrality ,weight);
  }
  else if (isReco)
  {
    pRecoEv2->Fill( TagE->pt(),mCentrality  ,std::cos( 2*(TagE_phi-mEP)),weight*phiweight); 
    hRecoEPtPhiCent->Fill(TagE->pt(),deltaPhi, mCentrality ,weight*phiweight);
    hRecoEPtPhiCentRaw->Fill(TagE->pt(),deltaPhi, mCentrality ,weight);
  }
}

void StMcAnalysisMaker::fillPi0v2(StMcTrack const * const mcTrack )
{
  float ptweight = mPi0Spectra->Eval(mcTrack->pt()); 
  float mother_v2 = fPi0v2->Eval(mcTrack->pt()); // it should be added after collected the v2 inputs
  float phiweight = 1+ 2*mother_v2*std::cos(2*(mcTrack->momentum().phi()-mEP));
  pPi0Ev2->Fill( mcTrack->pt(),mCentrality, std::cos(2*(mcTrack->momentum().phi()-mEP)),10.0/(1.0*mMuDst->event()->refMult())*ptweight*phiweight*mCentWeight);
  // cout<< " mother pt "<<mcTrack->momentum().perp()<<" "<<mother_v2 << endl;
  float mcphi = mcTrack->momentum().phi();
  if (mcphi<0) mcphi+=2*TMath::Pi();
  float delta = mcphi-mEP;
  if (delta<0) delta+=2*TMath::Pi();
  else if (delta>TMath::Pi()) delta-=TMath::Pi();
  hPi0PtPhiCent->Fill(mcTrack->pt(),delta, mCentrality , 10.0/(1.0*mMuDst->event()->refMult())* phiweight * ptweight*mCentWeight); 
  hPi0PtPhiCentRaw->Fill(mcTrack->pt(),delta, mCentrality, 10.0/(1.0*mMuDst->event()->refMult())* ptweight*mCentWeight ); 
}

bool StMcAnalysisMaker::passReconstructionCut(StMuTrack const * const pe_1, StMuTrack const * const pe_2, float weight)
{
  //the reconstruction cut for partner electron
  StPhysicalHelixD p1Helix = pe_1->helix();
  StPhysicalHelixD p1Line = pe_1->helix();
  StPhysicalHelixD p2Helix = pe_2->helix();
  StPhysicalHelixD p2Line= pe_2->helix();
  StThreeVectorF const p1Mom = pe_1->momentum();
  StThreeVectorF const p2Mom = pe_2->momentum(); //global mom is at the dca to pvtx of the helix
  // StThreeVectorF const DCApos_1 = p1Helix.at(p1Helix.pathLength(mpVtx));
  // StThreeVectorF const DCApos_2 = p2Helix.at(p2Helix.pathLength(mpVtx));

  // StPhysicalHelixD const p1Line(p1Mom, DCApos_1, mField*kilogauss , pe_1->charge()); //B
  // StPhysicalHelixD const p2Line(p2Mom, DCApos_2, mField*kilogauss , pe_2->charge());

  pair<double, double> const ss = p1Line.pathLengths(p2Line);
  StThreeVectorF const p1AtDca2p2 = p1Line.at(ss.first);
  StThreeVectorF const p2AtDca2p1 = p2Line.at(ss.second);

  // calculate DCA of pion to kaon at their DCA
  float DcaDaughters = (p1AtDca2p2 - p2AtDca2p1).mag();

  // calculate Lorentz vector of e+e- pair
  StThreeVectorF const p1MomAtDca = p1Helix.momentumAt(ss.first, mField * kilogauss);
  StThreeVectorF const p2MomAtDca = p2Helix.momentumAt(ss.second, mField * kilogauss);

  StLorentzVector<float> p2FourMom(p2MomAtDca,p2MomAtDca.massHypothesis(M_ELECTRON));
  StLorentzVector<float> p1FourMom(p1MomAtDca,p1MomAtDca.massHypothesis(M_ELECTRON));
  // cout <<"electron mass: " <<M_ELECTRON<< endl;
  StLorentzVector<float> mother = p2FourMom+p1FourMom;
  // calculate pointing angle and decay length
  StThreeVectorF const V0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5;
  StThreeVectorF const vtxToV0 = (p2AtDca2p1 + p1AtDca2p2) * 0.5 - mpVtx;

  bool passPEtopocut = fabs(DcaDaughters) < McAnaCuts::EEdcaDaughter;

  bool passMassCut = mother.m()<0.15; // this is to check if the mass smearing make the mass non-zero

  //fill a mass histogram to compare with the real data peak
  if (passPEtopocut) hMassDiE->Fill(mother.m(),pe_1->primaryTrack()->pt() , mCentrality,weight); 
  hDcaPair->Fill(DcaDaughters, pe_1->primaryTrack()->pt(),mCentrality,weight);
  hPairDecayL->Fill(vtxToV0.perp(), pe_1->primaryTrack()->pt(),weight);
  // hDecayL->Fill();
  return passPEtopocut && passMassCut;
}

bool StMcAnalysisMaker::isGoodMcTrack(StMcTrack const* const mcTrack) const
{
  // return 
  // mcTrack->geantId() == McAnaCuts::geantId &&
  // mcTrack->startVertex()->position().perp() < McAnaCuts::mcTrackStartVtxR;
  return true;
  // return (mcTrack->geantId() == 2 || mcTrack->geantId() == 3) && mcTrack->pt()>McAnaCuts::minPt; 
  // && mcTrack->startVertex()->position().perp() < McAnaCuts::mcTrackStartVtxR;
}

bool StMcAnalysisMaker::isGoodRcTrack(StMuTrack const* const rcTrack) const
{
  int nHitsFit = rcTrack->nHitsFit();
  int nHitsMax = rcTrack->nHitsPoss();
  int nHitsdEdx = rcTrack->nHitsDedx();
  bool passnHits = fabs(nHitsFit) >McAnaCuts::nHitsFit &&  
    fabs(nHitsFit/(1.*nHitsMax))>McAnaCuts::nFit2nMax &&
    fabs(nHitsdEdx)>McAnaCuts::nHitsdEdx;
  return passnHits;
}

bool StMcAnalysisMaker::isGoodTagE(StMuTrack const* const rcTrack) const
{
  float dca = rcTrack->dcaGlobal().mag();
  int nHitsFit = rcTrack->globalTrack()->nHitsFit();
  int nHitsdEdx = rcTrack->globalTrack()->nHitsDedx();
  int nHitsMax = rcTrack->globalTrack()->nHitsPoss();
  bool passnHits = fabs(nHitsFit) >McAnaCuts::nHitsFit &&
                   fabs(nHitsFit/(1.*nHitsMax))>McAnaCuts::nFit2nMax &&
                   fabs(nHitsdEdx)>McAnaCuts::nHitsdEdx;
  bool hasFirst3Hit = false;
  for (int i=1;i<=3;i++)
  {
    hasFirst3Hit = hasFirst3Hit || rcTrack->topologyMap().hasHitInRow(kTpcId,i); 
  }
  float phi =rcTrack->momentum().phi();
  bool passPhiCut = !( fabs(phi)<McAnaCuts::phicut.first && fabs(phi)>McAnaCuts::phicut.second);
  // bool goodRcTrack = passPhiCut && passnHits && hasFirst3Hit && fabs(dca)<1.5 && rcTrack->pt()>McAnaCuts::tagPt && fabs(rcTrack->momentum().pseudoRapidity())<McAnaCuts::TagEta && fabs(rcTrack->momentum().pseudoRapidity())>0.05;
  bool goodRcTrack = passPhiCut && passnHits && hasFirst3Hit && fabs(dca)<1.5 && rcTrack->pt()>McAnaCuts::tagPt && fabs(eta)<McAnaCuts::TagEta;
  return goodRcTrack; 
}

bool StMcAnalysisMaker::isGoodPartE(StMuTrack const* const rcTrack) const
{
  float dca = rcTrack->dcaGlobal().mag();
  int nHitsFit = rcTrack->nHitsFit();
  int nHitsdEdx = rcTrack->nHitsDedx();
  int nHitsMax = rcTrack->nHitsPoss();
  bool passnHits = nHitsFit >15;
  // bool passnHits = nHitsFit >McAnaCuts::nHitsFit &&
  //   nHitsFit/(1.*nHitsMax)>McAnaCuts::nFit2nMax &&
  //   fabs(nHitsdEdx)>McAnaCuts::nHitsdEdx;
  bool goodRcTrack = passnHits && rcTrack->pt()>McAnaCuts::partnerPt && fabs(rcTrack->eta())<McAnaCuts::partnerEta;
  // bool goodRcTrack = passnHits && rcTrack->pt()>0.3 && fabs(rcTrack->eta())<0.5;
  return goodRcTrack; 
}

bool StMcAnalysisMaker::passTrigger()
{
  LOG_INFO << "Checking triggers..." << endm;
  bool interesting_event = false;

  if (!mEvent)
  {
    LOG_FATAL << "mEvent doesn't exist" << endm;
  }

  if (McAnaCuts::interesting_triggers.size() == 0)
  {
    LOG_WARN << "No triggers in McAnaCuts::interesting_triggers ... accepting event anyway" << endm;
    return true;
  }

  const StTriggerId* st_trgid = mEvent->triggerIdCollection()->nominal();

  for (unsigned int ii = 0; ii < firedTriggersIndices.size(); ++ii)
  {
    firedTriggersIndices[ii] = -1;
  }

  // Fill interesting triggers
  LOG_INFO << "Interesting fired triggers: " << "\n";

  for (unsigned int ii = 0; ii < st_trgid->maxTriggerIds(); ++ii)
  {
    unsigned int id = st_trgid->triggerId(ii);
    int trgIndex = -1;

    for (unsigned int jj = 0; jj < McAnaCuts::interesting_triggers.size(); ++jj)
    {
      if (McAnaCuts::interesting_triggers[jj] == id)
      {
        trgIndex = jj;
        interesting_event = true;
        LOG_INFO << id << " ";
        break;
      }
    }

    if (trgIndex != -1) firedTriggersIndices[trgIndex] = 1.0;
  }

  LOG_INFO << endm;

  return interesting_event;
}

StTrack const* StMcAnalysisMaker::findPartner(StMcTrack* mcTrack, int& maxCommonTpcHits) const
{
  //..StMcTrack find partner from the StTracks
  pair<mcTrackMapIter, mcTrackMapIter> p = mAssoc->mcTrackMap()->equal_range(mcTrack);

  const StTrack* maxTrack = 0;
  maxCommonTpcHits = 0;
  for (mcTrackMapIter k = p.first; k != p.second; ++k)
  {
    int commonTpcHits = k->second->commonTpcHits();
    const StTrack* track = k->second->partnerTrack()->node()->track(global);//should be global
    if (track && commonTpcHits > maxCommonTpcHits)
    {
      maxTrack = track;
      maxCommonTpcHits = commonTpcHits;
    }
  }
  return maxTrack;
}

void StMcAnalysisMaker::getDca(StTrack const* const rcTrack, float& dca, float& dcaXY, float& dcaZ) const
{
  StPhysicalHelixD helix = rcTrack->geometry()->helix();
  dca = helix.distance(mEvent->primaryVertex()->position());
  dcaXY = helix.geometricSignedDistance(mEvent->primaryVertex()->position().x(), mEvent->primaryVertex()->position().y());

  StThreeVectorF dcaPoint = helix.at(helix.pathLength(mEvent->primaryVertex()->position()));
  dcaZ = dcaPoint.z() - mEvent->primaryVertex()->position().z();
}

StMcTrack const* StMcAnalysisMaker::findPartner(StGlobalTrack* rcTrack, int& maxCommonTpcHits) const
{
  //.. StGlobalTracks find partner from StMcTracks.
  //.. See example from StRoot/StMcAnalysisMaker
  pair<rcTrackMapIter, rcTrackMapIter> p = mAssoc->rcTrackMap()->equal_range(rcTrack);

  const StMcTrack* maxTrack = 0;
  maxCommonTpcHits = 0;
  for (rcTrackMapIter k = p.first; k != p.second; ++k)
  {
    int commonTpcHits = k->second->commonTpcHits();

    const StMcTrack* track = k->second->partnerMcTrack();

    if (track && commonTpcHits > maxCommonTpcHits)
    {
      maxTrack = track;
      maxCommonTpcHits = commonTpcHits;
    }
  }
  return maxTrack;
}

bool StMcAnalysisMaker::checkTheDecayChannel()
{
  std::map<int , int> motherIdxMap;
  std::vector<std::vector<int>> decayChain;
  int NumPi0=0;
  for (unsigned int iTrk = 0;  iTrk < mMcEvent->tracks().size(); ++iTrk)
  {
    StMcTrack* const mcTrack = mMcEvent->tracks()[iTrk];
    if (!mcTrack)
    {
      LOG_WARN << "Empty mcTrack container" << endm;
      continue;
    }
    int particle = mcTrack->geantId();
    if (particle == McAnaCuts::parentId) //pi0
    {
      motherIdxMap.insert(std::pair<int,int>( mcTrack->key() ,NumPi0 ));
      NumPi0++;
      std::vector<int> vpi0;
      vpi0.push_back(mcTrack->key());
      decayChain.push_back(vpi0);
    }
    else 
    {
      StMcTrack* mother = mcTrack->parent();
      if (!mother) {
        LOG_FATAL<<"Have particles not pi0 or from pi0 decay???" << endm;
        continue;
      }
      if (mother->geantId()==McAnaCuts::parentId)
      {
        //here skip check, as mother mctrack store befor the daughter.
        decayChain[motherIdxMap[mother->key()]].push_back(mcTrack->key()); 
      }
      else 
      {
        StMcTrack* grandmother = mcTrack->parent()->parent();
        if (!grandmother)
        {
          LOG_FATAL<<"Have particles not pi0 or from pi0 decay???" << endm;
          continue;
        }    
        if (grandmother->geantId() == McAnaCuts::parentId)
        {
          decayChain[motherIdxMap[grandmother->key()]].push_back(mcTrack->key()); 
        }
        else 
        {
          LOG_WARN<< "Have particles neither from pi0->e or pi0->others->e ???" <<endl;
        }
      }
    }
  }

  //print all of the information
  cout<<" print all of the decay channel..." << endl;
  for (int ipi0=0;ipi0<decayChain.size();ipi0++)
  {
    cout <<"print the No. "<< ipi0 << " pi0 decay  ";
    for (int ip=0;ip<decayChain[ipi0].size();ip++)
    {
      cout <<decayChain[ipi0][ip] <<" "; 
    } 
    cout<<endl;
  }
  motherIdxMap.clear();
  for (int i=0;i<decayChain.size();i++)
  {
    decayChain[i].clear();
  }
  decayChain.clear();
  return 1;
}

int StMcAnalysisMaker::Finish()
{
  mFile->cd();
  mFile->Write();
  mFile->Close();
  return kStOk;
}

int StMcAnalysisMaker::getCentralityBin(float vz,int runId,float mult,float &weight,double &refmultcor) const
{
  float mult_corr = mult;
  // if trigger 580001, correct the refmult
  mult+=gRandom->Rndm();
  if (runId<18156031) {
    float fvz = 0;
    for (int i=0;i<McAnaCuts::nparVz_mult;i++){
      fvz +=McAnaCuts::parVz_mult[i]*std::pow(vz,i);
    }
    mult=mult*1.0*McAnaCuts::parVz_mult[0]/fvz;
  }
  refmultcor=mult;
  weight = reweight(mult);
  for (int cent=0;cent<McAnaCuts::nCent;cent++)
  {
    if (mult<McAnaCuts::Refmult_cent[cent]) return cent-1;
  }
  return McAnaCuts::nCent-1;
}

float StMcAnalysisMaker::reweight(float x) const
{
  x+=gRandom->Rndm();
  // float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  float p[5] = {0.811,238.9,24.31,-25,6.325e-5};
  // return 1;
  if (x>70) return 1;
  // else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]);
}
