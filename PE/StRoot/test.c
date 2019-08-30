int test(float mult =361){
   float Refmult_cent[9] = {7,16,31,54,89,138,205,299,361}; //refmult > par[i],  70-80%, 60-70%, ... ,0-5%
   for (int cent=0;cent<9;cent++)
   {
      if (mult<Refmult_cent[cent]) return cent-1;
   }
    return 8;
}

float fun(float x){
  float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  if (x>70) return 1;
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  /* else return p[0] + p[1]*x +p[2]*x*x+p[3]*std::pow(x,3)+p[4]*std::pow(x,4)+p[5]*std::pow(x,5)+p[6]*std::pow(x,6); */
}

/* void test(){ */
/*  */
/*   TH1D* h = new TH1D("h","h",600,0,600);   */
/*   for (int i=0;i<600;i++) */
/*     h->SetBinContent(i+1,fun(h->GetBinCenter(i+1))); */
/*  */
/*   h->Draw(); */
/* } */
/* float fun(float x){ */
/*   float fvz=0; */
/*   float parVz_mult[7]={435.9,-0.02413,-0.003707,0.0002204,1.487e-5,-2.95e-07,-1.866e-8}; */
/*   for (int i=0;i<7;i++){  */
/*       fvz +=parVz_mult[i]*std::pow(x,i); */
/*     }   */
/*     #<{(| mult=mult*1.0*parVz_mult[0]/fvz; |)}># */
/*  return fvz; */
/* } */
/*  */
/* void test(){ */
/*  */
/*   TH1D* h = new TH1D("h","h",80,-40,40);   */
/*   for (int i=0;i<80;i++) */
/*     h->SetBinContent(i+1,fun(h->GetBinCenter(i+1))); */
/*  */
/*   h->Draw(); */
/* } */
