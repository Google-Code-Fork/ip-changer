#include "history.h"

History::History(std::string file):
        filename(file)
{}

bool History::read(std::list<std::string> &addresses){
    std::ifstream history(filename.c_str());
    if(!history.is_open())
        return false;

    while(history.good()){
        char line[maxLineSize];
        history.getline(line, maxLineSize);
        if(strlen(line)>=static_cast<int>(minIPSize))
            addresses.push_front(line);
    }
    history.close();
    return true;
}

bool History::write(std::string ip){
    std::fstream history(filename.c_str(), std::fstream::out|std::fstream::in|std::fstream::app);

    if(!history.is_open())
        return false;

    while(history.good()){
        char line[maxLineSize];
        history.getline(line, maxLineSize);
        if(ip==line)
            return true;
    }
    history.clear();
    history<<ip<<std::endl;
    history.close();

    return true;
}
