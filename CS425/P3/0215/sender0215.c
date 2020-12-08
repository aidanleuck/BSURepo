#include <stdlib.h>

#include <stdio.h>

#include <math.h>

#include <errno.h>

#include <string.h>

#include <sys/socket.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <signal.h>

#include <sys/time.h>



//#include "bppacket.h"



#define DATA_PACKET_SIZE 512

#define BUFFER_SIZE 65536

//max message size 4GB - 65536 * BUFFER_SIZE

#define PCKT_LEN 512

//define bit fields

#define DAT_OFF (1 << 7)

#define ACK_OFF (1 << 6)

#define RWA_OFF (1 << 5)

#define EOM_OFF (1 << 4)



/*bronco-protocol features:

* protocol header: segment #, ack, flags, window, size, data (optional)

* recovery from: lost datagrams, duplicate datagram delivery, out-of-order delivery, overrun

*	-- most of these can be addressed by use of a buffer / list of structs tracking old packet data

*/

void usage();



struct bppacket;



struct bppacket {



 unsigned short				seg_num; //identifying number in sequence of segments - sender-to-receiver



 unsigned short      	ack;//identfiying number (seg_num) of last-received packet - receiver-to-sender



 unsigned int				DAT : 1;

 unsigned int				ACK : 1;

 unsigned int				RWA : 1;

 unsigned int				EOM : 1;

 unsigned char 					: 4; //fill to 1 byte

 unsigned char window; //number of segments the receiver is prepared to accept (window advertisement). Included in EVERY receiver-to-sender packet



 unsigned short				size; //number of bytes of data transmitted if data segment bit field is 1 - sender-to-receiver



 char data[DATA_PACKET_SIZE]; //512



};



struct sent_buffer {

	struct bppacket* bp_list[BUFFER_SIZE];

};



struct sent_buffer *sent, *unack;



void freeBuffer(struct sent_buffer*, int size);

void freeBuffer(struct sent_buffer* buff, int size){

	int i;

	for(i = 0; i < size; i++) {free(buff->bp_list[i]);}//free bppackets

};



struct sockaddr_in source;

int sock;



sigset_t signal_set;

unsigned char window;

unsigned short receiversWindowSize, curSent, curAck, unackCnt;





int resend_last(int size); //resend unacknowledged packets

