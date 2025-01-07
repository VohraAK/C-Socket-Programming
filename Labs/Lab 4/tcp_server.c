#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // socket file descriptors
    int serverSocketFD, clientSocketFD;

    // addresses
    struct sockaddr_in serverAddress, clientAddress;

    // size of addresses
    socklen_t serverAddressSize, clientAddressSize;

    char* message = "Hello Darkness my old friend,\nI've come to talk to you again...\0";

    // socket creation (IPv4 TCP socket)
    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    // socket validation
    if (serverSocketFD == -1)
    {
        printf("Error: Socket could not be created...\n");
        exit(1);
    }

    else
        printf("Socket created: %d\n", serverSocketFD);


    // zero out server address block
    memset(&serverAddress, 0, sizeof(serverAddress));
    
    // zero out client address block
    memset(&clientAddress, 0, sizeof(clientAddress));

    // populate serverAddress
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // set to listen for incoming connections on all network interfaces

    // bind serverAddress to server socket
    if (bind(serverSocketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != 0)
    {
        printf("Error: failed to bind socket...\n");
        exit(1);
    }

    else
        printf("Socket bound successfully...\n");

    // listen for incoming connections (1 in queue)
    if (listen(serverSocketFD, 1) != 0)
    {
        printf("Error: failed to listen...\n");
        exit(1);
    }

    else
        printf("Now listening...\n");


    // accept incoming connection request
    clientSocketFD = accept(serverSocketFD, (struct sockaddr *) &clientAddress, &clientAddressSize);

    if (clientSocketFD == -1)
    {
        printf("Error: failed to accept incoming request...\n");
        exit(1);
    }

    else
        printf("Connection established successfully!\n");

    
    // write message to client
    int bytesWritten = write(clientSocketFD, message, strlen(message));
    printf("\nMessage: %s || %d bytes sent successfully!\n", message, bytesWritten);

    // close sockets
    close(clientSocketFD);
    close(serverSocketFD);

    printf("\nSockets closed! Farewell...\n");

    return 0;
}