#ifndef HISTORY_H
#define HISTORY_H

#include <cstring>
#include <fstream>
#include <list>

class History
{
    const std::string filename;

    static const unsigned maxLineSize=128;
    static const unsigned minIPSize=4;

public:
    History(std::string);
    bool read(std::list<std::string> &);
    bool write(std::string);
};

#endif // HISTORY_H
