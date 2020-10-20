#include "functions.h"
int main(int argc, char *argv[]){
    
    int endpoint = createSocket();
    bindSender(endpoint, argv[0], argv[1]);
    return 0;
}


void bindSender(int endpoint, char *destPort, char* ip){

    int destPortNum = atoi(destPort);
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof dest);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(destPortNum);
    dest.sin_addr = convertByteOrder(ip);
    
    bind(endpoint, (struct sockaddr *) &dest, sizeof(dest));
    sendDatagram(endpoint, "Hello World", dest);

    
    
}
void sendDatagram(int endpoint, char* msg, struct sockaddr_in dest){
    sendto(endpoint, msg, strlen(msg), 0, (const struct sockaddr*) &dest, sizeof(dest) );
}

