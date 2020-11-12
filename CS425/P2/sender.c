#define maxPayLoad 512
#include "header.h"
#include "functions.h"


volatile sig_atomic_t recvFlag = 0;

int main(int argc, char *argv[]){
    
    int endpoint = createSocket();
    bindSender(endpoint, argv[2], argv[1]);

    return 0;
}

struct sockaddr_in bindReceiverS(int endpoint, char *destPort){

    
    int destPortNum = atoi(destPort);
    struct sockaddr_in recv;
    memset(&recv, 0, sizeof recv);

    recv.sin_family = AF_INET;
    recv.sin_port = htons(destPortNum);
    recv.sin_addr.s_addr = INADDR_ANY;
    bind(endpoint, (struct sockaddr *) &recv, sizeof(recv));
    return recv;

}

/**
 * Binds addressing information for sender
 **/
void bindSender(int endpoint, char *destPort, char* ip){

    int destPortNum = atoi(destPort);
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof dest);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(destPortNum);
    dest.sin_addr = convertByteOrder(ip);
    
    bind(endpoint, (struct sockaddr *) &dest, sizeof(dest));
   

    
    
    sendDatagram(endpoint, dest);
                   
 }
 

 void sendDatagram(int endpoint, struct sockaddr_in dest){

     

     requestRWA(dest, endpoint);

     long bfr_len = 4294967295;
     char *bfr = (char *) malloc(bfr_len);
     char *curpos = bfr; // pointer to bfr
   
         freopen(NULL, "rb", stdin); // Read the actual binary data of the file
         size_t numBytes = fread(bfr, 1, bfr_len, stdin); // Reads in file byte by byte
         long fileSize = numBytes;       // Save size of the file read
         long fileLeft = fileSize;       // Keep track how much file left to send

         struct BPHead sendHeader;
         
            while(fileLeft > 0){
                 if(fileLeft >= 512){ // If there is more than 512 bytes left to transfer
                memcpy(&sendHeader.data, curpos, maxPayLoad);
                sendHeader.flag.bits.DAT = 1;
                sendHeader.size = maxPayLoad;
                
                sendto(endpoint, &sendHeader, sizeof sendHeader, 0,(const struct sockaddr*) &dest, sizeof(dest));
            
                fileLeft -= maxPayLoad;
                curpos += maxPayLoad;
                 }
                 else if(fileLeft < 512 && fileLeft > 0){ // If there are less than 512 bytes left, only send remaining data
                     sendHeader.size = fileLeft;
                     sendHeader.flag.bits.DAT = 1;
                     memcpy(&sendHeader.data, curpos, fileLeft);
                     
                     sendto(endpoint, &sendHeader, HEADER_SIZE + fileLeft, 0,(const struct sockaddr*) &dest, sizeof(dest));
                     
                     fileLeft -= maxPayLoad;
                     curpos += maxPayLoad;
                    
                 }
                  memset(&sendHeader,0, sizeof sendHeader); // Reset the header to default state
                  
                
             }
                
                 sendHeader.flag.bits.EOM = 1;
                 
                 sendto(endpoint, &sendHeader, HEADER_SIZE, 0,(const struct sockaddr*) &dest, sizeof(dest));
                 free(bfr);
 }

 void handle_alarm(int sig){
    recvFlag = 1;
 }

 void requestRWA( struct sockaddr_in dest, int endpoint){

     

     

     struct BPHead sendHeader;
     struct BPHead recvHeader;
    
     struct sigaction handler;
     
     //struct sockaddr_in source = bindReceiverS(endpoint, "12345");

     handler.sa_flags = 0;
     handler.sa_sigaction = handle_alarm;
     sigemptyset(&handler.sa_mask);

     memset(&sendHeader,0, sizeof sendHeader);
     memset(&recvHeader,0, sizeof sendHeader);

     

     int length;
     sendHeader.window = 0;
     sendHeader.flag.bits.RWA = 1;
     
     sigaction(SIGALRM, &handler, NULL);
     
     int n = sendto(endpoint, &sendHeader, HEADER_SIZE, 0,(const struct sockaddr*) &dest, sizeof(dest));
    
        while(recvHeader.window == 0){
        alarm(10);
  
        if(recvFlag){
            int n = sendto(endpoint, &sendHeader, HEADER_SIZE, 0,(const struct sockaddr*) &dest, sizeof(dest));
            recvFlag = 0;
            
        }
        
        
        if(n > 0){
               recvfrom(endpoint, &recvHeader, sizeof dest ,0, (struct sockaddr *) &dest, (socklen_t *) &length );
                printf("%d", recvHeader.window);
            
            
        }
         
        
         } 
         
     
   
 }

  
 
 
 


