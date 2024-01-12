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
  int len = sizeof(server_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(1);
  }

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Use provided server IP

  printf("enter domain name to view IP :");
  fgets(domain, sizeof(domain), stdin);

  sendto(sockfd, domain, strlen(domain), 0, (struct sockaddr*) &server_addr, len);

  recvfrom(sockfd, new_ip, sizeof(new_ip), 0, (struct sockaddr*) &server_addr, &len);


  if (strcmp(new_ip, "INVALID") == 0)
  {
    printf("invalid domain name\n");
  }
  else
  {
    printf("ip for the domain name is : %s", new_ip);
  }

  close(sockfd);
  


}