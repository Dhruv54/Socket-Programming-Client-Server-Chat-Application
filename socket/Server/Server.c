#include "../SocketUtils.h"

#define IP		""
#define PORT 	2000

struct AcceptSocket
{
    int iAcceptSocketFD;
    struct sockaddr_in address;
    int iError;
    bool bAcceptedSuccessfully;
};

struct AcceptSocket AcceptIncomingConnection(int iServerSocketFD)
{
    struct sockaddr_in ClientAddress;
    int ClientAddressSize = sizeof(struct sockaddr_in);
    // int accept (int __fd, __SOCKADDR_ARG __addr,socklen_t *__restrict __addr_len);
    int iClientSocketFD = accept(iServerSocketFD,(struct sockaddr*)&ClientAddress,&ClientAddressSize);

    struct AcceptSocket acceptedSocket;
    acceptedSocket.address=ClientAddress;
    acceptedSocket.iAcceptSocketFD = iClientSocketFD;
    acceptedSocket.bAcceptedSuccessfully = iClientSocketFD>0;

    if(!acceptedSocket.bAcceptedSuccessfully)
        acceptedSocket.iError = iClientSocketFD;
    else
        acceptedSocket.iError = 0;

    return acceptedSocket;
} 

void ReceiveAndPrintIncomingData(int iSocketFD)
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
}

int main()
{
	int iServerSocketFD = CreateTCPIpv4Socket();

	if(iServerSocketFD == -1)
    {
		printf("\nSocket Creation Error : %d\n",iServerSocketFD);
        return -1;
    }
	else
    {
		printf("\nSocket Created Successfully.\nFile Descriptor : %d\n",iServerSocketFD);
    }

	struct sockaddr_in address = CreateIPV4Address(IP,PORT);

    // int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
    int iBindResult = bind(iServerSocketFD,(struct sockaddr*)&address,sizeof(address));

	if(iBindResult == 0)
	{
		printf("\nSocket Was Bound Successfully!!!\n");
	}
	else
	{
		printf("\nError while Binding!\nError Code : %d\n",iBindResult);
	}

    // int listen (int __fd, int __n)
    int iListenerResult = listen(iServerSocketFD,10);   //max 10 request allowed
    if(iListenerResult==0)
	{
		printf("\nListening......\n");
	}
	else
	{
		printf("\nError while Listening!\nError Code : %d\n",iListenerResult);
	}

   // struct sockaddr_in ClientAddress;
   // int ClientAddressSize = sizeof(struct sockaddr_in);
    // int accept (int __fd, __SOCKADDR_ARG __addr,socklen_t *__restrict __addr_len);
   // int iClientSocketFD = accept(iServerSocketFD,(struct sockaddr*)&ClientAddress,&ClientAddressSize);

    struct AcceptSocket ClientSocket = AcceptIncomingConnection(iServerSocketFD);

    if(ClientSocket.iAcceptSocketFD == -1)
    {
        printf("\nError while Accepting!\nError Code : %d\n",ClientSocket.iAcceptSocketFD);
        return -1;
    }
    else
    {
        printf("\nAccept Done.\nClient Socket FD : %d\n",ClientSocket.iAcceptSocketFD);
    }

    ReceiveAndPrintIncomingData(ClientSocket.iAcceptSocketFD);

    // int close (int __fd);
    int iCloseResult = close(ClientSocket.iAcceptSocketFD);

    // int shutdown (int __fd, int __how);
    int iShutDownResult = shutdown(iServerSocketFD,SHUT_RDWR);
    if(iShutDownResult == -1)
        printf("\nError while shutting down Socket!\nError Code : %d\n",iShutDownResult);

    printf("\niCloseResult : %d\niShutDownResult : %d\n",iCloseResult,iShutDownResult);
    return 0;
}
//  lsof -i :2000