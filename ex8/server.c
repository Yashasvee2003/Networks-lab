#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <threads.h>
#include <unistd.h>

struct data {
  char message[100];
  int rbits;
};
struct data *d;

void decode_message(int r, char *msg);
char buffer[1024];
char buffer2[1024];

int main(int arc, char *argv[]) {

  int Port = atoi(argv[1]);
  // printf("%d",Port);

  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  char buffer[1024];

  // Create Socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Socket Creation Failed!");
    exit(1);
  }

  // Fill Server information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind function

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Binding Failed!");
    exit(1);
  }

  // Listening Function

  if (listen(server_socket, 5) == -1) {
    perror("Listening Failed!");
    exit(1);
  }

  printf("Server Listening on Port %d....\n", Port);

  // Accepting Client Connection

  client_socket =
      accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
  if (client_socket == -1) {
    perror("Accepting Failed!");
    exit(1);
  }

  printf("Connected to Client on Port %d....\n", Port);

  // Reading Binary Data

  d = (struct data *)malloc(sizeof(struct data));
  ssize_t bytes_read = recv(client_socket, d, sizeof(struct data), 0);
  if (bytes_read <= 0) {
    perror("Buffer Empty!");
  }
  // printf("Received message : %s\n",d->message);
  // printf("R bits : %d\n",d->rbits);
  decode_message(d->rbits, d->message);

  return 0;
}

void decode_message(int r, char *msg) {

  int length = strlen(msg) + 1;
  // printf("Length = %d\n",length);

  printf("\nDecoding Message:");
  int binary_num = 0;
  for (int count = 0; count < r; count++) {
    if (count == 0) {
      int ones_count = 0;
      int start = 0;
      for (; start < length; start += 2) {
        // printf("Index value = %d Bit %c\n", start, msg[start]);
        if (msg[start] == '1') {
          ones_count++;
        }
      }
      // printf("Number of ones = %d\n", ones_count);
      binary_num = (ones_count % 2 == 1) ? 1 : 0;
    } else {
      int ones_count = 0;
      int start = pow(2, count) - 1;
      int end = start + pow(2, count);
      while (start < length - 1) {
        while (start < end && start < length - 1) {
          // printf("Index value = %d Bit %c\n", start, msg[start]);
          if (msg[start] == '1') {
            ones_count++;
          }
          start++;
        }
        start--;
        // printf("Start = %d\n",start);
        start = start + (pow(2, count + 1) - 1);
        end = start + pow(2, count);
        // printf("Start = %d\n",start);
        // printf("End = %d\n",end);
        // printf("Done\n");
      }
      // printf("Ones count = %d\n", ones_count);
      int digit = (ones_count % 2 == 1) ? 1 : 0;
      binary_num = (digit << count) | binary_num;
    }
  }
  printf("\nError Bit Position = %d", binary_num);
  msg[binary_num - 1] = (msg[binary_num - 1] == '1') ? '0' : '1';
  printf("\nCorrected Message  = %s\n", msg);
}
