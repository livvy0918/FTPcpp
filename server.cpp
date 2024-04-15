#include "head.h"

#define BUF_SIZE 1024
int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    int str_len, i;
    char message[BUF_SIZE];

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);      // 创建套接字
    if (serv_sock == -1)
        printf("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;                   // ipv4
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);    // 监听任意地址的请求
    serv_adr.sin_port = htons(atoi("60001"));        // 端口号

    // 将服务端的地址和端口绑定到socket
    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) // 分配地址
        printf("bind() error");

    // 开始监听
    if (listen(serv_sock, 5) == -1)     
        printf("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    for (i = 0; i < 5; i++) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);   // 受理连接
        if (clnt_sock == -1)
            printf("accept() error");
        else
          printf("Connect client %d : %s \n", i + 1, inet_ntoa(clnt_adr.sin_addr));

        // 读到消息就处理
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {    
            printf("Message : %s from client %s \n", message, inet_ntoa(clnt_adr.sin_addr));
            write(clnt_sock, message, str_len);   // 把读的发回去
        }
        close(clnt_sock);    // 别忘了关
    }
    close(serv_sock);
    return 0;   
}
