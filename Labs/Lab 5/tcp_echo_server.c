#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define QUEUE 3

int main(int argc, char *argv[])
{
    int serverSocketFD, clientSocketFD;
    int bytesReceived;

    struct sockaddr_in clientAddress, serverAddress;
    socklen_t clientAddressSize;

    char message[BUFFER_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // socket creation
    serverSocketFD = socket(PF_INET, SOCK_STREAM, 0);

    if (serverSocketFD == -1)
    {
        printf("Error: failed to create server socket...\n");
        exit(1);
    }     

    // zero out address blocks (important)
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // populate serverAddress
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(atoi(argv[1]));

    // bind server socket
    if (bind(serverSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Error: failed to bind...\n");
        exit(1);
    }

    // start listening
    if (listen(serverSocketFD, QUEUE) == -1)
    {
        printf("Error: failed to listen...\n");
        exit(1);
    }

    clientAddressSize = sizeof(clientAddress);

    // iterative approach
    for (int i = 0; i < QUEUE; i++)
    {
        // accept incoming connection request
        clientSocketFD = accept(serverSocketFD, (struct sockaddr *) &clientAddress, &clientAddressSize);
        if (clientSocketFD == -1)
        {
            // failed to accept
            perror("accept");
            printf("Error: failed to accept incoming connection request...\n");
            exit(1);
        }

        else
        {
            printf("\nConnected to client #%d\n", i + 1);
        }


        // read data
        while ((bytesReceived = read(clientSocketFD, message, BUFFER_SIZE)) != 0)
        {
            // echo to client (exactly the number of bytes received)
            write(clientSocketFD, message, bytesReceived);
        }

        // client closes connection...
        close(clientSocketFD);
    }

    // queue is empty, close server
    close(serverSocketFD);

    return 0;
}