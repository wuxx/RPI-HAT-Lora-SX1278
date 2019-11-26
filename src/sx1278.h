#ifndef __SX1278_H__
#define __SX1278_H__

#include <stdint.h>

#define BROADCAST_ADDR  (0xFFFF)

#pragma pack(1)
struct frame_head
{
    uint16_t addr;
    uint8_t channel;
    uint8_t type;
    uint16_t len;
    uint8_t value[0];
};

#pragma pack()

enum SX1278_FRAME_TYPE_E
{
    FT_RAW_DATA = 0,
};

int sx1278_send(uint16_t addr, uint8_t channel, uint8_t *buf, uint32_t size);
int sx1278_recv(uint8_t *buf, uint32_t size);

int sx1278_init();

#endif /* __SX1278_H__ */
