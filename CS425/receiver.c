#include "functions.h"

int main(int argc, char* argv[]){
    int endpoint = createSocket();
    bindReceiver(endpoint, argv[0]);
}
void bindReceiver(int endpoint, char *destPort){

    
    int destPortNum = atoi(destPort);
    struct sockaddr_in recv;
    memset(&recv, 0, sizeof recv);

    recv.sin_family = AF_INET;
    recv.sin_port = htons(destPortNum);
    recv.sin_addr.s_addr = INADDR_ANY;
    
    bind(endpoint, (struct sockaddr *) &recv, sizeof(recv));
    receiveDatagram(endpoint);
}

void receiveDatagram(int endpoint ){
    struct sockaddr_in source;
    char buffer[65536];
    int len;
    int n = recvfrom(endpoint, (char *)buffer, sizeof buffer, 0, (struct sockaddr *) &source, (socklen_t *) &len );
    buffer[n] = '\0';
}