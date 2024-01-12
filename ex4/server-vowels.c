// client sends text terminated by . to the server and the server returns no of vowels, characters, words etc back to client


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {

	char s[200];
	char reply[200];
	int alphabets = 0, digits = 0, vowels = 0, others = 0;
	char alphabetstr[sizeof(int)*8+1], digitstr[sizeof(int)*8+1], vowelstr[sizeof(int)*8+1], otherstr[sizeof(int)*8+1];
	int server_socket = socket(AF_INET, SOCK_STREAM,0);
	if(server_socket>=0) {
		struct sockaddr_in server_add, client_add;
		server_add.sin_family = AF_INET;
		server_add.sin_port = htons(atoi(argv[1]));
		server_add.sin_addr.s_addr = INADDR_ANY;
		bind(server_socket, (struct sockaddr *) &server_add, sizeof(server_add));
		if(listen(server_socket,4) == -1) {
			printf("ERROR\n");
			close(server_socket);
			exit(EXIT_FAILURE);
		}
		int client_socket = accept(server_socket,NULL, NULL);

		if(client_socket < 0) {
			perror("Error client\n");
		}

		else {
			read(client_socket, s, sizeof(s));
			for(int temp = 0; s[temp]!='\0';temp++) {
				if(s[temp] == 'a' || s[temp] == 'e' || s[temp] == 'i' || s[temp] == 'o' || s[temp] == 'u') vowels++;
				if(isalpha(s[temp])) alphabets++;
				else if(isdigit(s[temp])) digits++;
				else others++;
			}
			strcpy(reply, "Alphabets: ");
			snprintf(alphabetstr, sizeof(alphabetstr), "%d", alphabets);
			strcat(reply, alphabetstr);
			strcat(reply, ", Digits: ");
			snprintf(digitstr, sizeof(digitstr), "%d", digits);
			strcat(reply, digitstr);
			strcat(reply, ", Vowels: ");
			snprintf(vowelstr, sizeof(vowelstr), "%d", vowels);
			strcat(reply, vowelstr);
			strcat(reply, ", Other characters: ");
			snprintf(otherstr, sizeof(otherstr), "%d", others);
			strcat(reply, otherstr);
			if(write(client_socket, reply, sizeof(reply)) < 0) {
				perror("");
			}
		}

		
		
		
		close(client_socket);
		close(server_socket);
	}
	return 0;
}
