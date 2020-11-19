#include "header.h"
#include "sender.h"
#include "list.h"

volatile sig_atomic_t recvFlag = 0;
int receiverWindowSize = 0;

int main(int argc, char *argv[])
{

    int endpoint = createSocket();
    struct sockaddr_in sender = createSender(endpoint, argv[1], argv[2]);
    struct sockaddr_in dest = createDest(argv[1], argv[2]);

    bindSender(endpoint, sender);

    struct BPHead recvHeader = sendRWA(endpoint, dest, sender); // Start by requesting window adv from sender
    sendDatagram(endpoint, &recvHeader, dest, sender);

    return 0;
}

struct sockaddr_in createDest(char *ip, char *destPort)
{
    int destPortNum = atoi(destPort);
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof dest);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(destPortNum);
    dest.sin_addr = convertByteOrder(ip);

    return dest;
}

struct sockaddr_in createSender(int endpoint, char *ip, char *destPort)
{
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
void bindSender(int endpoint, struct sockaddr_in sender)
{
    bind(endpoint, (struct sockaddr *)&sender, sizeof(sender));
}
// Sets flag if alarm fails
void handle_alarm(int sig)
{
    recvFlag = 1;
}

void requestACK(int endpoint, struct BPHead *sendHeader, struct DLL *list, struct sockaddr_in dest, struct sockaddr_in sender)
{

    struct sigaction handler;
    int length;
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);
    handler.sa_handler = handle_alarm;
    sigaction(SIGALRM, &handler, NULL);

    alarm(10);
    // Catches the rare case when there are no more to acknowledge but the window advertisement was lost
    if (list->count == 0)
    {
        sendRWA(endpoint, dest, sender);
    }
    // Receives acknowledgement
    int val = recvfrom(endpoint, sendHeader, sizeof *sendHeader, 0, (struct sockaddr *)&sender, (socklen_t *)&length);

    if (val > 0 && sendHeader->flag.bits.ACK == 1)
    {
        struct Node *pointer = getPointer(list, sendHeader->ack);
        if (pointer != NULL)
        {
            removeSegs(list, pointer);
        }

        receiverWindowSize = sendHeader->window;
    }

    if (recvFlag)
    {
        struct Node *temp = list->tail;
        while (temp != NULL)
        {
            sendto(endpoint, temp->val, HEADER_SIZE + temp->val->size, 0, (const struct sockaddr *)&dest, sizeof(dest));
            temp = temp->prev;
        }

        recvFlag = 0;
    }
    sendHeader->flag.bits.ACK = 0;
}

