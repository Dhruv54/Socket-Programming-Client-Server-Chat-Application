#include "SocketUtils.h"
#define IP		"127.0.0.1"
#define PORT 	2000

int main()
{
	int iClientSocketFD = CreateTCPIpv4Socket();

	if(iClientSocketFD == -1)
		printf("\nSocket Creation Error : %d\n",iClientSocketFD);
	else
		printf("\nSocket Created Successfully.File Descriptor : %d\n",iClientSocketFD);

	struct sockaddr_in address = CreateIPV4Address(IP,PORT);
	// int connect(int socket_descriptor, const struct sockaddr *address, socklen_t length_of_address);
	int iResult = connect(iClientSocketFD,(const struct sockaddr*)&address,sizeof(address));	
	if(iResult==0)
	{
		printf("\nConnection Was Successfully Established!!\n");
	}
	else
	{
		printf("\nError while Connecting!\nError Code : %d\n",iResult);
	}

	char* pcMsg = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
	send(iClientSocketFD,pcMsg,strlen(pcMsg),0);

	char buffer[1024];
	recv(iClientSocketFD,buffer,1024,0);

	printf("\nResponse was : \n%s\n",buffer);
	return 0;
}
