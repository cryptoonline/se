#include "Server.h"

int main(int argc, char * argv[])
{
    Server bserver;

    if (argc >= 2) {
        int port = atoi(argv[1]);
        bserver.setPort(port);
    }

    bserver.run();
}
