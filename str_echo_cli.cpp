#include <unp.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <arpa/inet.h>

using namespace std;


void str_cli(FILE *fp, int sock_fd){
    char send_line[MAXLINE], recieve_line[MAXLINE];
    while (fgets(send_line, MAXLINE, fp) != NULL){
        write(sock_fd, send_line, strlen(send_line));
        if (read(sock_fd, recieve_line, MAXLINE) == 0){
            cout << "str_cli: read error" << endl;
        }
        fputs(recieve_line, stdout);
    }
}


int main(int argc, char **argv){
    int sock_fd;
    struct sockaddr_in server_address;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVPORT);
    //转换ip地址为网络格式并填入套接字结构
    inet_pton(AF_INET, argv[1], &server_address.sin_addr);
    connect(sock_fd, (sockaddr*)&server_address, sizeof(server_address));
    str_cli(stdin, sock_fd);
    exit(0);
}
