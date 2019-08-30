#include "TFile.h"
// #include "TH3F.h"
#include "TNtuple.h"
#include "TSystem.h"

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
   mGRefMultCorrUtil = new StRefMultCorr("refmult");
   mGRefMultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
   mOutfileName = mOutfileName.ReplaceAll(".root", "");
   mFile = new TFile(Form("%s.McAna.root", mOutfileName.Data()), "recreate");
   assert(mFile && !mFile->IsZombie());
      
   for (int ii = 0; ii < McAnaCuts::maxNumberOfTriggers; ++ii)
   {
      firedTriggersIndices.push_back(-1);
   };

   hnHitsvsPt = new TH2F("hnHits","hnHits;nHits;p_{T}",55,0,55,10,0,5); 
   hDCAvsPt = new TH2F("hDCA","hDCA;DCA;p_{T}",100,0,3,10,0,5);
   hnPi0 = new TH1F("hnPi0","hnPi0",100,0,100); 
   hRefmult = new TH1F("hRefmult","hRefmult",500,0,500);
   hCentrality = new TH1F("hCentrality","hCentrality",9,-0.5,8.5);
   hPi2tot = new TH1F("hPi2tot","hPi2tot",30,0,0.3);
   hPi0Pt = new TH1F("hPi0Pt","hPi0Pt;pt;#eta;#phi",120,0,15);
   hMcElectronPtvsCent = new TH2F("hMcElectronPtvsCent","hMcElectron;p_{T};Cent",80,0,4,9,-0.5,8.5);
   hRcElectronPtvsCent = new TH2F("hRcElectronPtvsCent","hRcElectron;p_{T};Cent",80,0,4,9,-0.5,8.5);
   LOG_INFO << "Init() - DONE" << endm;
   TFile * fweight = new TFile("hMc.root");
   hweight = (TH1F*)fweight->Get("hMcElectronPtvsCent_px"); 
   hweight->SetDirectory(0);
   return kStOk;
}

