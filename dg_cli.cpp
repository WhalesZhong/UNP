//
// Created by root on 18-1-6.
//

#include <unp.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstdlib>

using namespace std;

void dg_cli(FILE *fp, int sockfd, const sockaddr *pservaddr, socklen_t servlen){
    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL){
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
}


int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2){
        cout << "ip address is required" << endl;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    dg_cli(stdin, sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    exit(0);
}