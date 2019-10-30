void fun1(double x,double p0,double p1,double p2,double p3)
{
  return p0+p1*pow(x,1)+p2*pow(x,2)+p3*pow(x,3); 
}


void fun2(double x,double p1,double p2,double p3)
{

  return p1*pow(x,1)+p2*pow(x,2)+p3*pow(x,3); 

}
void fitEffFun(double* x, double* p)
{
   if (x[0]<1.1)  return fun1(x[0],p[0],p[1],p[2],p[3]);
   else if (x[0]>1.1&&x[0]<1.5) return fun1(x[0],p[4],p[5],p[6],p[7]);
   // else if (x[0]>1.5) return fun1(x[0],p[8],p[9],p[10],p[11])-fun1(1.5,p[8],p[9],p[10],p[11])+fun1(1.5,p[4],p[5],p[6],p[7]);
   else if (x[0]>1.5) return fun1(x[0],p[8],p[9],p[10],p[11]);
}
void fitEffFun2(double* x, double* p)
{
   if (x[0]<1.1)  return fun1(x[0],p[0],p[1],p[2],p[3]);
   double turning1 = fun1(1.1,p[0],p[1],p[2],p[3]); 
   double C1=turning1-fun1(1.1,0,p[5],p[6],p[7]);
   if (x[0]>=1.1&&x[0]<1.5) return fun1(x[0],C1,p[5],p[6],p[7]);
   // else if (x[0]>1.5) return fun1(x[0],p[8],p[9],p[10],p[11])-fun1(1.5,p[8],p[9],p[10],p[11])+fun1(1.5,p[4],p[5],p[6],p[7]);
   double C2=fun1(1.5,C1,p[5],p[6],p[7])-fun1(1.5,0,p[9],p[10],p[11]);
   if (x[0]>=1.5) return fun1(x[0],C2,p[9],p[10],p[11]);
}
void fitEff()
{
  TFile* file = new TFile("PhoEff_comb.root");
  TF1* fitfun = new TF1("fitfun",fitEffFun,0,4,12);
  TH1F* hRecoEff = (TH1F*)file->Get("hRecoEff_6_8");
  hRecoEff->Fit(fitfun);
  double par[12];
  TF1* fitfun2 = new TF1("fitfun2",fitEffFun2,0,4,12);
  fitfun->GetParameters(par);
  fitfun2->SetParameters(par);
  hRecoEff->Fit(fitfun2);
  fitfun2->SetNpx(10000);
  fitfun2->SaveAs("fitRecoEff_6_8.root");
}
