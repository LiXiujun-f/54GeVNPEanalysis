//this code is to turn hist in to graph, so as to better estimate the purity vs pt

TGraph* turnhist2graph(TH1* h,TGraph* g, TString name)
{
  int ibin=0;
  double pt[1000],val[1000];
  for (int ib=1;ib<h->GetXaxis()->GetNbins()+1;ib++)
  {
    double p = h->GetBinCenter(ib);
    if (p>0.2) pt[ibin]=g->Eval(p);
    else pt[ibin]=p*0.65+0.73;
    val[ibin]=h->GetBinContent(ib);
    ibin++;
  }
  TGraph* graph = new TGraph(ibin,pt,val);
  graph->SetName(name.Data());
  return graph;
}

void writepurity()
{
  // TString purityname = "0630/Nsigma_0_8";
  // TString purityname = "Nsigma_2_5";
  // TString purityname = "Nsigma_6_8";
  TString purityname = "Nsigma_2_8";
  TFile* fdef = new TFile(Form("%s.root",purityname.Data()));
  // TFile* fdef = new TFile("Nsigma_0_8.root");
  TH1F* hpdef = (TH1F*)fdef->Get("hpurity")->Clone("hpurity_def");
  hpdef->SetDirectory(0);
  TH1F* hpideff_def = (TH1F*)fdef->Get("hpideff")->Clone("hpideff_def");
  hpideff_def->SetDirectory(0);
  TH1F* hpar[4];
  TH1F* hpar_sys[4];
  char name[4][50]={"piratio","kratio","pratio","mgpiratio"};
  for (int i=0;i<4;i++){
    hpar[i]= (TH1F*)fdef->Get(Form("h%s",name[i]))->Clone(Form("h%s_def",name[i]));
    hpar[i]->SetDirectory(0);
  }
  // TFile* fsys = new TFile("Nsigma_0_8_sys.root");
  TFile* fsys = new TFile(Form("%s_sys.root",purityname.Data()));
  TH1F* hpsys = (TH1F*)fsys->Get("hpurity")->Clone("hpurity_sys");
  hpsys->SetDirectory(0);
  TH1F* hpideff_sys = (TH1F*)fsys->Get("hpideff")->Clone("hpideff_sys");
  for (int i=0;i<4;i++){
    hpar_sys[i]= (TH1F*)fsys->Get(Form("h%s",name[i]))->Clone(Form("h%s_sys",name[i]));
    hpar_sys[i]->SetDirectory(0);
  }
  TFile* prj = new TFile("p_pt_MB_54.root");
  TH1F* hprj = (TH1F*)prj->Get("hePtvsP_pfy");
  // TFile* file = new TFile("fpurity.root","recreate");
  //turn it into graph
  
  TFile* file = new TFile(Form("%s_purity.root",purityname.Data()),"recreate");
  int nbins=0;
  double pt[1000],p[1000];
  for (int i=1;i<=hprj->GetXaxis()->GetNbins();i++)
  {
    pt[nbins]=hprj->GetBinContent(i);
    p[nbins]=hprj->GetBinCenter(i);
    nbins++;
  }

  TGraph* gP_Pt = new TGraph(nbins,p,pt);
  gP_Pt->SetName("gP_Pt");

 TGraph* gptsys=(TGraph*)turnhist2graph(hpsys ,gP_Pt ,"gpurity_ptsys");
 cout <<"start" << endl;
 TGraph* gptdef=(TGraph*)turnhist2graph(hpdef ,gP_Pt ,"gpurity_ptdef");
 cout <<"start" << endl;
 TGraph* gptpideff_sys = (TGraph*)turnhist2graph(hpideff_sys,gP_Pt,"gptpideff_sys"); 
 cout <<"start" << endl;
 TGraph* gptpideff_def = (TGraph*)turnhist2graph(hpideff_def,gP_Pt,"gptpideff_def"); 
 cout <<"start" << endl;
 TGraph* gptpardef[4];
 TGraph* gptparsys[4];
 for (int ip=0;ip<4;ip++)
 {
    gptparsys[ip]=(TGraph*)turnhist2graph(hpar_sys[ip],gP_Pt,Form("g%s_ptsys",name[ip]));
 cout <<"start" << endl;
    gptpardef[ip]=(TGraph*)turnhist2graph(hpar[ip],gP_Pt,Form("g%s_ptdef",name[ip])); 
 cout <<"start" << endl;
 }

  cout <<"finish" << endl; 
  hpsys->Write();
  gptsys->Write();
  hpdef->Write();
  gptdef->Write();
  gptpideff_sys->Write();
  gptpideff_def->Write();
  for (int j=0;j<4;j++) {
     gptpardef[j]->Write();
     gptparsys[j]->Write();
  }
  file->Close();
}


void rewriteHists(int nbin,double* ptedge,double* ptBin , TH1* hmom,TH1* hpt,TString name)
{
    hpt = new TH1F(Form("%s_ptsys",name.Data()),Form("%s;p_{T};ratio",name.Data()),nbins-1,ptedge);
    for (int i=1;i<=nbins;i++)
    {
       hpt->SetBinContent(i,hmom->GetBinContent(ptBin[i]));
    }
}
