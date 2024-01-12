#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<ctype.h>
#include <stdbool.h>


bool bitmap[100];
int tot_packets = 0;

int main(int argc, char *argv[])
{
    int sockfd;

    struct sockaddr_in seradd;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Failed Socket Creation\n");
        exit(1);
    }
    else
    {
        printf("Socket Creation successful\n");
    }

    seradd.sin_family=AF_INET;
    seradd.sin_addr.s_addr=INADDR_ANY;
    seradd.sin_port=htons(atoi(argv[1]));

    int bi=bind(sockfd,(struct sockaddr*)&seradd,sizeof(seradd));
    if(bi<0)
    {
        printf("\nBinding Failed\n");
    }
    else
    {
        printf("Successful Binding\n\n");
    }

    listen(sockfd,10);

    int new_s;
    int len;
    if((new_s=accept(sockfd,(struct sockaddr*)&seradd,&len))<0)
    {
        printf("Cant accept the connection\n");
    }

    else
    {
        printf("Connection accepted\n");
    }
    char buffer[20];
    int ack=0;
    int pos;

    // getting to know the tot no of packets
    recv(new_s, &tot_packets, sizeof(int), 0);
    printf("the total no. of packets to be recieved : %d\n", tot_packets);




    while((len=recv(new_s,buffer,sizeof(buffer),0))>0)
    {
        if(buffer[0]!='\0' && buffer[0]=='F')
        {
            printf("%s\n",buffer);
            int num=buffer[4]-'0';
            bitmap[num] = true;
            if(ack==num)
            {
                ack+=1;
            }
            send(new_s,&ack,sizeof(int),0);

            printf("Sent ACK %d\n--------------------\n",ack);

        }
        //break;
    }
    // printf("***********************");
    // printf("sending acks for remaining packets\n");
    // int temp = 0;
    // for(int i = 0 ; i < tot_packets; i++)
    // {
    //     if (!bitmap[i])
    //     {
    //         // send ack for that packet no and recieve the packet
    //         temp = i;
    //         bitmap[i] = true;
    //         send(new_s, &temp, sizeof(int), 0);


    //     }
    // }

    printf("resent packets\n");
    len = 0;
    while((len=recv(new_s,buffer,sizeof(buffer),0))>0)
    {
        printf("%s\n", buffer);
        // send(new_s, " ", 1, 0);
    }

    close(new_s);
    close(sockfd);



}