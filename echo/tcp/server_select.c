#include<sys/select.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>

#define SERV_PORT 55555
#define ACCEPTQ 1024
#define MAXLINE 4096
int main()
{
    struct sockaddr_in servaddr, cliaddr;
    int listenfd, connfd, maxi, maxfd, i, sockfd;
    ssize_t n;
    int client[FD_SETSIZE];
    int buf[MAXLINE], nready;
    socklen_t clilen;
    fd_set rset,allset;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    listenfd  = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, ACCEPTQ);


    maxfd = listenfd;
    maxi = -1;
    for(i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for(;;)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(listenfd, &rset))
        {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&servaddr, &clilen);
            
            for(i = 0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            if(i == FD_SETSIZE )
			{
                perror("too many clients");
				break;
			}
			
            FD_SET(connfd, &allset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }
        for( i = 0; i <=  maxi; i++)
        {
            sockfd = client[i];
            if(sockfd < 0)
                continue;
			
            if(FD_ISSET( sockfd, &rset))
            {
                if((n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else
                    write(sockfd, buf, n);

                if ( --nready <= 0)
                    break;
            }
        }
    }
}
