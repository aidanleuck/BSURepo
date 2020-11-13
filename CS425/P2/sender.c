#include "header.h"
#include "sender.h"


volatile sig_atomic_t recvFlag = 0;

int main(int argc, char *argv[]){
    
    int endpoint = createSocket();
    struct sockaddr_in sender = createSender(endpoint, argv[1], argv[2]);
    struct sockaddr_in dest = createDest(argv[1], argv[2]);

    bindSender(endpoint, sender);

    requestRWA(endpoint, dest, sender); // Start by requesting window adv from sender
    sendDatagram(endpoint, dest, sender);

    return 0;
}

struct sockaddr_in createDest(char *ip, char *destPort){
    int destPortNum = atoi(destPort);
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof dest);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(destPortNum);
    dest.sin_addr = convertByteOrder(ip);

    return dest;
}

struct sockaddr_in createSender(int endpoint, char *ip, char *destPort){
    struct sockaddr_in sender;
    memset(&sender, 0, sizeof sender);
    sender.sin_family = AF_INET;
    sender.sin_port = htons(0);
    sender.sin_addr.s_addr = INADDR_ANY;

    return sender;
}

/**
 * Binds addressing information for sender
 **/
void bindSender(int endpoint, struct sockaddr_in sender){
    bind(endpoint, (struct sockaddr *) &sender, sizeof(sender));
                   
 }
 

 void sendDatagram(int endpoint, struct sockaddr_in dest, struct sockaddr_in sender){

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

 void requestRWA( int endpoint,struct sockaddr_in dest, struct sockaddr_in sender){

     struct BPHead sendHeader;
     struct BPHead recvHeader;
    
     struct sigaction handler;
     
     handler.sa_flags = 0;
     sigemptyset(&handler.sa_mask);
     handler.sa_handler = handle_alarm;

     memset(&sendHeader,0, sizeof sendHeader);
     memset(&recvHeader,0, sizeof sendHeader);

     

     int length;
     sendHeader.window = 0;
     sendHeader.flag.bits.RWA = 1;
     
     sigaction(SIGALRM, &handler, NULL);
     
     int n = sendto(endpoint, &sendHeader, HEADER_SIZE, 0,(const struct sockaddr*) &dest, sizeof(dest));
    
    while(recvHeader.window == 0){

        if(recvFlag){
            int n = sendto(endpoint, &sendHeader, HEADER_SIZE, 0,(const struct sockaddr*) &dest, sizeof(dest));
            recvFlag = 0;
            
        }
        alarm(10);
        recvfrom(endpoint, &recvHeader, sizeof recvHeader ,0, (struct sockaddr *) &sender, (socklen_t *) &length );
            
        
            
            
        
         
        
    } 
         
     
   
 }

  
 
 
 


