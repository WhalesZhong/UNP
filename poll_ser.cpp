//
// Created by root on 18-1-6.
//
#include <bits/local_lim.h>
#include "unp.h"
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <poll.h>
#include <algorithm>
#include <iostream>
#include <zconf.h>
#include <cerrno>


using namespace std;


int main(int argc, char **argv){
    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    int open_max = 10;

    struct pollfd client[open_max];
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;

    for (i = 1; i < open_max; i++){
        client[i].fd = -1;
    }
    maxi = 0;

    for ( ; ; ){
        nready = poll(client, maxi + 1, 9999);
        if (client[0].revents & POLLRDNORM){
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr*)&cliaddr, &clilen);

            for (i = 1; i < open_max; i++){
                if (client[i].fd < 0){
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == open_max){
                cout << "too many client" << endl;
            }
            client[i].events = POLLRDNORM;

            if (i > maxi){
                maxi = i;
            }
            if (--nready <= 0){
                continue;
            }
        }

        for (i = 1; i <= maxi; i++){
            if ((sockfd = client[i].fd) < 0){
                continue;
            }
            if (client[i].revents & (POLLRDNORM | POLLERR)){
                if ((n = read(sockfd, buf, MAXLINE)) < 0){
                    if (errno == ECONNRESET){
                        close(sockfd);
                        client[i].fd = -1;
                    }
                } else if (n == 0){
                    close(sockfd);
                    client[i].fd = -1;
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


