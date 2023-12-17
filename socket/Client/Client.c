#include "../SocketUtils.h"
#define IP		"127.0.0.1"
#define PORT 	2000

int main()
{
	int iClientSocketFD = CreateTCPIpv4Socket();

	if(iClientSocketFD == -1)
	{
		printf("\nSocket Creation Error : %d\n",iClientSocketFD);
		return -1;
	}
	else
	{
		printf("\nSocket Created Successfully.File Descriptor : %d\n",iClientSocketFD);
	}

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
		return -1;
	}

	char* line = NULL;
	size_t lineSize = 0;
	while(1)
	{
		printf("\nPlease Enter Message :\n");
		// ssize_t getline(char **restrict lineptr, size_t *restrict n,FILE *restrict stream);
		ssize_t charCount = getline(&line,&lineSize,stdin);
		printf("\ncharCount : %ld\n",charCount);
		if(charCount > 0)
		{
			if(strcmp(line,"exit\n") == 0)
			{
				break;
			}
			else
			{
				// ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);
				printf("\nELSE\n");
				ssize_t amoutWasSent = send(iClientSocketFD,line,charCount,0);
				printf("\namoutWasSent : %ld\n",amoutWasSent);
				if(amoutWasSent>0)
					printf("\nData Send Successfully. Size : %ld\n",amoutWasSent);
				else
					printf("\nData Not Sended\n");
			}
		}
	};
	// int close (int __fd);
	int iCloseResult = close(iClientSocketFD);
	printf("\niCloseResult : %d\n",iCloseResult);
	return 0;
}
//  lsof -i :2000