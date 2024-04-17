using namespace std;

// common.cpp
#include <locale.h>
#include <codecvt>
#include <locale>

using namespace std;

class Common{
public:
    Common();
    ~Common();

    //将utf8转换为GBK,windows引入windows.h
    string utf8ToGbk(const char *src_str);
    //将gbk编码字符转换为utf8
    string gbkToUtf8(const char *src_str);
};

// ftpmain.cpp
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>

#define MAX_MSG_SIZE 1024
#define BUF_SIZE 1024

class Index {
public:
    Index();
    ~Index();
    void appStart();

  private:
    Common *pCommon;
    
    // 主机名
    string hostName = "";
    // 端口号
    int portNum = 0;
    // 用户名
    string userName = "";
    // 密码
    string userPwd = "";

    // 命令字符串
    string requestString = "";
    // 接收的响应字符串
    string responseString = "";
    // 响应码
    unsigned int replyCode = 0;
    // 错误原因字符串
    string errorMsg;

    // socket文件描述符
    int ctrlSock;
    

    bool login();
    void menuList();
    void localInput();
    bool sendToRemote(bool returnReplyCode);
    bool recvFromRemote();
    bool getReplyCode();
    void error(string errorStr);
    void success(string sucStr);
    bool uploadFile(string filePath);
    bool downloadFile(string fileName);
    bool deleteFile(string fileName);
    
};



