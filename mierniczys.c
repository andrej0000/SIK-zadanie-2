#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFER_SIZE	1000
#define QUEUE_LENGTH	5
void failwith(const char * msg){
	fprintf(stderr, "FAILED WITH: ");
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{


	short tcp_port;
	printf("Type in port number: \n");
	scanf("%d", &tcp_port);

	int tcp_socket;

	struct sockaddr_in client_udp_address;
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
	//TODO

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

	len = read(client_sock, buffer, BUFFER_SIZE);
	if (len < 0)
		failwith("read");
	printf("read udp port: %s\n", buffer);
	udp_port = ntohs(atoi(buffer));
	
	printf("read udp port: %d\n", udp_port);
	//Get data
	//TODO

	//Calculate data size
	//TODO

	//Connect to client on UDP
	//TODO

	//Send data size
	//TODO

	//Close all and finish
	return 0;
}
