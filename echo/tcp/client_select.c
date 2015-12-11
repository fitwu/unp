#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/select.h>

#define MAXLINE 1024
#define SERV_PORT 55555 

#define max(a,b) (a)>(b)?(a):(b)
void str_cli(FILE *fp, int sockfd)
{

    fd_set rset;
    int maxfd, stdineof;
    int n;
    char buf[MAXLINE];

    FD_ZERO(&rset);
    stdineof = 0;
    int i= 0;
    for(;;)
    {
        if(stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;

       i =  select(maxfd, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &rset))
        {
            if( (n = read(sockfd, buf, MAXLINE)) == 0)
            {
                if(stdineof == 1)
                    return;
                else
                {
                    perror("read error");
                    exit(0);
                }
            }
            write(fileno(stdout), buf, n);
        }
        if(FD_ISSET(fileno(fp), &rset))
        {
            if( (n = read(fileno(fp), buf, MAXLINE)) == 0)
            {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, buf, n);
        }

    }


}

int main(int argc, char** argv)
{
    struct sockaddr_in servaddr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);
    exit(0);
}


