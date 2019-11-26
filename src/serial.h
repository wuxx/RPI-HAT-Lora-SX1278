#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>

void dump_byte(char *buf, uint32_t len);

int32_t serial_send(void *buf, uint32_t len);
int32_t serial_recv(void *buf, uint32_t len, uint32_t timeout_ms);
int serial_init();

#endif /* __SERIAL_H__ */
