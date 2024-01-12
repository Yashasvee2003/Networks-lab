#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_MESSAGE_SIZE 1024

int clientSocket;

// Function to receive messages from the server
void *receiveMessages(void *arg) {
    char buffer[MAX_MESSAGE_SIZE];

    while (1) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            printf("Server disconnected.\n");
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received message
        printf("Received from server: %s", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    struct sockaddr_in serverAddr;
    pthread_t threadId;

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Create a thread to receive messages from the server
    if (pthread_create(&threadId, NULL, receiveMessages, NULL) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    char message[MAX_MESSAGE_SIZE];

    // Send messages to the server
    while (1) {
        printf("Enter message to send to server (Type 'exit' to end): ");
        fgets(message, sizeof(message), stdin);

        if (strcmp(message, "exit\n") == 0) {
            break;
        }

        send(clientSocket, message, strlen(message), 0);
    }

    close(clientSocket);

    return 0;
}
