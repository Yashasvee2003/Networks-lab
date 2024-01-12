#include<stdio.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024
bool found = false;
char *dest_path;

void ls_recursive(char *bpath, char *file);

int main(int argc, char* argv[]) {
  int PORT = atoi(argv[1]);
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  socklen_t client_address_len = sizeof(client_address);
  char buffer[BUFFER_SIZE];

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Error creating server socket");
    exit(EXIT_FAILURE);
  }

  // Initialize server address
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  // Bind the socket to the server address
  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Error binding server socket");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_socket, 5) == -1) {
    perror("Error listening for connections");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d...\n", PORT);

  // Accept a client connection
  client_socket = accept(server_socket, (struct sockaddr *)&client_address,
                         &client_address_len);
  if (client_socket == -1) {
    perror("Error accepting client connection");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Client connected. Waiting for a message...\n");

  // Receive a message from the client
  ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
  if (bytes_received <= 0) {
    perror("Error receiving data from client");
  } else {
    buffer[bytes_received] = '\0'; // Null-terminate the received data
    printf("Received message from client: %s\n", buffer);
    dest_path = (char *)malloc(sizeof(char) * 1024);
    char path[100];
    char file[100];
    strcpy(file, buffer);
    memset(buffer, 0, sizeof(buffer));
    strcpy(path, "/home/yash");
    ls_recursive(path, file);
    if (!found) {
      printf("File Not Found\n");
    } else {
      printf("Absolute Path : %s\n", dest_path);
      FILE *file = fopen(dest_path, "r");
      if (file == NULL) {
        perror("Error opening file for reading");
        close(client_socket);
        exit(EXIT_FAILURE);
      }
      // printf("Opened File");
      ssize_t bytes_read;
      while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
      }
      fclose(file);
    }
  }

  close(client_socket);
  close(server_socket);

  return 0;
}

void ls_recursive(char *bpath, char *file) {
  char path[1000];
  struct dirent *dir_pointer;
  DIR *dir_stream = opendir(bpath);
  if (!dir_stream)
    return;

  while ((dir_pointer = readdir(dir_stream)) != NULL) {
    //printf("Name : %s type : %d\n", dir_pointer->d_name, dir_pointer->d_type);
    if (strcmp(dir_pointer->d_name, ".") != 0 &&
        strcmp(dir_pointer->d_name, "..") != 0) {
      if (strcmp(file, dir_pointer->d_name) == 0) {
        // printf("%s\n", dir_pointer ->d_name);
        // printf("%s\n",bpath);
        strcpy(dest_path, bpath);
        strcat(dest_path, "/");
        strcat(dest_path, dir_pointer->d_name);
        found = true;
        return;
      }
      strcpy(path, bpath);
      strcat(path, "/");
      strcat(path, dir_pointer->d_name);

      ls_recursive(path, file);
    }
  }

  closedir(dir_stream);
}