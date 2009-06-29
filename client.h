#ifndef CLIENT_H
#define CLIENT_H

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

const char RSA_KEY[]="109120132967399429278860960508995541528237502902798129123468757937\
266291492576446330739696001110603907230888610072655818825358503429057592827629436\
413108566029093628212635953836686562675849720620786279431090218017681061521755056\
710823876476444260558147179707119674283982419152118103759076030616683978566631413";

class Client
{
    const unsigned int pid;
    std::string version;
    class Const;
public:
    Client(int, std::string);
    bool changeIP(std::string, int);
};

class Client::Const{
public:
    bool load(std::string);
    int LoginServersStart;
    int ServerStep;
    int PortStep;
    int RSA;

    unsigned int ServersCount;
};

#endif // CLIENT_H
