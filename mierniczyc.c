#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

void failwith(const char * msg){
	fprintf(stderr, "FAILED WITH: ");
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}
#define BUFFER_SIZE 512
int main(int argc, const char *argv[])
{
	if (argc != 4) {
		failwith("wrong arguments");
	}
	char buffer[BUFFER_SIZE];
	//Obtain UDP port TCP port server addres

	short udp_port = atoi(argv[1]);
	short tcp_port = atoi(argv[2]);
	const char *tcp_port_char = argv[2];
	const char *host = argv[3];

	//Open UDP Socket

	int udp_socket; //socket FD

	struct sockaddr_in udp_sockaddr;

	socklen_t udp_sockaddr_len;

	udp_sockaddr.sin_family = AF_INET;
	udp_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_sockaddr.sin_port = htons(udp_port);

	udp_sockaddr_len = (socklen_t) sizeof(udp_sockaddr);

	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (udp_socket < 0)
		failwith("socket");

	if (bind(
		udp_socket, 
		(struct sockaddr *)&udp_sockaddr, 
		udp_sockaddr_len
		) < 0) 
	{
		failwith("BIND");
	}

	//Opening UDP done

	//Connect to server by TCP

	struct addrinfo addr_tcp_hints;
	struct addrinfo *addr_tcp;

	int tcp_socket;

	memset(&addr_tcp_hints, 0, sizeof(struct addrinfo));

	addr_tcp_hints.ai_family	= AF_INET;
	addr_tcp_hints.ai_socktype	= SOCK_STREAM;
	addr_tcp_hints.ai_protocol	= IPPROTO_TCP;

	if (getaddrinfo(host, tcp_port_char, &addr_tcp_hints, &addr_tcp) != 0)
		failwith("getaddrinfo");

	tcp_socket = socket(addr_tcp->ai_family, addr_tcp->ai_socktype, addr_tcp->ai_protocol);

	if (tcp_socket < 0)
		failwith("socket");

	if (connect(tcp_socket, addr_tcp->ai_addr, addr_tcp->ai_addrlen) < 0)
		failwith("connect");

	//Connected to TCP server

	short conv_udp_port = htons(udp_port);
	if (write(tcp_socket, &conv_udp_port, sizeof(conv_udp_port)) < 0)
		failwith("write");

	//UDP Port number send

	//Sending data
	while (fgets(buffer, BUFFER_SIZE, stdin)) {
		if (write(tcp_socket, buffer, strlen(buffer)) < 0) {
			failwith("write");
		}
	}

	close(tcp_socket);

	//Receive UDP from serwer
	struct sockaddr_in udp_client;
	int rcvlen;
	int udp_client_len = (ssize_t)sizeof(udp_client);
	do {
		rcvlen = recvfrom(udp_socket, buffer, sizeof(buffer),
			0, (struct sockaddr *)&udp_client, &udp_client_len);
		if (rcvlen < 0) {
			failwith("recvfrom udp");
		}
		int data = ntohl(atoi(buffer));
		printf("Number of octets: %u\n", data);

	} while(udp_client.sin_addr.s_addr != ((struct sockaddr_in *)addr_tcp->ai_addr)->sin_addr.s_addr);

	close(udp_socket);
	freeaddrinfo(addr_tcp);
	return 0;
}
