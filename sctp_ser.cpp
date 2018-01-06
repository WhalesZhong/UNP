//
// Created by root on 18-1-6.
//


#include "unp.h"
//#include <netinet/sctp.h>
#include <memory.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <cstdlib>

//一到多式流分回射服务器程序

int main(int argc, char **argv){
    int sockfd, msg_flags;
    char readbuf[BUFSIZ];
    struct sockaddr_in servaddr, cliaddr;

    struct sctp_sndrcvinfo sri;
    struct sctp_event_subscribe events;
    int stream_increment = 1;
    socklen_t len;
    size_t rd_sz;

    if (argc == 2){
        stream_increment = atoi(argv[1]);
    }
    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    bzero(&events, sizeof(events));
    //开启为１，则与消息相关的细节信息则会填充到sctp_sndrcvinfo
    events.sctp_data_io_event = 1;
    //设置描述符
    setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));

    listen(sockfd, LISTENQ);

    for ( ; ; ){
        len = sizeof(struct sockaddr_in);
        rd_sz = sctp_recvmsg(sockfd, readbuf, sizeof(readbuf), (sockaddr*)&cliaddr, &len, &sri, &msg_flags);

        if (stream_increment){
            sri.sinfo_stream++;
            //如果流号大于最大则设置为０
            if (sri.sinfo_stream >= 10){
                sri.sinfo_stream = 0;
            }
        }
        sctp_sendmsg(sockfd, readbuf, rd_sz, (sockaddr*)&cliaddr, len, sri.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0);
    }
}