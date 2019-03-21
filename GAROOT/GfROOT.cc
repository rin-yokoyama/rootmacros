#include<TROOT.h>
#include "GAROOT/src/GfMain.h"

void GfROOT(string hist_prefix = "", Int_t id_low=0, Int_t id_up=0, string fname = "_file0")
{
  gROOT->ProcessLine(".L GAROOT/src/subfunctions.cc+");
  gROOT->ProcessLine(".L GAROOT/src/ROOTFileReader.cc+");
  gROOT->ProcessLine(".L GAROOT/src/GPeakFitBase.cc+");
  gROOT->ProcessLine(".L GAROOT/src/AOrder.cc+");
  gROOT->ProcessLine(".L GAROOT/src/GPeakSearch.cc+");
  gROOT->ProcessLine(".L GAROOT/src/GPeakFit.cc+");
  gROOT->ProcessLine(".L GAROOT/src/MkMouse.cc+");
  //gROOT->ProcessLine(".L GAROOT/src/GSource.cc+");
  //gROOT->ProcessLine(".L GAROOT/src/GSourceList.cc+");
  //gROOT->ProcessLine(".L GAROOT/src/GRoughCalib.cc+");
  //gROOT->ProcessLine(".L GAROOT/src/CSVFileStream.cc+");
  gROOT->ProcessLine(".L GAROOT/src/Gf3ROOT.cc+");
  gROOT->ProcessLine(".L GAROOT/src/Gf3ROOTShortCuts.cc+");
  gROOT->ProcessLine(".L GAROOT/src/GfMain.cc+");

  GfMain(hist_prefix, id_low, id_up, fname);    
}
