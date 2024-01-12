#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  // Define Port no
  int PORT = atoi(argv[1]);

  // Declare Variables
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[1024];

  // Create Socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Failed to Create Socket!");
    exit(1);
  }

  // Fill Server Information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Connect to Server
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Connection Failed");
    exit(1);
  }

  // Write to Server
  printf("Enter a message : ");
  char *temp2 = fgets(buffer, sizeof(buffer), stdin);
  if (strcmp(buffer, "quit") == 0) {
    printf("Quitting....\n");
    exit(1);
  }
  buffer[strcspn(buffer, "\n")] = '\0';
  int temp1 = write(client_socket, buffer, strlen(buffer));
  memset(buffer, 0, sizeof(buffer));

  // Read From Server
  ssize_t bytes_received = read(client_socket, buffer, sizeof(buffer));
  if (bytes_received < 0) {
    perror("Reading Failed!");
    exit(1);
  }
  printf("Received Message : %s\n", buffer);

  close(client_socket);
  return 0;
}