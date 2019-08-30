void resolution(){
   TFile* file = TFile::Open("getshift.root");
   TProfile* prfres= (TProfile*)file->Get("EPRes"); 
   TH1F* resolution = new TH1F("resolution","resolution",9,-5,85);
   TH1F* rescheck = new TH1F("rescheck","res sqrt(deltasub)",9,-5,85);
   for (int ic=0;ic<9;ic++)
   {
      double deltaRes2Sub=0.005;
      double res2sub = prfres->GetBinContent(prfres->FindBin(ic));
      double res2suberr = prfres->GetBinError(prfres->FindBin(ic))/(2.*res2sub);
      double chiSub2 = chi(res2sub);
      double chiSub2Delta = chi(res2sub + deltaRes2Sub);
      double res2 = resEventPlane(::sqrt(2.) * chiSub2); // full event plane res.
      double mRes2Delta = resEventPlane(::sqrt(2.) * chiSub2Delta);
      double res2error = res2suberr * fabs((double)res2 - mRes2Delta)/ deltaRes2Sub;
      cout<<ic<<" "<<res2<<endl; 
      resolution->SetBinContent(9-ic,res2);
      resolution->SetBinError(9-ic,res2error);
      rescheck->SetBinContent(9-ic,sqrt(res2sub));
      rescheck->SetBinError(9-ic,sqrt(res2error*2*res2sub));
   }
   resolution->Draw();
   rescheck->Draw();
}
//-----------------------------------------------------------------------
double chi(double res) {
  // Calculates chi from the event plane resolution

  double chi   = 2.0;
  double delta = 1.0;

  for (int i = 0; i < 15; i++) {
    chi   = (resEventPlane(chi) < res) ? chi + delta : chi - delta;
    delta = delta / 2.;
  }

  return chi;
}

//-----------------------------------------------------------------------

double resEventPlane(double chi) {
  // Calculates the event plane resolution as a function of chi

  double con = 0.626657;                   // sqrt(pi/2)/2
  double arg = chi * chi / 4.;

  double res = con * chi * exp(-arg) * (TMath::BesselI0(arg) +
					  TMath::BesselI1(arg)); 

  return res;
}

