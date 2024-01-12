#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <Server_IP> <Port>\n", argv[0]);
    exit(1);
  }

  int Port = atoi(argv[2]);
  char buffer[MAX];
  char new_ip[MAX];
  char domain[MAX];
  const char *request = "modify";

  int sockfd;
  struct sockaddr_in server_addr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Use provided server IP

  int choice;
  printf("Enter 1 to Modify Domain Name\nEnter 2 to Modify IP Address\n");
  int s = scanf("%d", &choice);
  getchar();

  if (choice == 1) {
    printf("Enter Domain Name : ");
    char *f = fgets(buffer, MAX, stdin);
    sendto(sockfd, buffer, strlen(buffer), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message Sent From Client\n");

    char ip_buffer[MAX];
    socklen_t len = sizeof(server_addr);
    int n = recvfrom(sockfd, ip_buffer, sizeof(ip_buffer), 0,
                     (struct sockaddr *)&server_addr, &len);
    ip_buffer[n] = '\0';
    printf("Received IP Address from Server: %s\n", ip_buffer);
  } else if (choice == 2) {
    sendto(sockfd, request, strlen(request), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message Sent From Client\n");
    printf("Enter Domain : ");
    char * g = fgets(domain,MAX,stdin);
    sendto(sockfd, domain, strlen(domain), 0,
       (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Enter new IP : ");
    char *f = fgets(new_ip, MAX, stdin);
    sendto(sockfd, new_ip, strlen(new_ip), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message Sent From Client\n");
  } else {
    printf("Invalid choice. Exiting.\n");
  }
}
