#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
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
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d...\n", PORT);

    while (1) {
        // Receive message from client
        ssize_t recvBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (recvBytes == -1) {
            perror("Error receiving message");
            continue;
        }

        // Display received message and client information
        buffer[recvBytes] = '\0';
        printf("Received message from %s:%d: %s\n",
               inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        // Echo the message back to the client
        if (sendto(sockfd, buffer, recvBytes, 0, (struct sockaddr *)&clientAddr, addrLen) == -1) {
            perror("Error sending message");
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}
