#include"subfunctions.h"

string AppendMacroPath(string fname)
{
   string mpath = gROOT->GetMacroPath();
   std::vector<string> results;
   string::size_type pos = 0;
   while(pos != string::npos){
      string::size_type p = mpath.find(":",pos);
      if(p == string::npos){
         results.push_back(mpath.substr(pos));
	 break;
      }
      else{
         results.push_back(mpath.substr(pos,p-pos));
      }
      pos = p+1;
   }
   for(auto path: results){
      path.append("/");
      ifstream fin(path.append(fname),ios::in);
      if(fin)
         return path;
   }
   return "path_not_found";
}
