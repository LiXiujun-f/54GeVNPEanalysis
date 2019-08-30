#include "TString.h"
#include <sstream>
void splitlist(int n, string listname){
  ifstream inlist;
  ofstream outlist;
  int const MAX = 1000;
  char path[MAX];
  inlist.open(listname.c_str());
  int nlines = 0;
  string presentname;
  stringstream stream;
  string tmp;
  while (inlist.good()){
    inlist.getline(path, MAX);
    if (nlines%n==0) {
      if (nlines!=0) outlist.close();
      stream << nlines/n;
      stream>>tmp;
      cout<<tmp<<endl;
      stream.clear();
      presentname = listname.substr(0, listname.find(".list"))+"_"+tmp+".list";
      cout<< presentname.c_str()<<endl;
      outlist.open(presentname.c_str());
    }
    outlist<<path<<endl;
    nlines++;
  }
  inlist.close();
  outlist.close();
  cout<<"end"<<endl;
}
