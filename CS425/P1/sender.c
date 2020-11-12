#include "functions.h"
#define maxPayLoad 512

int main(int argc, char *argv[]){
    
    int endpoint = createSocket();
    bindSender(endpoint, argv[2], argv[1]);
    return 0;
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
    int bfr_len = pow(2, 20);
    char *bfr = (char *) malloc(bfr_len);
    char *curpos = bfr; // pointer to bfr
   
         freopen(NULL, "rb", stdin); // Read the actual binary data of the file
         size_t numBytes = fread(bfr, 1, bfr_len, stdin); // Reads in file byte by byte
         int fileSize = numBytes;       // Save size of the file read
         int fileLeft = fileSize;       // Keep track how much file left to read
          
             while(fileLeft > 0){
                 if(fileLeft >= 512){ // If there is more than 512 bytes left to transfer
                int n = sendto(endpoint, curpos, maxPayLoad, 0,(const struct sockaddr*) &dest, sizeof(dest));
                fileLeft -= n;
                curpos += n;
                 }
                 else{ // If there are less than 512 bytes left, only send remaining data
                     int n = sendto(endpoint, curpos, fileLeft, 0,(const struct sockaddr*) &dest, sizeof(dest));
                     fileLeft -= n;
                     curpos += n;
                 }
                
                
             }
                // Communicate EOF to receiver
                 sendto(endpoint, "", 0, 0,(const struct sockaddr*) &dest, sizeof(dest));
                   
         }
/**
 * Sends a datagram
 **/
void sendDatagram(int endpoint, char* msg, struct sockaddr_in dest){
    sendto(endpoint, msg, maxPayLoad, 0, (const struct sockaddr*) &dest, sizeof(dest));
}

