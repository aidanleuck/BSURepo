#include <stdint.h>
#define HEADER_SIZE 8

typedef struct data dataPacket;

struct data{
    char dataVal[512];
};
// Allows each individual flag bit to be retrieved and set.
 union BPFlags{
    struct{
        uint8_t : 4; // Sets first 4 bits to 0
        uint8_t EOM: 1;
        uint8_t RWA: 1;
        uint8_t ACK: 1;
        uint8_t DAT: 1;
    }bits;
    uint8_t flagValue;
}flag;

struct BPHead{
    uint16_t segNum;
    uint16_t ack;
    union BPFlags flag;
    uint8_t window;
    uint16_t size;
    dataPacket data;
};

