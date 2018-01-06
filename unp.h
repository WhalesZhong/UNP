#ifndef C_WEBTCP_H
#define C_WEBTCP_H
#endif

#define LISTENQ 1024
#define MAXLINE 4096
#define SERVPORT 9877
#define BUFFSIZE 8192
#define SCTP_MAXLINE 800

#include <stdio.h>

void sig_chld(int signo);

typedef void Sigfunc(int);

