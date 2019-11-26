#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>


#define DUMP_VAR1(var)          printf(#var":\t 0x%02x\n", var)
#define DUMP_VAR2(var)          printf(#var":\t 0x%04x\n", var)
#define DUMP_VAR4(var)          printf(#var":\t 0x%08x\n", var)

int32_t dumpb(uint8_t *p, uint32_t byte_nr);

#endif /* __COMMON_H__ */
