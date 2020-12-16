void bindReceiver(int);
void receiveDatagram(int);
void receiveStartSeg(int);
struct sockaddr_in createSource(char *);
void sendRWA(int, struct sockaddr_in);