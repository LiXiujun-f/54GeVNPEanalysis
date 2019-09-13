#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TNtuple.h"
#include "TF1.h"
#include "TClonesArray.h"
#include "TPythia6Decayer.h"
#include "TRandom.h"
#include "TParticle.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <TDatime.h>
#include <TRandom3.h>

using namespace std;

const int nbin = 100;

//.. mass of cocktail particles ....
const float m_pi0 = 0.135;
const float m_eta = 0.548;

//..ratio of cocktail particle over pion0  ....
//.. this ratio is constant according to PHENIX measurements. 
//.. see Phys. Rev. C 82, 011902(R) (2010)
const float r_eta= 0.5;  // - defult value for pho. reco. eff
//const float r_eta= 0.35;	// syst. error check: 0.35  - 0.6


//.. Branch Ratio of cocktail particles
const float BR_pi0 = 0.99; //pi0->2g 
const float BR_eta = 0.39; //eta->2g

//....  cocktail functions .................................
double fpi0(double *x, double *par);
double feta(double *x, double *par);

//.. decay the particles and fill the histogram ....
void Decay_Fill(TPythia6Decayer* pydecay, TLorentzVector* b, float px, float py, float pz, float E, int kf, TClonesArray& ptl, TNtuple* nt_sngl, TNtuple* nt_pair, TH1F* h, float weight, float peta, float ppt, float BR);

//.. set parameter  ....
void SetParameter(TF1* fun, float p0, float p1, float p2, float p3, float p4, float p5);

//.. rough acceptance ...
bool Accp(float eta, float pt, int pid);

//.. get kinetics ....
void get_kinetics(TF1* f_in, float& pt, float& eta, float& phi, float& px, float& py, float& pz);

