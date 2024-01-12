#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
    int servID, portnum;
    struct sockaddr_in serveraddr;

    servID = socket(AF_INET, SOCK_STREAM, 0);

    portnum = atoi(argv[1]);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portnum);
    serveraddr.sin_addr.s_addr = INADDR_ANY;



    connect(servID, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    while(1)
            {
                char cmsg[255] = "";
                printf("enter message to server : ");
                fgets(cmsg, sizeof(cmsg), stdin);
                send(servID, cmsg, sizeof(cmsg), 0);

                if (strcmp(cmsg,"exit\n")==0)
                {
                    break;
                }

                char smsg[255] = "";
                recv(servID, smsg, sizeof(smsg), 0);
                printf("message from server : %s \n",smsg);
            }

    close(servID);
    return 0;
}