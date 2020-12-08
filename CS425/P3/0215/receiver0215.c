//sender.c

#include <stdlib.h>

#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <unistd.h> /* for close() for socket */ 



//#include "bppacket.h"



#define DATA_PACKET_SIZE 512

#define BUFFER_SIZE 65536

//max message size 4GB - 65536 * BUFFER_SIZE

#define MESSAGE_SIZE 8388608

#define WINDOW 10



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



 //bit field representations of DAT (data segment packet), ACK (acknowledgement packet), RWA (request window advertisement), EOM (end-of-message)

 unsigned int				DAT : 1;

 unsigned int				ACK : 1;

 unsigned int				RWA : 1;

 unsigned int				EOM : 1;

 unsigned char 					: 4; //fill to 1 byte

 unsigned char window; //number of segments the receiver is prepared to accept (window advertisement). Included in EVERY receiver-to-sender packet



 unsigned short				size; //number of bytes of data transmitted if data segment bit field is 1 - sender-to-receiver



 char data[PCKT_LEN]; //should be 512 at max



};



unsigned short curRec = 0, outOfOrder = 0, ackCnt = 0, seg_num = 0xFFFF, overflow = 0, unorderCnt = 0;



struct received_buffer{

	struct bppacket* bp_list[BUFFER_SIZE];

};



struct in_buffer{

	struct bppacket* bp_list[WINDOW]; //should only be requiring reordering of most recent/last window

};



void freeBuffer(struct received_buffer*);

void freeBuffer(struct received_buffer* buff){

	int i;

	for(i = 0; i < BUFFER_SIZE; i++) free(buff->bp_list[i]);

};



int sock;

struct sockaddr_in source; 

struct received_buffer* received;

struct in_buffer* reorder;



void sortBuffer(struct in_buffer* buff); //shouldn't expect to see duplicates, should have been parsed out prior

void sortBuffer(struct in_buffer* buff){ //use unorderCnt

	//sort through buffer's unordered list, organize by smallest

	struct bppacket* temp = buff->bp_list[0];

	int i;

	for(i = 0; i < unorderCnt; i++){

		unsigned int smallest = i;

		int j;

		for(j = i+1; j < unorderCnt; j++) {

			if(buff->bp_list[j]->seg_num < buff->bp_list[smallest]->seg_num){

				smallest = j;

			}

		}

		if(smallest != i) {//smaller seg_num found, time to swap smallest to front

			temp = buff->bp_list[i];

			buff->bp_list[i] = buff->bp_list[smallest];

			buff->bp_list[smallest] = temp;

		}

	}

	//out-of-order buffer should now be in order, ready to integrate into recieved

};



int checkDup(struct in_buffer*, struct bppacket*); //check for duplicates in current window

int checkDup(struct in_buffer* rec, struct bppacket* in){

	int i;

	for(i = 0; i < unorderCnt; i++) {

	//printf("bp_list[%d]->seg_num %d\n", i, rec->bp_list[i]->seg_num);

		if(rec->bp_list[i]->seg_num == in->seg_num){

			 printf("Duplicate? bp_list[%d]->seg_num %d  = in->seg_num %d\n",i,rec->bp_list[i]->seg_num,in->seg_num);

			return 1;

		}

	}

	return 0;

}