int resend_last(int size) {

	struct bppacket* mess;

	int i;

	for(i = 0; i <= size; i++){

		if(i < size) mess = unack->bp_list[i];

		else { mess = (struct bppacket*) malloc(sizeof(struct bppacket)); mess->seg_num = curSent;}

		//mess->seg_num = curSent;

		if(unackCnt == 1 || i == size) {

			mess->ack = 0;

			mess->DAT = 0;

			mess->ACK = 0;

			mess->RWA = 1;

		} else {

			mess->ack = 0;

			mess->DAT = 1;

			mess->ACK = 0;

			mess->RWA = 0;

		}

		if(mess->EOM == 1){ }//final packet unacknowledged, definitely needs to be resent 

		mess->window = 0;

		//mess->size = unack[i]->size;

		



		int bytes_sent = sendto(sock, mess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

		if (bytes_sent < 0) {

			return 0;

		} else if(i == size) free(mess);

	}

	return 1;

};

//mergeBuffers has not yet proven necessary in my implementation

/*

void mergeBuffers(struct sent_buffer*, struct sent_buffer*);

void mergeBuffers(struct sent_buffer* s, struct sent_buffer* u){

	unsigned int dif = curSent-curAck;

	for(int i = 0 ; i < dif; i++){

		s->bp_list[curAck+1] = u->bp_list[

	}

}*/ 



void timer_handler(int sig){

	//need to trigger resubmission of recent packets

	printf("Acknowledgement/window timeout\n");

	//if curack == 1, first RWA window request, else State 2 window timeout

	if(resend_last(unackCnt) == 0){

		printf("Resend error.\n");

	}

};



int main(int argc, char **argv)

{

	char* endPtr;

	if(argc < 3) //should have this file, ipaddr (127.0.0.1), port # (>1024), and '<' + input filename

		{

			printf("ERR - Invalid argcount\n");

			usage();

			exit(EXIT_FAILURE);

		}



	int bytes_sent;

	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == -1) {

	printf("Error -- Socket Creation Failed");

	exit(EXIT_FAILURE);

	}

	memset(&source, 0, sizeof(source)); //empty source

	source.sin_family = AF_INET; //ipv4



	sent = (struct sent_buffer*) malloc(sizeof(struct sent_buffer));

	unack = (struct sent_buffer*) malloc(sizeof(struct sent_buffer));



 	

	/* IPv4 addresses is a uint32_t, convert a string representation of the octets to the appropriate value */

	source.sin_addr.s_addr = inet_addr(argv[1]);

	source.sin_port = htons(strtol(argv[2], &endPtr, 10));



	char *tempbuf = (char *) malloc(DATA_PACKET_SIZE); //again, size of packet - size of header

	unsigned int tracker = 0;

	int end = 0;

	

	//timer

	struct sigaction sa;

	struct itimerval timer;

	memset (&sa, 0, sizeof (sa));

	sa.sa_handler = &timer_handler;

	sigaction (SIGALRM, &sa, NULL);

	//sigaction(EWOULDBLOCK, &sa, NULL);

	timer.it_value.tv_sec = 10;

 	timer.it_value.tv_usec = 0;

	timer.it_interval.tv_sec = 0;

	timer.it_interval.tv_usec = 0;

	sigemptyset(&signal_set);

	sigaddset(&signal_set, SIGALRM);

	//sigaddset(&signal_set, EWOULDBLOCK);

	int sig_number;

	if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&(timer.it_value),sizeof(timer.it_value)) < 0)

	{

		perror("Error: setsockopt\n");

		exit(1);

	}



	/*

When the sender starts, it initializes receiversWindowSize=0 and transmits a RWA segment to the receiver, and enters S0 to await a window advertisement from the receiver.



S0 (Waiting for receiver to become ready): If the timer expires, the sender re-transmits the RWA segment to the receiver and waits.  When the sender receives a WIN > 0 (window advertisement) then it transitions to S1.



S1 (Sending data segments): The sender loops transmitting data segments and decrementing the receiversWindowSize.  When the sender receives a WIN advertisement from the receiver, it sets the receiversWindowSize equal to the advertised value.  When receiversWIndowSize reaches 0, then the sender transitions to S2.  When the entire message has been transmitted, the sender enters S3.  When the sender receives an ACK-n (acknowledgment for segment, n), it frees all in-memory segments for sequence numbers <= n.  



S2 (Paused):  The sender waits.  When it receives a WIN > 0 (window advertisement) it sets the receiversWindowSize equal to the advertised value and transitions to S1.  If the timer expires, the sender retransmits any unacknowledged segments.  When the sender receives an ACK-n (acknowledgment for segment, n), it frees all in-memory segments for sequence numbers <= n.



S3 (Finishing):  The sender waits to receive ACKs for all transmitted data segments.  When the sender receives an ACK-n (acknowledgment for segment, n), it frees all segments for sequence numbers <= n.  When the timer expires, the sender retransmits any unacknowledged segments.  When there are no unacknowledged segments, the sender program exits.

	*/

	struct bppacket* mess;

	socklen_t fromlen;

	fromlen = sizeof(source);

	int state = 0;

	unsigned int flags = 0;

	receiversWindowSize = 0; curSent = 0; curAck = 0; unackCnt = 0;

