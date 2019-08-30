#include <string>
#include <fstream>
void load();
void runPicoElectronMaker(TString picolist="try.list",  TString outFileName="test")
{
  TStopwatch*   stopWatch = new TStopwatch();
  stopWatch->Start();
  //Check STAR Library. Please set SL_version to the original star library used in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL18c";
  string env_SL = getenv("STAR");
  if (env_SL.find(SL_version) == string::npos)
  {
    cout << "Environment Star Library does not match the requested library in runPicoD0EventMaker.C. Exiting..." << endl;
    exit(1);
  }

  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  // load();
  gSystem->Load("StPicoEvent");
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoElectronMaker");

  chain = new StChain();

  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(2, picolist, "picoDstMaker");
  StPicoPionMaker*  picoPionMaker = new StPicoPionMaker("picoPionMaker",picolist , outFileName.Data(), picoDstMaker);
  picoPionMaker->getBadruns("StRoot/macros/badrunlist.txt");
  // -------------- USER variables -------------------------
  chain->Init();
  int nEntries = picoDstMaker->chain()->GetEntries();
  cout<<"Processing "<<nEntries<<" events..."<<endl;
  for (int iEvent = 0; iEvent < nEntries; ++iEvent)
  // for (int iEvent = 0; iEvent < 7000; ++iEvent)
  {
    chain->Clear();
    if(iEvent && iEvent%100 == 0) cout<<"... finished processing "<<iEvent<<" events."<<endl;

    int iret = chain->Make();
    if (iret)
    {
      cout << "Bad return code!" << iret << endl;
      break;
    }
  }
  cout<<"Finished processing "<<nEntries<<" events."<<endl;

  chain->Finish();
  delete chain;

  stopWatch->Stop();   
  stopWatch->Print();
}
void load(){
  gSystem->Load("StTpcDb");
  gSystem->Load("StEvent");
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  gSystem->Load("StDaqLib");
  gSystem->Load("libgen_Tables");
  gSystem->Load("libsim_Tables");
  gSystem->Load("libglobal_Tables");
  gSystem->Load("StMagF");

  gSystem->Load("St_g2t.so");
  gSystem->Load("St_geant_Maker.so");
  gSystem->Load("StAssociationMaker");
  gSystem->Load("StMcAnalysisMaker");
  gSystem->Load("libgeometry_Tables");   
  gSystem->Load("StTriggerUtilities");

  gSystem->Load("StEmcUtil");
  gSystem->Load("StEmcRawMaker");
  gSystem->Load("StEmcADCtoEMaker");
  gSystem->Load("StPreEclMaker");
  gSystem->Load("StEpcMaker");
  gSystem->Load("StEmcSimulatorMaker");

  gSystem->Load("StDbLib");
  gSystem->Load("StDbUtilities");
  gSystem->Load("StDbBroker");
  gSystem->Load("StDetectorDbMaker");
  gSystem->Load("St_db_Maker");

  gSystem->Load("StMtdHitMaker");
  gSystem->Load("StMtdUtil");
  gSystem->Load("StMtdMatchMaker");
  gSystem->Load("StMtdCalibMaker");
  gSystem->Load("StBTofUtil");
  gSystem->Load("StVpdCalibMaker");

}
