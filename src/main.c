#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#include "sx1278.h"

char buf_send[1024];
char buf_recv[1024];

extern int server_echo_main();
extern int client_echo_main();

uint32_t mode;

int main(int argc, char **argv)
{
    sx1278_init();

    if (argc != 2) {
        fprintf(stderr, "usage: %s [server|client]\r\n", argv[0]);
        exit(-1);
    }

    if (strcmp(argv[1], "server") == 0) {
        printf("server start\r\n");
        server_echo_main();
    } else if (strcmp(argv[1], "client") == 0) {
        printf("client start\r\n");
        client_echo_main();
    } else {
        fprintf(stderr, "invalid arg [%s]\n", argv[1]);
        exit(-1);
    }


    return 0;
}
