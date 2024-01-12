#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Define Port Number
  int PORT = atoi(argv[1]);

  // Declare Variables
  int client_socket1, client_socket2, server_socket;
  struct sockaddr_in client_addr1, server_addr, client_addr2;
  socklen_t client_len1 = sizeof(client_addr1);
  socklen_t client_len2 = sizeof(client_addr2);
  char buffer[1024];

  // Create a Socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Error in Creating Socket!!");
    exit(1);
  }

  // Fill Server Information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind socket to address
  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Binding Failed!");
    exit(1);
  }

  // Listen For Connections
  if (listen(server_socket, 5) == -1) {
    perror("Listening Failed!");
    exit(1);
  }
  printf("Listening on Port %d......\n", PORT);

  // Accept Connections
  client_socket1 =
      accept(server_socket, (struct sockaddr *)&client_addr1, &client_len1);
  if (client_socket1 == -1) {
    perror("Accepting Failed!");
    exit(1);
  }
  printf("Client 1 Connected to Server.......\n");
  client_socket2 =
      accept(server_socket, (struct sockaddr *)&client_addr2, &client_len2);
  if (client_socket2 == -1) {
    perror("Accepting Failed!");
    exit(1);
  }
  printf("Client 2 Connected to Server.......\n");

  // Read From Client 1
  ssize_t bytes_received = read(client_socket1, buffer, sizeof(buffer));
  if (bytes_received < 0) {
    perror("Reading Failed!");
    exit(1);
  }
  printf("Received Message : %s\n", buffer);

  // Write to Client2
  int temp1 = write(client_socket2, buffer, strlen(buffer));
  memset(buffer, 0, sizeof(buffer));

  // Read From Client 2
  bytes_received = read(client_socket2, buffer, sizeof(buffer));
  if (bytes_received < 0) {
    perror("Reading Failed!");
    exit(1);
  }
  printf("Received Message : %s\n", buffer);

  // Write to Client1
  int temp2 = write(client_socket1, buffer, strlen(buffer));
  memset(buffer, 0, sizeof(buffer));

  // printf("Enter a message : ");
  // fgets(buffer, sizeof(buffer), stdin);
  // buffer[strcspn(buffer, "\n")] = '\0';
  // write(client_socket, buffer, strlen(buffer));

  close(server_socket);
  close(client_socket1);
}