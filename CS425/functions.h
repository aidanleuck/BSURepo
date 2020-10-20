#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 4096

int createSocket(void);
struct in_addr convertByteOrder(char *);
void bindSender(int, char*, char *);
void sendDatagram(int, char*, struct sockaddr_in);
void bindReceiver(int, char *);
void receiveDatagram(int);


int createSocket(){
    int endpoint = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(endpoint == -1){
        perror("Cannot create socket endpoint");
        return -1;
    }
    return endpoint;
}
struct in_addr convertByteOrder(char* ip){
    struct in_addr destIP;
    if(inet_aton(ip, &destIP) <0){
        perror("Error -- destination IPv4 address");
        destIP.s_addr = -1;
        return destIP;
    }
       return destIP;
}