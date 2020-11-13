void bindReceiver(int, struct sockaddr_in);
void receiveDatagram(int);
void receiveStartSeg(int, struct sockaddr_in);
struct sockaddr_in createSource(char*);