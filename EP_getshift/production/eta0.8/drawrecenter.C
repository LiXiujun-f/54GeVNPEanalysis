void drawrecenter()
{
  TFile* file = TFile::Open("getshift_0.root");
  TH2F* hEventPlaneCent = (TH2F*)file->Get("hEventPlaneCent_M_Re"); 
  TH1F* mbep = (TH1F*)hEventPlaneCent->ProjectionY();
  mbep->Draw();
}
