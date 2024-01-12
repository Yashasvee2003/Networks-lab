#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12343
#define MAX_BUFFER_SIZE 1024

int client_socket;
struct sockaddr_in server_addr;
char buffer[MAX_BUFFER_SIZE];

void *receive_messages(void *arg) {
  int client_socket = *(int *)arg;
  while (1) {
    ssize_t amountReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (amountReceived <= 0) {
      // Server disconnected or an error occurred
      printf("Server disconnected.\n");
      exit(1);
    }

    buffer[amountReceived] = '\0';
    printf("Received: %s", buffer);
  }
}

void *send_messages(void *arg) {
  int client_socket = *(int *)arg;
  while (1) {
    fgets(buffer, sizeof(buffer), stdin); // Comment out to remove the prompt

    if (strcmp(buffer, "exit\n") == 0) {
      // Exit the chat
      break;
    }

    send(client_socket, buffer, strlen(buffer), 0);
  }
}

int main() {
  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Socket creation failed");
    exit(1);
  }

  // Configure server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Connect to the server
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Connection failed");
    exit(1);
  }

  printf("Connected to the server. Type 'exit' to quit.\n");

  // Create threads for sending and receiving messages
  pthread_t receive_thread, send_thread;
  if (pthread_create(&receive_thread, NULL, receive_messages, &client_socket) !=
          0 ||
      pthread_create(&send_thread, NULL, send_messages, &client_socket) != 0) {
    perror("Thread creation failed");
    exit(1);
  }

  // Wait for threads to finish
  pthread_join(receive_thread, NULL);
  pthread_join(send_thread, NULL);

  close(client_socket);

  return 0;
}
