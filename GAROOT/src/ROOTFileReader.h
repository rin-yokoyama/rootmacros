#ifndef GAROOT_ROOTFILEREADER_H
#define GAROOT_ROOTFILEREADER_H
#include <iostream>
#include <TWebFile.h>
#include <TFile.h>
#include <TList.h>
#include <TKey.h>

using namespace std;

// class for reading a certain type of object from a root file.
class ROOTFileReader
{
 protected:
  Int_t n_obj;
  Bool_t is_web;
  TList *fList;
  TObject **fTObject;
  TFile *fTFile;
  TWebFile *fTWebFile;
 public:
  ROOTFileReader(TFile* file);
  ROOTFileReader(TWebFile* file);
  ROOTFileReader(string fname, Option_t* option = "");
  ~ROOTFileReader(void);
  Int_t ReadObj(string ClassType1, string ClassType2 = "");
  TObject* GetObj(Int_t i_obj);
};

#endif
