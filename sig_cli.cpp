#include <unp.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <arpa/inet.h>
#include <signal.h>

using namespace std;

void str_cli(FILE *fp, int sock_fd){
    char send_line[MAXLINE], recieve_line[MAXLINE];
    while (fgets(send_line, MAXLINE, fp) != NULL){
        write(sock_fd, send_line, strlen(send_line));
        if (read(sock_fd, recieve_line, MAXLINE) == 0){
            cout << "str_cli: read error" << endl;
        }
        fputs(recieve_line, stdout);
    }
}


void str_cli_select(FILE *fp, int sockfd){
    int maxfd;
    fd_set rset;
    char send_line[MAXLINE], recieve_line[MAXLINE];

    FD_ZERO(&rset);
    for ( ; ; ){
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        //最大描述符加一
        maxfd = max(fileno(fp), sockfd) + 1;
        select(maxfd, &rset, NULL, NULL, NULL);

        //如果检测到套接字可读
        if FD_ISSET(sockfd, &rset){
            if (read(sockfd, recieve_line, MAXLINE) == 0){
                cout << "str_cli: read error" << endl;
            }
            fputs(recieve_line, stdout);
        }

        //如果输入流可读
        if FD_ISSET(fileno(fp), &rset){
            if (fgets(send_line, MAXLINE, fp) == NULL){
                return;
            }
            write(sockfd, send_line, strlen(send_line));
        }
    }
}


void str_cli_v2(FILE *fp, int sockfd){
    int maxfd, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);

    for ( ; ; ){
        if (stdineof == 0){
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;

        select(maxfd, &rset, NULL, NULL, NULL);

        //监测套接字可读
        if (FD_ISSET(sockfd, &rset)){
            if ((n = read(sockfd, buf, MAXLINE)) == 0){
                if (stdineof == 1){
                    return;
                }else{
                    cout << "str_cli: read error" << endl;
                }
            }
            write(fileno(stdout), buf, n);
        }

        //监测输入流
        if (FD_ISSET(fileno(fp), &rset)){
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0){
                stdineof = 1;
                //从输入流读取eof时关闭写
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            write(sockfd, buf, n);
        }
    }



}

int main(int argc, char **argv){
    int i, sockfd[5];
    struct sockaddr_in servaddr;

    for (i = 0; i < 5; i++){
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVPORT);
        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
        connect(sockfd[i], (sockaddr*)&servaddr, sizeof(servaddr));
    }
    str_cli(stdin, sockfd[0]);
    exit(0);
}
