#include <iostream>
#include <string>

void Load();

using namespace std;

void run_StMcAnalysisMaker(string file, std::string outFile = "test")
{
   TStopwatch*   stopWatch = new TStopwatch();
   stopWatch->Start();

   //Check STAR Library. Please set SL_version to the original star library used
   // in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl

   string SL_version = "SL18c";
   string env_SL = getenv("STAR");

   if (env_SL.find(SL_version) == string::npos)
   {
      cout << "Environment Star Library does not match the requested library in run_st_etree.C. Exiting..." << endl;
      return;
   }

   // Load shared libraries
   gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
   loadSharedLibraries();
   Load();
   // gSystem->Load("StMcEvent");
   // gSystem->Load("StMcEventMaker");
   // gSystem->Load("StAssociationMaker");
   //
   gSystem->Load("StMcAnalysisMaker");
   // Create chain
   StChain* chain = new StChain;

   // I/O maker
   StIOMaker* ioMaker = new StIOMaker;
   if(file.find(".root")!=string::npos) ioMaker->SetFile(file.c_str());
   else {
   ifstream in;
   in.open(file.c_str());
   string infile;
   while (in.good()){
     getline(in, infile);
     cout<<infile<<endl;
     ioMaker->SetFile(infile.c_str());
    }
   }
   ioMaker->SetIOMode("r");
   ioMaker->SetBranch("*", 0, "0");
   // ioMaker->SetBranch("McEventBranch",0,"r");
   ioMaker->SetBranch("geantBranch", 0, "r");
   ioMaker->SetBranch("eventBranch", 0, "r");

   // TString mudstfile = file;
   //
   // if(mudstfile.First("$") != -1)
   // {
   //   mudstfile.ReplaceAll("$","");
   //   mudstfile = getenv(mudstfile.Data());
   // }
   //
   // mudstfile.ReplaceAll(".event.root", ".MuDst.root");
   // mudstfile.ReplaceAll(".geant.root", ".MuDst.root");
   // cout << "Reading MuDst file " << mudstfile << endl;
   // StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", mudstfile.Data(), "", 100000, "MuDst");
     // create list of picoDst files
   TString command = "sed 's/event/MuDst/g' " + file + " >correspondingPico.list";
   gSystem->Exec(command.Data());
   StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", "correspondingPico.list", "", 100000, "MuDst");

   StMcEventMaker *mcEventMaker = new StMcEventMaker();
   mcEventMaker->doPrintEventInfo = false;
   mcEventMaker->doPrintMemoryInfo = false;

   StAssociationMaker* assoc = new StAssociationMaker;
   assoc->useInTracker();
   assoc->SetDebug(1);

   int refMultCorrLoad = gSystem->Load("StRefMultCorr.so");
   // StRefMultCorr* refmultCorrUtil = NULL;
   //
   // if (refMultCorrLoad == -1)
   // {
   //    cout << "StRefMultCorr library is not available" << endl;
   // }
   // else
   // {
   //    refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
   // }

   // Monte Carlo event maker
   StMcAnalysisMaker* analysis = new StMcAnalysisMaker;
   
   analysis->setOutFileName(outFile);
   analysis->getBadruns("StRoot/macros/badrunlist.txt");
   // analysis->setRefMultCorr(refmultCorrUtil);

   chain->Init();
   cout << "processing "<< muDstMaker->chain()->GetEntries()<< " events..." <<endl;
   // Initialize chain
   for (int i=0;i<muDstMaker->chain()->GetEntries();i++)
   {
      chain->Clear();
      int iret = chain->Make(i);
      if (iret)
      {
        cout << "Bad return code!" << iret << endl;
        break;
      }
   }
   // chain->EventLoop(1e6);
   chain->Finish();

   delete chain;
   stopWatch->Stop();   
   stopWatch->Print();
}

void Load()
{
   // gROOT->Macro("loadMuDst.C");
   // gROOT->Macro("LoadLogger.C");
   gSystem->Load("StMcEvent");
   gSystem->Load("StMcEventMaker");
   gSystem->Load("StAssociationMaker");
   gSystem->Load("StDbLib.so");
   gSystem->Load("StDbBroker.so");
   gSystem->Load("libglobal_Tables.so");
   gSystem->Load("St_db_Maker.so");
   gSystem->Load("StDetectorDbMaker");
   gSystem->Load("StTpcDb");
   gSystem->Load("StDbUtilities");
   gSystem->Load("StEmcRawMaker");
   gSystem->Load("StEmcADCtoEMaker");
   gSystem->Load("StPreEclMaker");
   gSystem->Load("StEpcMaker");
   gSystem->Load("StEmcSimulatorMaker");
   gSystem->Load("StEmcUtil");
   gSystem->Load("StEEmcUtil");
   gSystem->Load("StEEmcDbMaker");
   gSystem->Load("StEmcTriggerMaker");
   gSystem->Load("StDaqLib");
   // gSystem->Load("StMcAnalysisMaker");
}
