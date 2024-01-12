// client sends text terminated by . to the server and the server returns no of vowels, characters, words etc back to client

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char message[200];
	char new[200];
	printf("Enter message: ");
	scanf("%[^.]", message);
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_add;
	server_add.sin_family = AF_INET;
	server_add.sin_port = htons(atoi(argv[1]));
	server_add.sin_addr.s_addr = INADDR_ANY;
	if (client_socket)
	{
		if (connect(client_socket, (struct sockaddr *)&server_add, sizeof(server_add)) == 0)
		{
			write(client_socket, message, strlen(message));
			int val = read(client_socket, new, sizeof(new));
			printf("Reply:\n %s\n", new);
		}
		else
		{
			perror("");
		}
	}
	else
	{
		perror("");
	}
	return 0;
}
