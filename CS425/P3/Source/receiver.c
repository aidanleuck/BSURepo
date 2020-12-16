

#include "header.h"
#include "recv.h"
#include "list.h"
#define WINDOW_SIZE 5
/**
 * Creates and binds socket to receiver
 **/
int windowSize = WINDOW_SIZE;
long expectedSequenceNum = 0; // Keeps track of the expected sequence num
struct sockaddr_in source;    // receiver sockaddr
int main(int argc, char *argv[])
{
    // Create a socket
    int endpoint = createSocket();

    if (argc != 2)
    {
        fprintf(stderr, "Incorrect arguments\n");
        fprintf(stderr, "Usage: receiver port\n");

        exit(EXIT_FAILURE);
    }
    // Bind addressing information to source
    source = createSource(argv[1]);

    // Bind source to the endpoint
    bind(endpoint, (struct sockaddr *)&source, sizeof(source));

    // Receive the RWA
    receiveStartSeg(endpoint);

    // Main loop receiving datagrams
    receiveDatagram(endpoint);

    return 0;
}

/**
 * Binds addressing information to receiver
 **/

struct sockaddr_in createSource(char *destPort)
{
    int destPortNum = atoi(destPort);
    struct sockaddr_in recv;
    memset(&recv, 0, sizeof recv);

    recv.sin_family = AF_INET;
    recv.sin_port = htons(destPortNum);
    recv.sin_addr.s_addr = INADDR_ANY;

    return recv;
}
void bindReceiver(int endpoint)
{
}
/**
 * Receives a datagram
 **/
void receiveDatagram(int endpoint)
{
    int length;

    struct BPHead recvHead;
    memset(&recvHead, 0, sizeof recvHead);

    recvHead.segNum = 1000; // Set some value for recvHead that wont be the first segment value

    struct DLL outOfOrder;       // Stores segments that were received out of order
    initializeList(&outOfOrder); // Initializes list

    do
    { // The loop should run once to receive a zero length datagram

        uint16_t compareSequence = 0;

        while (windowSize > 0 && recvHead.flag.bits.EOM == 0)
        {
            recvfrom(endpoint, &recvHead, sizeof recvHead, 0, (struct sockaddr *)&source, (socklen_t *)&length);
            printSegment(&recvHead);
            windowSize--;
            if (recvHead.flag.bits.RWA)
            {
                sendRWA(endpoint, source);
            }

            if (recvHead.flag.bits.DAT == 1 && getPointer(&outOfOrder, recvHead.segNum) == NULL)
            {

                addToFront(&outOfOrder, &recvHead);
                sort(&outOfOrder);
            }
        }

        if (windowSize == 0)
        {
            windowSize = WINDOW_SIZE;
        }

        // Only add to list if we received a data segment

        if (outOfOrder.count > 0)
        {

            struct Node *prevItPtr;

            struct Node *tailPtr = outOfOrder.tail;

            while (tailPtr != NULL && tailPtr->val->segNum == compareSequence)
            {

                fwrite(&tailPtr->val->data, tailPtr->val->size, 1, stdout); // Write buffer to stdout.
                prevItPtr = tailPtr;
                tailPtr = tailPtr->prev;
                expectedSequenceNum++;
                compareSequence = (uint16_t) ~((~(~0 << 16) << 16)) & expectedSequenceNum;
            }

            struct BPHead sendACK; // Send acknowledgement
            memset(&sendACK, 0, sizeof(sendACK));
            sendACK.ack = prevItPtr->val->segNum;
            sendACK.flag.bits.ACK = 1;

            removeSegs(&outOfOrder, prevItPtr); // Remove every element we just wrote.

            // The window size is 5 - number of items that were received out of order
            sendto(endpoint, &sendACK, HEADER_SIZE, 0, (const struct sockaddr *)&source, sizeof(source));
        }

    } while (!recvHead.flag.bits.EOM); // Check if empty buffer sent
    clearList(&outOfOrder);
}

void sendRWA(int endpoint, struct sockaddr_in send)
{

    struct BPHead sendRWA; // Send header with advertisement
    memset(&sendRWA, 0, sizeof sendRWA);
    sendRWA.window = windowSize;

    sendto(endpoint, &sendRWA, HEADER_SIZE, 0, (const struct sockaddr *)&send, sizeof(send));
}

void receiveStartSeg(int endpoint)
{
    int length;
    struct sockaddr_in test;
    struct BPHead recv;     // Received header
    struct BPHead sendRecv; // Send header with advertisement
    memset(&recv, 0, sizeof recv);
    memset(&sendRecv, 0, sizeof sendRecv);
    sendRecv.window = WINDOW_SIZE;

    // Continue looping receiving until we receive a RWA
    while (!recv.flag.bits.RWA)
    {

        recvfrom(endpoint, &recv, sizeof recv, 0, (struct sockaddr *)&test, (socklen_t *)&length);
        printSegment(&recv);
    }
    // Send window advetisement to sender
    if (recv.flag.bits.RWA)
    {
        sendto(endpoint, &sendRecv, HEADER_SIZE, 0, (const struct sockaddr *)&test, sizeof(test));
    }
}
