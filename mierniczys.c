#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define BUFFER_SIZE	1024
#define QUEUE_LENGTH	5
void failwith(const char * msg){
	fprintf(stderr, "FAILED WITH: ");
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		failwith("no port number");
	}

	short tcp_port = atoi(argv[1]);

	int tcp_socket;

	struct sockaddr_in tcp_addr;

	socklen_t tcp_addr_len;

	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	tcp_addr.sin_port = htons(tcp_port);

	tcp_addr_len = (socklen_t) sizeof(tcp_addr);

	tcp_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (tcp_socket < 0)
		failwith("socket");

	if (bind(
		tcp_socket, 
		(struct sockaddr *)&tcp_addr, 
		tcp_addr_len
		) < 0) 
	{
		failwith("BIND");
	}

	//Listen for connection TCP

	if (listen(tcp_socket, QUEUE_LENGTH) < 0)
		failwith("listen");

	//Get port number UDP

	int client_sock;
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	client_sock = accept(
			tcp_socket, 
			(struct sockaddr *) &client_address, 
			&client_address_len);

	if (client_sock < 0)
		failwith("accept");

	short udp_port;

	int len;
	char buffer[BUFFER_SIZE];

	len = read(client_sock, &udp_port, sizeof(udp_port));
	if (len < 0)
		failwith("read");

	udp_port = ntohs(udp_port);
	
	memset(buffer, 0, BUFFER_SIZE);

	long datasize = 0;

	while(len > 0) {
		len = read(client_sock, buffer, BUFFER_SIZE);
		if (len < 0)
			failwith("read");
		datasize += len;
	}
	
	printf("Read: %d\n", datasize);

	close(client_sock);
	close(tcp_socket);

	struct sockaddr_in udp_addr;

	udp_addr.sin_family	= AF_INET;
	udp_addr.sin_port	= htons(udp_port);
	udp_addr.sin_addr.s_addr= client_address.sin_addr.s_addr;

	int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_socket < 0) {
		failwith("Socket");
	}
	
	sprintf(buffer, "%d", htonl(datasize));
	int buflen = strlen(buffer);
	int sndlen = sendto(udp_socket, buffer, buflen,
		0, (struct sockaddr *)&udp_addr, sizeof(udp_addr));
	
	if (sndlen != buflen){
		failwith("Write");
	}
	return 0;
}
