#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  int PORT = atoi(argv[1]);
  int client_socket;
  struct sockaddr_in server_address;
  char buffer[BUFFER_SIZE];

  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Error creating client socket");
    exit(EXIT_FAILURE);
  }

  // Initialize server address
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
    perror("Invalid server address");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("Error connecting to the server");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server. Sending a message...\n");

  // Send a message to the server
  char *message = malloc(100);
  printf("enter file name : ");
  int s = scanf("%s", message);
  ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
  if (bytes_sent == -1) {
    perror("Error sending data to server");
  } else {
    printf("Message sent to server: %s\n", message);
  }

  recv(client_socket, buffer, sizeof(buffer), 0);
  printf("Server response: %s\n\n", buffer);

  close(client_socket);

  return 0;
}
