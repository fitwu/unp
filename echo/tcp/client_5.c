#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define MAXLINE 1024
#define SERV_PORT 55555 

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline, MAXLINE, fp)!=NULL)
    {
        write(sockfd, sendline, strlen(sendline));
        if(read(sockfd, recvline, MAXLINE) == 0)
        {
            perror("str_cli error");
            exit(0);
        }
        fputs(recvline, stdout);
        bzero(&sendline, MAXLINE); 
        bzero(&recvline, MAXLINE); 
        
    }


}

int main(int argc, char** argv)
{
    struct sockaddr_in servaddr;
    int sockfd;
    int i;
for(i = 0; i < 5; i++){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
}    
    str_cli(stdin, sockfd);
    exit(0);
}


