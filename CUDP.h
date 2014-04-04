
#ifndef __CUDP_H__
#define __CUDP_H__

#include <unistd.h>
#include <netinet/in.h>

class CUDP {
public:
    CUDP();
    virtual ~CUDP();

    bool Config(size_t szPort, char* pAddr, bool bSender = true);
    bool SendUdp(char * msg, short len);
    bool RecvUdp(char * &buff);
    void DisConnect();

private:
    struct sockaddr_in m_stSock;
    int m_nSock;
    bool m_bSender;
    bool m_bIsConnect;

};


#endif //__CUDP_H__
