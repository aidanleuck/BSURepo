#include <stdint.h>
#define HEADER_SIZE 8
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <poll.h>


struct in_addr convertByteOrder(char *);
int createSocket(void);

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


// Creates and returns a socket fd
int createSocket(){
    int endpoint = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(endpoint == -1){
        perror("Cannot create socket endpoint");
        return -1;
    }
    return endpoint;
}
// Converts the ip address to byte order
struct in_addr convertByteOrder(char* ip){
    struct in_addr destIP;
    if(inet_aton(ip, &destIP) <0){
        perror("Error -- destination IPv4 address");
        destIP.s_addr = -1;
        return destIP;
    }
       return destIP;
}