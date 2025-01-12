#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define INT_BYTE_SIZE 4

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int socketFD;
    char *payload;

    struct sockaddr_in serverAddress;

    int numberOfOperands;
    int result;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (socketFD == -1)
    {
        printf("Error: failed to create socket...\n");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));
    serverAddress.sin_family = AF_INET;

    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Error: failed to connect to server...\n");
        exit(1);
    }

    // Get calculation information
    while (1)
    {
        printf("Enter number of operands (1-9): ");
        scanf("%d", &numberOfOperands);

        if (numberOfOperands < 1 || numberOfOperands > 9)
            printf("Incorrect range... try again.\n");
        else
            break;
    }

    // Allocate payload
    payload = malloc(numberOfOperands * INT_BYTE_SIZE + 2);

    // First byte: number of operands
    payload[0] = (char)numberOfOperands;

    // Next bytes: operands
    for (int i = 0; i < numberOfOperands; i++)
    {
        printf("Operand #%d: ", i + 1);
        scanf("%d", (int *)&payload[1 + i * INT_BYTE_SIZE]); // Store each operand (4 bytes)
    }

    getchar();
    // Last byte: operator
    printf("Operator: ");
    scanf("%c", &payload[1 + numberOfOperands * INT_BYTE_SIZE]);

    // Send payload
    write(socketFD, payload, numberOfOperands * INT_BYTE_SIZE + 2);

    // Receive result
    read(socketFD, &result, sizeof(result));

    printf("Operation result: %d\n", result);

    free(payload);
    close(socketFD);

    return 0;
}
