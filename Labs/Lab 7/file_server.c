#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define QUEUE 3

int main(int argc, char *argv[])
{
    int serverSocketFD, clientSocketFD;
    long bytesReceived = 0;
    long bytesRead = 0, bytesWritten = 0;
    int chunkCount = 0;

    struct sockaddr_in clientAddress, serverAddress;
    socklen_t clientAddressSize;

    char buffer[BUFFER_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Socket creation
    serverSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocketFD == -1)
    {
        perror("Error: failed to create server socket");
        exit(1);
    }

    memset(&clientAddress, 0, sizeof(clientAddress));
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        perror("Error: failed to bind");
        exit(1);
    }

    if (listen(serverSocketFD, QUEUE) == -1)
    {
        perror("Error: failed to listen");
        exit(1);
    }

    // Open file
    FILE *file = fopen("speech.txt", "r");
    if (file == NULL)
    {
        perror("Error: could not open file");
        exit(1);
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    clientAddressSize = sizeof(clientAddress);

    for (int i = 0; i < QUEUE; i++)
    {
        printf("Waiting for client %d...\n", i + 1);

        clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientAddressSize);
        if (clientSocketFD == -1)
        {
            perror("Error: failed to connect");
            exit(1);
        }

        printf("Connected to client %d. Sending file size: %ld bytes\n", i + 1, fileSize);

        // Send file size
        write(clientSocketFD, &fileSize, sizeof(fileSize));

        // Reset file pointer to the beginning
        rewind(file);

        // Read and send file chunks
        while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
        {
            chunkCount++;
            long totalWritten = 0;

            while (totalWritten < bytesRead)
            {
                bytesWritten = write(clientSocketFD, buffer + totalWritten, bytesRead - totalWritten);
                if (bytesWritten < 0)
                {
                    perror("Error: failed to write to socket");
                    exit(1);
                }
                totalWritten += bytesWritten;
            }
            printf("Chunk %d sent, bytes: %ld\n", chunkCount, bytesRead);
        }

        printf("%d chunks read\n\n", chunkCount);
        chunkCount = 0;

        // Close input buffer
        shutdown(clientSocketFD, SHUT_WR);
        printf("File transfer complete. Waiting for receipt...\n");

        // Read receipt
        bytesReceived = read(clientSocketFD, buffer, BUFFER_SIZE);
        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = 0; // Null-terminate
            printf("Message from client: %s\n", buffer);
        }
        else if (bytesReceived == 0)
        {
            printf("Client closed connection without sending receipt.\n");
        }
        else
        {
            perror("Error reading receipt from client");
        }

        close(clientSocketFD);
    }

    fclose(file);
    close(serverSocketFD);

    return 0;
}
