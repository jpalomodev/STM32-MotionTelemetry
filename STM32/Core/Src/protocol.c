#include "protocol.h"
#define PACKET_SIZE 11

enum PACKET_CONFIG {
    SYNC_1 = 0xA2,
    SYNC_2 = 0x55,
    SEQ = 0,
    XDATA_H = 0,
    XDATA_L = 0,
    YDATA_H = 0,
    YDATA_L = 0,
    ZDATA_H = 0,
    ZDATA_L = 0,
    CRC_1 = 0,
    CRC_2 = 0
};

static uint8_t packet[PACKET_SIZE] = {SYNC_1,  SYNC_2,  SEQ,     XDATA_H,
                                      XDATA_L, YDATA_H, YDATA_L, ZDATA_H,
                                      ZDATA_L, CRC_1,   CRC_2};

uint8_t* Create_Packet(uint8_t* buff, uint8_t seq) {
    packet[2] = seq;
    packet[3] = buff[0];
    packet[4] = buff[1];
    packet[5] = buff[2];
    packet[6] = buff[3];
    packet[7] = buff[4];
    packet[8] = buff[5];

    return packet;
}
