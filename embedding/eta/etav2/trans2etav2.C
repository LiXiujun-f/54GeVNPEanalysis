//this function is to estimate the pt shift of eta if we use K v2
void trans2etav2()
{

   //[0]--> mass_eta, [1]--> mass_K
   TF1*f = new TF1("f","(sqrt(pow(sqrt(x*x+[0]*[0])-[0]+[1] ,2)-pow([1],2))-x)/x",0.1,15);
   f->SetParameter(0,0.55);
   f->SetParameter(1,0.5);
   f->GetYaxis()->SetTitle("(p_{T}^{K}-p_{T}^{#eta})/p_{T}^{#eta}");
   f->GetXaxis()->SetTitle("p_{T}^{#eta}");
   f->Draw();

} 