//============== main  program ==================
void pi0_eta_gamma_decay_syst_err(int npart = 1e6, int icent = 0, const char* infileName = "pi_AuAu_200GeV_0_60.txt", bool systErrCheck=false)
{
    //.. ntuple
    TNtuple* nt_sngl = new TNtuple("nt_sngl", "", "pt:eta:peta:ppt:pid:ppid:w");
    TNtuple* nt_pair = new TNtuple("nt_pair", "", "m:peta:ppt:ppid:w");
    TH1F* h_pi0 = new TH1F("h_pi0", "", 100, 0, 20);
    TH1F* h_eta = new TH1F("h_eta", "", 100, 0, 20);

    float p0;
    float p1;
    float p2;
    float p3;
    float p4;

    // systErrCheck - flag to check how different input pi0 pt shape channge the efficiency	

    if(icent == -1) {
 

           TDatime* timeForSeed = new TDatime();
           TRandom3* randGen = new TRandom3((UInt_t)timeForSeed->GetTime());
           float statErrShift = 0.;

	   TGraphErrors* grdNdpT = new TGraphErrors(infileName,"%lg %lg %lg");

	   if(systErrCheck){
        	   for(int i = 0; i<grdNdpT->GetN();i++){
                	 statErrShift = randGen->Gaus();
	                 grdNdpT->GetY()[i] += statErrShift*grdNdpT->GetEY()[i];
        	   }
	}	
	
	TF1* fitFun = new TF1("fitFun","[0]*TMath::Power(TMath::Exp(-[1]*x-[2]*x*x)+x/[3],-[4])",0.2,15);
	fitFun->SetParameter(0,2000);
	fitFun->SetParameter(1,0.1);
	fitFun->SetParameter(2,0.5);
	fitFun->SetParameter(3,0.5);
	fitFun->SetParameter(4,8.5);
	Int_t res = grdNdpT->Fit(fitFun,"R");
	
	if(res!=0) return;

        //combined pi0 from Phenix and STAR pi- for 0-60% Au+Au 200 GeV 
        p0=fitFun->GetParameter(0); p1=fitFun->GetParameter(1); p2=fitFun->GetParameter(2); p3=fitFun->GetParameter(3); p4=fitFun->GetParameter(4);
    } else if(icent==0){
        //.. PHNX 0-10% 
        p0 = 1302; p1 =0.564; p2 = 0.1222; p3 = 0.7899; p4 = 8.532;
    } else if(icent==1){
        //.. PHNX 10-20% 
        p0 = 853.1; p1=0.5369; p2=0.062; p3=0.8268; p4 =8.626;
    } else if(icent==2) {
        //.. PHNX 20-30% 
        p0 = 656.5; p1=0.5181; p2=0.08416; p3=0.7946; p4=8.436;
    } else if(icent==3) {
        //.. PHNX 30-40% 
        p0 = 486.5; p1=0.5034;p2=0.13; p3=0.7511; p4=8.22;
    } else if(icent==4) {
        //.. PHNX 40-50% 
        p0 = 311.1; p1=0.4898; p2=0.0593; p3=0.7918; p4 =8.425; 
    } else if(icent==5) {
        //.. PHNX 50-60% 
        p0 = 196.3; p1=0.5112; p2=0.04127; p3=0.7721; p4=8.314;      
 
    } else if(icent==6) {
        //.. PHNX p+pi 200 GeV
        p0 = 377;
        p1 = 0.356;
        p2 = 0.068;
        p3 = 0.7;
        p4 = 8.25;
    }

    TF1* fun_pi0 = new TF1("fun_pi0", fpi0, 0, 20, 6);
    TF1* fun_eta = new TF1("fun_eta", feta, 0, 20, 6);
    SetParameter(fun_pi0, p0, p1, p2, p3, p4, 1.0);
    SetParameter(fun_eta, p0, p1, p2, p3, p4, r_eta);

    cout<<"eta/pi0(dN/dy) = "<<fun_eta->Integral(0, 20)/fun_pi0->Integral(0, 20)<<" PHNX value: 0.112"<<endl;

    //.. decayer for each particle. One can not force-decay all cocktail 
    //.. particles at the same time since it break the branching ratio  
    //.. e.g. omega->pi0+e+e- will always be omega->gamma+e+e-e+e-
    TPythia6Decayer* pydecay = new TPythia6Decayer;
    pydecay->Init();

    pydecay->ForceParticleDecay(111, 22, 2); 
    pydecay->ForceParticleDecay(221, 22, 2); 

    // flat pt distribution for all ptcls
    TF1* f_in = new TF1("f_in", "1", 0,30); 

    //.. kinetic variables for each ptcls. 
    TLorentzVector* b_pi0= new TLorentzVector;
    TLorentzVector* b_eta= new TLorentzVector;

    //.. holder of decay particles ...
    TClonesArray ptl_pi0("TParticle", 100);
    TClonesArray ptl_eta("TParticle", 100);

    float pt = -999, eta = -999, phi = -999, px = -999, py = -999, pz = -999;
    for(int ipart = 0; ipart<npart; ipart++) {
        if(!(ipart%100000))
            cout<<"____________ ipart = "<<ipart<<" ________________"<<endl;

        get_kinetics(f_in, pt, eta, phi, px, py, pz);

        //.. weight from cocktail particles ...
        float w_pi0 = fun_pi0->Eval(pt);
        float w_eta = fun_eta->Eval(pt);

        float E_pi0= sqrt(px*px+py*py+pz*pz+m_pi0*m_pi0);
        float E_eta= sqrt(px*px+py*py+pz*pz+m_eta*m_eta);

        //.. decay and fill the particles ...
        Decay_Fill(pydecay, b_pi0, px, py, pz, E_pi0, 111, ptl_pi0, nt_sngl, nt_pair, h_pi0, w_pi0, eta, pt, BR_pi0); 
        Decay_Fill(pydecay, b_eta, px, py, pz, E_eta, 221, ptl_eta, nt_sngl, nt_pair, h_eta, w_eta, eta, pt, BR_eta);
    }

    //.. save the ntuple ....
    char hname[200];
    if(icent==0) {
        sprintf(hname, "pi0_eta_gamma_decay_cent0_10p.root");
    } else if(icent==1) {
        sprintf(hname, "pi0_eta_gamma_decay_cent10_20p.root");
    } else if(icent==2) {
        sprintf(hname, "pi0_eta_gamma_decay_cent20_30p.root");
    } else if(icent==3) {
        sprintf(hname, "pi0_eta_gamma_decay_cent30_40p.root");
    } else if(icent==4) {
        sprintf(hname, "pi0_eta_gamma_decay_cent40_50p.root");
    } else if(icent==5) {
        sprintf(hname, "pi0_eta_gamma_decay_cent50_60p.root");
    } else if(icent==-1) {
        sprintf(hname, "pi0_eta_gamma_decay_minbias.root");
    } else if(icent==6) {
        sprintf(hname, "pi0_eta_gamma_decay_pp.root");
    }
    TFile* result = new TFile(hname, "recreate");
    //nt_sngl->Write();
    //nt_pair->Write();
    h_pi0->Write();
    h_eta->Write();
    fun_pi0->Write();
    fun_eta->Write();
    result->Close();
}
//.. produce decay kinetics ....
void get_kinetics(TF1* f_in, float& pt, float& eta, float& phi, float& px, float& py, float& pz)
{
        pt = f_in->GetRandom();
        eta = -2 + 4*gRandom->Rndm();
        phi = 6.28*gRandom->Rndm();
        px = pt*cos(phi);
        py = pt*sin(phi);
        pz = pt/tan(2*atan(exp(-eta)));
}
//.. decay the particles and fill the histogram
void Decay_Fill(TPythia6Decayer* pydecay, TLorentzVector* b, float px, float py, float pz, float E, int kf, TClonesArray& ptl, TNtuple* nt_sngl, TNtuple* nt_pair, TH1F* h, float weight, float peta, float ppt, float BR)
{
    b->SetPxPyPzE(px, py, pz, E);

    pydecay->Decay(kf, b);

    pydecay->ImportParticles(&ptl);

    int ntrk = ptl.GetEntriesFast();

    float spx[2] = {}, spy[2] = {}, spz[2] = {}, sE[2] = {};
    int ie = 0;
    for(int i = 0; i<ntrk; i++) {
        TParticle* ptl0 = (TParticle*)ptl.At(i);
        float pt = ptl0->Pt();
        float eta = ptl0->Eta();
        int pid = ptl0->GetPdgCode();

        if(Accp(eta, pt, pid)) {
            //nt_sngl->Fill(pt, eta, peta, ppt, (float)pid, (float)kf, weight);
            h->Fill(pt, weight*BR);
        }

        if(abs(pid)==22) {
            if(ie>=2) continue;

            spx[ie] = ptl0->Px();
            spy[ie] = ptl0->Py();
            spz[ie] = ptl0->Pz();
            sE[ie] = ptl0->Energy();

            ie++;
        }
    }

    /*	
    //.. for electron pairs ..
    TLorentzVector s1;
    TLorentzVector s2;
    s1.SetPxPyPzE(spx[0], spy[0], spz[0], sE[0]);
    s2.SetPxPyPzE(spx[1], spy[1], spz[1], sE[1]);

    float mass = (s1+s2).M();

    nt_pair->Fill(mass, peta, ppt, (float)kf, weight);
    */ 
    ptl.Clear();
}

