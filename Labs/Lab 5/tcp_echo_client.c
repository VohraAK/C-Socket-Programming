#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int socketFD;
    char message[BUFFER_SIZE];
    int bytesReceived, bytesSent, receivedCount;
    struct sockaddr_in serverAddress;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }


    // socket creation
    socketFD = socket(PF_INET, SOCK_STREAM, 0);

    if (socketFD == -1)
    {
        puts("Error: failed to create socket...");
        exit(1);
    }

    // zero out serverAddress block
    memset(&serverAddress, 0, sizeof(serverAddress));

    // populate serverAddress
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));


    // connect to server
    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        puts("Error: failed to connect to server...");
        exit(1);
    }

    else
    {
        printf("Connected to %s:%s\n", argv[1], argv[2]);
    }

    
    while (1)
    {
        fputs("Input message (Q/q to quit): ", stdout);
        fgets(message, BUFFER_SIZE, stdin);

        // check quits
        if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break;

        write(socketFD, message, strlen(message));

        bytesReceived = read(socketFD, message, BUFFER_SIZE - 1);   // reserve last byte for null-terminator
        message[bytesReceived] = 0;
    
        printf("Message from server: %s", message);
    }

    close(socketFD);
    return 0;
}