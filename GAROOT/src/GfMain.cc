#include "GAROOT/src/GfMain.h"

//This will read all histograms from file and create Gf3ROOT class.
void GfMain(string hist_prefix, Int_t id_low, Int_t id_up, string fname)
{
  if(fname == "_file0"){
    fname = gROOT->GetFile()->GetName();
    //fname = fname.substr(0, fname.size() - 2);
  }
  file = new ROOTFileReader(fname);
  
  hist_array = new TObjArray();

  if(hist_prefix==""){
    const Int_t n_obj = file->ReadObj("TH1","TH2");
    for(int i=0; i<n_obj; i++)
      hist_array->Add( (TH1F*)file->GetObj(i) );
  }
  else{
    if(!id_low && !id_up){
      const Int_t n_obj = file->ReadObj("TH1","TH2");
      Int_t nchar = hist_prefix.size();
      for(int i=0; i<n_obj; i++){
	TH1F* tmp = (TH1F*)file->GetObj(i);
	string hname = tmp->GetName();
	if(hname.substr(0,nchar) == hist_prefix)
	  hist_array->Add( tmp );
      }
    }
    else{
      const Int_t n_obj = id_up - id_low +1;
      TFile *ifile = new TFile(fname.c_str());
      for(int i=0; i<n_obj; i++){
	ostringstream oss;
	oss << hist_prefix << id_low + i;
	hist_array->Add( (TH1F*)ifile->Get(oss.str().c_str()) );
      }
    }
  }
  std::cout << "[GfMain()]: " << hist_array->GetEntries() << " histograms have been loaded." << std::endl;

  g = new Gf3ROOT(hist_array);
  return;
}

