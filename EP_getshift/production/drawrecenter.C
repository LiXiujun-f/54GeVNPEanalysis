void drawrecenter()
{
  TFile* file = TFile::Open("getshift.root");
  // TFile* file = TFile::Open("incEv2.root");
  // TH2F* hEventPlaneCent2 = (TH2F*)file->Get("hEventPlaneCent_M_Sh"); 
  TH2F* hEventPlaneCent1 = (TH2F*)file->Get("hEventPlaneCent_M_Re"); 
  TH2F* hEventPlaneCent0 = (TH2F*)file->Get("hEventPlaneCent_P_Re"); 
  TH1F* mbep1 = (TH1F*)hEventPlaneCent1->ProjectionY("hh1",1,1);
  // TH1F* mbep2 = (TH1F*)hEventPlaneCent2->ProjectionY("hh2",1,1);
  TH1F* mbep0 = (TH1F*)hEventPlaneCent0->ProjectionY("hh0",1,1);
  // mbep2->SetLineColor(kRed);
  mbep0->SetLineColor(kBlue);
  // double  mean = mbep1->GetEntries()*1.0/(1.0*mbep1->GetNbinsX());
//   TLine* line = new TLine(0,mean,3.14,mean);
//   line->SetLineStyle(2);
  mbep0->Draw();
//   // mbep2->Draw("same");
  mbep1->Draw("same");
//   line->Draw("same");
//   TLegend*  l = new TLegend(0.6,0.6,0.8,0.8);
//   l->AddEntry(mbep0,"eta<-0.05 raw","l" );
//   l->AddEntry(mbep1,"eta<-0.05 recenter","l" );
//   // l->AddEntry(mbep2,"eta<-0.05 shift","l" );
//   l->Draw();
}
