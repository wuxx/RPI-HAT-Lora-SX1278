#include <stdint.h>

#include "common.h"
#include "sx1278.h"

static uint8_t frame_data[1024];

int server_echo_main()
{
    int r;
    struct frame_head *f;
    uint32_t slen;

    while (1) {
        r = sx1278_recv(frame_data, sizeof(frame_data));
        if (r > 0) {
            f = (struct frame_head *)frame_data;

            DUMP_VAR2(f->addr);
            DUMP_VAR1(f->channel);
            DUMP_VAR1(f->type);
            DUMP_VAR2(f->len);
            dumpb(f->value, f->len);

            f->value[0]++;

            slen = sizeof(struct frame_head) + f->len;
            printf("send %d\n", slen);

            sx1278_send(f->addr, f->channel, f->value, f->len);


        } else {
            printf(".");
        }


    }

}
