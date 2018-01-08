//
// Created by root on 18-1-8.
//

#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <sys/socket.h>
#include "unp.h"

ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd){
    struct msghdr msg;
    struct iovec iov[1];

    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    //创建辅助数据结构
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);

    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;

    //将发送的描述符转换并赋给cmsg_data成员
    *(int *) CMSG_DATA(cmptr) = sendfd;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //发送,描述符处于飞行中状态时,即使退出关闭描述符,在接收端也能够接收这个描述符
    //因为实际涉及的是在接收进程创建一个新的描述符,而这个描述符和发送进程中的飞行前的描述符指向内核中相同的文件表项
    return (sendmsg(fd, &msg, 0));
}

int main(int argc, char **argv){
    int fd;
    if (argc != 4){
        printf("openfile <sockfd> <filename> <mode>");
    }
    if ((fd = open(argv[2], atoi(argv[3]))) < 0){
        exit((errno > 0) ? errno:255);
    }
    if (write_fd(atoi(argv[1]), NULL, 1, fd) < 0){
        exit((errno > 0) ? errno : 255);
    }
}