#define maxPayLoad 512
void bindSender(int);
void sendDatagram(int, struct BPHead *);
struct BPHead sendRWA(int);
struct sockaddr_in createDest(char *, char *);
struct sockaddr_in createSender(int, char *, char *);
void requestAck(int, struct BPHead *, int *);
