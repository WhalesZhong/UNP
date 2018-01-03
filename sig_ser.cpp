//
// Created by root on 18-1-3.
//
#include <unp.h>
#include <netinet/in.h>
#include <algorithm>
#include <memory.h>
#include <csignal>
#include <cerrno>
#include <iostream>
#include <zconf.h>

using namespace std;

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

int main(int argc, char **argv){
    int listenfd, connfd;
    pid_t child_pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;

    void sig_chld(int);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVPORT);

    bind(listenfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, LISTENQ);

    signal(SIGCHLD, sig_chld);

    for ( ; ; ){
        cli_len = sizeof(cli_addr);
        if ((connfd = accept(listenfd, (sockaddr*)&cli_addr, &cli_len)) < 0){
            if (errno == EINTR){
                continue;
            }else{
                cout << "accept error" << endl;
            }
        }

        if ((child_pid = fork()) == 0){
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
}



