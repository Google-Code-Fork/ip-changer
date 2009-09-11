#ifndef PLIST_H
#define PLIST_H

#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace pList{
    struct Proc{
      Proc(unsigned pid, std::string name) :
              pid(pid), name(name)
      {}

      unsigned pid;
      std::string name;
    };

    int getList(std::vector<Proc> &, bool);
}

#endif
