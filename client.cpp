#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define BUF_SIZE 1024
int main(int argc, char *argv[]) {
    int sock, str_len;
    struct sockaddr_in serv_adr; // 32位目的地址和16位端口号分开
    char message[BUF_SIZE];

    // 创建socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        cout << "socket() error" << endl;

    // 服务端ip和端口号设置
    memset(&serv_adr, 0, sizeof(serv_adr)); // 结构体清空
    serv_adr.sin_family = AF_INET;                       // ipv4
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");   // 在本地起一个客户端
    serv_adr.sin_port = htons(atoi("60001"));            // 端口号

    // 客户端发起链接
    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(sockaddr)) == -1)   // 客户端的ip和端口自动分配
        cout << "connect() error!" << endl;
    else
        cout << "connect() success!" << endl;


    while (1) {

        cout << "请输入发送消息：";
        cin >> message;
        if (!strcmp(message, "q\n"))
            break;
        send(sock, message, strlen(message), 0);       // 写
        str_len = recv(sock, message, BUF_SIZE - 1, 0); // 读
        message[str_len] = '\0';
        cout << "Message from server: "<< message << "\"" << endl;
    }
    close(sock);
    return 0;
}

