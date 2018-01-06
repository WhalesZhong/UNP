//
// Created by root on 18-1-6.
//

#include <unp.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <cerrno>
#include <zconf.h>
#include <wait.h>

using namespace std;


static int count;
static void recvfrom_int(int signo){
    cout << "receive " + count << endl;
    exit(0);
}


//void dg_echo(int sockfd, sockaddr *pcliaddr, socklen_t clilen){
//    int n;
//    socklen_t len;
//    char mesg[MAXLINE];
//
//    for ( ; ; ){
//        len = clilen;
//        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
//        sendto(sockfd, mesg, n, 0, pcliaddr, len);
//    }
//}


void dg_echo(int sockfd, sockaddr *pcliaddr, socklen_t clilen){
    socklen_t len;
    char mesg[MAXLINE];

    signal(SIGINT, recvfrom_int);

    for ( ; ; ){
        len = clilen;
        recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
        count++;
    }
}





//int main(int argc, char **argv){
//    int sockfd;
//    struct sockaddr_in servaddr, cliaddr;
//
//    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//    bzero(&servaddr, sizeof(servaddr));
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    servaddr.sin_port = htons(SERVPORT);
//
//    bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
//    dg_echo(sockfd, (sockaddr*)&cliaddr, sizeof(cliaddr));
//}

void str_echo(int sock_fd){
    ssize_t n;
    char buf[MAXLINE];
    bool close = false;

    while (!close) {
        while ((n = read(sock_fd, buf, MAXLINE)) > 0){
            write(sock_fd, buf, (size_t)n);
        }
        if (n < 0 && errno == EINTR){
            continue;
        } else if (n < 0) {
            cout << "read error" << endl;
        }
        close = true;
    }
}


void sig_chld(int signo){
    pid_t pid;
    int stat;

//    pid = wait(&stat);
//    printf("child %d terminated \n", pid);
//    return;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        printf("child %d terminated \n", pid);
    }
    return;
}


//使用select
int main(int argc, char **argv){
    int listenfd, connfd, udpfd, nready, maxfd;
    char mesg[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;

    //创建tcp 监听套接字
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    //
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    //创建udp套接字绑定同个端口
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);
    bind(udpfd, (sockaddr*)&servaddr, sizeof(servaddr));

    //注册信号处理函数
    signal(SIGCHLD, sig_chld);

    //描述符初始化
    FD_ZERO(&rset);
    maxfd = max(listenfd, udpfd) + 1;
    for ( ; ; ){
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        if ((nready = select(maxfd, &rset, NULL, NULL, NULL)) < 0){
            if (errno == EINTR){
                continue;
            } else {
                cout << "select error" << endl;
            }
        }

        //如果监听套接字可读，子进程处理新的连接
        if (FD_ISSET(listenfd, &rset)){
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr*)&cliaddr, &len);
            //子进程处理连接
            if ((childpid = fork()) == 0){
                close(listenfd);
                str_echo(connfd);
                exit(0);
            }
            close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)){
            len = sizeof(cliaddr);
            n = recvfrom(udpfd, mesg, MAXLINE, 0, (sockaddr*)&cliaddr, &len);
            sendto(udpfd, mesg, n, 0, (sockaddr*)&cliaddr, len);
        }
    }
}


