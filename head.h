using namespace std;
// client.cpp \ server.cpp

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

// ftpmain.cpp
#include <iomanip>

#define MAX_MSG_SIZE 1024

class Index {
public:
    Index();
    ~Index();
    void appStart();

  private:
    
    //主机名
    string hostName = "";
    //端口号
    int portNum = 0;
    //用户名
    string userName = "";
    //密码
    string userPwd = "";

    //命令字符串
    string requestString = "";
    //接收的响应字符串
    string responseString = "";
    

    bool login();
    void menuList();
    void localInput();
    bool sendToRemote(bool returnReplyCode);
    bool recvFromRemote();
    bool getReplyCode();
    bool uploadFile(string filePath);
    bool downloadFile(string fileName);
    bool deleteFile(string fileName);
    
};


