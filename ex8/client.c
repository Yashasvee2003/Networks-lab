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

char buffer[100];

void redundant_bits(int m, int r, char *msg, char *data);
void encode_message(int r, int length, char *msg);

int Port;

int main(int arc, char *argv[]) {

  Port = atoi(argv[1]);

  int client_socket;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];

  // Creating a Socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Socket Creation Failed!");
    exit(1);
  }

  // Fill Server information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Connect to Server
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Connection Failed!");
    exit(1);
  }

  printf("Connected to server!\n");

  char data[100];
  printf("Enter data : ");
  int s1 = scanf("%s", data);
  int m = strlen(data);
  // printf("Length of data : %d\n", m);
  int r = 0;
  while (pow(2, r) < (m + r + 1)) {
    r++;
  }
  // printf("Number of redundant bits = %d\n", r);

  int length = m + r + 1;
  char msg[length];

  d = (struct data *)malloc(sizeof(struct data));

  // data is the input data( m bits), msg is the empty array to store final data(m+r bits)
  redundant_bits(m, r, msg, data);
  // printf("Encoded Message : %s\n", msg);
  encode_message(r, length, msg);
  printf("Encoded Message After finding r bits: %s\n", msg);
  int error_bit;
  printf("Enter the bit at which to introduce error: ");
  int s2 = scanf("%d", &error_bit);

  if (error_bit >= 1 && error_bit <= length) {
    msg[error_bit - 1] = (msg[error_bit - 1] == '1') ? '0' : '1';
    printf("Message with Error: %s\n", msg);
    strcpy(d->message, msg);
  } else {
    printf(
        "Invalid bit position. Please enter a bit within the valid range.\n");
  }
  d->rbits = r;
  // printf("R bits : %d\n", d->rbits);
  //  Send the struct containing msg and r to the server
  send(client_socket, d, sizeof(struct data), 0);

  return 0;
}

// inserts an 'r' wherever a redundant bit has to be introduced
void redundant_bits(int m, int r, char *msg, char *data) {
  int power = 0;
  int data_idx = m - 1;
  for (int index = 0; index < (m + r + 1); index++) {
    if (index == ((pow(2, power) - 1))) {
      power++;
      msg[index] = 'r';
    } else {
      msg[index] = data[data_idx--];
    }
  }
  msg[m + r] = '\0';
  printf("msg(in redundant bits func) is %s\n", msg);
}

void encode_message(int r, int length, char *msg) {

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
      // printf("Number of ones = %d", ones_count);
      msg[(int)(pow(2, count) - 1)] = (ones_count % 2) ? '1' : '0';
      // printf("\nEncoded Message : %s\n", msg);
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
      msg[(int)(pow(2, count) - 1)] = (ones_count % 2) ? '1' : '0';
      // printf("\nEncoded Message : %s\n", msg);
    }
  }
}
