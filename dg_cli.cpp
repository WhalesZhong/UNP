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
#include <zconf.h>

using namespace std;

//void dg_cli(FILE *fp, int sockfd, const sockaddr *pservaddr, socklen_t servlen){
//    int n;
//    char sendline[MAXLINE], recvline[MAXLINE];
//    socklen_t len;
//    struct sockaddr *preply_addr;
//
//    //显式连接
//    connect(sockfd, (sockaddr*)pservaddr, servlen);
//
//    while (fgets(sendline, MAXLINE, fp) != NULL){
//        //显式连接时不用sendto/recvfrom,直接用write/read,
//        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
//        len = servlen;
//        n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
//        recvline[n] = 0;
//        fputs(recvline, stdout);
//        write(sockfd, sendline, strlen(sendline));
//        if ((n = read(sockfd, recvline, MAXLINE)) < 0){
//            cout << "no content back" << endl;
//            break;
//        }
//        recvline[n] = 0;
//        fputs(recvline, stdout);
//    }
//}


#define NDG 2000
#define DGLEN 1400


void dg_cli(FILE *fp, int sockfd, const sockaddr *pservaddr, socklen_t servlen){
    int i;
    char sendline[DGLEN];

    for (i = 0; i < NDG; i++){
        sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
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


