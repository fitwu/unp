#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>
#include<stdio.h>


#define ACCEPTQ 100
#define SERV_PORT 55555
#define MAXLINE 1024

void str_echo(int sockfd);
void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("child %d terminated \n", pid);
    return ;
}

int main()
{
    int listenfd, connfd;
    pid_t pid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, ACCEPTQ);
    signal(SIGCHLD, sig_chld);

    for(;;)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if((pid = fork()) == 0)
        {
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];
again:
    while( (n = read(sockfd, buf, MAXLINE)) > 0)
        write(sockfd, buf, n);
    if(n < 0 && errno == EINTR)
        goto again;
    else if(n < 0)
	{
        perror("read error");
		return ;
	}
}
