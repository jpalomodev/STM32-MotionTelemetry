#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>

void create_packet(uint8_t seq_prev, uint8_t* packet, uint8_t* x, uint8_t* y,
                   uint8_t* z);

#endif
