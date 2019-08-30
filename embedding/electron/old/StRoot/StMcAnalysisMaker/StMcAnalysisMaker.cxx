#include "TFile.h"
#include "TH3F.h"
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
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuDebug.h"

#include "StMcEvent/StMcEventTypes.hh"
#include "StMcEvent/StMcContainers.hh"
#include "StMcEvent/StMcVertex.hh"
#include "StMcEvent/StMcEvent.hh"

#include "StAssociationMaker/StAssociationMaker.h"
#include "StAssociationMaker/StTrackPairInfo.hh"

#include "StRefMultCorr/StRefMultCorr.h"
#include "StMcAnaCuts.h"
#include "StMcAnalysisMaker.h"

ClassImp(StMcAnalysisMaker);

StMcAnalysisMaker::StMcAnalysisMaker(const char *name, const char *title): StMaker(name),
   mGRefMultCorrUtil(NULL), mMuDst(NULL), mField(-999), mCentrality(-999), mFillTpcHitsNtuple(false), 
   mFile(NULL), mTracks(NULL), mEventCount(NULL), mMcEvent(NULL), mEvent(NULL), mAssoc(NULL)
{
   LOG_INFO << "StMcAnalysisMaker() - DONE" << endm;
}
//__________________________________
int StMcAnalysisMaker::Init()
{
   if (!mOutfileName.Length())
   {
      // StBFChain* bfChain = (StBFChain *) StMaker::GetChain();
      //
      // if (!bfChain) return kStFatal;
      //
      // mOutfileName = bfChain->GetFileIn();

      if (mOutfileName.Length())
      {
         LOG_INFO << mOutfileName << endm;
         mOutfileName = gSystem->BaseName(mOutfileName.Data());
         mOutfileName = mOutfileName.ReplaceAll(".McEvent.root", "");
         mOutfileName = mOutfileName.ReplaceAll(".geant.root", "");
         mOutfileName = mOutfileName.ReplaceAll(".MuDst.root", "");
      }
      else
      {
         mOutfileName = "mcAnalysis";
      }
   }

   mOutfileName = mOutfileName.ReplaceAll(".root", "");
   mFile = new TFile(Form("%s.McAna.root", mOutfileName.Data()), "recreate");
   assert(mFile && !mFile->IsZombie());
   
   for (int ii = 0; ii < McAnaCuts::maxNumberOfTriggers; ++ii)
   {
      firedTriggersIndices.push_back(-1);
   };

   mEventCount = new TNtuple("eventCount", "eventCount", "runId:eventId:mcVx:mcVy:mcVz:vx:vy:vz:vzVpd:"
                             "centrality:gRefMult:posRefMult:negRefMult:zdc:bbc:nMcTracks:nRTracks:magField:t0:t1:t2:t3:t4:t5");

   mTracks = new TNtuple("tracks", "", "centrality:pt:p:eta:y:phi:geantId:eventGenLabel:startVtxX:startVtxY:startVtxZ:stopVtxX:stopVtxY:stopVtxZ:" // MC
                         "gPt:gEta:gPhi:nFit:nMax:nCom:nDedx:dedx:nSigPi:nSigK:dca:dcaXY:dcaZ:hftTopo:hftTruth:trkMap0:trkMap1"); // global
	
	//hRc = new TH3F();
	//hMc = new TH3F();

   LOG_INFO << "Init() - DONE" << endm;

   return kStOk;
}

