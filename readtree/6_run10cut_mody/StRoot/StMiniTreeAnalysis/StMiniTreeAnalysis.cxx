//----2019.0828----
//use the full eta region instead of rm (-0.05,0.05) so as to have a flat eta. I hope the eta gap is enough
#include "StAnaCuts.h"
#include "StMiniTreeAnalysis.h"   //could also include other headers
#include "myTree.h"  // tree

#include "string.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <set>
#include <vector>
#define Debug 0

ClassImp(StMiniTreeAnalysis)
StMiniTreeAnalysis::StMiniTreeAnalysis(TString const  inputfilelist, TString const outfilename):mInputlist(inputfilelist),
mOutputname(outfilename)
{}

//------------------------------------------------------------
StMiniTreeAnalysis::~StMiniTreeAnalysis()
{
}
//------------------------------------------------------------
int StMiniTreeAnalysis::Init()
{
  mOutputname +=".v2.root";
  cout << "output file name" << mOutputname.Data() << endl;
  //add files to the chain
  mChain = new TChain("Tree");
  if (mInputlist.Contains(".root")) 
  {
     mChain->Add(mInputlist.Data());
  }
  else
  {
    int nfile = 0;
    char tmp[2000];
    ifstream readlists;
    readlists.open(mInputlist.Data());
    while (readlists.good()){
      readlists.getline(tmp,2000);
      TFile *ftmp = new TFile(tmp);
      if (!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) {
        cout<<"Could not open this file: "<< tmp  <<endl;
      }
      else {
         if(Debug && nfile%30==0) cout<<"read in "<<nfile<<"th file: "<< tmp <<endl;
          mChain->Add(tmp);
          nfile++;      
        }
    }
  }
  //get bad runs
  setRunMap(mRunNumList,anaCuts::nRunNum);

  bookHists();
  initHists(anaCuts::nRunNum); 
  mOutfile = new TFile(mOutputname.Data(),"recreate");
  mTree = new myTree(mChain);
  return 1;
}
//------------------------------------------------------------
int StMiniTreeAnalysis::Make()
{
  Long64_t nEvents = mChain->GetEntries();
  cout << "total "<<nEvents <<" events"<<endl;
  
  for (Long64_t ievt =0;ievt<nEvents;ievt++){
    if (Debug && ievt%10000==0) cout <<"process "<<ievt <<" events" <<endl;
    if (!Debug && ievt%100000==0) cout <<"process "<<ievt <<" events" <<endl;
    mTree->GetEntry(ievt);
    //add bad runs rejection
    Int_t runId = mTree->runId;
    if (isBadrun(runId)) continue;
    // bool refusepileup = mTree->refMult*2.88-155 < mTree->nTofMult;
    bool refusepileup = mTree->refMult*anaCuts::refusepileup[0]+ anaCuts::refusepileup[1]< mTree->nTofMult* anaCuts::refusepileup[2]+ anaCuts::refusepileup[3];
    bool refusebadtof = !(mTree->refMult<20 &&mTree->nTofMult>250);
    // bool refusebadtof = mTree->refMult *anaCuts::refusebadtof[0]+ anaCuts::refusebadtof[1]> mTree->nTofMult*anaCuts::refusebadtof[2]-anaCuts::refusebadtof[3];
    bool goodevent = fabs(mTree->pVtx_z) < anaCuts::vz &&  
                     fabs(mTree->vzVpdVz) < anaCuts::vzVpdVz &&
                     sqrt(mTree->pVtx_x*mTree->pVtx_x+mTree->pVtx_y*mTree->pVtx_y)<anaCuts::Vr;
    if (!refusepileup) continue; 
    if (!refusebadtof) continue; 
    if (!goodevent) continue;
    // Int_t cent = mTree->centrality;
    // Float_t pVtx_z = mTree->pVtx_z;
    // float ZDCx = mTree->ZDCx;
    float weight=1.;
    // float weight=mTree->weight;
    int cent = getCentralityBin(mTree->pVtx_z,runId,mTree->refMult,weight);
    hcent->Fill(cent);
    hcentwg->Fill(cent,weight);
    float  EP_P_sh=0,EP_M_sh=0;
    // if either side has 0 tracks , 0.1% in 0-60%, 1% in 70-80%
    if (mTree->nQvecP<0.5 || mTree->nQvecM<0.5) continue;
    if (!EventPlane(cent,runId,EP_P_sh,EP_M_sh)) continue;
    // EP_M_sh=mTree->EP_M_sh;
    // EP_P_sh=mTree->EP_P_sh;
    getIncEv2(EP_M_sh,EP_P_sh,cent,weight);
  } //event loop
  return 1;
}
//------------------------------------------------------------
int StMiniTreeAnalysis::EventPlane(int cent, int runId, float &EP_P_sh,float &EP_M_sh)
{
   //I write the wrong name in tree, will change it after group meeting
   //I store the Qx after recenter, if needed can re run the tree
  // the new version after 0630 have been corrected 
   double nQvecP = mTree->nQvecP;
   double nQvecM = mTree->nQvecM;
   
   TVector2 QvecM(mTree->Qx_M,mTree->Qy_M);
   double EP_M = QvecM.Phi()*0.5;
   double Qx_M = mTree->Qx_M-nQvecM*pQxRecenterM[cent]->GetBinContent(runnum[runId]+1);
   double Qy_M = mTree->Qy_M-nQvecM*pQyRecenterM[cent]->GetBinContent(runnum[runId]+1);
   TVector2 QvecM_re(Qx_M,Qy_M);
   double EP_M_re = QvecM_re.Phi()*0.5;
   EP_M_sh = getEventPlaneShift(EP_M_re,-1,cent); 
   if (EP_M_sh<0.) EP_M_sh+=TMath::Pi();
   if (EP_M_sh>TMath::Pi()) EP_M_sh-=TMath::Pi();
   // cout << (EP_M_re-mTree->EP_M_re)<< "  " << (float)EP_M-mTree->EP_M_raw<<" "<<EP_M_sh-mTree->EP_M_sh<<endl;
   if (QvecM.Mod()>0) {
     hEventPlaneCent_M->Fill(cent,EP_M);
     hEventPlaneCent_M_re->Fill(cent,EP_M_re);
     hEventPlaneCent_M_sh->Fill(cent,EP_M_sh);
   }
   TVector2 QvecP(mTree->Qx_P,mTree->Qy_P);
   double EP_P = QvecP.Phi()*0.5;
   double Qx_P = mTree->Qx_P-nQvecP*pQxRecenterP[cent]->GetBinContent(runnum[runId]+1);
   double Qy_P = mTree->Qy_P-nQvecP*pQyRecenterP[cent]->GetBinContent(runnum[runId]+1);
   TVector2 QvecP_re(Qx_P,Qy_P);
   double EP_P_re = QvecP_re.Phi()*0.5;
   EP_P_sh = getEventPlaneShift(EP_P_re,1,cent); 
   if (QvecP.Mod()>0)
   { 
     hEventPlaneCent_P->Fill(cent,EP_P);
     hEventPlaneCent_P_re->Fill(cent,EP_P_re);
     hEventPlaneCent_P_sh->Fill(cent,EP_P_sh);
   }
   double resolution = cos(2.*(EP_P_sh-EP_M_sh));
   EPconsRes->Fill(cent,resolution);
   return 1; 
}
int StMiniTreeAnalysis::getIncEv2(float EP_M_sh,float EP_P_sh,int cent,float weight)
{
  std::map<int,int> incEmap;
  //inclusive electron
  for (int itrk1 = 0;itrk1<mTree->nIncE;itrk1++)
  {
    //in case we need this info in photonic electron
    incEmap.insert(pair<int,int>(mTree->idx_inc[itrk1],itrk1));  
    bool isgoodtrack = fabs(mTree->eta_inc[itrk1])<anaCuts::eEta &&
                       fabs(mTree->gDca_inc[itrk1])<anaCuts::Dca &&
                       fabs(mTree->pt_inc[itrk1])>anaCuts::GPt  &&
                       fabs(mTree->ndEdx_inc[itrk1])>=anaCuts::NHitsDedx &&
                       fabs(mTree->nFit_inc[itrk1])>=anaCuts::NHitsFit;
    if (!isgoodtrack) continue;
    //if not tofmatch beta=-9999 in mini tree
    TVector3 incE;
    incE.SetPtEtaPhi(mTree->pt_inc[itrk1],mTree->eta_inc[itrk1],mTree->phi_inc[itrk1]);

    bool iselectron = isElectron(mTree->nSigE_inc[itrk1],mTree->beta_inc[itrk1],incE.Mag()); 
    // bool passTPChit = mTree->has2hit_inc[itrk1];
    bool passTPChit = mTree->has3hit_inc[itrk1];
    bool passPhiCut = !( fabs(mTree->phi_inc[itrk1])<anaCuts::PhiCut.first && fabs(mTree->phi_inc[itrk1])>anaCuts::PhiCut.second  );
    // float cos2deltaPhi = mTree->cos2phi_inc[itrk1];
    // float deltaPhi = mTree->deltaphi_inc[itrk1];
    float EP=0;
    if (mTree->eta_inc[itrk1]<-0.0) EP = EP_P_sh;
    else if (mTree->eta_inc[itrk1]>0.0) EP = EP_M_sh;
    else continue;
    float phi_1 = mTree->phi_inc[itrk1];
    if (phi_1<0.) phi_1+=2*TMath::Pi();
    double cos2deltaPhi=std::cos(2*phi_1-2*EP);
    float  deltaPhi = phi_1-EP;
    if (deltaPhi<0) deltaPhi+=2*TMath::Pi();
    if (deltaPhi>TMath::Pi()) deltaPhi-=TMath::Pi(); 

    if (iselectron)
    {
      hIncEv2vsPtvsCent->Fill(deltaPhi,mTree->pt_inc[itrk1],cent,weight);
      hIncEPtvsCent->Fill(mTree->pt_inc[itrk1],cent,weight);
      pIncEv2->Fill(mTree->pt_inc[itrk1],cent,cos2deltaPhi/anaCuts::resolution[cent],weight);
      hPhi->Fill(mTree->phi_inc[itrk1],weight);
      if (passTPChit) 
      {
        hPhi_hitcut->Fill(mTree->phi_inc[itrk1],weight);
        hEta->Fill(mTree->eta_inc[itrk1],weight);
      }
      hePtvsP->Fill(mTree->pt_inc[itrk1], incE.Mag());     
      if (passTPChit && passPhiCut){
        pIncEv2_hitcut->Fill(mTree->pt_inc[itrk1],cent,cos2deltaPhi/anaCuts::resolution[cent],weight);
        hIncEv2vsPtvsCent_hitcut->Fill(deltaPhi,mTree->pt_inc[itrk1],cent,weight);
        hIncEPtvsCent_hitcut->Fill(mTree->pt_inc[itrk1],cent,weight);
        hPhi_allcut->Fill(mTree->phi_inc[itrk1],weight);
      }
    }  
  }
  //photonic electron
  for (int itrk2 = 0;itrk2<mTree->nPhoE;itrk2++)
  { 
    TVector3 tagE;
    tagE.SetPtEtaPhi(mTree->pt_phe[itrk2],mTree->eta_phe[itrk2],mTree->phi_phe[itrk2]);
    // bool iselectron = isElectron(mTree->nSigE_phe[itrk2],mTree->beta_phe[itrk2],mTree->pt_phe[itrk2]); 
    bool iselectron = isElectron( mTree->nSigE_phe[itrk2],mTree->beta_phe[itrk2],tagE.Mag()); 
    // bool passTPChit = (mTree->topomap0_phe[itrk2]>>8 & 0x7) && (mTree->topomap0_parte[itrk2]>>8 & 0x7);
    bool passTPChit = (mTree->topomap0_phe[itrk2]>>8 & 0x7) ;
    bool passPhiCut = !( fabs(mTree->phi_phe[itrk2])<anaCuts::PhiCut.first && fabs(mTree->phi_phe[itrk2])>anaCuts::PhiCut.second);
    //this cut is a check for the pi contamination to the partner electron
    //will not apply this cut for the final result
    // bool isTightPidPartE = fabs(mTree->nSigE_parte[itrk2])<2.5; 
    // bool isPartnerElectron = isSecondPE(mTree->nSigE_parte[itrk2],mTree->beta_parte[itrk2],mTree->gpt_parte[itrk2]) && isTightPidPartE; 
    bool isPartnerElectron = isSecondPE(mTree->nSigE_parte[itrk2],mTree->beta_parte[itrk2],mTree->gpt_parte[itrk2]); 
    bool passPEtopocut = mTree->DCA_pair[itrk2] < anaCuts::EEdcaDaughter;
    //
    bool isgoodtrack_PhoE = 
                       fabs(mTree->eta_phe[itrk2])<anaCuts::eEta &&
                       fabs(mTree->gDca_phe[itrk2])<anaCuts::Dca &&
                       fabs(mTree->pt_phe[itrk2])>anaCuts::GPt  &&
                       passPhiCut &&
                       fabs(mTree->ndEdx_phe[itrk2])>=anaCuts::NHitsDedx &&
                       fabs(mTree->nFit_phe[itrk2])>=anaCuts::NHitsFit;

    bool isgoodtrack_PartE = 
                       fabs(mTree->geta_parte[itrk2])<anaCuts::Eta &&   
                       // fabs(mTree->gDca_parte[itrk2])<anaCuts::Dca &&     //only for check
                       // fabs(mTree->gpt_parte[itrk2])>anaCuts::GPt;
                       // fabs(mTree->gpt_parte[itrk2])>anaCuts::GPt  &&
                       fabs(mTree->gpt_parte[itrk2])>anaCuts::GPt_Parte  &&
                       // fabs(mTree->ndEdx_parte[itrk2])>=anaCuts::NHitsDedx &&
                       fabs(mTree->nFit_parte[itrk2]/(1.*mTree->nMax_parte[itrk2])) >= anaCuts::NHitsFit2Poss &&
                       fabs(mTree->nFit_parte[itrk2]) >= anaCuts::nFit_parte;
    
    //keep the track quality is the same as Tag e
    bool isgoodtofmatch_PartE = 
                       fabs(mTree->geta_parte[itrk2])<anaCuts::eEta &&   
                       fabs(mTree->gDca_parte[itrk2])<anaCuts::Dca &&
                       fabs(mTree->gpt_parte[itrk2])>anaCuts::GPt &&
                       fabs(mTree->ndEdx_parte[itrk2])>=anaCuts::NHitsDedx &&
                       fabs(mTree->nFit_parte[itrk2]/(1.*mTree->nMax_parte[itrk2])) >= anaCuts::NHitsFit2Poss &&
                       fabs(mTree->nFit_parte[itrk2]) >= anaCuts::NHitsFit;
    if (!isgoodtrack_PartE || !isgoodtrack_PhoE) continue;
    TLorentzVector mother;
    mother.SetXYZM(mTree->px_pair[itrk2],mTree->py_pair[itrk2],mTree->pz_pair[itrk2],mTree->M_pair[itrk2]);
    // float cos2deltaPhi = mTree->cos2phi_phe[itrk2];
    // float deltaPhi = mTree->deltaphi_phe[itrk2];
    float EP=0;
    if (mTree->eta_phe[itrk2]<-0.0) EP = EP_P_sh;
    else if (mTree->eta_phe[itrk2]>0.0) EP = EP_M_sh;
    else continue;
    float phi = mTree->phi_phe[itrk2];
    double cos2deltaPhi=std::cos(2*phi-2*EP);
    float  deltaPhi = phi-EP;
    if (deltaPhi<0) deltaPhi+=2*TMath::Pi();
    if (deltaPhi>TMath::Pi()) deltaPhi-=TMath::Pi();
    
    TVector3 V0(mTree->V0x_pair[itrk2],mTree->V0y_pair[itrk2],mTree->V0z_pair[itrk2]);

    //check the pair DCA
    if (iselectron && isPartnerElectron && mTree->DCA_pair[itrk2]<3)
    {
      bool unlike = mTree->charge_phe[itrk2] * mTree->charge_parte[itrk2] <0;
      if (mTree->M_pair[itrk2]<0.3) 
      {
         if (mTree->M_pair[itrk2]<0.15 && unlike) {
            hPairDCA->Fill( mTree->DCA_pair[itrk2],mTree->pt_phe[itrk2],cent,weight);     
            hDecayL->Fill( V0.Perp(),mTree->gpt_parte[itrk2] ,weight);
         }
         if ( mTree->M_pair[itrk2]<0.15 && (!unlike)){
            hPairDCALS->Fill( mTree->DCA_pair[itrk2],mTree->pt_phe[itrk2],cent,weight);     
            hDecayL_LS->Fill( V0.Perp(),mTree->gpt_parte[itrk2] ,weight);
         }
      } // mass cut  
    }
    if (iselectron && isPartnerElectron && passPEtopocut)
    {
      bool unlike = mTree->charge_phe[itrk2] * mTree->charge_parte[itrk2] <0;
      if (mTree->M_pair[itrk2]<0.3) 
      {
         if (mTree->M_pair[itrk2]<anaCuts::MassCut && unlike && passTPChit) 
         {
            pTagEv2->Fill(mTree->pt_phe[itrk2],cent,cos2deltaPhi/anaCuts::resolution[cent],weight);
            hPhEv2vsPtvsCent->Fill(deltaPhi,mTree->pt_phe[itrk2],cent,weight);
            hV0->Fill(mTree->V0x_pair[itrk2],mTree->V0y_pair[itrk2],mTree->V0z_pair[itrk2]);
            // if (mTree->M_pair[itrk2]<0.1) hNFitsvsPt->Fill(mTree->gpt_parte[itrk2],mTree->nFit_parte[itrk2],weight);
            hNFitsvsPt->Fill(mTree->gpt_parte[itrk2],mTree->nFit_parte[itrk2],weight);
            hNFitsvsTagPt->Fill(mTree->pt_phe[itrk2],mTree->nFit_parte[itrk2],weight);
            //for tof match calculation
            if (isgoodtofmatch_PartE)
            {
               hPartETpc->Fill(mTree->gpt_parte[itrk2],cent,weight);
               if (mTree->beta_parte[itrk2]>0) 
                 hPartETof->Fill(mTree->gpt_parte[itrk2],cent,weight);
            }

            hPartEptetaphi->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight);
            //partner electron pt eta phi distribution
            if (mTree->decayL_pair[itrk2]<2) 
            {
              hPartEptetaphi_Dz->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight); 
              hDcavsPt_Dz->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
            }
            hDcavsPt->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
            //Tag e pt eta phi?
            //the phi distribution can be well described by embedding, so we focus on eta distribution
            double etafillarray[4]={mTree->pt_phe[itrk2],mTree->gpt_parte[itrk2],mTree->eta_phe[itrk2],mTree->geta_parte[itrk2]};
            hPartETagEetapt->Fill(etafillarray,weight); 

            //for gamma conversion QA
            if (mTree->decayL_pair[itrk2]>3.5) 
            {  
              hPartEptetaphi_Gm->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight); 
              hDcavsPt_Gm->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
            }
         }
         //sideband unlike sign
         // if (mTree->M_pair[itrk2]>0.15 && mTree->M_pair[itrk2]<0.25 && unlike && passTPChit)
         // {
         //    hPartEptetaphi_SB->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight);
         //    //check sideband v2
         // }
         //like sign
         if ( mTree->M_pair[itrk2]<anaCuts::MassCut && (!unlike) && passTPChit)
         {
           pTagEv2_LS->Fill(mTree->pt_phe[itrk2],cent,cos2deltaPhi/anaCuts::resolution[cent],weight);
           hPhEv2vsPtvsCentLS->Fill(deltaPhi,mTree->pt_phe[itrk2],cent,weight);
           hV0_LS->Fill(mTree->V0x_pair[itrk2],mTree->V0y_pair[itrk2],mTree->V0z_pair[itrk2]);
           
           //for tof match calculation
           if (isgoodtofmatch_PartE)
           {
              hPartETpcLS->Fill(mTree->gpt_parte[itrk2],cent,weight);
              if (mTree->beta_parte[itrk2]>0) 
                hPartETofLS->Fill(mTree->gpt_parte[itrk2],cent,weight);
           }

           hPartEptetaphi_LS->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight);
           // if (mTree->M_pair[itrk2]<0.1) hNFitsvsPt_LS->Fill(mTree->gpt_parte[itrk2],mTree->nFit_parte[itrk2]);
           hNFitsvsPt_LS->Fill(mTree->gpt_parte[itrk2],mTree->nFit_parte[itrk2],weight);
           hNFitsvsTagPt_LS->Fill(mTree->pt_phe[itrk2],mTree->nFit_parte[itrk2],weight);
           if (mTree->decayL_pair[itrk2]<2)
           {  
              hPartEptetaphi_Dz_LS->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight);
              hDcavsPt_Dz_LS->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
           }
           hDcavsPt_LS->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
           //tag e eta and part eta distribution
           double etafillarray[4]={mTree->pt_phe[itrk2],mTree->gpt_parte[itrk2],mTree->eta_phe[itrk2],mTree->geta_parte[itrk2]};
           hPartETagEetapt_LS->Fill(etafillarray, weight ); 
           //for gamma conversion QA
           if (mTree->decayL_pair[itrk2]>3.5) 
           {  
              hPartEptetaphi_Gm_LS->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight); 
              hDcavsPt_Gm_LS->Fill(mTree->gpt_parte[itrk2],mTree->gDca_parte[itrk2],weight);
           }
         }
         // if (mTree->M_pair[itrk2]>0.15 && mTree->M_pair[itrk2]<0.25 && !unlike && passTPChit)
         // {
         //    hPartEptetaphi_SB_LS->Fill(mTree->gpt_parte[itrk2],mTree->geta_parte[itrk2],mTree->gphi_parte[itrk2] ,weight);
         //    //check sideband v2
         // }
         if (unlike) {
           hphoto->Fill(mTree->M_pair[itrk2],mTree->pt_phe[itrk2], cent,weight); 
           if (passTPChit) hphoto_hitcut->Fill(mTree->M_pair[itrk2],mTree->pt_phe[itrk2], cent,weight); 
           hphotoVsPt->Fill(mTree->M_pair[itrk2],mother.Perp(),cent,weight);
         }
         else {
           hphoto_LS->Fill(mTree->M_pair[itrk2],mTree->pt_phe[itrk2],cent,weight); 
           if (passTPChit) hphoto_LS_hitcut->Fill(mTree->M_pair[itrk2],mTree->pt_phe[itrk2], cent,weight); 
           hphotoVsPt_LS->Fill(mTree->M_pair[itrk2],mother.Perp(),cent,weight);
         }  
      } // mass cut  
    }   // is electron and pass dca_pair cut
  }
  return 1;
}
//------------------------------------------------------------
int StMiniTreeAnalysis::Finish()
{
  for (int ic=0;ic<9;ic++)
  {
    delete pQxRecenterM[ic];
    delete pQyRecenterM[ic];
    delete pQxRecenterP[ic];
    delete pQyRecenterP[ic];
  }
  for (int ic=0;ic<9;ic++)
  {
    delete pSinEtaM[ic];
    delete pCosEtaM[ic];
    delete pSinEtaP[ic];
    delete pCosEtaP[ic];
  }

  //make write easier...
  WriteHists(mOutfile);
  // mOutfile->Write();
  mOutfile->Close();
  delete mChain;
  mChain=NULL;
  return 1;
}
//------------------------------------------------------------
void StMiniTreeAnalysis::bookHists()
{
  TFile* file = TFile::Open(mRecenterFile.c_str());
  if (!file->IsOpen()){
    cout<<"Error: Cannot read recenter file!!!"<<endl;
    return;
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
    return;
  }
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
  file->Close();
}
//------------------------------------------------------------
void StMiniTreeAnalysis::initHists(int nRunNum)
{
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  float PI = TMath::Pi();
  hEventPlaneCent_M = new TH2F("hEventPlaneCent_M_raw","hEventPlaneCent",9,0,9,180,0,PI);
  hEventPlaneCent_P = new TH2F("hEventPlaneCent_P_raw","hEventPlaneCent",9,0,9,180,0,PI);
  hEventPlaneCent_M_re = new TH2F("hEventPlaneCent_M_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  hEventPlaneCent_M_sh = new TH2F("hEventPlaneCent_M_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  hEventPlaneCent_P_re = new TH2F("hEventPlaneCent_P_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  hEventPlaneCent_P_sh = new TH2F("hEventPlaneCent_P_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  EPconsRes = new TProfile("EPRes","Event Plane resolution;Cent;Res",9,-0.5,8.5);
  hIncEv2vsPtvsCent = new TH3F("hIncEv2vsPtvsCent","hIncEv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  hIncEPtvsCent = new TH2F("hIncEPtvsCent","hIncEPtvsCent;p_{T} [GeV/c];Centrality",80,0,4,9,-0.5,8.5);
  pIncEv2 = new TProfile2D("pIncEv2","pIncEv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pIncEv2_hitcut = new TProfile2D("pIncEv2_hitcut","pIncEv2_hitcut;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  hIncEv2vsPtvsCent_hitcut = new TH3F("hIncEv2vsPtvsCent_hitcut","hIncEv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  hIncEPtvsCent_hitcut = new TH2F("hIncEPtvsCent_hitcut","hIncEPtvsCent;p_{T} [GeV/c];Centrality",80,0,4,9,-0.5,8.5);
  hPhEv2vsPtvsCent = new TH3F("hPhEv2vsPtvsCent","hPhEv2vsPtvsCent;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  hPhEv2vsPtvsCentLS = new TH3F("hPhEv2vsPtvsCentLS","hPhEv2vsPtvsCentLS;#phi-#Psi_{2};p_{T} [GeV/c];Centrality",360,0,PI,80,0,4,9,-0.5,8.5);
  pTagEv2 = new TProfile2D("pTagEv2","pTagEv2;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  pTagEv2_LS = new TProfile2D("pTagEv2_LS","pTagEv2_LS;p_{T};Centrality",80,0,4,9,-0.5,8.5);
  hphoto = new TH3F("hphoto","Mee;Mee;Tag e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphoto_LS = new TH3F("hphoto_LS","Mee like sign vs e pt vs cent;Mee;Tag e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphoto_hitcut = new TH3F("hphoto_hitcut","Mee;Mee;Tag e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);
  hphoto_LS_hitcut = new TH3F("hphoto_LS_hitcut","Mee like sign vs e pt vs cent;Mee;Tag e p_{T};cent",300,0,0.3,100,0,5,9,-0.5,8.5);

  hphotoVsPt_LS = new TH3F("hphotoVsPt_LS","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);
  hphotoVsPt = new TH3F("hphotoVsPt","Mee like sign vs photon pt vs cent;Mee;photon p_{T};cent",120,0,0.3,80,0,8,9,-0.5,8.5);
  hPhi = new TH1F("hPhi","hPhi",360,-1*TMath::Pi(),TMath::Pi());
  hPhi_hitcut = new TH1F("hPhi_hitcut","hPhi_hitcut",360,-1*TMath::Pi(),TMath::Pi());
  hPhi_allcut= new TH1F("hPhi_allcut","hPhi_allcut",360,-1*TMath::Pi(),TMath::Pi());
  hEta = new TH1F("hEta","hEta",50,-1,1);
  hV0 = new TH3F("hV0","hV0;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  hV0_LS = new TH3F("hV0_LS","hV0_LS;x(cm);y(cm);z(cm)",300,-60,60,300,-60,60,500,-100,100);
  hePtvsP = new TH2F("hePtvsP","hePtvsP",200,0,10,200,0,10);

  //tof matching efficiency
  //use partner electron which pass the track quality cut
  hPartETpc = new TH2F("hPartETpc","hPartETpc;p_{T}[GeV];Cent",80,0,4,9,-0.5,8.5 );
  hPartETpcLS = new TH2F("hPartETpcLS","hPartETpcLS;p_{T}[GeV];Cent",80,0,4,9,-0.5,8.5);
  hPartETof = new TH2F("hPartETof","hPartETof;p_{T}[GeV];Cent",80,0,4,9,-0.5,8.5);
  hPartETofLS = new TH2F("hPartETofLS","hPartETofLS;p_{T}[GeV];Cent",80,0,4,9,-0.5,8.5);

  //check if the LS can describe the bkgd
  //use the sideband
  // hPartEptetaphi_SB = new TH3F("hPartEptetaphi_SB", "hPartEptetaphi;p_{T};#eta;#phi",80,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  // hPartEptetaphi_SB_LS = new TH3F("hPartEptetaphi_SB_LS","hPartEptetaphi_LS;p_{T};#eta;#phi",80,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  //
  //for embedding QA comparison
  hDcavsPt_LS = new TH2F("hDcavsPt_LS","hDcavsPt_LS;p_{T};gDca;",10,0,5,100,0,3);
  hDcavsPt = new TH2F("hDcavsPt","hDcavsPt;p_{T};gDca;",10,0,5,100,0,3);
  hDcavsPt_Gm_LS = new TH2F("hDcavsPt_Gm_LS","hDcavsPt_LS;p_{T};gDca;",10,0,5,100,0,3);
  hDcavsPt_Gm = new TH2F("hDcavsPt_Gm","hDcavsPt;p_{T};gDca;",10,0,5,100,0,3);
  hDcavsPt_Dz_LS = new TH2F("hDcavsPt_Dz_LS","hDcavsPt_LS;p_{T};gDca;",10,0,5,100,0,3);
  hDcavsPt_Dz = new TH2F("hDcavsPt_Dz","hDcavsPt;p_{T};gDca;",10,0,5,100,0,3);
  hNFitsvsTagPt_LS = new TH2F("hNFitsvsTagPt_LS","hNFitsvsTagPt_LS;Tag e p_{T};NFits",10,0,5,50,0,50);
  hNFitsvsTagPt = new TH2F("hNFitsvsTagPt","hNFitsvsTagPt;Tag e p_{T};NFits",10,0,5,50,0,50);

  hNFitsvsPt_LS = new TH2F("hNFitsvsPt_LS","hNFitsvsPt_LS;p_{T};NFits",10,0,5,50,0,50);
  hNFitsvsPt = new TH2F("hNFitsvsPt","hNFitsvsPt;p_{T};NFits",10,0,5,50,0,50);
  hPartEptetaphi = new TH3F("hPartEptetaphi", "hPartEptetaphi;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  hPartEptetaphi_LS = new TH3F("hPartEptetaphi_LS","hPartEptetaphi_LS;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  
  hPartEptetaphi_Dz = new TH3F("hPartEptetaphi_Dz","hPartEptetaphi_Dz;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  hPartEptetaphi_Dz_LS = new TH3F("hPartEptetaphi_Dz_LS","hPartEptetaphi_Dz_LS;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  hPartEptetaphi_Gm = new TH3F("hPartEptetaphi_Gm","hPartEptetaphi_Gm;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );
  hPartEptetaphi_Gm_LS = new TH3F("hPartEptetaphi_Gm_LS","hPartEptetaphi_Gm_LS;p_{T};#eta;#phi",100,0,4,100,-1,1,180,-1*TMath::Pi(),TMath::Pi() );

  int const nAxis=4;
  int nbins[nAxis]={50,50,100,100};
  //tage pt;
  double xmin[nAxis] = {0, 0,-1,-1.2};
  double xmax[nAxis] = {2, 2, 1,1.2 };
  hPartETagEetapt = new THnSparseF("hPartETagEetapt","hPartETagEetapt;Tag e p_{T};Part e p_{T};Tag e #eta;Part e #eta",nAxis,nbins,xmin,xmax);
  hPartETagEetapt_LS = new THnSparseF("hPartETagEetapt_LS","hPartETagEetapt_LS;Tag e p_{T};Part e p_{T};Tag e #eta;Part e #eta",nAxis,nbins,xmin,xmax);

  hPairDCA = new TH3F("hPairDCA","hPairDCA;DCA;p_{T};Cent", 100,0,3,80,0,4,9,-0.5,8.5);
  hDecayL= new TH2F("hDecayL","hDecayL;DecayL;p_{T}", 150,0,30,60,0,4);
  hPairDCALS = new TH3F("hPairDCALS","hPairDCALS;DCA;p_{T};Cent", 100,0,3,80,0,4,9,-0.5,8.5);
  hDecayL_LS= new TH2F("hDecayL_LS","hDecayL_LS;DecayL;p_{T}", 150,0,30,60,0,4);
}
//------------------------------------------------------------
void StMiniTreeAnalysis::WriteHists(TFile* out)
{
  out->cd();
  //write the hists
  hcent->Write();
  hcentwg->Write();

  hEventPlaneCent_M->Write();
  hEventPlaneCent_P->Write();
  hEventPlaneCent_M_re->Write();
  hEventPlaneCent_P_re->Write();
  hEventPlaneCent_M_sh->Write();
  hEventPlaneCent_P_sh->Write();
  EPconsRes->Write();
  hIncEv2vsPtvsCent->Write();
  hIncEPtvsCent->Write();
  hIncEv2vsPtvsCent_hitcut->Write();
  hIncEPtvsCent_hitcut->Write();
  hIncEPtvsCent_hitcut->Write();
  hPhEv2vsPtvsCent->Write();
  hPhEv2vsPtvsCentLS->Write();
  pIncEv2->Write();
  pIncEv2_hitcut->Write();
  pTagEv2_LS->Write();
  pTagEv2->Write();
  hPhi_hitcut->Write();
  hPhi_allcut->Write();
  hPhi->Write();  
  hEta->Write();

  hphotoVsPt->Write();
  hphotoVsPt_LS->Write();
  hphoto->Write();
  hphoto_LS->Write();
  hphoto_hitcut->Write();
  hphoto_LS_hitcut->Write();

  hDcavsPt_Dz->Write();
  hDcavsPt_Dz_LS->Write();
  hDcavsPt->Write();
  hDcavsPt_LS->Write();
  hDcavsPt_Gm_LS->Write();
  hDcavsPt_Gm->Write();
  hNFitsvsPt->Write();
  hNFitsvsPt_LS->Write();
  hNFitsvsTagPt->Write();
  hNFitsvsTagPt_LS->Write();

  hPartEptetaphi->Write();
  hPartEptetaphi_LS->Write();
  hPartEptetaphi_Gm_LS->Write();
  hPartEptetaphi_Gm->Write();
  hPartEptetaphi_Dz_LS->Write();
  hPartEptetaphi_Dz->Write();

  //tag e vs part e eta
  hPartETagEetapt->Write();
  hPartETagEetapt_LS->Write();

  hPairDCA->Write();
  hPairDCALS->Write();
  hDecayL_LS->Write();
  hDecayL->Write();

  hPartETpc->Write();
  hPartETpcLS->Write();
  hPartETof->Write();
  hPartETofLS->Write(); 

  hePtvsP->Write();
}
//------------------------------------------------------------
bool StMiniTreeAnalysis::isSecondPE(float nSigE,float beta,float pt)
{
   // bool isTOFElectron = beta>0?fabs(1./beta-1.)<0.025:false; 
   // if (pt>0.8) isTPCElectron =  nSigE<2 && nSigE>0;
   // else isTPCElectron = nSigE<2 && nSigE>(nSigE*3.5-2.8); 
   // return isTPCElectron && isTOFElectron;
   // return true;
   return fabs(nSigE)<anaCuts::nESigma_partner;
}
bool StMiniTreeAnalysis::isElectron(float nSigE,float beta,float pt)
{
   bool isTOFElectron = beta>0?fabs(1./beta-1.)<0.025:false; 
   bool isTPCElectron  = false;
   if (pt>0.8) isTPCElectron =  nSigE<2 && nSigE>0;
   else isTPCElectron = nSigE<2 && nSigE>(pt*3.5-2.8); 
   // isTPCElectron = nSigE>0;
   return isTPCElectron && isTOFElectron;
}
int StMiniTreeAnalysis::getCentralityBin(float vz,int runId,double mult,float &weight)
{
  float mult_corr = mult;
  // if trigger 580001, correct the refmult
  mult+=gRandom->Rndm();
  if (runId<18156031) {
    float fvz = 0;
    for (int i=0;i<anaCuts::nparVz_mult;i++){
      fvz +=anaCuts::parVz_mult[i]*std::pow(vz,i);
    }
    mult=mult*1.0*anaCuts::parVz_mult[0]/fvz;
  }
  // refmultcor=mult;
  weight = reweight(mult);
  for (int cent=0;cent<anaCuts::nCent;cent++)
  {
    if (mult<anaCuts::Refmult_cent[cent]) return cent-1;
  }
  return anaCuts::nCent-1;
}
float StMiniTreeAnalysis::reweight(float x) const
{
  x+=gRandom->Rndm();
  // float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  float p[5] = {0.811,238.9,24.31,-25,6.325e-5};
  // return 1;
  if (x>70) return 1;
  // else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]);
}
double StMiniTreeAnalysis::getEventPlaneShift(double EP_Re,int side,int cent) const
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
