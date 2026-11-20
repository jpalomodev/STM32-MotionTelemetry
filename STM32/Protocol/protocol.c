#include "protocol.h"
#include <stdint.h>
#include <stdio.h>

void create_packet(uint8_t seq_prev, uint8_t* packet, uint8_t* x, uint8_t* y,
                   uint8_t* z) {
    packet[0] = 0xAA;
    packet[1] = 0x55;

    packet[2] = (seq_prev + 1) % 256;

    packet[3] = x[0];
    packet[4] = x[1];

    packet[5] = y[0];
    packet[6] = y[1];

    packet[7] = z[0];
    packet[8] = z[1];
}