/*********************

//random chance generator, used to drop/scramble packets

time_t t;

srand((unsigned) time(&t));

int random = rand() % 50;

if(random < 2) continue; // 1/25th of the time, drop packet

int random = rand() % 50;

if(random < 2) { // 1/25th of the time, hold packet to send later

	

}

*********************/

	while(1)

	{

		switch(state) {

		case 0: {

		 //printf("Case0\n");

		//send RWA message

			struct bppacket* mess = (struct bppacket *) malloc(sizeof(struct bppacket));

			mess->seg_num = 0; //first RWA message gets 0, subsequent ones get the window frame they sit on

			mess->ack = 0;

			mess->DAT = 0;

			mess->ACK = 0;

			mess->RWA = 1;

			mess->EOM = 0;

			mess->window = 0;

			mess->size = 0;

			memset(mess->data, 0, sizeof(mess->data));



			bytes_sent = sendto(sock, mess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

			if (bytes_sent < 0) {

				printf("Error sending packet: %s\n", strerror(errno));

				exit(EXIT_FAILURE);

			}

			//wait to receive window

			unack->bp_list[0] = mess;

			struct bppacket* received = (struct bppacket*) malloc(sizeof(struct bppacket));

			if( setitimer(ITIMER_REAL, &timer, NULL) == -1) { perror("Error calling setitimer()"); exit(EXIT_FAILURE); };

			int recsize = -1;

			unackCnt = 1;

			if( (recsize = recvfrom(sock, received, sizeof(struct bppacket), 0, (struct sockaddr*)&source, &fromlen)) < 0 ){

				//error

			}

			if( recsize > 0) printf("Sender receives response - Advertised Window Size: %d\n", received->window);

			freeBuffer(unack, 1);

			//sigwait (&signal_set, &sig_number);

			

			////////////////////////////////// useless data to collect? do we need or could we use anything outside of window size?

			unsigned int DAT = received->DAT;

			unsigned int ACK = received->ACK;		

			unsigned int RWA = received->RWA;

			unsigned int EOM = received->EOM;

			unsigned short seg_num = received->seg_num;

			unsigned short ack = received->ack;

			//////////////////////////////////

	    		//should track 10 sec waiting, then pass a SIGALRM or SIGINT to indicate failure to communicate

	    		

			if(received->window > 0){

				window = received->window;

				receiversWindowSize = received->window;

				state = 1;

			}

			unackCnt = 0;

			curSent++;

			//}

			break;

		}

		case 1: { //printf("Case1\n");

		//sending standard messages

			ssize_t data_seg = fread(tempbuf, sizeof(char), DATA_PACKET_SIZE, stdin);

			int i = strlen(tempbuf);

			if(data_seg < DATA_PACKET_SIZE) { //likely at eof

				i = data_seg;

				end = 1;

				//printf("Should be ending...\n");

			}

			char* s = tempbuf;

			//printf("tracker : %d\t strlen(tempbuf): %d\n", tracker, i);

			tracker+=i;



			



			mess = (struct bppacket *) malloc(sizeof(struct bppacket));

			mess->seg_num = curSent;

			mess->ack = 0;

			mess->DAT = 1;

			mess->ACK = 0;

			mess->RWA = 0;

			mess->EOM = 0;

			if(end == 1) mess->EOM = 1;

			mess->window = 0;

			mess->size = i;

			memcpy(&mess->data, s, i);







			bytes_sent = sendto(sock, mess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

			if (bytes_sent < 0) {

				printf("Error sending packet: %s\n", strerror(errno));

				exit(EXIT_FAILURE);

			}

			unack->bp_list[unackCnt] = mess;

			unackCnt++;

			curSent++;

			receiversWindowSize--;

			if(receiversWindowSize <= 0) {state = 2; break;}

			if(end == 1) {

				state = 3;

			}

			break;

		}	

		case 2: {

		//printf("Case2\n");//waiting on valid receiverWindowSize & cumulative ack

			mess = (struct bppacket *) malloc(sizeof(struct bppacket));

			printf("Sent RWA seg_num:%d\n",curSent);

			mess->seg_num = curSent;

			mess->ack = 0;

			mess->DAT = 0;

			mess->ACK = 0;

			mess->RWA = 1;

			mess->EOM = 0;

			mess->window = 0;

			mess->size = 0;

			//memset(&mess->data, 0, sizeof(&mess->data));//clear buffer for sending

			memset(mess->data, 0, sizeof(mess->data));



			bytes_sent = sendto(sock, mess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

			if (bytes_sent < 0) {

				printf("Error sending packet: %s\n", strerror(errno));

				exit(EXIT_FAILURE);

			}

			//wait to receive window & ack

			for(;;) {

				//

				struct bppacket* received = (struct bppacket*) malloc(sizeof(struct bppacket));

				if( setitimer(ITIMER_REAL, &timer, NULL) == -1) { perror("Error calling setitimer()"); exit(EXIT_FAILURE); };

				int recsize = -1;

				if( (recsize = recvfrom(sock, received, sizeof(struct bppacket), 0, (struct sockaddr*)&source, &fromlen)) < 0 ){

					//waiting 

				}

				//sigwait (&signal_set, &sig_number);

				unsigned int DAT = received->DAT;

				unsigned int ACK = received->ACK;		

				unsigned int RWA = received->RWA;

				unsigned int EOM = received->EOM;



				unsigned short seg_num = received->seg_num; 

				unsigned short ack = received->ack; //need to check with and update unack and sent

				if(ack != (curAck+window)%65536) {//ack should be for all packets in last window, need to resend window

					printf("Not expected ack %d: Was %d Resending last window. \n", (curAck+window+1)%65536, ack);

					resend_last(window);

					sleep(100);

					continue;//skip rest of loop, resume waiting

				}

				int i;

				for(i = 0; i < unackCnt; i++){//more efficient implementation of mergeBuffer

					sent->bp_list[curSent+i]=unack->bp_list[i];

					memset(unack->bp_list[i],0,sizeof(unsigned int)); //clear unack pointers, without freeing memory of bppackets

				}

				curAck += unackCnt+1;//should always be window+1 for RWA, excluding EOM ack

				

				unackCnt = 0;

		    		sleep(1);

		    		printf("Sender receives response - Advertised Window Size: %d\n", received->window);

				if(received->window > 0){

					window = received->window;

					receiversWindowSize = received->window;

					state = 1;

				}

				

				//should track 10 sec waiting, then pass a SIGALRM to indicate failure to communicate

				break;

			}

			curSent++;

			break;

		}

		case 3: {

		

			//waiting on cumulative ack

			for(;;) 

			{

				//

				struct bppacket* received = (struct bppacket*) malloc(sizeof(struct bppacket));

				if( setitimer(ITIMER_REAL, &timer, NULL) == -1) { perror("Error calling setitimer()"); exit(EXIT_FAILURE); };

				int recsize = -1;

				if( (recsize = recvfrom(sock, received, sizeof(struct bppacket), 0, (struct sockaddr*)&source, &fromlen)) < 0 )

				{

					//waiting for final ack, will resend if necessary

				}

				//sigwait (&signal_set, &sig_number);

				unsigned int DAT = received->DAT;

				unsigned int ACK = received->ACK;		

				unsigned int RWA = received->RWA;

				unsigned int EOM = received->EOM;



				unsigned short seg_num = received->seg_num;

				unsigned short ack = received->ack; 

		    		sleep(1);

		    		printf("Sender receives EOM ACK response - Receiver closing: %d\n", EOM);

				if(received->window > 0)

				{

					window = received->window;

					receiversWindowSize = received->window;

					state = 1;

				}

				//should track 10 sec waiting, then pass a SIGALRM or SIGINT to indicate failure to communicate

				break;

			}

			freeBuffer(unack, unackCnt);

			freeBuffer(sent, curSent);

			close(sock);

			exit(EXIT_SUCCESS);

			break;

		}

		if(end) 

		{

			break;

		}



		}//endof case?

		//TDOD: additional requirements of P2? 

	}



	if (bytes_sent < 0) {

	  printf("Error sending packet: %s\n", strerror(errno));

	  exit(EXIT_FAILURE);

	}

  close(sock); /* close the socket */

}



void usage(void) {

	printf("Usage:\t sender [ip addr] [port no.] < [input filename]"); 

}