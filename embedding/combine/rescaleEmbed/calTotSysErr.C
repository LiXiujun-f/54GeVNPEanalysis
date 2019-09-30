void calTotSysErr()
{
  int const nItems = 3;
  double sys[nItems] = {0.42,0.18,1.5};
  double totalsys;
  for (int i=0;i<nItems;i++)
  {
    totalsys=sys[i]*sys[i];
  }
  //totalsys=sqrt(totalsys);
  totalsys*=0.01*0.01;
  cout << totalsys <<endl; 

  TFile* fsys = TFile::Open("sysFromEmbedd.root");
  TGraph* g = (TGraph*)fsys->Get("gInvMassSys");
  int const  nbins=20;
  double x[nbins],y[nbins];

  for (int i=0;i<g->GetN();i++)
  {
     g->GetPoint(i,x[i],y[i]);
     y[i]=totalsys+y[i]*y[i];
     y[i]=sqrt(y[i]);
  }
  TGraph* gtot = new TGraph(g->GetN(),x,y);
  gtot->SaveAs("gTotSysErr.root");
 
}
