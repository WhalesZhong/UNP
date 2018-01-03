//
// Created by root on 18-1-2.
//
#include <signal.h>
#include <wait.h>
#include <cstdio>
#include "unp.h"

Sigfunc *signal(int signo, Sigfunc *func){
    struct sigaction act, oact;
    act.sa_handler = func;
    //允许在调用信号处理函数时不阻塞其他信号递交给进程
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (signo == SIGALRM){
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }else{
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0){
        return (SIG_ERR);
    }
    return (oact.sa_handler);
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





