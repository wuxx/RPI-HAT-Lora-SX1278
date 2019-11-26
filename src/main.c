#include <stdio.h>
#include <stdint.h>

#include "sx1278.h"

char buf_send[1024];
char buf_recv[1024];

extern int server_echo_main();
extern int client_echo_main();

uint32_t mode;

int main()
{
    sx1278_init();

    if(mode) {
        printf("server start\r\n");
        server_echo_main();
    } else {
        printf("client start\r\n");
        client_echo_main();
    }

    return 0;
}
