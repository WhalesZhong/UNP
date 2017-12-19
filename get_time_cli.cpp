#include <iostream>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "unp.h"

using namespace std;


int get_server_time(string ip){
    int sock_fd;
    ssize_t n;
    char receive_line[MAXLINE];
    struct sockaddr_in servaddr;
    //创建网际字节流套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0){
        cout << "socket error" << endl;
        return -1;
    }
    //初始化,填写套接字的配置
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    //将ip地址转换为正确的格式赋值给套接字配置
    if (inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr) <= 0){
        cout << "ip invalid" << endl;
        return -1;
    }

    sockaddr *servaddr_std = (sockaddr*)(&servaddr);
    if (connect(sock_fd, servaddr_std, sizeof(servaddr)) < 0){
        cout << "connect error" << endl;
    }

    while ((n = read(sock_fd, receive_line, MAXLINE)) > 0){
        cout << receive_line << endl;
    }
    if (n < 0){
        cout << "read error" << endl;
    }
    return 0;
}


int main(int argc, char **argv){
    string ip = "192.168.2.219";
    get_server_time(ip);
}
