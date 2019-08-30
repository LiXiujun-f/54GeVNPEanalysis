/* int test(float mult =299){ */
   /* float Refmult_cent[9] = {7,16,31,54,89,138,205,299,361}; //refmult > par[i],  70-80%, 60-70%, ... ,0-5% */
   /* for (int cent=0;cent<9;cent++) */
   /* { */
   /*    if (mult<Refmult_cent[cent]) return cent-1; */
   /* } */
   /*  return 8; */
/* } */

float fun(float x){
  float p[7] = {3.9,-204.4,1.85,24.3,-0.01746,6405,3.7e-5};
  if (x>89) return 1;
  else return p[0] + p[1]/(p[2]*x + p[3]) + p[4]*(p[2]*x + p[3]) + p[5]/(p[2]*x + p[3])/(p[2]*x + p[3]) + p[6]*(p[2]*x + p[3])*(p[2]*x + p[3]);
  /* else return p[0] + p[1]*x +p[2]*x*x+p[3]*std::pow(x,3)+p[4]*std::pow(x,4)+p[5]*std::pow(x,5)+p[6]*std::pow(x,6); */
}

void test(){
  
  TH1D* h = new TH1D("h","h",600,0,600);  
  for (int i=0;i<600;i++)
    h->SetBinContent(i+1,fun(h->GetBinCenter(i+1)));

  h->Draw();
}
