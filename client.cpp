#include "process.h"
#include "client.h"

Client::Client(int PID, std::string ver) :
        pid(PID), version(ver)
{}

bool Client::changeIP(std::string IP, int port){
    errno=0;
    Const consts;
    if(!consts.load(version))
        return false;


    Process process(pid);

    bool result=process.writeMemory(consts.RSA, RSA_KEY, strlen(RSA_KEY)+1);
    if(!result||errno)
        return false;

    for(unsigned s=0;s<consts.ServersCount;s++){
        result=process.writeMemory(consts.LoginServersStart+s*consts.ServerStep,
                        IP.c_str(), IP.size()+1);

        if(!result||errno)
            return false;
    }

    for(unsigned s=0;s<consts.ServersCount;s++){
        result=process.writeMemory(consts.LoginServersStart+s*consts.ServerStep+consts.PortStep,
                        (char*)&port, 4);
        if(!result||errno)
            return false;
    }

    return true;
}

bool Client::Const::load(std::string filename){
    std::ifstream file(filename.c_str());
    if(!file.is_open())
        return false;

    LoginServersStart=
            ServerStep=
            PortStep=
            RSA=
            ServersCount=0;


    while(file.good()){
        char buff[512];
        std::stringstream line;
        std::string s;
        file.getline(buff, 512);
        line<<buff;
        line>>s;
        line<<std::setbase(0);
        if(s=="LoginServersStart")
            line>>LoginServersStart;
        else if(s=="ServerStep")
            line>>ServerStep;
        else if(s=="PortStep")
            line>>PortStep;
        else if(s=="RSA")
            line>>RSA;
        else if(s=="ServersCount")
            line>>ServersCount;
    }

    if(!LoginServersStart|!ServerStep|!PortStep|!RSA|!ServersCount)
        return false;

    return true;
}
