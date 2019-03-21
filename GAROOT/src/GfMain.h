#ifndef GAROOT_GFMAIN_H
#define GAROOT_GFMAIN_H

#include "GAROOT/src/Gf3ROOT.h"
#include "GAROOT/src/ROOTFileReader.h"
#include <TList.h>
#include <TKey.h>

Gf3ROOT *g;
ROOTFileReader* file;
TObjArray *hist_array;

void GfMain(string hist_prefix="", Int_t id_low=0, Int_t id_up=0, string fname="_file0");

#endif
