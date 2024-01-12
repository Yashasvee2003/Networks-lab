#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct  
{
    char  IP[50];
    char  MAC[50];
}data;



int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
    exit(1);
  }

  int Port = atoi(argv[1]);

  int server_socket;
  int client_socket1, client_socket2;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr1, client_addr2;
  socklen_t client_len1, client_len2;

  char buffer[1024];

  // Create a Socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Socket Creation Failed!");
    exit(1);
  }

  // Input Server Information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind Socket
  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Socket Bind Failed!");
    exit(1);
  }

  // Listen
  if (listen(server_socket, 5) == -1) {
    perror("Listen Failed");
    exit(1);
  }
  printf("Server Listening on Port %d .....\n", Port);

  client_len1 = sizeof(client_addr1);
  client_socket1 =
      accept(server_socket, (struct sockaddr *)&client_addr1, &client_len1);
  if (client_socket1 == -1) {
    perror("Socket Accept Failed!");
    exit(1);
  }
  printf("Client 1 Connected\n");

  client_len2 = sizeof(client_addr2);
  client_socket2 =
      accept(server_socket, (struct sockaddr *)&client_addr2, &client_len2);
  if (client_socket2 == -1) {
    perror("Socket Accept Failed!");
    exit(1);
  }
  printf("Client 2 Connected\n");

  char ip[100];
  printf("Enter IP Address to Broadcast : ");
  scanf("%s", ip);
  write(client_socket1, ip, sizeof(ip));
  write(client_socket2, ip, sizeof(ip));

  char mac1[100], mac2[100];
  memset(mac1, '\0', sizeof(mac1));
  memset(mac2, '\0', sizeof(mac2));
  data obj1, obj2;

  read(client_socket1, &obj1, sizeof(obj1));
  read(client_socket2, &obj2, sizeof(obj2));
  printf("%s, %s\n", obj1.IP, obj1. MAC);
  printf("%s, %s\n", obj2.IP, obj2. MAC);
//   if (strcmp(mac1, "0") != 0) {
//     printf("Mac Address : %s\n", mac1);
//   } else {
//     printf("Mac Address : %s\n", mac2);
//   }

  return 0;
}
