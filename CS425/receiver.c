#include "functions.h"
/**
 * Creates and binds socket to receiver
 **/
int main(int argc, char* argv[]){
    int endpoint = createSocket(); 
    bindReceiver(endpoint, argv[1]);

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
    int buffer_len = 65536;
    char buffer[buffer_len];
    int length;
    int n = 0; // Inital value for n
    
        do { // The loop should run once to receive a zero length datagram
            n = recvfrom(endpoint, (char *)buffer, sizeof (buffer) ,0, (struct sockaddr *) &source, (socklen_t *) &length );
             if(n > 0){ // Only write to stdout if we received a file greater than 0
                 fwrite(buffer, n, 1, stdout); // Write buffer to stdout.
             }
             
        } while(n != 0); // Check if empty buffer sent
        
       
        
        

      

        
       
        
        
        
        
        
        

        
        
       
    
    
}