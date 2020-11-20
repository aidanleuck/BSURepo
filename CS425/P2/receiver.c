

#include "header.h"
#include "recv.h"
#include "list.h"
#define WINDOW_SIZE 5
/**
 * Creates and binds socket to receiver
 **/

long expectedSequenceNum = 0;
struct sockaddr_in source;
int main(int argc, char *argv[])
{
    int endpoint = createSocket();

    source = createSource(argv[1]);

    bind(endpoint, (struct sockaddr *)&source, sizeof(source));
    receiveStartSeg(endpoint);
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

        uint16_t compareSequence = (uint16_t) ~((~(~0 << 16) << 16)) & expectedSequenceNum;

        recvfrom(endpoint, &recvHead, sizeof recvHead, 0, (struct sockaddr *)&source, (socklen_t *)&length);

        if (recvHead.flag.bits.DAT == 1 && getPointer(&outOfOrder, recvHead.segNum) == NULL && recvHead.segNum >= compareSequence)
        {
            addToFront(&outOfOrder, &recvHead);
            sort(&outOfOrder);
        }

        // Only add to list if we received a data segment

        if (compareSequence == recvHead.segNum && recvHead.flag.bits.EOM == 0)
        {
            struct Node *prevItPtr;
            if (recvHead.flag.bits.DAT)
            {
                struct Node *tailPtr = outOfOrder.tail;

                while (tailPtr != NULL && tailPtr->val->segNum <= compareSequence)
                {
                    fwrite(&tailPtr->val->data, tailPtr->val->size, 1, stdout); // Write buffer to stdout.
                    prevItPtr = tailPtr;
                    tailPtr = tailPtr->prev;
                    expectedSequenceNum++;
                    compareSequence = (uint16_t) ~((~(~0 << 16) << 16)) & expectedSequenceNum;
                }
            }

            struct BPHead sendACK; // Send acknowledgement
			memset(&sendACK, 0, sizeof(sendACK));
            sendACK.ack = prevItPtr->val->segNum;
            sendACK.flag.bits.ACK = 1;

            removeSegs(&outOfOrder, prevItPtr); // Remove every element we just wrote.

            // The window size is 5 - number of items that were received out of order
            sendACK.window = WINDOW_SIZE - outOfOrder.count;
            sendto(endpoint, &sendACK, HEADER_SIZE, 0, (const struct sockaddr *)&source, sizeof(source));

            if (recvHead.flag.bits.RWA)
            {
                receiveStartSeg(endpoint);
            }
        }

    } while (!recvHead.flag.bits.EOM); // Check if empty buffer sent
    clearList(&outOfOrder);
}
void receiveStartSeg(int endpoint)
{
    int length;
	struct sockaddr_in test;
    struct BPHead recv;
    struct BPHead sendRecv;
    memset(&recv, 0, sizeof recv);
    memset(&sendRecv, 0, sizeof sendRecv);
    sendRecv.window = 5;

    while (!recv.flag.bits.RWA)
    {

        recvfrom(endpoint, &recv, sizeof recv, 0, (struct sockaddr *)&test, (socklen_t *)&length);
    }

    if (recv.flag.bits.RWA)
    {
        sendRecv.window = 5;
        sendto(endpoint, &sendRecv, HEADER_SIZE, 0, (const struct sockaddr *)&test, sizeof(test));
    }
}
