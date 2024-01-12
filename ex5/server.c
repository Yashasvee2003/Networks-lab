#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  char buffer[1024];

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Socket creation failed");
    exit(1);
  }

  // Bind the socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Socket bind failed");
    exit(1);
  }

  // Listen for incoming connections
  // 5 is the max no of devices that one can connect to
  if (listen(server_socket, 5) == -1) {
    perror("Listen failed");
    exit(1);
  }

  printf("Server listening on port %d...\n", PORT);

  // Accept connections and handle data
  client_socket =
      accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
  if (client_socket == -1) {
    perror("Accept failed");
    exit(1);
  }
  printf("Connection established with client\n");

  while (1) {
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received: %s", buffer);
    //new
    memset(buffer, 0, sizeof(buffer));
    printf("Enter a message: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(client_socket, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
  }

  close(server_socket);
  close(client_socket);

  return 0;
}

