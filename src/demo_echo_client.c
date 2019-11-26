#include <stdint.h>

#include "common.h"
#include "sx1278.h"

static uint8_t frame_data_s[1024];
static uint8_t frame_data_r[1024];

int client_echo_main()
{
    int r;
    struct frame_head *f;
    uint32_t slen;


    f = (struct frame_head *)frame_data_s;
    f->addr = 0xaaaa;
    f->channel = 0x5;
    f->type = FT_RAW_DATA;
    f->len  = 1;
    f->value[0] = 0x0;

    while (1) {
        slen = sizeof(struct frame_head) + f->len;
        printf("send %d (%d + %d)\n", slen, sizeof(struct frame_head), f->len);

        sx1278_send(f->addr, f->channel, f->value, f->len);
        f->value[0]++;


        r = sx1278_recv(frame_data_r, sizeof(frame_data_r));
        if (r > 0) {
            f = (struct frame_head *)frame_data_r;

            DUMP_VAR2(f->addr);
            DUMP_VAR1(f->channel);
            DUMP_VAR1(f->type);
            DUMP_VAR2(f->len);
            dumpb(f->value, f->len);

        } else {
            printf("client recv timeout\r\n");
        }


    }

}
