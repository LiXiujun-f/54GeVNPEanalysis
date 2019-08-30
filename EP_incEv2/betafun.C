void betafun(){

  TF1* f = new TF1("f","x/sqrt(x*x+[0]*[0])-x/sqrt(x*x+[1]*[1])",0,3);
  f->SetParameter(0,0.019);
  f->SetParameter(1,0.019+0.003);
  f->Draw();
}
