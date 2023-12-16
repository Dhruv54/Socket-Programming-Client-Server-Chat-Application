#include "SocketUtils.h"

#define IP		""
#define PORT 	2000
int main()
{
	int iServerSocketFD = CreateTCPIpv4Socket();

	if(iServerSocketFD == -1)
		printf("\nSocket Creation Error : %d\n",iServerSocketFD);
	else
		printf("\nSocket Created Successfully.File Descriptor : %d\n",iServerSocketFD);

	struct sockaddr_in address = CreateIPV4Address(IP,PORT);

    // int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
    int iBindResult = bind(iServerSocketFD,(struct sockaddr*)&address,sizeof(address));

	if(iBindResult==0)
	{
		printf("\nSocket Was Bound Successfully!!!\n");
	}
	else
	{
		printf("\nError while Binding!\nError Code : %d\n",iBindResult);
	}

    // int listen (int __fd, int __n)
    int iListenerResult = listen(iServerSocketFD,10);
    if(iListenerResult==0)
	{
		printf("\nListening......\n");
	}
	else
	{
		printf("\nError while Listening!\nError Code : %d\n",iListenerResult);
	}

    struct sockaddr_in ClientAddress;
    int ClientAddressSize = sizeof(struct sockaddr_in);
    // int accept (int __fd, __SOCKADDR_ARG __addr,socklen_t *__restrict __addr_len);
    int iClientSocketFD = accept(iServerSocketFD,(struct sockaddr*)&ClientAddress,&ClientAddressSize);
    if(iClientSocketFD == -1)
    {
        printf("\nError while Accepting!\nError Code : %d\n",iClientSocketFD);
    }
    else
    {
        printf("\nAccept Done. Client Socket FD : %d\n",iClientSocketFD);
    }

    char buffer[1024];
    recv(iClientSocketFD,buffer,1024,0);
    
    printf("response was :\n%s\n",buffer);

    return 0;
}
