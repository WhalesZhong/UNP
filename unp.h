#ifndef C_WEBTCP_H
#define C_WEBTCP_H
#endif

#define LISTENQ 1024
#define MAXLINE 4096
#define SERVPORT 9877

#include <stdio.h>

void sig_chld(int signo);

typedef void Sigfunc(int);