//__________________________________
int StMcAnalysisMaker::Make()
{
   cout<< "start new event.."<<endl;
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
   cout<< "load event Mc event finish..."<<endl;
   cout<<"Get StAssociationMaker" <<endl;
   mAssoc = (StAssociationMaker*)GetMaker("StAssociationMaker");
   if (!mAssoc)
   {
      cout << "Could not get StAssociationMaker" << endl;
      return kStErr;
   }
   else if (mAssoc) {
        if (!(mAssoc->rcTpcHitMap()&&
              mAssoc->rcTrackMap()&&
              mAssoc->mcTrackMap()))
          {
            cout<<"Could not get StAssociationMaker maps" << endl;
            return kStErr;
          }
   }

   mField = (float)mEvent->summary()->magneticField();
   cout<<"start refmult"<<endl;
   if(mGRefMultCorrUtil && mMuDst)
   {
     mGRefMultCorrUtil->init(mEvent->runId());
     mGRefMultCorrUtil->initEvent(mMuDst->event()->refMult(),
                                  mMuDst->event()->primaryVertexPosition().z()); 
                                  // mMuDst->event()->primaryVertexPosition().z(), 
                                  // mMuDst->event()->runInfo().zdcCoincidenceRate() );

     if (mGRefMultCorrUtil->isBadRun(mEvent->runId()))
     {
       LOG_INFO << "This is a bad run from mGRefMultCorrUtil! Skip! " << endm;
       return kStSkip;
     }
     mCentrality  = mGRefMultCorrUtil->getCentralityBin9();
     // cout << " centrality "<< mCentrality << " refMult "<<  mMuDst->event()->refMult() << " "<< mMuDst->event()->primaryVertexPosition().z()<<" "<< mGRefMultCorrUtil->getWeight()<<endl;
   }
   else
   {
     mCentrality = -999;
   }
   // if (mCentrality<0) return kStSkip;
   cout<<"refmult finish..." <<endl;
   hRefmult->Fill(mMuDst->event()->refMult()); 
   hCentrality->Fill(mCentrality, mGRefMultCorrUtil->getWeight());
   cout << "check consistent: event: "<<mEvent->id() << " "<< mMuDst->event()->eventId()<< " run: "<< mMuDst->event()->runId()<<" "<<mEvent->runId()<<endl;
   // Fill
   int nRTracks = -1;
   int nMcTracks = -1;

   cout<< "check trigger and fill track..."<<endl;
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
   cout <<"fillMcTrack" <<endl;
   std::map<int,int> mc_rcpair;
   // std::vector<long> rckey;
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
      // ++nRTracks;
      if (!rcTrack || nCom<=10) {
        mc_rcpair.insert(std::pair<int,int>(mcTrack->key(),-999));
      }
      else {
        // ++nRTracks;
        mc_rcpair.insert(std::pair<int,int>(mcTrack->key(),rcTrack->key()));
        // rckey.push_back(rcTrack->key());
        // cout <<"mcTrack key: "<< mcTrack->key() << " "<< mcTrack->geantId()<<" ";
        // cout<< "paired rc: "<<rcTrack->key()<< " idtruth "<< rcTrack->idTruth()<<" qatruth: "<< rcTrack->qaTruth()<< "nCom "<< nCom<<endl;
      } 
      // cout<< "paired rc: "<<rcTrack->idTruth()<< " mc: "<<mcTrack->key() <<endl;
   }
   hnPi0->Fill(nPi0);
   hPi2tot->Fill(1.*nPi0/mMuDst->event()->refMult()); 

   cout <<"fillRcTrack..." <<endl;
   for (unsigned int ircTrk = 0;ircTrk<mMuDst->globalTracks()->GetEntries();ircTrk++)
   {
      StMuTrack* rcTrack = mMuDst->globalTracks(ircTrk);
      if (!rcTrack) {
        LOG_WARN<<" no rcTrack input! "<< endm;
        continue;
      }
      int idTruth = rcTrack->idTruth();
      // bool isfromMcTrack =  (idTruth>nPi0 && idTruth<=mMcEvent->tracks().size()); 
      bool isfromMcTrack =  (idTruth>0 && idTruth<=(mMcEvent->tracks().size())); 
      if (!isfromMcTrack) continue;
      ++nRTracks;
      // cout<< "idTruth " << idTruth<< endl;
      if (mc_rcpair[idTruth]!=rcTrack->id()) {
        cout<<"duplicated rctrack !" << endl;
        continue;
      }
      StMcTrack* const mcTrack = mMcEvent->tracks()[idTruth-1];
      if (!mcTrack) cout<<"idTruth is not correct!" << endl;
      if (mcTrack->geantId()!=2 &&  mcTrack->geantId()!=3) continue;
      fillRcTrack(rcTrack,mcTrack);
   }
    mc_rcpair.clear();
   //  StSPtrVecTrackNode& trackNode = mEvent->trackNodes();
  // int nTracks = trackNode.size();
  // for (int i=0; i < nTracks; i++) {
  //   StTrackNode *node = trackNode[i];
  //   if (!node) continue;
  //   StTrack *glTrack = node->track(global);
  //   if (! glTrack) continue;
  //   bool isfromMcTrack = (glTrack->idTruth()>0 && glTrack->idTruth()<=mMcEvent->tracks().size());
  //   if (!isfromMcTrack) continue;
  //   ++nRTracks;
  //   StMcTrack* const mcTrack2 = mMcEvent->tracks()[glTrack->idTruth()-1];
  //   // if (!mcTrack2) cout<<"idTruth is not correct!" << endl;
  //   cout <<"id truth " <<glTrack->idTruth()<<endl;
  //
  // }
   cout<<"MC Tracks:  " <<nMcTracks << " RC tracks: "<< nRTracks <<endl;
   // LOG_INFO << endm;
   return kStOk;
}

