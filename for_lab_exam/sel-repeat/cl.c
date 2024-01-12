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
int tot_packets; 

void packetMaker(char **packet,char *msg,int q, int rem,int tot)
{
    for(int i=0;i<q;i++)
    {
        int chk=4*i;
        packet[i][0]=msg[chk];
        packet[i][1]=msg[chk+1];
        packet[i][2]=msg[chk+2];
        packet[i][3]=msg[chk+3];
    }
    if(rem!=0)
    {
        int r=rem;
        for(int j=0;j<r;j++)
        {
            packet[q][j]=msg[(4*q)+j];
        }
        //printf("%c %c %c is the\n",packet[4][0],packet[4][1],packet[4][2]);
    }
    printf("Here is the packetized version\n\n");
    for(int i=0;i<tot;i++)
    {
        printf("Packet %d: ",i);
        for(int j=0;j<strlen(packet[i]);j++)
        {
            printf("%c",packet[i][j]);
        }
        printf("\n");
    }
}
void retrasmit(int from,int total,int sockfd,char **packet,char dest[])
{
    for(int i=from;i<total;i++)
    {
        int ack;
        char str[30];
        sprintf(str,"F-%s-%d-%s-FCS",dest,i,packet[i]);
        printf("sending %s\n",str);
        //int recack;
        send(sockfd,str,sizeof(str),0);
        recv(sockfd,&ack,sizeof(int),0);
        printf("Received ack %d\n\n",ack);
        printf("-------------------\n");
    }
}
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

    int co=connect(sockfd,(struct sockaddr*)&seradd,sizeof(seradd));
    if(co<0)
    {
        printf("COnnection failed\n");
    }
    else
    {
        printf("Connection succesfuuly established\n");
    }
    char msg[30];
    printf("Enter the msg\n");
    scanf("%s",msg);
    int rem=strlen(msg)%4;
    int q=strlen(msg)/4;
    int total;
    if(rem!=0)
    {
        total=q+1;
    }
    else
    {
        total=q;
    }
    char **packet;
    packet = (char **)malloc(total* sizeof(char *)); 
    for(int i=0;i<total;i++)
    {
        packet[i] = (char *)malloc(25 * sizeof(char)); 
    }


    //send(sockfd,msg,sizeof(msg),0);
    packetMaker(packet,msg,q,rem,total);
    int ack;
    char dest[2];
    char src[2];

    printf("Enter src");
    scanf("%s",src);
    printf("\n\nEnter dest");
    scanf("%s",dest);

    // sending total number of packets to be sent
    int temp_total = total;
    send(sockfd, &temp_total, sizeof(int), 0);
    int tot_packets = total;

    for (int i = 0; i < tot_packets; i++)
    {
        bitmap[i] = false;
    }
    



    for(int i=0;i<total;i++)
    {
        printf("\nDo you want to send packet %d ? [1/0]",i);
        int choice;
        scanf("%d",&choice);
        if(choice==1)
        {
            char str[30];
            sprintf(str,"F-%s-%d-%s-FCS",dest,i,packet[i]);
            bitmap[i] = true;
            int recack;
            send(sockfd,str,sizeof(str),0);
            recv(sockfd,&recack,sizeof(int),0);
            ack=recack;
            printf("Received ack %d\n\n",recack);
            printf("-------------------\n");

        }
    }
    // if(ack<total)
    // {
    //     printf("\n\nRetrasmitting!");
    //     retrasmit(ack,total,sockfd,packet,dest);

    // }
    // int len;
    // int ackno;
    // while ((len=recv(sockfd,&ackno,sizeof(int),0))>0)
    // {
    //     char str[30];
    //     sprintf(str,"F-%s-%d-%s-FCS",dest,ackno,packet[ackno]);
    //     printf("%s\n", str);
        
    // }

    // sending the reamining packets 

    printf("sending remaining packets\n");
    for (int i = 0; i < tot_packets; i++)
    {
        if (bitmap[i] == false)
        {
            char str[30];
            sprintf(str,"F-%s-%d-%s-FCS",dest,i,packet[i]);
            bitmap[i] = true;       
            send(sockfd,str,sizeof(str),0);
        }

        
    }
    


    
    

    close(sockfd);



}