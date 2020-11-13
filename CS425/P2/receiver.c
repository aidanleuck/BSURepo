#include "header.h"
#include "recv.h"



/**
 * Creates and binds socket to receiver
 **/
int main(int argc, char* argv[]){
    int endpoint = createSocket(); 
    
    struct sockaddr_in source = createSource(argv[1]);
    bindReceiver(endpoint, source);
    receiveStartSeg(endpoint, source);
    receiveDatagram(endpoint);

    return 0;

}

/**
 * Binds addressing information to receiver
 **/

struct sockaddr_in createSource(char* destPort){
    int destPortNum = atoi(destPort);
    struct sockaddr_in recv;
    memset(&recv, 0, sizeof recv);

    recv.sin_family = AF_INET;
    recv.sin_port = htons(destPortNum);
    recv.sin_addr.s_addr = INADDR_ANY;

    return recv;
}
void bindReceiver(int endpoint, struct sockaddr_in source){
    
    bind(endpoint, (struct sockaddr *) &source, sizeof(source));

}
/**
 * Receives a datagram
 **/
void receiveDatagram(int endpoint ){
    struct sockaddr_in source;
    struct BPHead recvHead;
    memset(&recvHead, 0, sizeof recvHead);
    int length;
     
    
    
        do { // The loop should run once to receive a zero length datagram
            recvfrom(endpoint, &recvHead, sizeof recvHead ,0, (struct sockaddr *) &source, (socklen_t *) &length );
            
            if(recvHead.flag.bits.DAT){
                fwrite(&recvHead.data, recvHead.size, 1, stdout); // Write buffer to stdout.
            }
        
            
             
             
        } while(!recvHead.flag.bits.EOM); // Check if empty buffer sent
           
    
}
void receiveStartSeg(int endpoint, struct sockaddr_in source){
    int length;
    struct BPHead recv;
    struct BPHead sendRecv;
    memset(&recv, 0, sizeof recv);
    memset(&sendRecv, 0, sizeof sendRecv);
    sendRecv.window = 5;


    while(!recv.flag.bits.RWA){
        
         recvfrom(endpoint, &recv, sizeof recv ,0, (struct sockaddr *) &source, (socklen_t *) &length );
    
    }

    if(recv.flag.bits.RWA){
                sendRecv.window = 5;
                sendto(endpoint, &sendRecv, HEADER_SIZE, 0,(const struct sockaddr*) &source, sizeof(source));
            }
        


    
    }
    
    
}