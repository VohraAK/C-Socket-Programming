#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // only one socket is required
    int socketFD;
    char message[BUFFER_SIZE];
    int bytesReceived;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressSize;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // socket creation
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFD == -1)
    {
        printf("Error: failed to create socket...\n");
        exit(1);
    }

    // zero out addresses
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // populate serverAddress
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));

    // bind socket
    if (bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Error: failed to bind...\n");
        exit(1);
    }

    clientAddressSize = sizeof(clientAddress);

    // no need to start listening
    while (1)
    {
        int bytesReceived = recvfrom(socketFD, message, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressSize);
        
        message[bytesReceived] = 0;
        printf("Echoing message: %s", message);

        // echo...
        sendto(socketFD, message, bytesReceived, 0, (struct sockaddr *)&clientAddress, clientAddressSize);
    }

    close(socketFD);

    return 0;
}