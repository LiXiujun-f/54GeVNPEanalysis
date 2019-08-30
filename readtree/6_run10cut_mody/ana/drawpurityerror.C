void drawpurityerror()
{
  TFile* f = new TFile("out.root");
  TH1F* h = (TH1F*)f->Get("hHFv2");
  h->SetDirectory(0);
  TFile* fsys = new TFile("out_sys.root");
  TH1F* hsys = (TH1F*)fsys->Get("hHFv2");
  hsys->SetDirectory(0); 
  hsys->SetLineColor(kRed);
  hsys->SetMarkerColor(kRed);
  hsys->Draw();
  h->Draw("same");
  TLegend* leg = new TLegend(0.5,0.2,0.8,0.4);
  leg->AddEntry(h,"with constrain","lp"); 
  leg->AddEntry(hsys,"w/o constrain","lp"); 
  leg->Draw();
}
