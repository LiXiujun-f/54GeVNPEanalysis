void myexp(Double_t *x, Double_t *par){
  if ((x[0]<1.55&&x[0]>0.8) || (x[0]<0.6&&x[0]>0.5)){
    TF1::RejectPoint();
  }
  else return std::exp(par[0]+par[1]*x[0]);
}
void constrainfit(){
  TFile* file = TFile::Open("Nsigma.root");
  TH1F* hey = (TH1F*)file->Get("hyield_e");
  TF1* fit = new TF1("fit",myexp,0.2,3,2); 
  hey->Draw();
  hey->GetXaxis()->SetRangeUser(1.5,2);
  gPad->SetLogy();
  hey->Fit(fit,"R");
} 
// void myexp(Double_t *x, Double_t *par){
//   if ((x[0]<1.9&&x[0]>1.7)||(x[0]>1&&x[0]<1.2)){
//     TF1::RejectPoint();
//   }
//   else return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_pi");
//   TF1* fit = new TF1("fit",myexp,0.5,3,2); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(0.6,3);
//   gPad->SetLogy();
//   hey->Fit(fit);
//
// } 
// void myexp(Double_t *x, Double_t *par){
//   if ((x[0]>0.4&&x[0]<0.45)){
//   TF1::RejectPoint();
//   }
//   else return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_pimg");
//   TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(0.2,0.6);
//   gPad->SetLogy();
//   hey->Fit(fit);
// } 
// void myexp(Double_t *x, Double_t *par){
//    if ((x[0]>1.7&&x[0]<1.9)){
//        TF1::RejectPoint();
//    }
//    return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_pi");
//   TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(1,3);
//   gPad->SetLogy();
//   hey->Fit(fit);
// } 
// // void myexp(Double_t *x, Double_t *par){
//    return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_p");
//   TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(2,3);
//   gPad->SetLogy();
//   hey->Fit(fit);
// } 
//
// void myexp(Double_t *x, Double_t *par){
//    if ((x[0]>0.4&&x[0]<0.65)){
//        TF1::RejectPoint();
//    }
//
//    return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_k");
//   TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(0.2,0.8);
//   gPad->SetLogy();
//   hey->Fit(fit);
// } 
