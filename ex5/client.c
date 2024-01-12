#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345

int main() {
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[1024];

  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Socket creation failed");
    exit(1);
  }

  // Configure server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  // here in inet_addr() we pass the address of the server
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Connect to the server
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Connection failed");
    exit(1);
  }

  while (1) {
    printf("Enter a message: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(client_socket, buffer, strlen(buffer), 0);

    //new
    memset(buffer, 0, sizeof(buffer));

    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Server response: %s", buffer);
    memset(buffer, 0, sizeof(buffer));
  }

  close(client_socket);

  return 0;
}