//... rough acceptance filter ...
bool Accp(float eta, float pt, int pid)
{
    if(pt>0 && fabs(eta)<0.5 && abs(pid)==22) 
        return true;

    return false;
}


//.. set parameter 
void SetParameter(TF1* fun, float p0, float p1, float p2, float p3, float p4, float p5)
{
    fun->SetParameter(0, p0);
    fun->SetParameter(1, p1);
    fun->SetParameter(2, p2);
    fun->SetParameter(3, p3);
    fun->SetParameter(4, p4);
    fun->SetParameter(5, p5);
}

//....  cocktail functions .................................
//.. pi0 function ...
double fpi0(double *x, double *par)
{
    //.. this is the new PHENIX fit in arXiv:0912.0244v1 tbl.VII ....
    return par[5]*(x[0]*par[0]*pow(exp(-par[1]*x[0] - par[2]*x[0]*x[0]) + x[0]/par[3], -par[4]));
}
//.. eta function ...
double feta(double *x, double *par)
{
    float pt = sqrt(x[0]*x[0] + m_eta*m_eta - m_pi0*m_pi0);
    //.. based on the new PHENIX fit in arXiv:0912.0244v1 tbl.VII ....
    return par[5]*(x[0]*par[0]*pow(exp(-par[1]*pt - par[2]*pt*pt) + pt/par[3], -par[4]));
}
