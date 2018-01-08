//
// Created by root on 18-1-8.
//
#include <unp.h>
#include <sys/socket.h>
#include <zconf.h>
#include <wait.h>
#include <cerrno>
#include <fcntl.h>
#include <cstdlib>


ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd){
    //用于发送接收传递的结构
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;
    union{
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //接收辅助数据
    if ((n = recvmsg(fd, &msg, 0)) <= 0){
        return (n);
    }
    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL && cmptr->cmsg_len == CMSG_LEN(sizeof(int))){
        if (cmptr->cmsg_level != SOL_SOCKET){
            printf("control level != sol_socket");
        }
        if (cmptr->cmsg_type != SCM_RIGHTS){
            printf("control type != scm_rights");
        }
        //获取传递过来的描述符
        *recvfd = *((int *)CMSG_DATA(cmptr));
    } else {
        *recvfd = -1;
    }
}

int my_open(const char *pathname, int mode){
    int fd, sockfd[2], status;
    pid_t childpid;
    char c, argssockfd[10], argmode[10];
    //创建一个流管道,返回两个描述符到sockfd数组
    socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

    if ((childpid == fork()) == 0){
        //子进程关闭流管道的一端,
        close(sockfd[0]);
        snprintf(argssockfd, sizeof(argssockfd), "%d",sockfd[1]);
        snprintf(argmode, sizeof(argmode), "%d",mode);
        //调用程序打开文件并发送描述符,函数不会返回,除非发生错误
        execl("./openfile", argssockfd, pathname, argmode, (char*)NULL);
        printf("execl error");
    }
    //父进程关闭流管道的另一端,并等待子进程终止,子进程的终止状态在status
    close(sockfd[1]);
    waitpid(childpid, &status, 0);
    if (WIFEXITED(status) == 0){
        printf("%s", "child did not terminated");
    }
    //如果子进程正常终止,把终止状态转换成退出状态
    if ((status = WEXITSTATUS(status)) == 0){
        //通过流管道接收描述符,第4个参数用于返回收取的描述符
        read_fd(sockfd[0], &c, 1, &fd);
    } else {
        errno = status;
        fd = -1;
    }
    close(sockfd[0]);
    return (fd);
}


int main(int argc, char **argv){
    int fd, n;
    char buff[BUFFSIZE];

    if (argc != 2){
        printf("usage mycat <pathname>");
    }
    if ((fd = my_open(argv[1], O_RDONLY)) < 0){
        printf("can not open %s", argv[1]);
    }

    //从套接字读取输出到标准输出
    while ((n = read(fd, buff, BUFFSIZE)) > 0){
        write(STDOUT_FILENO, buff, n);
    }
    exit(0);
}

