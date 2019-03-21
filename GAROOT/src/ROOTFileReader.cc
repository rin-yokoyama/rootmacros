#include"ROOTFileReader.h"

ROOTFileReader::ROOTFileReader(TFile* file)
{
  fTFile = file;
}

ROOTFileReader::ROOTFileReader(TWebFile* file)
{
  fTWebFile = file;
}

ROOTFileReader::ROOTFileReader(string fname, Option_t* option)
{
  string fname_cpy = fname;
  fname_cpy = fname_cpy.substr(0,4);
  if(fname_cpy == "http"){
    is_web = true;
    fTWebFile = new TWebFile(fname.c_str(), option);
  }else{
    is_web = false;
    fTFile = new TFile(fname.c_str(), option);
  }
}

ROOTFileReader::~ROOTFileReader(void)
{
}

Int_t ROOTFileReader::ReadObj(string ClassType1, string ClassType2)
{
  if(is_web)
    fList = fTWebFile->GetListOfKeys();
  else
    fList = fTFile->GetListOfKeys();
  Int_t nkey = fList->GetEntries();
  fTObject = new TObject*[nkey];
  int j = 0;
  for(int i=0; i<nkey; i++){
    TKey* fKey = (TKey*)fList->At(i);
    if(fKey->ReadObj()->InheritsFrom(ClassType1.c_str()) && !fKey->ReadObj()->InheritsFrom(ClassType2.c_str())){
      fTObject[j] = fKey->ReadObj();
      j++;
    }
  }
  n_obj = j;
  if(!n_obj){
    cout << "[ROOTFileReader-E]: There is no object which inherits from " << ClassType1 << endl;
  }
  return n_obj;
}

TObject* ROOTFileReader::GetObj(Int_t i_obj)
{
  if((i_obj<0) || (n_obj<=i_obj)){
    cout << "[ROOTFileReader-E]: invalid object number :" << i_obj << endl;
    return NULL;
  }
  return fTObject[i_obj];
}
