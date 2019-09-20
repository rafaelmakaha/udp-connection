// Includes ---------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

// Defines -----------------------------------------------------------------------
#define PORT 53
#define MAX 65536
#define NTP_TIMESTAMP_DELTA 2208988800ull

// Structs -----------------------------------------------------------------------
typedef struct{
	unsigned short int query_id;
	uint16_t flags;
	uint16_t question_count;
	uint16_t ans_RR;
	uint16_t auth_RR;
	uint16_t add_RR;
}udp_header;

typedef struct{
	uint16_t type;
	uint16_t class;
}infos;

// Functions ------------------------------------------------------------
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) {
    int lock = 0 , i;
	char * host_aux = (char *)(malloc(sizeof(host)+1));
	strcpy(host_aux,host);
    strcat((char*)host_aux,".");
     
    for(i = 0 ; i < strlen((char*)host_aux) ; i++) {
        if(host_aux[i]=='.') {
            *dns++ = i-lock;
            for(;lock<i;lock++) {
                *dns++=host_aux[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}

// Main ---------------------------------------------------------------------------
int main(int argc, char **argv){
	// Variables ------------------------------------------------------------------
	unsigned char* message = argv[1];					// Package's message
	unsigned char* server_address = argv[2];			// DNS server address
	int socket_fd; 										// Socket File Description
	struct sockaddr_in target;							// Socket Target Information
	char buffer[MAX];									// Buffer to Hold package
	struct timeval timeout={6,0}; 						//Valor do timeout em {segundos, microssegundos}
	
	memset(buffer, 0, MAX);
	// Pointers on the Buffer -------------------------------------------------------
	udp_header *datagram = (udp_header *) buffer;

	// Sets udp_header Values ---------------------------------------------------------
	datagram->query_id = htons(rand());
	datagram->flags = htons(0x0100);
	datagram->question_count = htons(1);
	datagram->ans_RR = htons(0);
	datagram->auth_RR = htons(0);
	datagram->add_RR = htons(0);
	
	// Sets Data Information Values --------------------------------------------------
	char * data = (buffer + sizeof(udp_header));					// Pointer to the beggining of payload
	ChangetoDnsNameFormat(data, message);							// Formats message to UDP pattern 3ww6google3com
	unsigned int len = strlen(data)+1;								// Length of message
	infos * end = (infos *)(buffer+ sizeof(udp_header) + len);
	end->type = htons(1);
	end->class = htons(1);

	// Creates Socket  ------------------------------------------------------------
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
		printf("Error trying to create socket\n");
		return 0;
	}

	// Configures Socket ----------------------------------------------------------
	target.sin_family = AF_INET;
	target.sin_port = htons(PORT);
	target.sin_addr.s_addr = inet_addr(server_address);
	// setsockopt(socket_fd, SOL_SOCKET ,SO_RCVTIMEO, NULL, 0 );		// Changes Socket options (don't know if it is necessary)
    setsockopt(socket_fd, SOL_SOCKET ,SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval) );

	// Checks Buffer Length --------------------------------------------------------
	const int packege_length = sizeof(udp_header) + len + sizeof(infos);

	// Sends Package and receives Response --------------------------------------------------------
	int n,s;													// Variables to hold sento() and recvfrom() returns
	int count = 0;
	socklen_t server_addr_len;									// Length of response
	// do{														// Timeout structure is not working
		count++;
		// printf("count: %d\n", count);
		s = sendto(socket_fd, buffer,
			packege_length, 0, (struct sockaddr *) &target,
			sizeof(target));
		printf("sendto(): %d, %s\n",s, strerror(errno));
		n = recvfrom(socket_fd, buffer,
			packege_length, 0, (struct sockaddr *) &target,
			&server_addr_len);
		printf("recvfrom(): %d, %s\n",n, strerror(errno));		
		sleep(2);												// Waits 2 seconds
	// }while(n < 0 || count < 3);


	close(socket_fd);
	// printf("%d %s\n",s, strerror(errno));
	return 0;
}
