//
// Created by root on 18-1-14.
//
#include <unp.h>
#include <zconf.h>
#include <fcntl.h>
#include <algorithm>
#include <cerrno>
#include <sys/socket.h>

using namespace std;

void str_cli(FILE *fp, int sockfd){
    int maxfd, val, stdineof;
    ssize_t n, nwritten;
    fd_set rset, wset;
    char to[MAXLINE], fr[MAXLINE];
    char *toiptr, *tooptr, *friptr, *froptr;

    //将连接服务器套接字,标准输入,标准输出三个描述符设置为非阻塞
    val = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDOUT_FILENO, F_GETFL, 0);
    fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to;
    friptr = froptr = fr;
    stdineof = 0;

    maxfd = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;

    for ( ; ; ){
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        //如果套接字缓冲区还有空间,则继续读取标准输入
        if (stdineof == 0 && toiptr < &to[MAXLINE]){
            FD_SET(STDIN_FILENO, &rset);
        }
        if (friptr < &fr[MAXLINE]){
            FD_SET(sockfd, &rset);
        }
        if (tooptr != toiptr){
            FD_SET(sockfd, &wset);
        }
        if (froptr != friptr){
            FD_SET(STDIN_FILENO, &wset);
        }

        select(maxfd, &rset, &wset, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &rset)){
            if ((n = read(STDIN_FILENO, toiptr, &to[MAXLINE] - toiptr)) < 0){
                if (errno != EWOULDBLOCK){
                    printf("read error on stdin");
                }
            } else if (n == 0){
                fprintf(stderr, "eof on stdin");
                stdineof = 1;
                //标准输入eof后,如果缓冲区没有数据发送,调用shutdown 发送FIN 到服务器
                if (tooptr == toiptr){
                    shutdown(sockfd, SHUT_WR);
                }
            } else {
                fprintf(stderr, "read %d bytes from stdin", n);
                toiptr += n;
                FD_SET(sockfd, &wset);
            }
        }

        if (FD_ISSET(sockfd, &rset)){
            if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0){
                printf("read error on sockfd");
            } else if (n == 0){
                fprintf(stderr, "eof on socket");
                if (stdineof){
                    return;
                } else {
                    printf("server terminated prematurely");
                }
            } else {
                fprintf(stderr, "read %d bytes from socket", n);
                friptr += n;
                FD_SET(STDOUT_FILENO, &wset);
            }
        }

        if (FD_ISSET(STDOUT_FILENO, &wset) && (n = friptr - froptr) > 0){
            if ((nwritten = write(STDOUT_FILENO, froptr, n)) < 0){
                if (errno != EWOULDBLOCK){
                    printf("write error to stdout");
                } else {
                    fprintf(stderr, "write %d bytes to stdout", nwritten);
                    froptr += nwritten;
                    if (froptr == friptr){
                        froptr = friptr = fr;
                    }
                }
            }
        }

        if (FD_ISSET(sockfd, &wset) && (n = toiptr - tooptr) > 0){
            if ((nwritten = write(sockfd, tooptr, n)) < 0){
                if (errno != EWOULDBLOCK){
                    printf("write error to socket");
                }
            } else {
                fprintf(stderr, "wrote %d bytes to socket", nwritten);
                tooptr += nwritten;
                if (tooptr == toiptr){
                    toiptr = tooptr = to;
                    if (stdineof){
                        shutdown(sockfd, SHUT_WR);
                    }
                }
            }
        }
    }
}


//int connect_nonb(int sockfd, const sockaddr* saptr, socklen_t salen, int nsec){
//    int flags, n, error;
//    socklen_t len;
//    fd_set rset, wset;
//    struct timeval tval;
//
//    flags = fcntl(sockfd, F_GETFL, 0);
//    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
//
//    error = 0;
//    //connect 非阻塞套接字
//    if ((n = connect(sockfd, saptr, salen)) < 0){
//        if (error != EINPROGRESS){
//            return (-1);
//        }
//    }
//
//    //等待连接完成期间做其他事情
//    //...
//
//    //连接建立,当服务器在客户所在主机时发生
//    if (n == 0){
//        fcntl(sockfd, F_SETFL, flags);
//        if (error){
//            close(sockfd);
//            errno = error;
//            return (-1);
//        }
//    }
//
//    FD_ZERO(&rset);
//    FD_SET(sockfd, &rset);
//    wset = rset;
//    tval.tv_sec = nsec;
//    tval.tv_usec = 0;
//
//    //select 返回0表示超时
//    if ((n = select(sockfd + 1, &rset, &wset, NULL, nsec ? &tval : NULL)) == 0){
//        close(sockfd);
//        errno = ETIMEDOUT;
//        return (-1);
//    }
//
//    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)){
//        len = sizeof(error);
//        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
//            return (-1);
//        }
//    } else {
//        printf("select error");
//    }
//
//    //恢复套接字文件状态标志
//    fcntl(sockfd, F_SETFL, flags);
//    if (error){
//        close(sockfd);
//        errno = error;
//        return (-1);
//    }
//
//}
