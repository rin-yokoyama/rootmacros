#include"subfunctions.h"

string AppendMacroPath(string fname)
{
   string mpath = gROOT->GetMacroPath();
   //mpath = mpath.substr(2);
   //mpath = mpath.substr(0,mpath.size()-1);
   //mpath.append("/");
   mpath.append(fname);
   return mpath;
}
