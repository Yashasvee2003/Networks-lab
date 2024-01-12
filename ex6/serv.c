#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
    int servID, clientID, portnum;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    servID = socket(AF_INET, SOCK_STREAM, 0);

    portnum = atoi(argv[1]);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portnum);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(servID, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
    listen(servID, 10);

    while(1)
    {
        clientID = accept(servID, (struct sockaddr *) &clientaddr, &clientlen);
        printf("NEW CONNETION FROM PORT NUMBER %d \n",ntohs(clientaddr.sin_port));

        pid_t pid = fork();

        if (pid==0)
        {
            //close(servID);

            while(1)
            {
                char cmsg[255] = "";
                recv(clientID, cmsg, sizeof(cmsg), 0);
                printf("message from client with port number %d : %s \n",ntohs(clientaddr.sin_port),cmsg);

                char smsg[255] = "";
                printf("enter reply : ");
                fgets(smsg, sizeof(smsg), stdin);
                send(clientID, smsg, sizeof(smsg), 0);

                
            }
            close(clientID);
            exit(0);
        }
        else
        {
            close(clientID);
        }
    }
    close(servID);
    return 0;
}