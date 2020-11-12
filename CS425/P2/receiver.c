#include "header.h"
#include "functions.h"

void receiveStartSeg(int, struct BPHead, struct sockaddr_in);

/**
 * Creates and binds socket to receiver
 **/
int main(int argc, char* argv[]){
    int endpoint = createSocket(); 
    bindReceiver(endpoint, argv[1]);

    return 0;

}
/**
 * Binds addressing information to receiver
 **/
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
/**
 * Receives a datagram
 **/
void receiveDatagram(int endpoint ){
    struct sockaddr_in source;
    struct BPHead recvHead;
    memset(&recvHead, 0, sizeof recvHead);
    int length;
     
    receiveStartSeg(endpoint, recvHead, source);
    
        do { // The loop should run once to receive a zero length datagram
            recvfrom(endpoint, &recvHead, sizeof recvHead ,0, (struct sockaddr *) &source, (socklen_t *) &length );
            
            if(recvHead.flag.bits.DAT){
                fwrite(&recvHead.data, recvHead.size, 1, stdout); // Write buffer to stdout.
            }
        
            
             
             
        } while(!recvHead.flag.bits.EOM); // Check if empty buffer sent
           
    
}
void receiveStartSeg(int endpoint, struct BPHead recv, struct sockaddr_in source){
    int length;
    
    struct BPHead sendRecv;
    memset(&sendRecv, 0, sizeof sendRecv);
    sendRecv.window = 5;


    while(!recv.flag.bits.RWA){
        
            int ret =  recvfrom(endpoint, &recv, sizeof recv ,0, (struct sockaddr *) &source, (socklen_t *) &length );
            if(ret > 0 && recv.flag.bits.RWA){
                sendRecv.window = 5;
                sendto(endpoint, &sendRecv, HEADER_SIZE, 0,(const struct sockaddr*) &source, sizeof(source));
            }
        


    
    }
    
    
}