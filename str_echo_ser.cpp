#include "unp.h"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>


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

    int listen_fd, conn_fd;
    pid_t child_pid;
    socklen_t cli_len;
    struct sockaddr_in cli_address, server_address;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;

    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVPORT);

    bind(listen_fd, (sockaddr*)&server_address, sizeof(server_address));
    listen(listen_fd, LISTENQ);

    for ( ; ; ){
        cli_len = sizeof(cli_address);
        conn_fd = accept(listen_fd, (sockaddr*)&cli_address, &cli_len);
        //fork在子进程返回0而不是父进程id, 代码分别在两个进程继续进行
        if ((child_pid = fork()) == 0){
            close(listen_fd);
            cout << "child pid: " << child_pid << endl;
            str_echo(conn_fd);
            exit(0);
        }
        close(conn_fd);
    }
}