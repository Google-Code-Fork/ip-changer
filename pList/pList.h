#ifndef PLIST_H
#define PLIST_H

#include <vector>
#include <string>
#include <algorithm>

namespace pList{
    struct Proc{
      std::string name;
      int pid;
    };

    int getList(std::vector<Proc> &, bool);
}

#endif
