#include <stdint.h>
#include <stdio.h>

static uint8_t cook(uint8_t c)
{
        /* please check the ascii code table */
        if (c >= 0x20 && c <= 0x7E) {
                return c;
        } else {
                return '.';
        }
}

int32_t dumpb(uint8_t *p, uint32_t byte_nr)
{
	uint32_t i, x;
	uint8_t buf[16];
	uint32_t count, left;

	count = byte_nr / 16;
	left  = byte_nr % 16;

	printf("[0x%08x]: ", (uint32_t)&p[0]);
	for(i = 0; i < count; i++) {
		for(x = 0; x < 16; x++) {
			buf[x] = p[i * 16 + x];
			printf("%02x ", buf[x]);
		}
		printf("  ");
		for(x = 0; x < 16; x++) {
			printf("%c", cook(buf[x]));
		}

		printf("\n[0x%08x]: ", (uint32_t)&p[(i + 1) * 16]);
	}

	if (left != 0) {
		for(x = 0; x < 16; x++) {
			if (x < left) {
				buf[x] = p[i * 16 + x];
				printf("0x%02x ", buf[x]);
			} else {
				buf[x] = ' ';
				printf("   ");
			}
		}
		printf("  ");
		for(x = 0; x < 16; x++) {
			printf("%c", cook(buf[x]));
		}

	}

	printf("\n");

	return 0;
}

