//
// Created by root on 18-1-3.
//
#include <zconf.h>
#include <netinet/in.h>
#include <memory.h>
#include <algorithm>
#include <iostream>
#include "unp.h"


using namespace std;

int main(int argc, char **argv){
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++){
        client[i] = -1;
    }
    FD_ZERO(&allset);
    //监测监听套接字
    FD_SET(listenfd, &allset);

    for ( ; ; ){
        rset = allset;
        //获取每一次就绪的描述符
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        //监测监听套接字是否就绪,如果就绪将accept建立的连接套接字加入描述符集中
        if (FD_ISSET(listenfd, &rset)){
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr*)&cliaddr, &clilen);

            for (i = 0; i < FD_SETSIZE; i++){
                if (client[i] < 0){
                    client[i] = connfd;
                    break;
                }
            }

            if (i == FD_SETSIZE){
                cout << "too many client" << endl;
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd){
                maxfd = connfd;
            }
            if (i > maxi){
                maxi = i;
            }
            if (--nready <= 0){
                continue;
            }
        }

        //除了监测监听套接字,还要遍历已连接的描述符数组,检查是否就绪,如果就绪处理数据
        for (i = 0; i <= maxi; i++){
            if ((sockfd = client[i]) < 0){
                continue;
            }
            if (FD_ISSET(sockfd, &rset)){
                if ((n = read(sockfd, buf, MAXLINE)) == 0){
                    close(connfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    write(sockfd, buf, n);
                }

                if (--nready <= 0){
                    break;
                }
            }
        }
    }




}
