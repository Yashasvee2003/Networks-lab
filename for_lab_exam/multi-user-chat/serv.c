#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_MESSAGE_SIZE 1024

int clientSockets[MAX_CLIENTS];

// Function to handle client communication
void *handleClient(void *arg) {
    int clientId = *((int *)arg);
    int clientSocket = clientSockets[clientId];
    char buffer[MAX_MESSAGE_SIZE];

    while (1) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            printf("Client %d disconnected.\n", clientId);
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received message
        printf("Received from client %d: %s", clientId, buffer);

        // Broadcast the message to all clients except the sender
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (i != clientId && clientSockets[i] != -1) {
                send(clientSockets[i], buffer, bytesRead, 0);
            }
        }
    }

    // Close the socket
    close(clientSocket);
    clientSockets[clientId] = -1;

    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    pthread_t threadId[MAX_CLIENTS];

    // Initialize clientSockets array
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clientSockets[i] = -1;
    }

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Accept connection from client
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) == -1) {
            perror("Accept failed");
            continue;
        }

        // Find an available slot in the clientSockets array
        int clientId;
        for (clientId = 0; clientId < MAX_CLIENTS; ++clientId) {
            if (clientSockets[clientId] == -1) {
                clientSockets[clientId] = clientSocket;
                break;
            }
        }

        if (clientId == MAX_CLIENTS) {
            // Reached the maximum number of clients, reject connection
            printf("Connection rejected. Server full.\n");
            close(clientSocket);
            continue;
        }

        printf("New client connected, ID: %d\n", clientId);

        // Create a thread to handle client communication
        if (pthread_create(&threadId[clientId], NULL, handleClient, (void *)&clientId) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        // Detach the thread to allow it to terminate independently
        pthread_detach(threadId[clientId]);
    }

    close(serverSocket);

    return 0;
}
