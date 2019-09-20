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

	datagram = calloc(1, sizeof(udp_header));

	// Sets udp_header Values ---------------------------------------------------------
	datagram->query_id = htons(rand());
	datagram->flags = 0x0100;
	datagram->question_count = htons(1);
	datagram->ans_RR = 0x0000;
	datagram->auth_RR = 0x0000;
	datagram->add_RR = 0x0000;
	datagram->query = htons(1);

	// Sets Data Information Values --------------------------------------------------
	char * data = (buffer + sizeof(datagram));				// Pointer do the beggining of payload
	strcpy(data, message);									// Copy message to payload
	int len = strlen(data)+1;								// Length of message
	infos * end = (infos *) (data+len);						//
	end->type = htons(1);
	end->class = htons(1);

	// Creates Socket  ------------------------------------------------------------
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("Error trying to create socket\n");
		return 0;
	}

	target.sin_family = AF_INET;
	target.sin_port = htons(PORT);
	target.sin_addr.s_addr = inet_addr(server_address);

	// Checks Buffer Length -----------------------------------------------
	const int packege_length = sizeof(datagram) + sizeof(end);

	// Sends Package -----------------------------------------------------
	int x = sendto(socket_fd, buffer,
		packege_length, 0, (const struct sockaddr *) &target,
		sizeof(target));

	close(socket_fd);
	printf("%d %s\n",x, strerror(errno));
	return 0;
}
