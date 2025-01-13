#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 256
#define QUEUE 3

int main(int argc, char *argv[])
{
    int socketFD;
    long bytesReceived = 0;
    long fileSize = 0;

    struct sockaddr_in serverAddress;

    char buffer[BUFFER_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));

    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Error: could not connect to server...\n");
        exit(1);
    }

    else
    {
        printf("Connected to %s:%s\n", argv[1], argv[2]);
    }

    // create destination file
    FILE *file = fopen("received.txt", "w");

    // get filesize
    read(socketFD, &fileSize, sizeof(fileSize));

    printf("Incoming filesize: %ld\n\n", fileSize);

    long totalReceived = 0;

    while (totalReceived < fileSize)
    {
        bytesReceived = read(socketFD, (void *)buffer, BUFFER_SIZE);

        // write bytes to file
        fwrite(buffer, 1, bytesReceived, file);

        totalReceived += bytesReceived;
    }

    if (totalReceived == fileSize)
    {
        write(socketFD, "File received successfully!\n", 29);
    }
    else
    {
        write(socketFD, "File transfer incomplete!\n", 27);
    }

    fclose(file);
    close(socketFD);

    return 0;
}