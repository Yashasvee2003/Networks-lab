#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SERVER_PORT 5432
#define MAX_LINE 256

int main(){
	struct sockaddr_in sin;
	int len,s;
	char buffer[1024];
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	while (fgets(buffer, sizeof(buffer), stdin)) {
		buffer[MAX_LINE-1] = '\0';
		len = strlen(buffer) + 1;
		send(s, buffer, len, 0);
		if (strncmp(buffer,"end",3)==0){
			printf("Client stopped communication\n");
			break;
		}
		
	}
	
}