int main(int argc, char **argv) {

//setup

////////////////////////////////////////////////////////////////////////////////////////////////

	char* endPtr;

	if(argc < 2) //should have this file, port no, and '&' as args

		{

			perror("ERR - Invalid argcount");

			usage();

			exit(EXIT_FAILURE);

		}



	received = (struct received_buffer*) malloc(sizeof(struct received_buffer));

	reorder = (struct in_buffer*) malloc(sizeof(struct in_buffer));

  char buffer[BUFFER_SIZE];

  ssize_t recsize;

  socklen_t fromlen;



  memset(&source, 0, sizeof(source));

  source.sin_family = AF_INET;

  source.sin_addr.s_addr = htonl(INADDR_ANY);

  source.sin_port = htons(strtol(argv[1], &endPtr, 10));

  fromlen = sizeof(source);



  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (bind(sock, (struct sockaddr *)&source, sizeof(source)) == -1) {

    perror("ERR - Bind failed");

    close(sock);

    exit(EXIT_FAILURE);

  }

	



//packet managing loop

////////////////////////////////////////////////////////////////////////////////////////////////////

	struct bppacket* temp;

  for (;;) 

	{

		//should temporarily buffer in another bppacket until we identify duplicate or out-of-order

		temp = (struct bppacket*) malloc(sizeof(struct bppacket));

    		recsize = recvfrom(sock, (void *)temp, sizeof(struct bppacket), 0, (struct sockaddr*)&source, &fromlen);



		if (recsize < 0) {

		fprintf(stderr, "%s\n", strerror(errno));

				close(sock);

		exit(EXIT_FAILURE);

		}



		//inprocess data as a bppacket -- then filter into correct bucket

		/////////////////////////////////////////////////////////////////

		//flag retrieval

		unsigned short DAT = temp->DAT;

		unsigned short ACK = temp->ACK;		

		unsigned short RWA = temp->RWA;

		unsigned short EOM = temp->EOM;



		int dup = checkDup(reorder, temp);

		if(dup == 1  && RWA != 1){ free(temp); continue; }//ignore packet, skip to next if duplicate and not an RWA

		





		if(DAT == 1 && (seg_num+1)%65536 == temp->seg_num) {//is directly sequential and is data packet

			seg_num = temp->seg_num; //update seg_num, our last recognized, in-order packet

			reorder->bp_list[unorderCnt] = temp;

			//printf("reorder->bp_list[%d]->seg_num %d  = temp->seg_num %d\n",unorderCnt,reorder->bp_list[unorderCnt]->seg_num,temp->seg_num);

			unorderCnt++;

			

		} else if(DAT == 1 && dup == 0 && temp->seg_num >= seg_num) {//not directly sequential nor duplicate in current window (and is data packet), identify difference and work around 

			if(outOfOrder == 0) printf("Out of order sequence!\n");

			outOfOrder = 1; //we have an ordering issue to deal with, keep that in mind

			reorder->bp_list[unorderCnt] = temp;

			unorderCnt++;

		} else if(RWA == 1 && (seg_num+1)%65536 == temp->seg_num) {//RWA request in proper sequence

			seg_num = temp->seg_num; //update segment number, but don't include in bp_packet list nor counting

		}



   		printf("recsize: %u\n ", (int)recsize);

		printf("sequence number: %u curRec: %u\n", temp->seg_num, curRec);

		printf("flags: dat: %d ack: %d rwa: %d eom: %d\n", DAT, ACK, RWA, EOM);

		printf("window: %d \t size: %d \n", temp->window, temp->size);

    //if(DAT == 1 && outOfOrder != 1) printf("datagram: %.*s\n", (int)recsize, temp->data);



		if(RWA == 1 && EOM != 1) 

		{ //sender waiting on a valid window-- transmit window size we're willing to accept, as well as ack, if necessary

			struct bppacket* mess = (struct bppacket*) malloc(sizeof(struct bppacket));

			if(temp->seg_num == 0) 

			{ //first RWA, nothing to acknowledge

				mess->seg_num=0; 

				mess->ack = 0; 

				mess->DAT = 0;

				mess->ACK = 0; 

				mess->RWA = 0;

				mess->EOM = 0;

			} 

			else 

			{ //subsequent RWA/ACK

				if(unorderCnt == WINDOW) //only flush buffer into received list if window filled

				{

					if(outOfOrder == 1) 

					{

						sortBuffer(reorder); 

						printf("reordered datagram:\n");

						int i;

						for(i = 0; i < unorderCnt; i++) 

						{

							received->bp_list[curRec] = reorder->bp_list[i];

							printf("%.*s", received->bp_list[curRec]->size, received->bp_list[curRec]->data);

							//reorder->bp_list[i]=0; //clear reorder pointers??

							curRec++;

						}

						//seg_num = received->bp_list[curRec-1]->seg_num; //accurately represent new most recent seg

						unorderCnt = 0;

						outOfOrder = 0;

					} 

					else 

					{ //was in order

						printf("ordered datagram:\n");

						int i;

						for(i = 0; i < unorderCnt; i++) 

						{

							received->bp_list[curRec] = reorder->bp_list[i];

							printf("%.*s", received->bp_list[curRec]->size, received->bp_list[curRec]->data);

							//reorder->bp_list[i] = 0; //reset pointers in reorder?

							curRec++;

						}

						//seg_num = received->bp_list[curRec-1]->seg_num; //accurately represent new most recent seg

						unorderCnt = 0;

					}

				mess->DAT = 0;

				mess->ACK = 1; 

				mess->ack = curRec; //acknowledge last-seen only

				}

				else //not full window, return ack for highest sequential seg seen

				{

					mess->ACK = 1; 

					mess->ack = seg_num;

				}

			}

			mess->RWA = 0;

			mess->EOM = 0;

			mess->window = WINDOW; //arbitrarily chosen window size of 10, is there a specific desireable value?

			mess->size = 0;

			memset(mess->data, 0, sizeof(mess->data));

			printf("\nSending ack #%d...\n", mess->ack);

			int bytes_sent = sendto(sock, mess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

			free(mess);

			free(temp);

			//receiver not required to verify sender's receipt of the RWA response

		} 



		//////////////////////////////////////////////////0x8048DCE

		

		//counter to identify when to send ACK -- how about every 10? If packets get lost, expect last window to retransmit

		//better implementation is list of recent seqnum, to be cleared

		if(temp->EOM == 1) 

		{//send final cumulative ack	

			int i;	

			if(outOfOrder == 1) {

				sortBuffer(reorder);

				printf("reordered datagram: \n");

				

				for(i = 0; i < unorderCnt; i++) 

					{

						received->bp_list[curRec] = reorder->bp_list[i];

						printf("%.*s", received->bp_list[curRec]->size, received->bp_list[curRec]->data);

						//memset(reorder->bp_list[i],0,sizeof(unsigned int)); //clear reorder pointers

						curRec++;

					}

					seg_num = received->bp_list[curRec-1]->seg_num; //accurately represent new most recent seg

					unorderCnt = 0;

					outOfOrder = 0; 

			} else {

				printf("ordered datagram: \n");

				for(i = 0; i < unorderCnt; i++) 

					{

						received->bp_list[curRec] = reorder->bp_list[i];

						printf("%.*s", received->bp_list[curRec]->size, received->bp_list[curRec]->data);

						//memset(reorder->bp_list[i],0,sizeof(unsigned int)); //clear reorder pointers

						curRec++;

					}

					seg_num = received->bp_list[curRec-1]->seg_num; //accurately represent new most recent seg

					unorderCnt = 0;

			}

			struct bppacket* ackmess = (struct bppacket *) malloc(sizeof(struct bppacket));

			ackmess->seg_num = seg_num;

			ackmess->ack = seg_num; //acknowledge last-seen seg_num

			ackmess->DAT = 0;

			ackmess->ACK = 1;

			ackmess->RWA = 0;

			ackmess->EOM = 0;

			ackmess->window = 10; //arbitrarily chosen window size, is there a specific desireable value?

			ackmess->size = 0;

			memset(ackmess->data, 0, sizeof(ackmess->data));

			

			int bytes_sent = sendto(sock, ackmess, sizeof(struct bppacket), 0,(struct sockaddr*)&source, sizeof(source));

			if(bytes_sent > 0) printf("Seg_num %d acknowledged.\n", seg_num);

			free(ackmess);

			if(temp->DAT != 1) {free(temp);} //if it is a data segment, temp will be freed by below code

			printf("End of communication. Bye! \n");

			close(sock);

			if(overflow == 1) {for(i = 0; i < BUFFER_SIZE; i++) free(received->bp_list[i]);}

			else {for(i = 0; i < curRec-1; i++) free(received->bp_list[i]);}

			free(received);

			free(reorder);

			exit(EXIT_SUCCESS);

		}

		

		//free(temp);

		

		if(curRec == 0xFFFE) {overflow = 1;}

  }

  close(sock);

}



void usage(void) {

	printf("Usage:\t receiver [port no.] &"); 

}