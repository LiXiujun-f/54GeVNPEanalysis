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
StMiniTreeAnalysis::StMiniTreeAnalysis(TString const  inputfilelist, TString const outfilename):mOutputname(outfilename),
  mInputlist(inputfilelist)
{
  }
StMiniTreeAnalysis::~StMiniTreeAnalysis()
{
}
int StMiniTreeAnalysis::Init()
{
  mOutputname +=".recenter.root";
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

  // bookHists();
  initHists(anaCuts::nRunNum); 
  mOutfile = new TFile(mOutputname.Data(),"recreate");
  mTree = new myTree(mChain);
  return 1;
}
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
    //bool refusepileup = mTree->refMult < mTree->nTofMult*0.36+45;
    //bool refusebadtof = mTree->refMult > mTree->nTofMult*0.28-115;
    bool refusepileup = mTree->refMult*2.88-155 < mTree->nTofMult;
    bool refusebadtof = !(mTree->refMult<20 &&mTree->nTofMult>250); 
    if (!refusepileup) continue; 
    if (!refusebadtof) continue; 
    bool goodevent = fabs(mTree->pVtx_z) < anaCuts::vz
                     && fabs(mTree->vzVpdVz) < anaCuts::vzVpdVz;
    if (!goodevent) continue;
    Int_t cent = mTree->centrality;
    // Float_t pVtx_z = mTree->pVtx_z;
    // float ZDCx = mTree->ZDCx;
    float weight=1.;
    // int cent = getCentralityBin(mTree->pVtx_z,runId,mTree->refMult,&weight);
    hcent->Fill(cent);
    hcentwg->Fill(cent,weight);
    //I write the wrong name ...
    int nQvecM = mTree->nQvecM;
    int nQvecP = mTree->nQvecP;
    TVector2 QvecM(mTree->Qx_M,mTree->Qy_M);

    if (QvecM.Mod()>0) {
      pQxRecenterM[cent]->Fill(runnum[runId],mTree->Qx_M/(1.*nQvecM),nQvecM);
      pQyRecenterM[cent]->Fill(runnum[runId],mTree->Qy_M/(1.*nQvecM),nQvecM);
      hEventPlaneCent_M->Fill(cent,QvecM.Phi()*0.5);
    }
    TVector2 QvecP(mTree->Qx_P,mTree->Qy_P);
    if (QvecP.Mod()>0)
    // if (nQvecP>0) 
    { 
      pQxRecenterP[cent]->Fill(runnum[runId],mTree->Qx_P/(1.*nQvecP),nQvecP);
      pQyRecenterP[cent]->Fill(runnum[runId],mTree->Qy_P/(1.*nQvecP),nQvecP);
      if (nQvecP<1) cout<< cent <<endl;
      hEventPlaneCent_P->Fill(cent,QvecP.Phi()*0.5);
    }
  } //event loop
  return 1;
}

int StMiniTreeAnalysis::Finish()
{
  WriteHists(mOutfile);
  mOutfile->Close();
  delete mChain;
  mChain=NULL;
  return 1;
}

void StMiniTreeAnalysis::bookHists()
{
}

void StMiniTreeAnalysis::initHists(int nRunNum)
{
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic] = new TProfile(Form("pQxRecenterMin_%d",ic),Form("pRecenterMin_%d",ic),nRunNum,0,nRunNum);  
    pQyRecenterM[ic] = new TProfile(Form("pQyRecenterMin_%d",ic),Form("pRecenterMin_%d",ic),nRunNum,0,nRunNum);  
    pQxRecenterP[ic] = new TProfile(Form("pQxRecenterPlu_%d",ic),Form("pRecenterPlu_%d",ic),nRunNum,0,nRunNum);  
    pQyRecenterP[ic] = new TProfile(Form("pQyRecenterPlu_%d",ic),Form("pRecenterPlu_%d",ic),nRunNum,0,nRunNum);  
  }  
  float PI = TMath::Pi();
  hEventPlaneCent_M = new TH2F("hEventPlaneCent_M_raw","hEventPlaneCent",9,0,9,180,0,PI);
  hEventPlaneCent_P = new TH2F("hEventPlaneCent_P_raw","hEventPlaneCent",9,0,9,180,0,PI);
}

void StMiniTreeAnalysis::WriteHists(TFile* out)
{
  out->cd();
  //write the hists
  for (int ic=0;ic<9;ic++){
    pQxRecenterM[ic]->Write();
    pQyRecenterM[ic]->Write();
    pQxRecenterP[ic]->Write();
    pQyRecenterP[ic]->Write();
  }

  hEventPlaneCent_M->Write();
  hEventPlaneCent_P->Write();
  hcent->Write();
  hcentwg->Write();
 }

int StMiniTreeAnalysis::getCentralityBin(float z,int runId,double mult,double &weight)
{
  // mult+=gRandom->Rndm();
  // weight = reweight(mult);
  weight = 1;
  // if  (mult<6)  return  -1;
  for (int cent=0;cent<anaCuts::nCent;cent++)
  {
    if (mult<anaCuts::Refmult_cent[cent]) return cent-1;
  }
  return anaCuts::nCent-1;
}
