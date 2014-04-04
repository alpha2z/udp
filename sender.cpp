
#include "CUDP.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    char* message = "udp packet";

    CUDP clSender;
    clSender.Config(54321, "127.0.0.1");

    while (1) {
        clSender.SendUdp(message, strlen(message));
        sleep(2);
    }

    return 1;
}