//__________________________________
int StMcAnalysisMaker::Make()
{
   StMuDstMaker* muDstMaker = (StMuDstMaker*)GetMaker("MuDst");

   if (!muDstMaker)
   {
      LOG_WARN << " No MuDstMaker, will try to take all event information from StEvent" << endm;
      return kStWarn;
      //mMuDst = NULL;
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

   mMuEvent = (StMuEvent*) mMuDst->event();

   
   mField = (float)mMuEvent->eventSummary().magneticField();

   if(mGRefMultCorrUtil && mMuDst)
   {
     mGRefMultCorrUtil->init(mMuEvent->runId());

     mGRefMultCorrUtil->initEvent(mMuEvent->refMult(),
                                  mMuEvent->primaryVertexPosition().z(), 
                                  mMuEvent->runInfo().zdcCoincidenceRate());

     mCentrality  = mGRefMultCorrUtil->getCentralityBin9();

     if (mGRefMultCorrUtil->isBadRun(mMuEvent->runId()))
     {
       LOG_INFO << "This is a bad run from mGRefMultCorrUtil! Skip! " << endm;

       return kStSkip;
     }
   }
   else
   {
     mCentrality = -999;
   }

   // Fill
   int nRTracks = -1;
   int nMcTracks = -1;

   int fillTracksStatus = kStOk;
   if (passTrigger())
   {
      fillTracksStatus = fillTracks(nRTracks, nMcTracks);
   }
   else
   {
      LOG_INFO << "No interesting triggers. Counting event then skipping." << endm;
   }

   int fillEventCountStatus = fillEventCounts((float)nRTracks, (float)nMcTracks);

   return fillTracksStatus && fillEventCountStatus;
}

int StMcAnalysisMaker::fillTracks(int& nRTracks, int& nMcTracks)
{
    nRTracks = 0;
    nMcTracks = 0;

   // LOG_INFO << "Filling " << mMcEvent->tracks().size() << " mcTracks..." << "\n";
	
	  TClonesArray *mcTracks = mMuDst->mcArray(MCTrack);
    int TOT_McTracks = mcTracks->GetEntriesFast();
	
	  StMuTrack* muTrack; 
    int TOT_Tracks;
    TOT_Tracks= mMuDst->globalTracks()->GetEntries();
    cout<<"Global track # = "<<TOT_Tracks<<endl;
	
	  //int const NUM = TOT_McTracks;
	  int const NUM = 10000;
	  int idTruthArray[NUM];
	  for (int i=0;i<NUM;i++) idTruthArray[i] = -999;
    for ( int iTrk=0; iTrk < TOT_Tracks; iTrk++){
		muTrack =  (StMuTrack*) mMuDst->globalTracks(iTrk); 
		if(!muTrack) continue;
		int idTruth = muTrack->idTruth();
		if (idTruth<0) continue;
		if (idTruth>TOT_McTracks) continue; //track from real data
		idTruthArray[idTruth] = iTrk;     //store all the RcTracks
		// cout<<"rctrack: "<<iTrk<<"\t"<<idTruth<<endl;
	}
   
    for (int iTrk = 0;  iTrk < TOT_McTracks; ++iTrk)
    {
       StMuMcTrack* const mcTrack = (StMuMcTrack *)mcTracks->UncheckedAt(iTrk);
		
       if (!mcTrack)
       {
          LOG_WARN << "Empty mcTrack container" << endm;
         continue;
       }
	   // if (iTrk!=mcTrack->Id()) {
		   // LOG_WARN << "idTruth is not the corresponding McTrack" << endm;
		//  return kStWarn;
	   // }
       if (!isGoodMcTrack(mcTrack)) continue;
       ++nMcTracks;
       float array[220];
       for (int ii = 0; ii < 220; ++ii) array[ii] = -999;

       int idx = 0;
       array[idx++] = mCentrality;

       fillMcTrack(array, idx, mcTrack);
	   
	  //as we donot have stevent file, so Check from mutrack  
      int nCommonHits = 0;
      if (idTruthArray[mcTrack->Id()]<0){
        mTracks->Fill(array);
        continue;
      }
      StMuTrack const* const rcTrack = (StMuTrack*) mMuDst->globalTracks(idTruthArray[mcTrack->Id()]);
	  // cout<<"Mctrack: "<<idTruthArray[mcTrack->Id()]<<"\t"<<mcTrack->Id()<<" geantid:"<<mcTrack->GePid()<<endl;
      if (rcTrack){
          ++nRTracks;
         fillRcTrack(array, idx, mcTrack, rcTrack);
         // if (mFillTpcHitsNtuple) fillTpcNtuple(mcTrack, rcTrack);  //skip
       }

       mTracks->Fill(array);
    }

   LOG_INFO << endm;

   return kStOk;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StMcAnalysisMaker::fillMcTrack(float* array, int& idx, StMuMcTrack const* const mcTrk)
{
   array[idx++] = mcTrk->pT();
   array[idx++] = mcTrk->Pxyz().mag();
   array[idx++] = mcTrk->Eta();
   array[idx++] = mcTrk->Rapidity();
   array[idx++] = mcTrk->Pxyz().phi();
   array[idx++] = mcTrk->GePid();
   // array[idx++] = mcTrk->eventGenLabel();
   array[idx++] = -999;
   array[idx++] = mcTrk->IdVx();
   array[idx++] = mcTrk->IdVx();
   array[idx++] = mcTrk->IdVx();
   // array[idx++] = mcTrk->startVertex()->position().y();
   // array[idx++] = mcTrk->startVertex()->position().z();

   if (mcTrk->IdVxEnd())
   {
      // array[idx++] = mcTrk->stopVertex()->position().x();
      // array[idx++] = mcTrk->stopVertex()->position().y();
      // array[idx++] = mcTrk->stopVertex()->position().z();
      array[idx++] = mcTrk->IdVxEnd();   //just tot fill the id only
      array[idx++] = mcTrk->IdVxEnd();
      array[idx++] = mcTrk->IdVxEnd();
   }
   else
   {
      idx += 3;
   }
}

void StMcAnalysisMaker::fillRcTrack(float* array, int& idx, StMuMcTrack const* const mcTrack, StMuTrack const* const rcTrack)
{
   if (!rcTrack) return;

   array[idx++] = rcTrack->momentum().perp();
   array[idx++] = rcTrack->momentum().pseudoRapidity();
   array[idx++] = rcTrack->momentum().phi();
   // array[idx++] = rcTrack->fitTraits().numberOfFitPoints(kTpcId);
   array[idx++] = rcTrack->nHitsFit(kTpcId);  //tpc
   // array[idx++] = rcTrack->nHitsFit();  //tpc
   
   // array[idx++] = rcTrack->numberOfPossiblePoints(kTpcId);
   array[idx++] = rcTrack->nHitsPoss(kTpcId);
   // array[idx++] = rcTrack->nHitsPoss();
   array[idx++] = rcTrack->nHits();     //should be nCommonHits

   // dedx info
   float nDedxPts = -9999;
   float dedx = -9999;
   float nSigPi = -9999;
   float nSigK = -9999;
   nSigPi = rcTrack->nSigmaPion();
   nSigK = rcTrack->nSigmaKaon();
   
   // static StTpcDedxPidAlgorithm aplus(McAnaCuts::dedxMethod);
   // static StPionPlus* Pion = StPionPlus::instance();
   // static StKaonPlus* Kaon = StKaonPlus::instance();
   // StParticleDefinition const* prtcl = rcTrack->pidTraits(aplus);

   // if (prtcl)
   // {
      // nDedxPts = aplus.traits()->numberOfPoints();
      // dedx = aplus.traits()->mean();
      // nSigPi = aplus.numberOfSigma(Pion);
      // nSigK = aplus.numberOfSigma(Kaon);
   // }

   array[idx++] = rcTrack->nHitsDedx();
//   array[idx++] = getNHitsDedx(rcTrack);
   array[idx++] = rcTrack->dEdx();      //dedx
   array[idx++] = nSigPi;
   array[idx++] = nSigK;

   unsigned int hftTruth = 999;
   float dca = -999.;
   float dcaXY = -999.;
   float dcaZ = -999.;

   hftTruth = getHftTruth(mcTrack, rcTrack);
   getDca(rcTrack, dca, dcaXY, dcaZ);

   array[idx++] = dca;
   array[idx++] = dcaXY;
   array[idx++] = dcaZ;
   array[idx++] = rcTrack->topologyMap().data(0) >> 1 & 0x7F;
   array[idx++] = hftTruth;
   array[idx++] = rcTrack->topologyMap().data(0);
   array[idx++] = rcTrack->topologyMap().data(1);
}

bool StMcAnalysisMaker::isGoodMcTrack(StMuMcTrack const* const mcTrack) const
{
   // return mcTrack->GePid() == McAnaCuts::geantId && mcTrack->startVertex()->position().perp() < McAnaCuts::mcTrackStartVtxR;
    return mcTrack->GePid() == McAnaCuts::geantId && mcTrack->IdVx()==1; 
}

int StMcAnalysisMaker::fillEventCounts(float nRTracks, float nMcTracks)
{
   float vars[50];

   float vpdVz = -999;
   StBTofHeader* tofheader = 0;
   // if (mEvent->btofCollection())  tofheader = mEvent->btofCollection()->tofHeader();
   // if (tofheader) vpdVz = tofheader->vpdVz();
	vpdVz = mMuEvent->vpdVz();
   int iVar = 0;
   vars[iVar++] = (float)mMuEvent->runId();
   vars[iVar++] = (float)mMuEvent->eventId();
   // vars[iVar++] = (float)mMcEvent->primaryVertex()->position().x();
   // vars[iVar++] = (float)mMcEvent->primaryVertex()->position().y();
   // vars[iVar++] = (float)mMcEvent->primaryVertex()->position().z();
   vars[iVar++] = -999;
   vars[iVar++] = -999;
   vars[iVar++] = -999;
   vars[iVar++] = (float)mMuEvent->primaryVertexPosition().x();
   vars[iVar++] = (float)mMuEvent->primaryVertexPosition().y();
   vars[iVar++] = (float)mMuEvent->primaryVertexPosition().z();
   vars[iVar++] = vpdVz;
   vars[iVar++] = mCentrality;
   vars[iVar++] = mMuDst? mMuDst->event()->grefmult() : -999;
   vars[iVar++] = (float)mMuEvent->refMultPos();
   vars[iVar++] = (float)mMuEvent->refMultNeg();
   vars[iVar++] = (float)mMuEvent->runInfo().zdcCoincidenceRate();
   vars[iVar++] = (float)mMuEvent->runInfo().bbcCoincidenceRate();
   vars[iVar++] = nMcTracks;
   vars[iVar++] = nRTracks;
   vars[iVar++] = (float)mMuEvent->eventSummary().magneticField() / 10;
   vars[iVar++] = firedTriggersIndices.at(0);
   vars[iVar++] = firedTriggersIndices.at(1);

   mEventCount->Fill(vars);

   return kStOk;
}

bool StMcAnalysisMaker::passTrigger()
{
   LOG_INFO << "Checking triggers..." << endm;
   bool interesting_event = false;

   if (!mMuEvent)
   {
      LOG_FATAL << "mMuEvent doesn't exist" << endm;
   }

   if (McAnaCuts::interesting_triggers.size() == 0)
   {
      LOG_WARN << "No triggers in McAnaCuts::interesting_triggers ... accepting event anyway" << endm;
      return true;
   }

   const StTriggerId st_trgid = mMuEvent->triggerIdCollection().nominal();

   for (unsigned int ii = 0; ii < firedTriggersIndices.size(); ++ii)
   {
      firedTriggersIndices[ii] = -1;
   }

   // Fill interesting triggers
   LOG_INFO << "Interesting fired triggers: " << "\n";

   for (unsigned int ii = 0; ii < st_trgid.maxTriggerIds(); ++ii)
   {
      unsigned int id = st_trgid.triggerId(ii);

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


void StMcAnalysisMaker::getDca(StMuTrack const* const rcTrack, float& dca, float& dcaXY, float& dcaZ) const
{
   StPhysicalHelixD helix = rcTrack->helix();
   dca = helix.distance(mMuEvent->primaryVertexPosition());
   dcaXY = helix.geometricSignedDistance(mMuEvent->primaryVertexPosition().x(), mMuEvent->primaryVertexPosition().y());

   StThreeVectorF dcaPoint = helix.at(helix.pathLength(mMuEvent->primaryVertexPosition()));
   dcaZ = dcaPoint.z() - mMuEvent->primaryVertexPosition().z();
}

unsigned int StMcAnalysisMaker::getHftTruth(StMuMcTrack const* const mcTrack, StMuTrack const* const rcTrack) const
{
	//could be gotten from mMuEvent
   // bool istTruth = true;
   // bool pxlTruth1 = true;
   // bool pxlTruth2 = true;
   // pxlTruth1 = mMuEvent->numberOfPxlInnerHits()>0?true:false;
   // pxlTruth2 = mMuEvent->numberOfPxlOuterHits()>0?true:false;
   // istTruth = mMuEvent->numberOfIstHits()>0?true:false;
   // bool ssdTruth = mMuEvent->numberOfSsdHits()>0?true:false;
   
   // StPtrVecHit rcIstHits = rcTrack->detectorInfo()->hits(kIstId);
   // int nRcIstHits = (int)rcIstHits.size();
   // for (int iHit = 0; iHit < nRcIstHits; ++iHit)
   // {
      // if (rcIstHits[iHit]->idTruth() != mcTrack->key())
      // {
         // istTruth = false;
         // break;
      // }
   // }

   // StPtrVecHit rcPxlHits = rcTrack->detectorInfo()->hits(kPxlId);
   // int nRcPxlHits = (int)rcPxlHits.size();
   // for (int iHit = 0; iHit < nRcPxlHits; ++iHit)
   // {
      // if (rcPxlHits[iHit]->idTruth() != mcTrack->key())
      // {
         // StThreeVectorF pos = rcPxlHits[iHit]->position();

         // float const R = pow(pos.x(), 2.0) + pow(pos.y(), 2.0);
         // if (R > 3.5 * 3.5) pxlTruth2 = false;
         // else pxlTruth1 = false;
      // }
   // }

   unsigned int hftTruth = 0;
   // if (pxlTruth1) hftTruth |= (1 << 0);
   // if (pxlTruth2) hftTruth |= (1 << 1);
   // if (istTruth)  hftTruth |= (1 << 2);
 //  hftTruth = rcTrack->nGoodHft();
   return hftTruth;
}


int StMcAnalysisMaker::Finish()
{
   mFile->cd();
   mFile->Write();
   mFile->Close();
   return kStOk;
}

