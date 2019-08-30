#include <iostream>
#include <map>
using namespace std;

int sort()
{
    map<int,int> mn;
    fstream read;
    read.open("runnumber.list",ios::in);
    int run;  
    for (int num=0;num<614;num++)
    {
       read>>run;
       mn.insert(pair<int,int>(run,num));
       cout<<run<<" "<<mn[run]<<endl;
    }
    read.close();
    read.open("sortrunnumber.list",ios::out);
    map<int,int>::iterator iter;

    for (iter = mn.begin(); iter!=mn.end(); iter++)
    {
        read<<iter->first<<endl;
    }
    read.close();
    
    return 0;
}
