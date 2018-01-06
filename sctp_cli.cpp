//
// Created by root on 18-1-6.
//

#include "unp.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/sctp.h>
#include <iostream>
#include <memory.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <cstdlib>

using namespace std;


void sctpstr_cli(FILE *fp, int sockfd, struct sockaddr *to, socklen_t tolen){
    struct sockaddr_in peeraddr;
    struct sctp_sndrcvinfo sri;
    char sendline[MAXLINE], recvline[MAXLINE];
    socklen_t len;
    int out_sz, rd_sz;
    int msg_flags;

    bzero(&sri, sizeof(sri));
    while (fgets(sendline, MAXLINE, fp) != NULL){
        if (sendline[0] != '['){
            cout << "line must be of form '[streamnum]'" << endl;
        }
        sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
        out_sz = strlen(sendline);
        sctp_sendmsg(sockfd, sendline, out_sz, to, tolen, 0, 0, sri.sinfo_stream, 0, 0);
        len = sizeof(peeraddr);
        rd_sz = sctp_recvmsg(sockfd, recvline, sizeof(recvline), (sockaddr*)&peeraddr, &len, &sri, &msg_flags);
        cout << sri.sinfo_stream << endl;
        printf("%.*s", rd_sz, recvline);
    }
}

void sctpstr_cli_all(FILE *fp, int sockfd, struct sockaddr *to, socklen_t tolen){
    struct sockaddr_in perraddr;
    struct sctp_sndrcvinfo sri;
    char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
    socklen_t len;
    int rd_sz, i, strsz;
    int msg_flags;

    bzero(sendline, sizeof(sendline));
    bzero(&sri, sizeof(sri));

    while (fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL){
        strsz = strlen(sendline);
        if (sendline[strsz-1] == '\n'){
            sendline[strsz-1] = '\0';
            strsz--;
        }
        for (i = 0; i < 10; i++) {
            snprintf(sendline + strsz, sizeof(sendline) - strsz, ".msg.%d", i);
            sctp_sendmsg(sockfd, sendline, sizeof(sendline), to, tolen, 0, 0, i, 0, 0);
        }

        for (i = 0; i < 10; i++){
            len = sizeof(perraddr);
            rd_sz = sctp_recvmsg(sockfd, recvline, sizeof(recvline), (sockaddr*)&perraddr, &len, &sri, &msg_flags);
            printf("from str:%d seq:%d (assoc:0x%x\n", sri.sinfo_stream, sri.sinfo_ssn, (u_int)sri.sinfo_assoc_id);
            printf("%.*s\n", rd_sz, recvline);
        }
    }

}


int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in servaddr;
    struct sctp_event_subscribe events;
    int echo_to_all = 0;

    if (argc < 2){
        cout << "missing host" << endl;
    }

    if (argc > 2){
        cout << "echo message to all" << endl;
        echo_to_all = 1;
    }

    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);

    //设置服务器地址
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    bzero(&events, sizeof(events));
    events.sctp_data_io_event = 1;
    setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof(events));

    if (echo_to_all == 0){
        sctpstr_cli(stdin, sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    } else {
        sctpstr_cli_all(stdin, sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    }
    close(sockfd);
    return (0);
}

