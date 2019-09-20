// Includes ---------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

// Defines -----------------------------------------------------------------------
#define PORT 53

// Structs -----------------------------------------------------------------------
typedef struct{
	uint16_t query_id;
	uint16_t flags;
	uint16_t question_count;
	uint16_t ans_RR;
	uint16_t auth_RR;
	uint16_t add_RR;
	uint16_t query;
	uint16_t type;
	uint16_t class;
}payload;

// Main ---------------------------------------------------------------------------
int main(int argc, char **argv){
	// Variables ------------------------------------------------------------------
	char* message = argv[1];							// Package's message
	const int len = strlen(message);							// Length of message
	char* server_address = argv[2];			// DNS server address
	int socket_fd; 										// Socket File Description
	struct sockaddr_in target;							
	payload datagram;
	srand(time(NULL));

	// Creates Payload ------------------------------------------------------------
	datagram.query_id = rand();
	datagram.

	// Creates Socket  ------------------------------------------------------------
	if(socket_fd = socket(AF_INET, SOCK_DGRAM, 0)<0){
		printf("Error trying to create socket\n");
		return 0;
	}

	target.sin_family = AF_INET;
	target.sin_port = htons(PORT);
	target.sin_addr.s_addr = inet_addr(server_address);

	sendto(socket_fd, (const payload *) datagram,
		sizeof(payload), 0, (const struct sockaddr *) &target,
		sizeof(target));


	return 0;
}
