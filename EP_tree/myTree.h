//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jun 23 12:00:16 2019 by ROOT version 5.34/38
// from TTree Tree/Tree
// found on file: 0906B0CB92D335B831CF556E7DAEBACA_100.tree.root
//////////////////////////////////////////////////////////

#ifndef myTree_h
#define myTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class myTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           runId;
   Int_t           eventId;
   Float_t         bfield;
   Int_t           centrality;
   Float_t         weight;
   Float_t         refMult;
   Float_t         pVtx_x;
   Float_t         pVtx_y;
   Float_t         pVtx_z;
   Float_t         vzVpdVz;
   Float_t         BBCx;
   Float_t         ZDCx;
   Float_t         nTofMult;
   Float_t         nTofmatch;
   Float_t         nQvecP;
   Float_t         nQvecM;
   Double_t        Qx_M;
   Double_t        Qy_M;
   Double_t        Qx_P;
   Double_t        Qy_P;
   Float_t         EP_M_raw;
   Float_t         EP_M_re;
   Float_t         EP_M_sh;
   Float_t         EP_P_raw;
   Float_t         EP_P_re;
   Float_t         EP_P_sh;
   Float_t         resolution;
   Int_t           nIncE;
   Int_t           idx_inc[15];   //[nIncE]
   Float_t         nSigE_inc[15];   //[nIncE]
   Float_t         beta_inc[15];   //[nIncE]
   Float_t         pt_inc[15];   //[nIncE]
   Float_t         eta_inc[15];   //[nIncE]
   Float_t         phi_inc[15];   //[nIncE]
   Float_t         deltaphi_inc[15];   //[nIncE]
   Float_t         cos2phi_inc[15];   //[nIncE]
   Float_t         gDca_inc[15];   //[nIncE]
   Float_t         charge_inc[15];   //[nIncE]
   Float_t         nFit_inc[15];   //[nIncE]
   Float_t         ndEdx_inc[15];   //[nIncE]
   UInt_t          topomap0_inc[15];   //[nIncE]
   Bool_t          has1hit_inc[15];   //[nIncE]
   Bool_t          has2hit_inc[15];   //[nIncE]
   Bool_t          has3hit_inc[15];   //[nIncE]
   Int_t           nPhoE;
   Int_t           idx_phe[118];   //[nPhoE]
   Float_t         nSigE_phe[118];   //[nPhoE]
   Float_t         beta_phe[118];   //[nPhoE]
   Float_t         pt_phe[118];   //[nPhoE]
   Float_t         eta_phe[118];   //[nPhoE]
   Float_t         phi_phe[118];   //[nPhoE]
   Float_t         gDca_phe[118];   //[nPhoE]
   Float_t         charge_phe[118];   //[nPhoE]
   Float_t         nFit_phe[118];   //[nPhoE]
   Float_t         ndEdx_phe[118];   //[nPhoE]
   Float_t         deltaphi_phe[118];   //[nPhoE]
   Float_t         cos2phi_phe[118];   //[nPhoE]
   UInt_t          topomap0_phe[118];   //[nPhoE]
   Int_t           idx_parte[118];   //[nPhoE]
   Float_t         nSigE_parte[118];   //[nPhoE]
   Float_t         beta_parte[118];   //[nPhoE]
   Float_t         gpt_parte[118];   //[nPhoE]
   Float_t         geta_parte[118];   //[nPhoE]
   Float_t         gphi_parte[118];   //[nPhoE]
   Float_t         gDca_parte[118];   //[nPhoE]
   Float_t         charge_parte[118];   //[nPhoE]
   Float_t         nFit_parte[118];   //[nPhoE]
   Float_t         ndEdx_parte[118];   //[nPhoE]
   UInt_t          topomap0_parte[118];   //[nPhoE]
   Float_t         DCA_pair[118];   //[nPhoE]
   Float_t         decayL_pair[118];   //[nPhoE]
   Float_t         V0x_pair[118];   //[nPhoE]
   Float_t         V0y_pair[118];   //[nPhoE]
   Float_t         V0z_pair[118];   //[nPhoE]
   Float_t         vtx2V0x_pair[118];   //[nPhoE]
   Float_t         vtx2V0y_pair[118];   //[nPhoE]
   Float_t         vtx2V0z_pair[118];   //[nPhoE]
   Float_t         M_pair[118];   //[nPhoE]
   Float_t         px_pair[118];   //[nPhoE]
   Float_t         py_pair[118];   //[nPhoE]
   Float_t         pz_pair[118];   //[nPhoE]

   // List of branches
   TBranch        *b_runId;   //!
   TBranch        *b_eventId;   //!
   TBranch        *b_bfield;   //!
   TBranch        *b_centrality;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_refMult;   //!
   TBranch        *b_pVtx_x;   //!
   TBranch        *b_pVtx_y;   //!
   TBranch        *b_pVtx_z;   //!
   TBranch        *b_vzVpdVz;   //!
   TBranch        *b_BBCx;   //!
   TBranch        *b_ZDCx;   //!
   TBranch        *b_nTofMult;   //!
   TBranch        *b_nTofmatch;   //!
   TBranch        *b_nQvecP;   //!
   TBranch        *b_nQvecM;   //!
   TBranch        *b_Qx_M;   //!
   TBranch        *b_Qy_M;   //!
   TBranch        *b_Qx_P;   //!
   TBranch        *b_Qy_P;   //!
   TBranch        *b_EP_M_raw;   //!
   TBranch        *b_EP_M_re;   //!
   TBranch        *b_EP_M_sh;   //!
   TBranch        *b_EP_P_raw;   //!
   TBranch        *b_EP_P_re;   //!
   TBranch        *b_EP_P_sh;   //!
   TBranch        *b_resolution;   //!
   TBranch        *b_nIncE;   //!
   TBranch        *b_idx_inc;   //!
   TBranch        *b_nSigE_inc;   //!
   TBranch        *b_beta_inc;   //!
   TBranch        *b_pt_inc;   //!
   TBranch        *b_eta_inc;   //!
   TBranch        *b_phi_inc;   //!
   TBranch        *b_deltaphi_inc;   //!
   TBranch        *b_cos2phi_inc;   //!
   TBranch        *b_gDca_inc;   //!
   TBranch        *b_charge_inc;   //!
   TBranch        *b_nFit_inc;   //!
   TBranch        *b_ndEdx_inc;   //!
   TBranch        *b_topomap0_inc;   //!
   TBranch        *b_has1hit_inc;   //!
   TBranch        *b_has2hit_inc;   //!
   TBranch        *b_has3hit_inc;   //!
   TBranch        *b_nPhoE;   //!
   TBranch        *b_idx_phe;   //!
   TBranch        *b_nSigE_phe;   //!
   TBranch        *b_beta_phe;   //!
   TBranch        *b_pt_phe;   //!
   TBranch        *b_eta_phe;   //!
   TBranch        *b_phi_phe;   //!
   TBranch        *b_gDca_phe;   //!
   TBranch        *b_charge_phe;   //!
   TBranch        *b_nFit_phe;   //!
   TBranch        *b_ndEdx_phe;   //!
   TBranch        *b_deltaphi_phe;   //!
   TBranch        *b_cos2phi_phe;   //!
   TBranch        *b_topomap0_phe;   //!
   TBranch        *b_idx_parte;   //!
   TBranch        *b_nSigE_parte;   //!
   TBranch        *b_beta_parte;   //!
   TBranch        *b_gpt_parte;   //!
   TBranch        *b_geta_parte;   //!
   TBranch        *b_gphi_parte;   //!
   TBranch        *b_gDca_parte;   //!
   TBranch        *b_charge_parte;   //!
   TBranch        *b_nFit_parte;   //!
   TBranch        *b_ndEdx_parte;   //!
   TBranch        *b_topomap0_parte;   //!
   TBranch        *b_DCA_pair;   //!
   TBranch        *b_decayL_pair;   //!
   TBranch        *b_V0x_pair;   //!
   TBranch        *b_V0y_pair;   //!
   TBranch        *b_V0z_pair;   //!
   TBranch        *b_vtx2V0x_pair;   //!
   TBranch        *b_vtx2V0y_pair;   //!
   TBranch        *b_vtx2V0z_pair;   //!
   TBranch        *b_M_pair;   //!
   TBranch        *b_px_pair;   //!
   TBranch        *b_py_pair;   //!
   TBranch        *b_pz_pair;   //!

   myTree(TTree *tree=0);
   virtual ~myTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef myTree_cxx
