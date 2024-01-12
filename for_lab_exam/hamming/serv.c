#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Function to perform Hamming code error detection and correction
void hammingCode(char *data, int dataSize) {
    // ... (Same as the previous example)
    // Calculate the number of redundant bits needed
    int r = 1;
    while ((1 << r) < dataSize + r + 1) {
        r++;
    }

    // Create an array to hold the data along with redundant bits
    int hammingSize = dataSize + r;
    int hamming[hammingSize];
    memset(hamming, 0, sizeof(hamming));

    // Copy data to the appropriate positions in the hamming array
    int j = 0;
    for (int i = 0; i < hammingSize; i++) {
        if (i + 1 != (1 << j)) {
            hamming[i] = data[j++] - '0';
        }
    }

    // Calculate the values of redundant bits
    for (int i = 0; i < r; i++) {
        int parityBit = 1 << i;
        for (int j = parityBit - 1; j < hammingSize; j += 2 * parityBit) {
            for (int k = 0; k < parityBit; k++) {
                hamming[parityBit - 1] ^= hamming[j + k];
            }
        }
    }

    // Print the Hamming code
    printf("Hamming Code: ");
    for (int i = 0; i < hammingSize; i++) {
        printf("%d", hamming[i]);
    }
    printf("\n");
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    // Accept connection from client
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    char data[100];

    // Receive data from the client
    ssize_t bytesRead = recv(clientSocket, data, sizeof(data) - 1, 0);
    if (bytesRead <= 0) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    data[bytesRead] = '\0';
    printf("%s\n", data);

    // Perform Hamming code error detection and correction
    hammingCode(data, bytesRead);

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
