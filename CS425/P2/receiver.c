

#include "header.h"
#include "recv.h"
#include "list.h"
#define WINDOW_SIZE 5
/**
 * Creates and binds socket to receiver
 **/

long expectedSequenceNum = 0;
int main(int argc, char *argv[])
{
    int endpoint = createSocket();

    struct sockaddr_in source = createSource(argv[1]);
    bindReceiver(endpoint, source);
    receiveStartSeg(endpoint, source);
    receiveDatagram(endpoint, source);

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
void bindReceiver(int endpoint, struct sockaddr_in source)
{

    bind(endpoint, (struct sockaddr *)&source, sizeof(source));
}
/**
 * Receives a datagram
 **/
void receiveDatagram(int endpoint, struct sockaddr_in source)
{
    struct BPHead recvHead;
    struct BPHead sendHead;
    memset(&recvHead, 0, sizeof recvHead);
    int length;
    sendHead.window = 5;

    recvHead.segNum = 1000; // Set some value for recvHead that wont be the first segment value

    struct DLL outOfOrder;
    initializeList(&outOfOrder);
    srand(time(NULL));
    int EOM = 0;

    do
    { // The loop should run once to receive a zero length datagram

        uint16_t compareSequence = (uint16_t) ~((~(~0 << 16) << 16)) & expectedSequenceNum;

        recvfrom(endpoint, &recvHead, sizeof recvHead, 0, (struct sockaddr *)&source, (socklen_t *)&length);

        EOM = recvHead.flag.bits.EOM;

        sendHead = recvHead;
        sendHead.window = WINDOW_SIZE - outOfOrder.count; // The number of segments the receiver is prepared to accept

        sendHead.ack = 1;

        // Only add to list if we received a data segment

        if (recvHead.flag.bits.DAT == 1 && getPointer(&outOfOrder, &sendHead) == NULL && recvHead.segNum >= expectedSequenceNum)
        {
            addToFront(&outOfOrder, &sendHead);
            sort(&outOfOrder);
        }

        if (compareSequence == recvHead.segNum)
        {
            struct Node *headPtr = outOfOrder.head;
            while (headPtr != NULL && headPtr->val->segNum <= recvHead.segNum)
            {
                sendHead.window = sendHead.window + 1;
                sendto(endpoint, headPtr->val, HEADER_SIZE, 0, (const struct sockaddr *)&source, sizeof(source));
                expectedSequenceNum++;

                if (recvHead.flag.bits.DAT)
                {
                    fwrite(&headPtr->val->data, headPtr->val->size, 1, stdout); // Write buffer to stdout.
                }
                headPtr = headPtr->next;
            }
            if (recvHead.flag.bits.RWA)
            {
                receiveStartSeg(endpoint, source);
            }
            clearList(&outOfOrder);
        }
        memset(&sendHead, 0, sizeof sendHead);

    } while (!EOM); // Check if empty buffer sent
}
void receiveStartSeg(int endpoint, struct sockaddr_in source)
{
    int length;
    struct BPHead recv;
    struct BPHead sendRecv;
    memset(&recv, 0, sizeof recv);
    memset(&sendRecv, 0, sizeof sendRecv);
    sendRecv.window = 5;

    while (!recv.flag.bits.RWA)
    {

        recvfrom(endpoint, &recv, sizeof recv, 0, (struct sockaddr *)&source, (socklen_t *)&length);
    }

    if (recv.flag.bits.RWA)
    {
        sendRecv.window = 5;
        sendto(endpoint, &sendRecv, HEADER_SIZE, 0, (const struct sockaddr *)&source, sizeof(source));
    }
}
