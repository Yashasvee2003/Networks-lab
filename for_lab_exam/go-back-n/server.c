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
    while((len=recv(new_s,buffer,sizeof(buffer),0))>0)
    {
        if(buffer[0]!='\0' && buffer[0]=='F')
        {
            printf("%s\n",buffer);
            int num=buffer[4]-'0';
            if(ack==num)
            {
                ack+=1;
            }
            send(new_s,&ack,sizeof(int),0);

            printf("Sent ACK %d\n--------------------\n",ack);

        }
        //break;
    }
    close(new_s);
    close(sockfd);



}