#define maxPayLoad 512
void bindSender(int, struct sockaddr_in);
void sendDatagram(int, struct BPHead *, struct sockaddr_in, struct sockaddr_in);
struct BPHead sendRWA(int, struct sockaddr_in, struct sockaddr_in);
struct sockaddr_in createDest(char *, char *);
struct sockaddr_in createSender(int, char *, char *);
void requestAck(int, struct BPHead *, int *, struct sockaddr_in, struct sockaddr_in);
