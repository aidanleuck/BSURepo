void bindReceiver(int, struct sockaddr_in);
void receiveDatagram(int, struct sockaddr_in);
void receiveStartSeg(int, struct sockaddr_in);
struct sockaddr_in createSource(char *);