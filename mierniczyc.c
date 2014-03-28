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

int main(int argc, const char *argv[])
{
	char buffer[100];
	//Obtain UDP port TCP port server addres
	//TODO

	short udp_port;
	short tcp_port;
	char tcp_port_char[10];
	

	char host[80];

	printf("UDP Port: ");
	scanf("%d", &udp_port);

	printf("TCP Port: ");
	scanf("%s", &tcp_port_char);
	tcp_port = atoi(tcp_port_char);
	

	printf("Server: ");
	scanf("%s", &host);

	//Open UDP Socket

	int udp_socket;

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

	freeaddrinfo(addr_tcp);
	
	printf("Sending %d %d\n", udp_port, htons(udp_port));
	sprintf(buffer, "%d", htons(udp_port));
	if (write(tcp_socket, buffer, strlen(buffer)) < 0)
		failwith("write");

	//Connected
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
	const char * host	= argv[3];

	int udp_socket;

	struct addrinfo addr_hints;
	struct addrinfo * addr;
	
	struct sockaddr_in client_udp_address;
	struct sockaddr_in server_address;

	socklen_t client_udp_address_len;

	(void) memset(&addr_hints, 0, sizeof(struct addrinfo));

	addr_hints.ai_family	= AF_INET;
	addr_hints.ai_socktype	= SOCK_DGRAM;
	addr_hints.ai_protocol	= IPPROTO_UDP;
	addr_hints.ai_flags	= 0;
	addr_hints.ai_addrlen	= 0;
	addr_hints.ai_addr	= NULL;
	addr_hints.ai_canonname	= NULL;
	addr_hints.ai_next	= NULL;
	
	if (getaddrinfo(host, NULL, &addr_hints, &addr) != 0) {
		failwith("getaddrinfo");
	}

	client_udp_address.sin_family = AF_INET;
	client_udp_address.sin_addr.s_addr =
		((struct sockaddr_in*) (addr->ai_addr))->sin_addr.s_addr;
	client_udp_address.sin_port = htons(udp_port);
	
	client_udp_address_len = (socklen_t) sizeof(client_udp_address);

	//Open UDP socket
	//TODO

	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (bind(udp_socket, client_udp_address, client_udp_address_len) != 0) {
		failwith("BIND");
	}
	//Connect to server
	//TODO

	//Senddata from stdin
	//TODO

	//Receive UDP from serwer
	//TODO

	//Write on stdout received number
	//TODO
	freeaddrinfo(addr);*/
	return 0;
}
