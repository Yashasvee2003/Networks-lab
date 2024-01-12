#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXCLIENTS 4

int main(int argc, char *argv[]) {
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd) {
        struct sockaddr_in serveraddr, client_addr[MAXCLIENTS];
        socklen_t client_len[MAXCLIENTS];
        for (int i = 0; i < MAXCLIENTS; i++) {
            client_len[i] = sizeof(client_addr[i]);
        }
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(atoi(argv[1]));
        serveraddr.sin_addr.s_addr = INADDR_ANY;
        bind(serverfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
        listen(serverfd, MAXCLIENTS);
        int client_fd[MAXCLIENTS];
        for (int i = 0; i < MAXCLIENTS; i++) {
            client_fd[i] = accept(serverfd, (struct sockaddr *) &client_addr[i], &client_len[i]);
            printf("Client %d connected\n", i+1);

            // Receive and print the packets from the client
            char packet[100];
            int bytesRead;
            int expectedSeqNo = 0; // Initialize the expected sequence number
             int outOfOrderCounter = 0;
            while ((bytesRead = read(client_fd[i], packet, sizeof(packet))) > 0) {
                printf("Data received from client %d: %s\n", i+1, packet);

                // Process the received packet
                // ...

                int seqNo;
                sscanf(packet, "F-A-%d-", &seqNo);

                if (seqNo == expectedSeqNo) {
                    // Packet is in order
                    // Process the packet
                    // ...

                    // Send ACK with the next expected sequence number to the client
                    char ack[10];
                    sprintf(ack, "ACK-%d", expectedSeqNo + 1);
                    write(client_fd[i], ack, strlen(ack) + 1);

                    expectedSeqNo++; // Increment the expected sequence number

                    outOfOrderCounter = 0;
                    printf("Sending ACK-%d\n",expectedSeqNo); // Print statement saying "Sending ACK"
                } else {
                    // Packet is out of order
                    printf("Discarded out-of-order packet\n");

                     outOfOrderCounter++;
                      if (outOfOrderCounter >= 2) {
                        // Send ACK with the expected sequence number to the client
                        char ack[10];
                        sprintf(ack, "ACK-%d", expectedSeqNo);
                        write(client_fd[i], ack, strlen(ack) + 1);

                        printf("Sending ACK-%d\n", expectedSeqNo); // Print statement saying "Sending ACK"

                        outOfOrderCounter = 0; // Reset the out-of-order packet counter
                    }
                    else{
                        write(client_fd[i], " ", 6);
                    }
                }
            }
            close(client_fd[i]);
        }
        close(serverfd);
        return 0;
    }
}    