void StMcAnalysisMaker::fillMcTrack(StMcTrack const* const mcTrk)
{
   // cout << " parent  "<< mcTrk->parent()->geantId() << endl;
   if (mcTrk->geantId()==10007) 
   {
      // cout << " test if the key is the same "<< mMcEvent->tracks()[mcTrk->key()-1]->key() << " " << mcTrk->key()<<endl; 
      nPi0++;    
      hPi0Pt->Fill(mcTrk->pt());
   } 
   else 
   {
      if ( mcTrk->geantId() == 2 ||  mcTrk->geantId() == 3)
      {
        // cout <<" test the mother key "<<mcTrk->key() <<" " << mcTrk->parent()->key()<<endl;; 
        if (fabs(mcTrk->momentum().pseudoRapidity())<1)
          // hMcElectronPtvsCent->Fill(mcTrk->momentum().perp(),mCentrality,hweight->GetBinContent(hweight->FindBin(mcTrk->pt())));
          // hMcElectronPtvsCent->Fill(mcTrk->momentum().perp(),mCentrality,mGRefMultCorrUtil->getWeight());
          hMcElectronPtvsCent->Fill(mcTrk->momentum().perp(),mCentrality);
        // cout << "check if pt is correct "<< mcTrk->momentum().perp() - mcTrk->pt()<<endl;
      }
   }
}

void StMcAnalysisMaker::fillRcTrack( StMuTrack const* const rcTrack,StMcTrack const * const mcTrack)
{
   float dca = rcTrack->dcaGlobal().mag();
   int nHitsFit = rcTrack->nHitsFit();
   int nHitsMax = rcTrack->nHitsPoss();
   bool passnHits = fabs(nHitsFit) >20 && fabs(1.*nHitsFit/(1.*nHitsMax))>0.52;
   // cout<<"check nHitsfit: " <<nHitsFit<<" "<<nHitsMax<< " "<< fabs(1.*nHitsFit/(1.*nHitsMax))<<endl;
   // cout << (nHitsFit >20)<<" "<< (nHitsFit/(1.*nHitsMax) > 0.52)  <<endl;
   // cout <<" nSigmaE "<<  rcTrack->nSigmaElectron()  <<endl;
   if (passnHits) hDCAvsPt->Fill(dca,rcTrack->pt());  
   bool goodRcTrack = passnHits && fabs(dca)<McAnaCuts::gDCA &&  fabs(rcTrack->eta())<1 && rcTrack->pt()>McAnaCuts::minPt;
   if (!goodRcTrack) return; 
   hnHitsvsPt->Fill(nHitsFit,rcTrack->pt()); 
   // hRcElectronPtvsCent->Fill(rcTrack->pt(),mCentrality,hweight->GetBinContent(hweight->FindBin(mcTrack->pt())));
   // hRcElectronPtvsCent->Fill(rcTrack->pt(),mCentrality,mGRefMultCorrUtil->getWeight());
   hRcElectronPtvsCent->Fill(rcTrack->pt(),mCentrality);
   // cout << "check if pt is correct "<< rcTrack->momentum().perp() - rcTrack->pt()<<endl;
}

bool StMcAnalysisMaker::isGoodMcTrack(StMcTrack const* const mcTrack) const
{
     // return 
     // mcTrack->geantId() == McAnaCuts::geantId &&
     // mcTrack->startVertex()->position().perp() < McAnaCuts::mcTrackStartVtxR;
     // return true;
     return (mcTrack->geantId() == 2 || mcTrack->geantId() == 3) &&
             // mcTrack->pt()>McAnaCuts::minPt; 
             // mcTrack->pt()>McAnaCuts::minPt && 
            mcTrack->startVertex()->position().perp() < McAnaCuts::mcTrackStartVtxR;
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

int StMcAnalysisMaker::Finish()
{
   mFile->cd();
   mFile->Write();
   mFile->Close();
   return kStOk;
}

