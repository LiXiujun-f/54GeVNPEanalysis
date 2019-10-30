// void myexp(Double_t *x, Double_t *par){
//   if ((x[0]<1.5&&x[0]>0.7) || (x[0]<0.6&&x[0]>0.5)){
//   // if (  (x[0]<0.65&&x[0]>0.5)){
//     TF1::RejectPoint();
//   }
//   else 
//       // return std::exp(par[0]+par[1]*x[0]+par[2]*x[0]*x[0]);
//       return std::exp(par[0]+par[1]*x[0]+par[2]*x[0]*x[0]+par[3]*x[0]*x[0]*x[0]);
//       // return std::exp(par[0]+par[1]*x[0]);
// }
// void myexp2(Double_t *x, Double_t *par){
//     // return std::exp(par[0]+par[1]*x[0]+par[2]*x[0]*x[0]);
//       return std::exp(par[0]+par[1]*x[0]+par[2]*x[0]*x[0]+par[3]*x[0]*x[0]*x[0]);
// }
//
//
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma_2_8.root");
//   // TFile* file = TFile::Open("Nsigma_2_5.root"); 
//   // TFile* file = TFile::Open("Nsigma_6_8.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_e");
//   TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   // TF1* fit = new TF1("fit",myexp,0.2,3,2); 
//   TF1* fit2 = new TF1("fit",myexp2,0.2,3,2); 
//   // TF1* fit = new TF1("fit",myexp,0.3,0.7,2); 
//   hey->Draw();
//   // hey->GetXaxis()->SetRangeUser(0.5,1);
//   hey->GetXaxis()->SetRangeUser(2.1,3);
//   // hey->GetXaxis()->SetRangeUser(0.3,0.7);
//   gPad->SetLogy();
//   hey->Fit(fit,"R");
//   fit2->SetParameters(fit->GetParameters());
//   fit2->SetLineColor(kMagenta);
//   fit2->Draw("same"); 
// } 
// void myexp(Double_t *x, Double_t *par){
//   if ((x[0]<1.9&&x[0]>1.7)||(x[0]>1&&x[0]<1.2)){
//   // if (  (x[0]<1.2&&x[0]>1) || x[0]<1.8&&x[0]>1.65){
//     TF1::RejectPoint();
//   }
//   else return std::exp(par[0]+par[1]*x[0]);
//   // else return std::exp(par[0]+par[1]*x[0]+par[2]*x[0]*x[0]);
// }
// void constrainfit(){
//   // TFile* file = TFile::Open("Nsigma.root");
//   // TFile* file = TFile::Open("Nsigma_2_5.root");
//   TFile* file = TFile::Open("Nsigma_6_8_sys.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_pi");
//   TF1* fit = new TF1("fit",myexp,0.5,3,2); 
//   // TF1* fit = new TF1("fit",myexp,0.5,3,3); 
//   hey->Draw();
//   hey->GetXaxis()->SetRangeUser(0.5,0.8);
//   // hey->GetXaxis()->SetRangeUser(0.7,3);
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
void myexp(Double_t *x, Double_t *par){
   if ((x[0]>1.7&&x[0]<1.9)){
       TF1::RejectPoint();
   }
   return std::exp(par[0]+par[1]*x[0]);
}
void constrainfit(){
  TFile* file = TFile::Open("Nsigma_2_8.root");
  TH1F* hey = (TH1F*)file->Get("hyield_pi");
  TF1* fit = new TF1("fit",myexp,0.2,3,2); 
  hey->Draw();
  hey->GetXaxis()->SetRangeUser(2.2,3);
  gPad->SetLogy();
  hey->Fit(fit);
} 
// void myexp(Double_t *x, Double_t *par){
//    return std::exp(par[0]+par[1]*x[0]);
// }
// void constrainfit(){
//   TFile* file = TFile::Open("Nsigma_2_5.root");
//   TH1F* hey = (TH1F*)file->Get("hyield_p");
//   TH1F* hez = (TH1F*)file->Get("hyield_k");
//   hez->SetMarkerColor(kBlue);
//   TF1* fit = new TF1("fit",myexp,1,1.5,2); 
//   hey->Draw();
//   hez->Draw("same");
//   hey->GetXaxis()->SetRangeUser(1,1.5);
//   gPad->SetLogy();
//   hey->Fit(fit);
// } 

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