void sendDatagram(int endpoint, struct BPHead *sendHeader, struct sockaddr_in dest, struct sockaddr_in sender)
{

    long bfr_len = 4294967295;           // ~4GB buffer
    char *bfr = (char *)malloc(bfr_len); // store 4gb of space for the buffer
    char *curpos = bfr;                  // Create a pointer to the start of the buffer

    long sequenceNum = 0;       // Keeps track of sequence
    struct DLL unackSegs;       // List of unacknowledged segments
    initializeList(&unackSegs); // Initializes the list

    freopen(NULL, "rb", stdin);                      // Read the actual binary data of the file
    size_t numBytes = fread(bfr, 1, bfr_len, stdin); // Reads in file byte by byte
    long fileSize = numBytes;                        // Save size of the file read
    long fileLeft = fileSize;                        // Keep track how much file left to send

    // Stop running when we have received an acknowledgement for every segment and every datagram has been transmitted.
    while (fileLeft > 0 || unackSegs.count > 0)
    {
        // Gets lower 16 bits of the sequence
        uint16_t sendSequence = (uint16_t) ~((~(~0 << 16) << 16)) & sequenceNum;

        // If the receivers window size is greater than 0 then continue sending
        if (receiverWindowSize > 0)
        {
            if (fileLeft >= 512)
            { // If there is more than 512 bytes left to transfer

                memcpy(&sendHeader->data, curpos, maxPayLoad); // Get 512 bits out of the buffer

                // Set appropriate flags
                sendHeader->flag.bits.ACK = 0;
                sendHeader->flag.bits.DAT = 1;
                sendHeader->size = maxPayLoad;
                sendHeader->segNum = sendSequence;
                // Send datagram
                sendto(endpoint, sendHeader, sizeof *sendHeader, 0, (const struct sockaddr *)&dest, sizeof(dest));
                // Increment sequence number and decrement the window size
                sequenceNum++;
                receiverWindowSize--;

                // If the unacknowledged segment is not already in the list, add and sort the list
                if (getPointer(&unackSegs, sendHeader->segNum) == NULL)
                {
                    addToFront(&unackSegs, sendHeader);
                    sort(&unackSegs);
                }
                // Request an acknowledgement
                requestACK(endpoint, sendHeader, &unackSegs, dest, sender);

                fileLeft -= maxPayLoad;
                curpos += maxPayLoad;
            }
            else if (fileLeft < 512 && fileLeft > 0)
            { // If there are less than 512 bytes left, only send remaining data

                // Set appropriate flags
                sendHeader->size = fileLeft;
                sendHeader->flag.bits.DAT = 1;
                sendHeader->segNum = sequenceNum;
                memcpy(&sendHeader->data, curpos, fileLeft);

                // Send datagram
                sendto(endpoint, sendHeader, HEADER_SIZE + fileLeft, 0, (const struct sockaddr *)&dest, sizeof(dest));

                sequenceNum++;        // Increment the sequence number
                receiverWindowSize--; // decrement window size

                // Checks for duplicates and if not a duplicate adds to list
                if (getPointer(&unackSegs, sendHeader->segNum) == NULL)
                {
                    addToFront(&unackSegs, sendHeader);
                    sort(&unackSegs);
                }

                // Request acknowledgement
                requestACK(endpoint, sendHeader, &unackSegs, dest, sender);

                fileLeft -= maxPayLoad;
                curpos += maxPayLoad;
            }
        }
        // Edge case if we received an acknowledgement for every segment, but failed to get a window update
        else
        {
            requestACK(endpoint, sendHeader, &unackSegs, dest, sender);
        }
    }
    // Send EOM
    sendHeader->flag.bits.EOM = 1;

    sendto(endpoint, sendHeader, HEADER_SIZE, 0, (const struct sockaddr *)&dest, sizeof(dest));
    free(bfr);             // free malloced buffer space
    clearList(&unackSegs); // clear the list
}

// Sends a request for a window update
struct BPHead sendRWA(int endpoint, struct sockaddr_in dest, struct sockaddr_in sender)
{

    struct BPHead sendHeader;
    struct BPHead recvHeader;
    // Timeout struct
    struct sigaction handler;

    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);
    handler.sa_handler = handle_alarm;

    memset(&sendHeader, 0, sizeof sendHeader);
    memset(&recvHeader, 0, sizeof sendHeader);

    int length;
    // Sets flags
    sendHeader.window = 0;
    sendHeader.flag.bits.RWA = 1;

    // Timeout
    sigaction(SIGALRM, &handler, NULL);

    // Sends RWA
    sendto(endpoint, &sendHeader, HEADER_SIZE, 0, (const struct sockaddr *)&dest, sizeof(dest));

    while (recvHeader.window == 0)
    {
        // If timeout occured
        if (recvFlag)
        {
            sendto(endpoint, &sendHeader, HEADER_SIZE, 0, (const struct sockaddr *)&dest, sizeof(dest));
            recvFlag = 0;
        }
        alarm(10); // Set an alarm
        // Receives receiver window update
        recvfrom(endpoint, &recvHeader, sizeof recvHeader, 0, (struct sockaddr *)&sender, (socklen_t *)&length);
    }
    receiverWindowSize = recvHeader.window;

    return recvHeader;
}
