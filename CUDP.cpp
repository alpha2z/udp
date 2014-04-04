



#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>

#include "CUDP.h"

CUDP::CUDP()
:m_bIsConnect(false)
,m_bSender(false) {

}


CUDP::~CUDP() {
	DisConnect();
}

void CUDP::DisConnect() {
    if (m_bIsConnect) {
        close(m_nSock);
        m_bIsConnect = false;
    }
}

bool CUDP::Config(size_t szPort, char* pAddr, bool bSender) {
    assert(!m_bIsConnect);

    memset(&m_stSock, 0, sizeof(m_stSock));
    m_stSock.sin_family = AF_INET;
    m_stSock.sin_port = htons(szPort);
    if (bSender) {
        m_stSock.sin_addr.s_addr = inet_addr(pAddr);
    } else {
        m_stSock.sin_addr.s_addr = INADDR_ANY;
    }

    m_nSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_nSock < 0) {
        perror("create sock failed");
        return false;
    }

    if (!bSender) {
        if (bind(m_nSock, (struct sockaddr*)&m_stSock, sizeof(m_stSock))) {
            perror("bind sock error");
            close(m_nSock);
            return false;
        }
    }
    m_bSender = bSender;
    m_bIsConnect = true;
    return true;
}

bool CUDP::SendUdp(char* message, short len) {
    assert(m_bSender);
    assert(m_bIsConnect);

    char header[2] = {0};
    header[0] = len & 0xFF00;
    header[1] = len & 0xFF;

    size_t sz = sizeof(m_stSock);
    int ret = sendto(m_nSock, header, 2, 0, (struct sockaddr *)&m_stSock, sz);
    if (ret != 2) {
        perror("send udp message failed");
        close(m_nSock);
        m_bIsConnect = false;
        return false;
    }

    ret = sendto(m_nSock, message, len, 0, (struct sockaddr *)&m_stSock, sz);
    if (ret != len) {
        perror("send udp message failed");
        close(m_nSock);
        m_bIsConnect = false;
        return false;
    }
    return true;
}

bool CUDP::RecvUdp(char* &buff) {
    assert(!m_bSender);
    assert(m_bIsConnect);
    assert(buff == NULL);

    size_t uLocal = sizeof(m_stSock);
    char header[2] = {0};
    int ret = recvfrom(m_nSock, header, 2, 0, (struct sockaddr*)&m_stSock, &uLocal);
    if (ret < 0) {
        perror("recv message error");
        close(m_nSock);
        m_bIsConnect = false;
        return false;
    }
    short len = ((header[0] << 8) & 0xFF00) | header[1];
    buff = reinterpret_cast<char *>(calloc(1, len));
    ret = recvfrom(m_nSock, buff, len, 0, (struct sockaddr*)&m_stSock, &uLocal);
    if (ret < 0) {
        perror("recv message error");
        close(m_nSock);
        m_bIsConnect = false;
        return false;
    }

    return true;
}

