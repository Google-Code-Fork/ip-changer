#include "pList.h"

int pList::getList(std::vector<Proc> &proc, bool all){
  FILE *stream;
  proc.clear();
  if(all)
    stream=popen("ps ax | awk '{print $1 \" \" $5}'", "r");
  else
    stream=popen("ps x | awk '{print $1 \" \" $5}'", "r");

  if(stream==0)
    return false;
  
  std::string str;
  const int max=1024;
  char buff[max];
  while(fgets(buff, max, stream)!=0)
    str.append(buff);

  fclose(stream);

  if(str.empty())
    return false;

  const int size=count(str.begin(), str.end(), '\n')-1;

  if(!size)
      return false;
  
  proc.resize(size);

  int start=0, i;

  for(i=0;i<size;i++){
    start=str.find('\n', start)+1;
    proc[i].pid=atoi(str.substr(start, str.find(' ', start)-start).c_str());

    proc[i].name=str.substr(str.find(' ', start)+1, str.find('\n', start)-str.find(' ', start)-1);
  }

  while(i--)
    if(proc[i].pid<=0||proc[i].name.empty())
      return false;

  return size;
}
