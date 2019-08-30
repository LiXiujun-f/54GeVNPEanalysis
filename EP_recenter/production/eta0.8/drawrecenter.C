void drawrecenter()
{
  TFile* file = TFile::Open("recenterRaw.root");
  TH2F* hEventPlaneCent = (TH2F*)file->Get("hEventPlaneCent"); 
  TH1F* mbep = (TH1F*)hEventPlaneCent->ProjectionY();
  mbep->Draw();
}
