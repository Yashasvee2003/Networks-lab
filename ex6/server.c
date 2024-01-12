#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12343
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast a message to all connected clients
void broadcast(char *message, int current_client) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < client_count; i++) {
    if (client_sockets[i] != current_client) {
      send(client_sockets[i], message, strlen(message), 0);
    }
  }
  pthread_mutex_unlock(&mutex);
}

// Function to handle each client
void *handle_client(void *arg) {
  int client_socket = *((int *)arg);
  char buffer[1024];

  while (1) {
    ssize_t amountReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (amountReceived <= 0) {
      // Client disconnected
      pthread_mutex_lock(&mutex);
      for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == client_socket) {
          for (int j = i; j < client_count - 1; j++) {
            client_sockets[j] = client_sockets[j + 1];
          }
          client_count--;
          break;
        }
      }
      pthread_mutex_unlock(&mutex);
      close(client_socket);
      pthread_exit(NULL);
    }

    buffer[amountReceived] = '\0';
    printf("Received from client %d: %s", client_socket, buffer);

    broadcast(buffer, client_socket);
  }
}

int main() {
  int server_socket;
  struct sockaddr_in server_addr;
  pthread_t thread_id;

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
  if (listen(server_socket, 5) == -1) {
    perror("Listen failed");
    exit(1);
  }

  printf("Server listening on port %d...\n", PORT);

  while (1) {
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Accept connections and handle data in a separate thread
    client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
      perror("Accept failed");
      exit(1);
    }

    pthread_mutex_lock(&mutex);
    client_sockets[client_count++] = client_socket;
    pthread_mutex_unlock(&mutex);

    printf("Connection established with client %d\n", client_socket);

    // Create a new thread to handle this client
    if (pthread_create(&thread_id, NULL, handle_client, &client_socket) != 0) {
      perror("Thread creation failed");
      exit(1);
    }
  }

  close(server_socket);

  return 0;
}
