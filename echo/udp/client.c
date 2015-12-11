#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#define SERV_PORT 55555
#define MAXLINE 4096

void dg_cli(int sockfd, FILE *fp,struct sockaddr *servaddr, socklen_t len); 

int main(int argc, char **argv)
{
    struct sockaddr_in servaddr;
    int sockfd;

    if(argc < 2) 
    {
        printf("usage : %s {ip}\n", argv[0]);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    dg_cli(sockfd, stdin, (struct sockaddr*)&servaddr, sizeof(servaddr));

}
void dg_cli(int sockfd, FILE *fp,  struct sockaddr *servaddr, socklen_t len)
{
    int n;
    char recvbuf[MAXLINE+1], sendbuf[MAXLINE];

    while(fgets(sendbuf, MAXLINE, fp)!=NULL)
    {
        sendto(sockfd, sendbuf, strlen(sendbuf), 0, servaddr, len);

        n = recvfrom(sockfd, recvbuf, MAXLINE, 0 ,NULL, NULL);
        recvbuf[n] = 0;
        fputs(recvbuf, stdout);
    }
}

