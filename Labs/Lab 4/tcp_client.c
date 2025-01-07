#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
    // socket file descriptors
    int socketFD;
    struct sockaddr_in serverAddress;

    // socket creation (IPv4 TCP socket)
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // data buffer (loading area for data)
    char buffer[BUFFER_SIZE];

    // socket validation
    if (socketFD == -1)
    {
        printf("Error: Socket could not be created...\n");
        exit(1);
    }

    else
        printf("Socket created: %d\n", socketFD);

    
    // zero out server address block
    memset(&serverAddress, 0, sizeof(serverAddress));

    // populate serverAddress
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost; 
    // DEPRECATED: inet_addr converts a dots-and numbers address to in_addr (converts to "n"etwork)

    // connect to server
    if(connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        printf("Error: failed to conect...\n");
        exit(1);
    }

    else
        printf("Connected to 127.0.0.1:8080\n");

    
    // read data from server
    int bytesRead = read(socketFD, buffer, BUFFER_SIZE);

    printf("\n----------\nData received!\nPayload: %s\nBytes received: %d\n----------\n", buffer, bytesRead);

    // close connection
    close(socketFD);

    return 0;
}