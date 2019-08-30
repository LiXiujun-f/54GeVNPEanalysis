#ifndef mTree_def
#define mTree_def

#define MAX_NUM_P  1000
struct mTreeDst{   //Ks reconstruct v0 Tree
    Int_t   mCentrality;
    Int_t   mRunId;
    Int_t   mEvtId;
    Float_t mVx;
    Float_t mVy;
    Float_t mVz;
    Float_t mVzVpd;
    Short_t mRefMult;
    Short_t mGRefMult;
    Float_t mWeight;
    Int_t   mNV0;
    UChar_t mFlag[MAX_NUM_P];        //
    Float_t mDca1[MAX_NUM_P];
    Float_t mDca2[MAX_NUM_P];
    Float_t mPt1[MAX_NUM_P];
    Float_t mPt2[MAX_NUM_P];
    Float_t mEta1[MAX_NUM_P];
    Float_t mEta2[MAX_NUM_P];
    Float_t mNSigma1[MAX_NUM_P];
    Float_t mNSigma2[MAX_NUM_P];
    Int_t   mTof1[MAX_NUM_P];
    Int_t   mTof2[MAX_NUM_P];
    Float_t mDiffInvBeta1[MAX_NUM_P];
    Float_t mDiffInvBeta2[MAX_NUM_P];
    Float_t mBeta1[MAX_NUM_P];
    Float_t mBeta2[MAX_NUM_P];
    Float_t mDcaDaughters[MAX_NUM_P];
    Float_t mV0x[MAX_NUM_P];
    Float_t mV0y[MAX_NUM_P];
    Float_t mV0z[MAX_NUM_P];
    Float_t mPt[MAX_NUM_P];
    Float_t mEta[MAX_NUM_P];
    Float_t mPhi[MAX_NUM_P];
    Float_t mMass[MAX_NUM_P];
    Float_t mCosTheta[MAX_NUM_P];
    Float_t mDca2Vtx[MAX_NUM_P];
    Float_t mDecayL[MAX_NUM_P];
    Float_t mDcaXY[MAX_NUM_P];
    Float_t mDcaZ[MAX_NUM_P];
};
//mTreeDst v0Tree;
#endif
