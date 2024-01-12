#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        printf("Enter message to send (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Exit loop if the user enters "exit"
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        // Send message to the server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Error sending message");
            continue;
        }

        // Receive echoed message from the server
        ssize_t recvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (recvBytes == -1) {
            perror("Error receiving echoed message");
            continue;
        }

        // Display echoed message
        buffer[recvBytes] = '\0';
        printf("Echoed message from server: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
