#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <errno.h>

class Process
{
    const unsigned int pid;
public:
    Process(int);
    bool readMemory(unsigned, char*, unsigned);
    bool writeMemory(unsigned, const char*, unsigned);
};

#endif // PROCESS_H
