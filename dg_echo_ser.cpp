//
// Created by root on 18-1-6.
//

#include <unp.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>

void dg_echo(int sockfd, sockaddr *pcliaddr, socklen_t clilen){
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for ( ; ; ){
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}


int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    dg_echo(sockfd, (sockaddr*)&cliaddr, sizeof(cliaddr));
}
