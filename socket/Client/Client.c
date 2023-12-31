#include "../SocketUtils.h"
#define IP		"127.0.0.1"
#define PORT 	2000

void ListenAndPrintIncomingData(int iSocketFD)
{
    char buffer[1024];
    while (true)
    {
        // recv (int __fd, void *__buf, size_t __n, int __flags)
        ssize_t iRecevedSize = recv(iSocketFD,buffer,1024,0);
        buffer[iRecevedSize]=0;
        if(iRecevedSize > 0)
        {
            printf("\nResponse was :\n%s\n",buffer);
            printf("\nReceived Size :%ld\n",iRecevedSize);
        }
        else if(iRecevedSize == 0)
        {
            printf("\n No Data Received!\nReceived Size : %ld\n",iRecevedSize);
            break;
        }
        else
        {
            printf("\nError while Receiving Data!\nError Code : %ld\n",iRecevedSize);
            break;
        }
    }
	// int close (int __fd);
    int iCloseResult = close(iSocketFD);
}

void StartListeningAndPrintDataFromSeperateThread(int iClientSocketFD)
{
    pthread_t pId;
    pthread_create(&pId,NULL,(void *)ListenAndPrintIncomingData,iClientSocketFD);
}

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
	}

	StartListeningAndPrintDataFromSeperateThread(iClientSocketFD);

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
				ssize_t amoutWasSent = send(iClientSocketFD,line,charCount,0);
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