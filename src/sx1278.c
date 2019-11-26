#include "sx1278.h"
#include "serial.h"

int sx1278_recv(uint8_t *buf, uint32_t size)
{
    serial_recv(buf, size, 10000);
    return 0;
}

int sx1278_send(uint16_t addr, uint8_t channel, uint8_t *buf, uint32_t size)
{
    struct frame_head *f;
    uint32_t fsize;

    fsize = sizeof(struct frame_head) + size;
    if ((f = (struct frame_head *)malloc(fsize)) == NULL) {
        printf("malloc fail!\n");
        return -1;
    }
    
    f->addr = addr;
    f->channel = channel;
    f->type = FT_RAW_DATA;
    f->len  = size;
    memcpy(f->value, buf, size);

    serial_send((uint8_t *)f, fsize);

    return 0;
}

int sx1278_init()
{
    serial_init();
    return 0;
}
