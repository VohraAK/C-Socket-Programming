#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PAYLOAD_BUFFER 1024
#define INT_BYTE_SIZE 4
#define QUEUE_SIZE 3

int calculateResult(int numberOfOperands, int operands[], char operator)
{
    int result = operands[0]; // Initialize result with the first operand

    switch (operator)
    {
    case '+':
        printf("Adding %d operands\n", numberOfOperands);
        for (int i = 1; i < numberOfOperands; i++)
        {
            result += operands[i];
        }
        break;

    case '-':
        printf("Subtracting %d operands\n", numberOfOperands);
        for (int i = 1; i < numberOfOperands; i++)
        {
            result -= operands[i];
        }
        break;

    case '*':
        printf("Multiplying %d operands\n", numberOfOperands);
        for (int i = 1; i < numberOfOperands; i++)
        {
            result *= operands[i];
        }
        break;

    default:
        printf("Invalid operator received.\n");
        return 0; // Return 0 for unsupported operators
    }

    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int serverSocketFD, clientSocketFD;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressSize;
    int numberOfOperands;
    char operator;
    int result;

    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocketFD == -1)
    {
        printf("Error: failed to create socket...\n");
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        printf("Error: failed to bind...\n");
        exit(1);
    }

    if (listen(serverSocketFD, QUEUE_SIZE) == -1)
    {
        printf("Error: failed to listen...\n");
        exit(1);
    }

    clientAddressSize = sizeof(clientAddress);

    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientAddressSize);

        if (clientSocketFD == -1)
        {
            printf("Error: failed to accept connection...\n");
            exit(1);
        }

        // Read number of operands (1 byte)
        if (read(clientSocketFD, &numberOfOperands, 1) <= 0)
        {
            printf("Error: failed to read number of operands...\n");
            close(clientSocketFD);
            continue;
        }

        printf("-------------------------------------------------\n");
        printf("Number of operands = %d\n", numberOfOperands);

        // Allocate memory for operands
        int *operands = malloc(numberOfOperands * INT_BYTE_SIZE);

        // Read operands
        int receivedByteCount = 0;
        while (receivedByteCount < numberOfOperands * INT_BYTE_SIZE)
        {
            int bytesReceived = read(clientSocketFD, ((char *)operands) + receivedByteCount, numberOfOperands * INT_BYTE_SIZE - receivedByteCount);
            if (bytesReceived <= 0)
            {
                printf("Error: failed to read operands...\n");
                free(operands);
                close(clientSocketFD);
                break;
            }
            receivedByteCount += bytesReceived;
        }

        // Read operator (1 byte)
        if (read(clientSocketFD, &operator, 1) <= 0)
        {
            printf("Error: failed to read operator...\n");
            free(operands);
            close(clientSocketFD);
            continue;
        }

        // Calculate result
        result = calculateResult(numberOfOperands, operands, operator);

        // Send result back
        if (write(clientSocketFD, &result, sizeof(result)) <= 0)
        {
            printf("Error: failed to send result...\n");
        }

        free(operands);
        close(clientSocketFD);
    }
    printf("-------------------------------------------------\n");

    printf("END OF SESSION\n");

    close(serverSocketFD);

    return 0;
}
