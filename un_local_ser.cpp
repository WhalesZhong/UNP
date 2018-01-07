//
// Created by root on 18-1-7.
//
#include <unp.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <iostream>
#include <zconf.h>
#include <cstdlib>
#include <cerrno>
#include <netinet/in.h>

using namespace std;

//int main(int argc, char **argv){
//    int sockfd;
//    socklen_t len;
//    struct sockaddr_un addr1, addr2;
//
//    if (argc != 2){
//        cout << "unixbind <pathname>" << endl;
//    }
//
//    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
//    unlink(argv[1]);
//
//    bzero(&addr1, sizeof(addr1));
//    addr1.sun_family = AF_LOCAL;
//    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
//    bind(sockfd, (sockaddr*)&addr1, SUN_LEN(&addr1));
//
//    len = sizeof(addr2);
//    getsockname(sockfd, (sockaddr*)&addr2, &len);
//    printf("bound name = %s, return len= %d\n", addr2.sun_path, len);
//    exit(0);
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


int main(int argc, char **argv){

    int listen_fd, conn_fd;
    pid_t child_pid;
    socklen_t cli_len;
    struct sockaddr_un cli_address, server_address;
    
    
    listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink("/tmp/un");
    bzero(&server_address, sizeof(server_address));
    server_address.sun_family = AF_LOCAL;
    strcpy(server_address.sun_path, "/tmp/un");
    
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