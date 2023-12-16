# Socket-Programming-Client-Server-Chat-Application


# Stages for Server
## A. Socket Creation
The first stage deals with the creation of a socket, which is the basic component for sending or receiving signals between nodes. The **sys/socket.h** header has the necessary functions to create a socket in C. A socket can be created by the **socket()** function with syntax,

**int socket(int domain, int type, int protocol);**

The domain represents the address family over which the communication will be performed. The domain is pre-fixed values present in the sys/socket.h header. Some domains are,
**AF_LOCAL** or **AF_UNIX** is used for local communication or in the case where the client and server are on the same node. These sockets are called UNIX domain sockets.

**AF_INET** is used to represent the IPv4 address of the client to which a connection should be made. Similarly **AF_INET6** is used for IPv6 addresses. These sockets are called internet domain sockets.

**AF_BLUETOOTH** is used for low-level Bluetooth connection.

The type represents the type of communication used in the socket. Some mostly used types of communication are,
**SOCK_STREAM** uses the TCP(Transmission Control Protocol) to establish a connection. This type provides a reliable byte stream of data flow and is a connection-based protocol. These sockets are called stream sockets.

**SOCK_DGRAM** uses the UDP(User Datagram Protocol) which is unreliable and a connectionless protocol. These sockets are also called datagram sockets.
The protocol represents the protocol used in the socket. This is represented by a number. When there is only one protocol in the protocol family, the protocol number will be 0, or else the specific number for the protocol has to be specified.

The socket() function creates a socket and returns a file descriptor which represents an open file that will be utilized by the socket in reading and writing operations and the file descriptor is used to represent the socket in later stages. In case of an error in creating the socket, **-1** is returned by the socket() function.
