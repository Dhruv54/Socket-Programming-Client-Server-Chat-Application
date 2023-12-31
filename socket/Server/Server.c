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

struct AcceptSocket acceptSockets[10];
int iAcceptSocketCount = 0;

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

void sendMessagetoOtherClients(char buffer[1024],int iSocketFD)
{
    for (int i = 0; i < iAcceptSocketCount; i++)
    {
        if(acceptSockets[i].iAcceptSocketFD != iSocketFD)
            send(acceptSockets[i].iAcceptSocketFD,buffer,strlen(buffer),0);
    }
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
            sendMessagetoOtherClients(buffer,iSocketFD);
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

void receiveAndPrintDataFromSeperateThread(int iClientSocketFD)
{
    pthread_t pId;
    pthread_create(&pId,NULL,(void *)ReceiveAndPrintIncomingData,iClientSocketFD);

/*
extern int pthread_create (pthread_t *__restrict __newthread,
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg)
*/
}

void startAcceptingIncomingConnections(int iServerSocketFD)
{
    while (true)
    {
        struct AcceptSocket ClientSocket = AcceptIncomingConnection(iServerSocketFD);
        acceptSockets[iAcceptSocketCount++]=ClientSocket;
        receiveAndPrintDataFromSeperateThread(ClientSocket.iAcceptSocketFD);
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

    //struct AcceptSocket ClientSocket = AcceptIncomingConnection(iServerSocketFD);

    startAcceptingIncomingConnections(iServerSocketFD);

    // int shutdown (int __fd, int __how);
    int iShutDownResult = shutdown(iServerSocketFD,SHUT_RDWR);
    if(iShutDownResult == -1)
        printf("\nError while shutting down Socket!\nError Code : %d\n",iShutDownResult);


   // printf("\niCloseResult : %d\niShutDownResult : %d\n",iCloseResult,iShutDownResult);
    return 0;
}

//  lsof -i :2000