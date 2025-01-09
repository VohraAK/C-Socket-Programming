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
    struct sockaddr_in serverAddress, recvAddress;
    socklen_t addressSize;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    socketFD = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketFD == -1)
    {
        printf("Error: failed to create socket...\n");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&recvAddress, 0, sizeof(recvAddress));

    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));

    
    addressSize = sizeof(recvAddress);

    while(1)
    {
        fputs("Enter message (Q/q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);

        if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break;

        sendto(socketFD, message, strlen(message), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

        bytesReceived = recvfrom(socketFD, message, BUFFER_SIZE, 0, (struct sockaddr *)&recvAddress, &addressSize);

        message[bytesReceived] = 0;
        printf("Message from server: %s\n", message);
    }

    close(socketFD);
    return 0;
}