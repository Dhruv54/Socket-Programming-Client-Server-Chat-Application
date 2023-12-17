#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <stdbool.h>

int CreateTCPIpv4Socket()
{
	// int socket(int domain, int type, int protocol);
	return socket(AF_INET,SOCK_STREAM,0);	// Create TCP IPV4 Socket
}

struct sockaddr_in CreateIPV4Address(char* pcIPAddr,int port)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (strlen(pcIPAddr) == 0)
	{
		address.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		inet_pton(AF_INET,pcIPAddr,&address.sin_addr.s_addr);
	}
	return address;
}