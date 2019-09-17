void readpm(TString filename,TString name,TFile* fout);
void pi0spectra()
{
  TFile * file = new TFile("sppi0.root","recreate");

  readpm("piminus.txt","piminus",file); 
  readpm("piplus.txt","piplus",file); 
  read0("pi0.txt","pi0",file);
}

void readpm(TString filename,TString name,TFile* fout)
{
  ifstream read;
  read.open(filename.Data());
  // TString name = "piminus";
  float x1[4][23], x2[4][23],x[4][23],y[4][23],stat[4][23],sys[4][23],toterr[4][23]; 
  char words1[20]; 
  char words2[20]; 
  string words;
  TString centname[4] = {"0_10","10_20","20_40","40_80"};
  TGraphErrors* gpim[4];
  float tmp; 
     // getline(read,words);
  for (int j=0;j<4;j++)
  { 
     // getline(read,words);
     // read>>words1>>tmp>>tmp;
     // cout<< words1<<" "<<tmp<<endl;
     for (int i=0;i<23;i++)
     {
        if (!read.good()) break;
        if (j==1&&i==22) continue;
        read>>x1[j][i]>>x2[j][i]>>y[j][i]>>stat[j][i]>>sys[j][i];
        cout <<x1[j][i]<< " ... "<< sys[j][i]<< endl;
        x[j][i]=0.5*(x1[j][i]+x2[j][i]);
        toterr[j][i]=sqrt(sys[j][i]*sys[j][i]+stat[j][i]*stat[j][i]); 
     }
     if (j!=1) gpim[j] = new TGraphErrors(23,x[j],y[j],0,toterr[j]);
     if (j==1) gpim[j] = new TGraphErrors(22,x[j],y[j],0,toterr[j]);
     gpim[j]->SetName(Form("%s_%s",name.Data(),centname[j].Data()));
     gpim[j]->Write();
  }
  read.close();
}
void read0(TString filename,TString name,TFile* fout)
{
  ifstream read;
  read.open(filename.Data());
  // TString name = "piminus";
  float x[5][23],y[5][23],stat[5][23],sys[5][23],toterr[5][23]; 
  char words1[20]; 
  char words2[20]; 
  string words;
  TString centname[5] = {"0_10","10_20","20_40","40_60","60_86"};
  TGraphErrors* gpim[5];
  float tmp; 
  int number[5]={23,23,23,23,22};
     // getline(read,words);
  for (int j=0;j<5;j++)
  { 
     for (int i=0;i<number[j];i++)
     {
        if (!read.good()) break;
        read>>x[j][i]>>y[j][i]>>stat[j][i]>>sys[j][i];
        cout <<x[j][i]<< " ... "<< sys[j][i]<< endl;
        toterr[j][i]=sqrt(sys[j][i]*sys[j][i]+stat[j][i]*stat[j][i]); 
     }
     gpim[j] = new TGraphErrors(number[j],x[j],y[j],0,toterr[j]);
     gpim[j]->SetName(Form("%s_%s",name.Data(),centname[j].Data()));
     gpim[j]->Write();
  }
  read.close();
}
