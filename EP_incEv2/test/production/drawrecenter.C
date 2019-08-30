void drawrecenter()
{
  // TFile* file = TFile::Open("getshift.root");
  TFile* file = TFile::Open("incEv2.root");
  TH2F* hEventPlaneCent2 = (TH2F*)file->Get("hEventPlaneCent_M_Sh"); 
  TH2F* hEventPlaneCent1 = (TH2F*)file->Get("hEventPlaneCent_M_Re"); 
  TH2F* hEventPlaneCent0 = (TH2F*)file->Get("hEventPlaneCent_M"); 
  TH1F* mbep1 = (TH1F*)hEventPlaneCent1->ProjectionY("hh1",8,8);
  TH1F* mbep2 = (TH1F*)hEventPlaneCent2->ProjectionY("hh2",8,8);
  TH1F* mbep0 = (TH1F*)hEventPlaneCent0->ProjectionY("hh0",8,8);
  mbep2->SetLineColor(kRed);
  mbep0->SetLineColor(kBlue);
  mbep1->Draw();
  mbep2->Draw("same");
  mbep0->Draw("same");
}
