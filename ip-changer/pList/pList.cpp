#include <boost/regex.hpp>
#include "pList.h"

int pList::getList(std::vector<Proc> &proc, bool all){
  FILE *stream;
  proc.clear();
  if(all)
    stream=popen("ps ax", "r");
  else
    stream=popen("ps x", "r");

  if(stream==0)
    return false;
  
  const int blockSize=1024;
  char block[blockSize];
  std::stringstream buff;
  while(fread(block, sizeof(char), blockSize, stream))
    buff.write(block, blockSize);

  fclose(stream);

  std::string line;
  while(getline(buff, line)){
    static const boost::regex exp(" *([0-9]+) +(.+) +([DRSTWXZ<NLsl+]+) +([0-9]+:[0-9]+) +(.*)");
    boost::match_results<std::string::const_iterator> results;
    if(!boost::regex_match(line, results, exp))
        continue;

    std::vector<std::string> columns;
    for(unsigned i=1;i<results.size();i++)
        columns.push_back(results[i]);

    proc.push_back(Proc(atoi(columns[0].c_str()), columns[4]));
  }

  return proc.size();
}
