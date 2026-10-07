#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in serv_addr;

    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Change IP if using another machine
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address\n");
        return -1;
    }

    // Connect
    if (connect(sock, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server.\n");

    while (1)
    {
        printf("\nEnter command (UP|text, LOW|text, REV|text) or 'bye':\n");

        fgets(input, BUFFER_SIZE, stdin);

        send(sock, input, strlen(input), 0);

        if (strncmp(input, "bye", 3) == 0)
            break;

        memset(buffer, 0, BUFFER_SIZE);

        int valread = read(sock, buffer, BUFFER_SIZE);

        if (valread <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server Response: %s\n", buffer);
    }

    close(sock);

    return 0;
}