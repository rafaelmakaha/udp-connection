// Includes ---------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

// Defines -----------------------------------------------------------------------
#define PORT 53
#define MAX 8192

// Structs -----------------------------------------------------------------------
typedef struct{
	unsigned short int query_id;
	uint16_t flags;
	uint16_t question_count;
	uint16_t ans_RR;
	uint16_t auth_RR;
	uint16_t add_RR;
	uint16_t query;
}udp_header;

typedef struct{
	uint16_t type;
	uint16_t class;
}infos;

// Main ---------------------------------------------------------------------------
int main(int argc, char **argv){
	// Variables ------------------------------------------------------------------
	char* message = argv[1];							// Package's message
	char* server_address = argv[2];						// DNS server address
	int socket_fd; 										// Socket File Description
	struct sockaddr_in target;							// Socket Target Information
	char buffer[MAX];									// Buffer to Hold package
	
	memset(buffer, 0, MAX);
	// Pointers on the Buffer -------------------------------------------------------
	udp_header *datagram = (udp_header *) buffer;

	// Sets udp_header Values ---------------------------------------------------------
	datagram->query_id = htons(rand());
	datagram->flags = htons(0x0100);
	datagram->question_count = htons(0x0001);
	datagram->ans_RR = htons(0);
	datagram->auth_RR = htons(0);
	datagram->add_RR = htons(0);
	datagram->query = htons(0x0001);

	// Transforms Message Format (unused: input from terminal already on the format for testing) ------------------
	// char * message_formatted = (char *) (malloc(sizeof(strlen(message) + 1)));
	// int count = 0;
	// for(int i=0; i < strlen(message); i++){
	// 	if(message[i] == '.'){
	// 		message_formatted[i-count] = count;
	// 	}
	// 	message_formatted[i+1] = message[i];
	// 	count++;
	// 	printf("%d %s\n", count, message_formatted);
	// }

	// Sets Data Information Values --------------------------------------------------
	char * data = (buffer + sizeof(udp_header));					// Pointer do the beggining of payload
	strcpy(data, message);											// Copy message to payload
	unsigned int len = strlen(data)+1;								// Length of message
	infos * end = (infos *)(buffer + sizeof(udp_header) + len);
	end->type = htons(0x0001);
	end->class = htons(0x0001);

	// Creates Socket  ------------------------------------------------------------
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("Error trying to create socket\n");
		return 0;
	}

	// Configures Socket ----------------------------------------------------------
	target.sin_family = AF_INET;
	target.sin_port = htons(PORT);
	target.sin_addr.s_addr = inet_addr(server_address);
	// setsockopt(socket_fd, SOL_SOCKET ,SO_RCVTIMEO, NULL, 0 );		// Changes Socket options (don't know if it is necessary)
	
	// Checks Buffer Length --------------------------------------------------------
	const int packege_length = sizeof(udp_header) + len + sizeof(infos);

	// Sends Package and receives Response --------------------------------------------------------
	int n,s;								// Variables to hold sento() and recvfrom() returns
	int count = 0;
	socklen_t server_addr_len;					// Length of response
	do{
		count++;
		s = sendto(socket_fd, buffer,
			packege_length, 0, (struct sockaddr *) &target,
			sizeof(target));
		printf("sendto(): %d, %s\n",s, strerror(errno));
		n = recvfrom(socket_fd, buffer,
			packege_length, 0, (struct sockaddr *) &target,
			&server_addr_len);
		printf("recvfrom(): %d, %s\n",n, strerror(errno));		
		sleep(2000);						// Waits 2 seconds to check response (not working: it waits for ever)
	}while(n < 0 || count < 3);


	close(socket_fd);
	// printf("%d %s\n",s, strerror(errno));
	return 0;
}