myTree::myTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("0906B0CB92D335B831CF556E7DAEBACA_100.tree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("0906B0CB92D335B831CF556E7DAEBACA_100.tree.root");
      }
      f->GetObject("Tree",tree);

   }
   Init(tree);
}

myTree::~myTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t myTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t myTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void myTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runId", &runId, &b_runId);
   fChain->SetBranchAddress("eventId", &eventId, &b_eventId);
   fChain->SetBranchAddress("bfield", &bfield, &b_bfield);
   fChain->SetBranchAddress("centrality", &centrality, &b_centrality);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("refMult", &refMult, &b_refMult);
   fChain->SetBranchAddress("pVtx_x", &pVtx_x, &b_pVtx_x);
   fChain->SetBranchAddress("pVtx_y", &pVtx_y, &b_pVtx_y);
   fChain->SetBranchAddress("pVtx_z", &pVtx_z, &b_pVtx_z);
   fChain->SetBranchAddress("vzVpdVz", &vzVpdVz, &b_vzVpdVz);
   fChain->SetBranchAddress("BBCx", &BBCx, &b_BBCx);
   fChain->SetBranchAddress("ZDCx", &ZDCx, &b_ZDCx);
   fChain->SetBranchAddress("nTofMult", &nTofMult, &b_nTofMult);
   fChain->SetBranchAddress("nTofmatch", &nTofmatch, &b_nTofmatch);
   fChain->SetBranchAddress("nQvecP", &nQvecP, &b_nQvecP);
   fChain->SetBranchAddress("nQvecM", &nQvecM, &b_nQvecM);
   fChain->SetBranchAddress("Qx_M", &Qx_M, &b_Qx_M);
   fChain->SetBranchAddress("Qy_M", &Qy_M, &b_Qy_M);
   fChain->SetBranchAddress("Qx_P", &Qx_P, &b_Qx_P);
   fChain->SetBranchAddress("Qy_P", &Qy_P, &b_Qy_P);
   fChain->SetBranchAddress("EP_M_raw", &EP_M_raw, &b_EP_M_raw);
   fChain->SetBranchAddress("EP_M_re", &EP_M_re, &b_EP_M_re);
   fChain->SetBranchAddress("EP_M_sh", &EP_M_sh, &b_EP_M_sh);
   fChain->SetBranchAddress("EP_P_raw", &EP_P_raw, &b_EP_P_raw);
   fChain->SetBranchAddress("EP_P_re", &EP_P_re, &b_EP_P_re);
   fChain->SetBranchAddress("EP_P_sh", &EP_P_sh, &b_EP_P_sh);
   fChain->SetBranchAddress("resolution", &resolution, &b_resolution);
   fChain->SetBranchAddress("nIncE", &nIncE, &b_nIncE);
   fChain->SetBranchAddress("idx_inc", idx_inc, &b_idx_inc);
   fChain->SetBranchAddress("nSigE_inc", nSigE_inc, &b_nSigE_inc);
   fChain->SetBranchAddress("beta_inc", beta_inc, &b_beta_inc);
   fChain->SetBranchAddress("pt_inc", pt_inc, &b_pt_inc);
   fChain->SetBranchAddress("eta_inc", eta_inc, &b_eta_inc);
   fChain->SetBranchAddress("phi_inc", phi_inc, &b_phi_inc);
   fChain->SetBranchAddress("deltaphi_inc", deltaphi_inc, &b_deltaphi_inc);
   fChain->SetBranchAddress("cos2phi_inc", cos2phi_inc, &b_cos2phi_inc);
   fChain->SetBranchAddress("gDca_inc", gDca_inc, &b_gDca_inc);
   fChain->SetBranchAddress("charge_inc", charge_inc, &b_charge_inc);
   fChain->SetBranchAddress("nFit_inc", nFit_inc, &b_nFit_inc);
   fChain->SetBranchAddress("ndEdx_inc", ndEdx_inc, &b_ndEdx_inc);
   fChain->SetBranchAddress("topomap0_inc", topomap0_inc, &b_topomap0_inc);
   fChain->SetBranchAddress("has1hit_inc", has1hit_inc, &b_has1hit_inc);
   fChain->SetBranchAddress("has2hit_inc", has2hit_inc, &b_has2hit_inc);
   fChain->SetBranchAddress("has3hit_inc", has3hit_inc, &b_has3hit_inc);
   fChain->SetBranchAddress("nPhoE", &nPhoE, &b_nPhoE);
   fChain->SetBranchAddress("idx_phe", idx_phe, &b_idx_phe);
   fChain->SetBranchAddress("nSigE_phe", nSigE_phe, &b_nSigE_phe);
   fChain->SetBranchAddress("beta_phe", beta_phe, &b_beta_phe);
   fChain->SetBranchAddress("pt_phe", pt_phe, &b_pt_phe);
   fChain->SetBranchAddress("eta_phe", eta_phe, &b_eta_phe);
   fChain->SetBranchAddress("phi_phe", phi_phe, &b_phi_phe);
   fChain->SetBranchAddress("gDca_phe", gDca_phe, &b_gDca_phe);
   fChain->SetBranchAddress("charge_phe", charge_phe, &b_charge_phe);
   fChain->SetBranchAddress("nFit_phe", nFit_phe, &b_nFit_phe);
   fChain->SetBranchAddress("ndEdx_phe", ndEdx_phe, &b_ndEdx_phe);
   fChain->SetBranchAddress("deltaphi_phe", deltaphi_phe, &b_deltaphi_phe);
   fChain->SetBranchAddress("cos2phi_phe", cos2phi_phe, &b_cos2phi_phe);
   fChain->SetBranchAddress("topomap0_phe", topomap0_phe, &b_topomap0_phe);
   fChain->SetBranchAddress("idx_parte", idx_parte, &b_idx_parte);
   fChain->SetBranchAddress("nSigE_parte", nSigE_parte, &b_nSigE_parte);
   fChain->SetBranchAddress("beta_parte", beta_parte, &b_beta_parte);
   fChain->SetBranchAddress("gpt_parte", gpt_parte, &b_gpt_parte);
   fChain->SetBranchAddress("geta_parte", geta_parte, &b_geta_parte);
   fChain->SetBranchAddress("gphi_parte", gphi_parte, &b_gphi_parte);
   fChain->SetBranchAddress("gDca_parte", gDca_parte, &b_gDca_parte);
   fChain->SetBranchAddress("charge_parte", charge_parte, &b_charge_parte);
   fChain->SetBranchAddress("nFit_parte", nFit_parte, &b_nFit_parte);
   fChain->SetBranchAddress("ndEdx_parte", ndEdx_parte, &b_ndEdx_parte);
   fChain->SetBranchAddress("topomap0_parte", topomap0_parte, &b_topomap0_parte);
   fChain->SetBranchAddress("DCA_pair", DCA_pair, &b_DCA_pair);
   fChain->SetBranchAddress("decayL_pair", decayL_pair, &b_decayL_pair);
   fChain->SetBranchAddress("V0x_pair", V0x_pair, &b_V0x_pair);
   fChain->SetBranchAddress("V0y_pair", V0y_pair, &b_V0y_pair);
   fChain->SetBranchAddress("V0z_pair", V0z_pair, &b_V0z_pair);
   fChain->SetBranchAddress("vtx2V0x_pair", vtx2V0x_pair, &b_vtx2V0x_pair);
   fChain->SetBranchAddress("vtx2V0y_pair", vtx2V0y_pair, &b_vtx2V0y_pair);
   fChain->SetBranchAddress("vtx2V0z_pair", vtx2V0z_pair, &b_vtx2V0z_pair);
   fChain->SetBranchAddress("M_pair", M_pair, &b_M_pair);
   fChain->SetBranchAddress("px_pair", px_pair, &b_px_pair);
   fChain->SetBranchAddress("py_pair", py_pair, &b_py_pair);
   fChain->SetBranchAddress("pz_pair", pz_pair, &b_pz_pair);
   Notify();
}

Bool_t myTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void myTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t myTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef myTree_cxx
