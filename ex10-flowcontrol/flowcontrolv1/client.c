#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>


struct Node {
    char packet[100];
    struct Node* next;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};

void enqueue(struct Queue* queue, char* packet) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->packet, packet);
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}




void printqueue(struct Queue* queue) {
    struct Node* current = queue->front;
    while (current != NULL) {
        printf("%s\n", current->packet);
        current = current->next;
    }
}


void dequeue(struct Queue* queue, int awk_seq) {
    struct Node* current = queue->front;
    struct Node* prev = NULL;

    while (current != NULL) {
        int seqNo;
        sscanf(current->packet, "%*[^-]-%*[^-]-%d-", &seqNo);

        if (seqNo <= awk_seq) {
            struct Node* temp = current;

            if (prev == NULL) {
                queue->front = current->next;
            } else {
                prev->next = current->next;
            }

            if (current == queue->rear) {
                queue->rear = prev;
            }

            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}


void encapsulatePacket(char *packet, int seqNo) {
    char header[20];
    char f[] = "F";
    char A[] = "A";
    char fcs[] = "FCS";

    sprintf(header, "%s-%s-%d-", f, A, seqNo);
    strcat(header, packet);
    strcat(header, "-");
    strcat(header, fcs);

    strcpy(packet, header);
}

int main(int argc, char *argv[]){
    struct Queue queue;
    queue.front = NULL;
    queue.rear = NULL;
    char buffer[100];
    printf("Enter message:");
    fgets(buffer, sizeof(buffer), stdin); // Read user input as a string
    int len = strlen(buffer);
    buffer[len - 1] = '\0'; // Remove the trailing newline character
    int clientfd=socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(atoi(argv[1]));
    serveraddr.sin_addr.s_addr=INADDR_ANY;

    if(clientfd){
        printf("created client soc\n");
        if(connect(clientfd,(struct sockaddr *) &serveraddr, sizeof(serveraddr))==0){
            printf("Connected\n");

            // Split the input into packets of maximum 4 bits each
            int len = strlen(buffer);
            int currentseqno = -1;


            int i;
            for(i = 0; i < len; i += 4) {
                currentseqno = (i / 4);
                char packet[100];
                strncpy(packet, buffer + i, 4);
                packet[4] = '\0'; 
                encapsulatePacket(packet, i / 4); // Pass the sequence number as an argument  


                // Send each packet to the server
                printf("Encapsulated Packet: %s\n", packet);
                char choice[10];
                printf("Do you want to drop this packet during transmission? (yes/no): ");
                fgets(choice, sizeof(choice), stdin);
                choice[strcspn(choice, "\n")] = '\0'; // Remove the trailing newline character

                if (strcmp(choice, "yes") == 0) {
                    // Enqueue the packet without sending it
                    enqueue(&queue, packet);
                } else {
                    printf("Sending packet: %s\n", packet);
                    enqueue(&queue, packet);
                    write(clientfd, packet, sizeof(packet));
                    printf("Queue:\n");
                    printqueue(&queue);

                    // Receive the ACK from the server
                    char ack[10];
                    read(clientfd, ack, sizeof(ack));



                    if(strcmp(ack, " ") != 0){
                        int awk_seq;
                        sscanf(ack, "ACK-%d", &awk_seq);
                        printf("ACK received: %s\n", ack);
                        printf("currentseqno: %d\n", currentseqno);
                        if (awk_seq < currentseqno) {
                            struct Node* current = queue.front;
                            while (current != NULL) {
                                char packet[100];
                                strcpy(packet, current->packet);
                                printf("Sending packet from queue: %s\n", packet);
                           
                                write(clientfd, packet, sizeof(packet));
                                read(clientfd, ack, sizeof(ack));
                                sscanf(ack, "ACK-%d", &awk_seq);
                                current = current->next;
                            }
                            printf("All packets in queue sent.\n");

                        }
                        dequeue(&queue, awk_seq-1);
                        printf("Queue:\n");
                        printqueue(&queue);
                        
                        
                        
                       
                    }
                    
                }
               

                

                usleep(1000); // Add a delay to send packets one by one
            }
        }
    }
    close(clientfd);
    return 0;
}