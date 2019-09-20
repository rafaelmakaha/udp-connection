/* DNS Client Project
Rafael Makaha Gomes Ferreira

Alot of work here. Most of the project is commented
for future studies and reviews.
*/
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
#define PORT 53										// DNS port
#define MAX 65536									// Max value to buffer
#define NTP_TIMESTAMP_DELTA 2208988800ull			// Value to time struct	

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
	char * host_aux = (char *)(malloc(sizeof(host)+1));		// Aux variable to avoid seg fault
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
	// struct sockaddr_in response;						// Struct to capture the response from socket
	unsigned char buffer[MAX];									// Buffer to Hold package
	struct timeval timeout={6,0}; 						// Timeout value in seconds {seconds, microseconds}
	// Response_data *response;
	
	memset(buffer, 0, MAX);

	// Pointers on the Buffer -------------------------------------------------------
	udp_header *datagram = (udp_header *) buffer;

	// Sets udp_header Values ---------------------------------------------------------
	datagram->query_id = htons(rand());					// Random query ID
	datagram->flags = htons(0x0100);					// Sets FLAGS
	datagram->question_count = htons(1);				// Number of queries
	datagram->ans_RR = htons(0);
	datagram->auth_RR = htons(0);
	datagram->add_RR = htons(0);
	
	// Sets Data Information Values --------------------------------------------------
	char * data = (buffer + sizeof(udp_header));					// Pointer to the beggining of payload
	ChangetoDnsNameFormat(data, message);							// Formats message to UDP pattern 3ww6google3com
	unsigned int len = strlen(data)+1;								// Length of message
	infos * end = (infos *)(buffer+ sizeof(udp_header) + len);		// Infos points to the end of the message
	end->type = htons(1);											// Type A
	end->class = htons(1);											// Class IN

	// Creates Socket  ------------------------------------------------------------
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0){
		printf("Error trying to create socket\n");
		return 0;
	}

	// Configures Socket ----------------------------------------------------------
	target.sin_family = AF_INET;																	// Sets IPv4 conection
	target.sin_port = htons(PORT);																	// Sets PORT
	target.sin_addr.s_addr = inet_addr(server_address);												// Sets destination address
    setsockopt(socket_fd, SOL_SOCKET ,SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval) );		// Sets Timeout

	// Checks Buffer Length --------------------------------------------------------
	const int packege_length = sizeof(udp_header) + len + sizeof(infos);

	// Sends Package and receives Response --------------------------------------------------------
	int n,s;													// Variables to hold sento() and recvfrom() returns
	int i;
	int server_addr_len;										// Length of response
	for(i =0 ; i < 3 ; i++){
		s = sendto(socket_fd, buffer,
			packege_length, 0, (struct sockaddr *) &target,
			sizeof(target));
		n = recvfrom(socket_fd, (char*) buffer,
			MAX, 0, (struct sockaddr *) &target,
			&server_addr_len);	
		sleep(2);												// Waits 2 seconds
		if(n > 0) break;
	}
	if (i == 3){
		printf("Não foi possível resovle %s\n", message);
		return 0;
	}else if(buffer[3]%4){										// Verify error on response
		printf("Host %s não encontrado\n", message);
	}else{	
		printf("%s <> %d.%d.%d.%d\n", message, buffer[n-4],buffer[n-3],buffer[n-2],buffer[n-1]);
	}
	close(socket_fd);
	return 0;
}
