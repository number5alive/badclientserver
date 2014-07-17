#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#define BE_A_SERVER 1
#define BE_A_CLIENT 0

typedef struct Arguments_s
{
    uint16_t            nSocketNum;
    char               *pInetAddr;
    struct sockaddr_in  serv_addr;
}Arguments;
 
int clientmain(Arguments *pArgs)
{
    int sockfd = 0,n = 0;
    char recvBuff[1024];

    memset(recvBuff, '0' ,sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr *)&pArgs->serv_addr, sizeof(pArgs->serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error");
        }
        printf("\n");
    }

    if( n < 0)
    {
        printf("\n Read Error \n");
    }

    return 0;
}
 
int servermain(Arguments *pArgs)
{
    int listenfd = 0,connfd = 0;
    char sendBuff[1025];  
    int numrv;  

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket retrieve success\n");

    memset(sendBuff, '0', sizeof(sendBuff));

    bind(listenfd, (struct sockaddr*)&pArgs->serv_addr,sizeof(pArgs->serv_addr));

    if(listen(listenfd, 10) == -1){
        printf("Failed to listen\n");
        return -1;
    }


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

        strcpy(sendBuff, "Message from server");
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);    
        sleep(1);
    }


    return 0;
}

int main(void)
{
    int programType = BE_A_SERVER;
    Arguments   args;

    args.nSocketNum = 5000;
    args.pInetAddr = "127.0.0.1";

    memset(&args.serv_addr, '0', sizeof(args.serv_addr));
    args.serv_addr.sin_family = AF_INET;
    args.serv_addr.sin_port = htons(args.nSocketNum);
    args.serv_addr.sin_addr.s_addr = inet_addr(args.pInetAddr);

    if( programType == BE_A_SERVER )
    {
        return servermain( &args );
    }
    else
    {
        return clientmain( &args );
    }

    return 0;
}

