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

  bookHists();
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
    // bool refusepileup = mTree->refMult < mTree->nTofMult*0.36+45;
    // bool refusebadtof = mTree->refMult > mTree->nTofMult*0.28-115;
    // if (!refusepileup) continue; 
    // if (!refusebadtof) continue; 
    bool refusepileup = mTree->refMult*2.88-155 < mTree->nTofMult;
    bool refusebadtof = !(mTree->refMult<20 &&mTree->nTofMult>250);
    bool goodevent = mTree->pVtx_z < anaCuts::vz &&
                     fabs(mTree->vzVpdVz) < anaCuts::vzVpdVz &&
                     sqrt(mTree->pVtx_x*mTree->pVtx_x+mTree->pVtx_y*mTree->pVtx_y)<anaCuts::Vr;
    if (!refusepileup) continue;
    if (!refusebadtof) continue;
    if (!goodevent) continue;
    Int_t cent = mTree->centrality;
    // Float_t pVtx_z = mTree->pVtx_z;
    // float ZDCx = mTree->ZDCx;
    float weight=1.;
    // int cent = getCentralityBin(mTree->pVtx_z,runId,mTree->refMult,&weight);
    hcent->Fill(cent);
    hcentwg->Fill(cent,weight);
    int nQvecM = mTree->nQvecM;
    int nQvecP = mTree->nQvecP;
    TVector2 QvecM(mTree->Qx_M,mTree->Qy_M);
    
    double Qx_M_re = mTree->Qx_M-nQvecM*pQxRecenterM[cent]->GetBinContent(runnum[runId]+1);
    double Qy_M_re = mTree->Qy_M-nQvecM*pQyRecenterM[cent]->GetBinContent(runnum[runId]+1);
    TVector2 QvecM_re(Qx_M_re,Qy_M_re);
    double EP_M_re = QvecM_re.Phi()*0.5;
    if (QvecM.Mod()>0) {
      // pQxRecenterM[cent]->Fill(runnum[runId],mTree->Qx_M/(1.*nQvecM),nQvecM);
      // pQyRecenterM[cent]->Fill(runnum[runId],mTree->Qy_M/(1.*nQvecM),nQvecM);
      for (int i=1;i<21;i++)
      {
        pCosEtaM[cent]->Fill(i,std::cos(2.*i*EP_M_re));
        pSinEtaM[cent]->Fill(i,std::sin(2.*i*EP_M_re));
      }
      hEventPlaneCent_M->Fill(cent,QvecM.Phi()*0.5);
      hEventPlaneCent_M_re->Fill(cent,EP_M_re);
    }
    TVector2 QvecP(mTree->Qx_P,mTree->Qy_P);
    double Qx_P_re = mTree->Qx_P-nQvecP*pQxRecenterP[cent]->GetBinContent(runnum[runId]+1);
    double Qy_P_re = mTree->Qy_P-nQvecP*pQyRecenterP[cent]->GetBinContent(runnum[runId]+1);
    TVector2 QvecP_re(Qx_P_re,Qy_P_re);
    double EP_P_re = QvecP_re.Phi()*0.5;
    if (QvecP.Mod()>0)
    // if (nQvecP>0) 
    { 
      // pQxRecenterP[cent]->Fill(runnum[runId],mTree->Qx_P/(1.*nQvecP),nQvecP);
      // pQyRecenterP[cent]->Fill(runnum[runId],mTree->Qy_M/(1.*nQvecP),nQvecP);
      for (int i=1;i<21;i++)
      {
        pCosEtaP[cent]->Fill(i,std::cos(2.*i*EP_P_re));
        pSinEtaP[cent]->Fill(i,std::sin(2.*i*EP_P_re));
      }
      hEventPlaneCent_P->Fill(cent,QvecP.Phi()*0.5);
      hEventPlaneCent_P_re->Fill(cent,EP_P_re);
    }
    double resolution = cos(2.*(EP_P_re-EP_M_re));
    EPconsRes->Fill(cent,resolution);
  } //event loop
  return 1;
}

int StMiniTreeAnalysis::Finish()
{
  WriteHists(mOutfile);
  mOutfile->Close();
  delete mChain;
  mChain=NULL;
  for (int ic=0;ic<9;ic++)
  {
    delete pQxRecenterM[ic];
    delete pQyRecenterM[ic];
    delete pQxRecenterP[ic];
    delete pQyRecenterP[ic];
  }
  return 1;
}

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
}

void StMiniTreeAnalysis::initHists(int nRunNum)
{
  hcent = new TH1F("hcent","hcent",9,-0.5,8.5);
  hcentwg = new TH1F("hcentwg","hcentwg",9,-0.5,8.5);
  float PI = TMath::Pi();
  hEventPlaneCent_M = new TH2F("hEventPlaneCent_M_raw","hEventPlaneCent",9,0,9,360,0,PI);
  hEventPlaneCent_P = new TH2F("hEventPlaneCent_P_raw","hEventPlaneCent",9,0,9,360,0,PI);
  hEventPlaneCent_M_re = new TH2F("hEventPlaneCent_M_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  // hEventPlaneCent_M_sh = new TH2F("hEventPlaneCent_M_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  hEventPlaneCent_P_re = new TH2F("hEventPlaneCent_P_Re","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  // hEventPlaneCent_P_sh = new TH2F("hEventPlaneCent_P_Sh","hEventPlaneCent recenter;Cent;Psi",9,-0.5,8.5,360,0,PI);
  EPconsRes = new TProfile("EPRes","Event Plane resolution;Cent;Res",9,-0.5,8.5);

  for (int ic=0;ic<9;ic++)
  {
    pSinEtaM[ic] = new TProfile(Form("pSinEtaM_Cent%d",ic),Form("pSinEtaM_Cent%d",ic),20,0.5,20.5);
    pCosEtaM[ic] = new TProfile(Form("pCosEtaM_Cent%d",ic),Form("pCosEtaM_Cent%d",ic),20,0.5,20.5);
    pSinEtaP[ic] = new TProfile(Form("pSinEtaP_Cent%d",ic),Form("pSinEtaP_Cent%d",ic),20,0.5,20.5);
    pCosEtaP[ic] = new TProfile(Form("pCosEtaP_Cent%d",ic),Form("pCosEtaP_Cent%d",ic),20,0.5,20.5);
  }
}

void StMiniTreeAnalysis::WriteHists(TFile* out)
{
  out->cd();
  //write the hists
  for (int ic=0;ic<9;ic++){
    pCosEtaP[ic]->Write();
    pSinEtaP[ic]->Write();
    pCosEtaM[ic]->Write();
    pSinEtaM[ic]->Write();
  }

  hEventPlaneCent_M->Write();
  hEventPlaneCent_P->Write();
  hEventPlaneCent_M_re->Write();
  hEventPlaneCent_P_re->Write();
  EPconsRes->Write();
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
