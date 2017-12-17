#include <iostream>
#include <netinet/in.h>
#include "unp.h"
#include <memory.h>
#include <cstdio>
#include <zconf.h>

using namespace std;

int main(int argc, char **argv){
    int listen_fd, conn_fd;
    struct sockaddr_in servaddr;
    string buff;
    time_t ticks;
    //监听套接字
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    //绑定套接字
    sockaddr *servaddr_std = (sockaddr*)(&servaddr);
    bind(listen_fd, servaddr_std, sizeof(servaddr));
    listen(listen_fd, LISTENQ);
    sockaddr *n = 0;

    for (; ; ){
        conn_fd = accept(listen_fd, n, NULL);
        cout << "send" << endl;
        buff = "sdf\r\n";
        write(conn_fd, buff.c_str(), strlen(buff.c_str()));
        close(conn_fd);
    }
}