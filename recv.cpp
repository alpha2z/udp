
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define RECV_PORT 54321

#include "CUDP.h"

int main(int argc, char* argv[]) {

    CUDP clRecv;
    clRecv.Config(RECV_PORT, "", false);
    char * data = NULL;
    while (1) {
        if (clRecv.RecvUdp(data)) {
            printf("recv %s\n", data);
            free(data);
            data = NULL;
        }

        else
            break;
    }

    return 1;
}

