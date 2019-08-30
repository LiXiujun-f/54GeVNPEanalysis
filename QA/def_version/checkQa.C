#include "rootlogon.h"
#include <string>
void drawThreeLine(double y,double y1, double y2, double xl,double xh,TCanvas* c);
void getmean(TProfile* p, double & mean, double &sigma,map<int,int> &mn,set<int> &badlist);
void getmean2(TProfile* p, double & mean1,double &mean2, double &sigma1,double &sigma2, map<int,int> &mn, set<int> &badlist);
void checkQa(){
  myStyle();
  //map runId
  int const totrun = 614;
  set<int> badlist;
  map<int,int> mn;
  fstream read;
  // read.open("runnumber.list",ios::in);
  read.open("sortrunnumber.list",ios::in);
  int run;  
  for (int id=1;id<=totrun;id++)
  {
    read>>run;
    mn.insert(pair<int,int>(id,run));
    // cout<<run<<" "<<mn[id]<<endl;
  }
  read.close();
  //book profile
  const int num = 18;
  string name[num]={"VpdVz","VzVpdVz","Refmult","pVpdHitEast","pVpdHitWest",
    "pVz","pVx","pVy","Tof","Dedx","gDCA","gPt","gPhi","gEta","NFits",
    "pPt","pEta","pPhi"}; 
  TProfile* p[num];
  TH1D* h[num];
  TFile* f = TFile::Open("picoQa0313.root");
  TCanvas* c = new TCanvas("c","c",1000,600);
  for (int i=0;i<num;i++){
    p[i] = (TProfile*)f->Get(name[i].c_str());
    double mean,sigma,m1,m2,s1,s2;
    cout<<"bad run for "<<name[i].c_str()<<endl;
    getmean2(p[i],m1,m2,s1,s2,mn,badlist);
    // getmean(p[i],mean,sigma,mn,badlist);
    cout<<"out of 3 sigma"<<endl;
    for (int id=1;id<=totrun;id++){
      double val = p[i]->GetBinContent(id);
      if (id>=111) {mean=m2;sigma=s2;}
      else {mean=m1;sigma=s1;}
      if (val<(mean-sigma*3) || val>(mean+sigma*3)) {
        cout<<mn[id]<<"  ";
        badlist.insert(mn[id]);
      }
    } 
    cout<<endl;
    //draw
    c->Clear();
    p[i]->Draw("p");
    drawThreeLine(m1,m1-3*s1,m1+3*s1,0,111,c);
    drawThreeLine(m2,m2-3*s2,m2+3*s2,111,614,c);
    c->SaveAs(Form("plot/%s.png",name[i].c_str()));
  }
    set<int>::iterator it;
    cout<<"summary the badrun:"<<endl;
    for (it=badlist.begin();it!=badlist.end();++it){
      cout<<*it <<" ";
    }
    cout<<endl;
}

void drawThreeLine(double y,double y1,double y2, double xl,double xh,TCanvas* c){
  c->cd();
  TLine* l = new TLine(xl,y,xh,y);
  TLine* l1 = new TLine(xl,y1,xh,y1);
  TLine* l2 = new TLine(xl,y2,xh,y2);
  l->SetLineWidth(2);
  l1->SetLineWidth(2);
  l2->SetLineWidth(2);
  l->SetLineStyle(4);
  l1->SetLineStyle(4);
  l2->SetLineStyle(4);
  l->SetLineColor(2);
  l1->SetLineColor(4);
  l2->SetLineColor(4);
  l->Draw();
  l1->Draw();
  l2->Draw();
}

void getmean(TProfile* p, double & mean, double &sigma, map<int,int> &mn, set<int> &badlist){
  double ymax = p->GetBinContent(613);
  double ymin = p->GetBinContent(613);
  TH1D* h = new TH1D("tmp","tmp",20,ymin,ymax);
  cout<<"empty"<<endl;
  for(int i=1;i<=p->GetNbinsX();i++)  
  {
    double cnt = p->GetBinContent(i);
    if (fabs(p->GetBinContent(i)-0.)>1e-6) {
      h->Fill(p->GetBinContent(i));
      if (cnt>ymax) ymax = cnt;
      if (cnt<ymin) ymin = cnt; 
    }
    else {
      cout<<mn[i]<<"  "; 
      badlist.insert(mn[i]);
    }
  }
  cout<<endl;
  // TF1* f = new TF1("fit","gaus",ymin,ymax);
  // h->Fit(f);
  // mean=f->GetParameter(1);
  // sigma=f->GetParameter(2);
  double d = ymax-ymin;
  p->GetYaxis()->SetRangeUser(ymin-d*0.3,ymax+d*0.3);
  mean = h->GetMean();
  sigma = fabs(h->GetStdDev());
  cout <<"mean: "<<mean<<" sigma: "<<sigma <<endl;
}
void getmean2(TProfile* p, double & mean1,double &mean2, double &sigma1,double &sigma2, map<int,int> &mn, set<int> &badlist){
  double ymax = p->GetBinContent(613);
  double ymin = p->GetBinContent(613);
  TH1D* h1 = new TH1D("tmp1","tmp",20,ymin,ymax);
  TH1D* h2 = new TH1D("tmp2","tmp",20,ymin,ymax);
  cout<<"empty"<<endl;
  for(int i=1;i<=p->GetNbinsX();i++)  
  {
    double cnt = p->GetBinContent(i);
    if (fabs(p->GetBinContent(i)-0.)>1e-6) {
      if (i<=100) h1->Fill(p->GetBinContent(i));
      else if (i>150 && i<500) h2->Fill(p->GetBinContent(i));
      if (cnt>ymax) ymax = cnt;
      if (cnt<ymin) ymin = cnt; 
    }
    else {
      cout<<mn[i]<<"  "; 
      badlist.insert(mn[i]);
    }
  }
  cout<<endl;
  double d = ymax-ymin;
  p->GetYaxis()->SetRangeUser(ymin-d*0.3,ymax+d*0.3);
  mean1 = h1->GetMean();
  sigma1 = fabs(h1->GetStdDev());
  mean2 = h2->GetMean();
  sigma2 = fabs(h2->GetStdDev());

  cout <<"mean: "<<mean1<<" "<<mean2 <<" sigma: "<<sigma1<<" "<<sigma2 <<endl;
}
