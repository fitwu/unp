#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>

#define SERV_PORT 55555
#define MAXLINE 4096

void dg_echo(int sockfd, struct sockaddr * cliaddr,  socklen_t clilen);

int main()
{
    struct sockaddr_in servaddr, cliaddr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    dg_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

}
void dg_echo(int sockfd, struct sockaddr * cliaddr,  socklen_t clilen)
{
    int n;
    char msg[MAXLINE];
    socklen_t len;

    for(;;)
    {
        len = clilen;
        n = recvfrom(sockfd, msg, MAXLINE, 0, cliaddr, &len);

        sendto(sockfd, msg, n, 0, cliaddr, len); 
    }

}
