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
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <Port> <IP_Address> <MAC_Address>\n", argv[0]);
    exit(1);
  }

  int Port = atoi(argv[1]);

  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[1024];

  // Create a Socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Socket Creation Failed!");
    exit(1);
  }

  // Input Server Information
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(Port);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Connect Socket
  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Socket Connect Failed!");
    close(client_socket); // Close the socket on connection failure
    exit(1);
  }

  char msg[100];
  char IP_Address[100];
  char MAC_Address[100];

  strcpy(IP_Address, argv[2]);
  strcpy(MAC_Address, argv[3]);

    data obj;
    // obj.IP = IP_Address;
    // obj.MAC = MAC_Address;
    memset(obj.IP,'\0',sizeof(obj.IP) );
    memset(obj.MAC,'\0',sizeof(obj.MAC) );
    
    strcpy(obj.IP, argv[2]);
    strcpy(obj.MAC, argv[3]);



  // Send IP and MAC Address to the server
  //write(client_socket, IP_Address, sizeof(IP_Address));
  //write(client_socket, MAC_Address, sizeof(MAC_Address));

  // Read the broadcasted message from the server
  read(client_socket, msg, sizeof(msg));
  printf("Broadcasted Message: %s\n", msg);

  if (strcmp(msg, IP_Address) == 0) {
    //printf("MAC Address: %s\n", MAC_Address);
    
    printf("inside if, strcmp value : %d\n",strcmp(msg, IP_Address));
    write(client_socket,&obj,sizeof(obj));
  }
  else
  {
  	write(client_socket, "0", 1);
  }

  close(client_socket);

  return 0;
}
