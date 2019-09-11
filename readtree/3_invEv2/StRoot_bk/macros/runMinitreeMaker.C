// #include "StRoot/StMiniTreeAnalysis/StMiniTreeAnalysis.h"
void runMinitreeMaker(TString filelist="try.list",  TString outFileName="test")
{
  TStopwatch*   stopWatch = new TStopwatch();
  stopWatch->Start();
  gSystem->Load("libStMiniTreeAnalysis.so");

  StMiniTreeAnalysis*  miniTreeMaker = new StMiniTreeAnalysis(filelist, outFileName );
  miniTreeMaker->setRunList("StRoot/macros/runnumber.list");
  miniTreeMaker->getBadruns("StRoot/macros/badrunlist.txt");
  miniTreeMaker->setRecenterFile("StRoot/StMiniTreeAnalysis/eventplane/recenter.root");
  miniTreeMaker->setShiftFile("StRoot/StMiniTreeAnalysis/eventplane/shift.root");
  // -------------- USER variables -------------------------
  miniTreeMaker->Init();
  miniTreeMaker->Make();
  miniTreeMaker->Finish();
  stopWatch->Stop();   
  stopWatch->Print();
}
