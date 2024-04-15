#include "head.h"

// 构造函数
Index::Index() {
  
    
}

// 析构函数
Index::~Index() {

}

// 程序开始
void Index::appStart() {
  

}

// 登录
bool Index::login() {
    cout << endl << setw(12) << "主机地址: ";
    cin >> this->hostName;

    cout << endl << setw(12) << "端口号: ";
    cin >> this->portNum;

    cout << endl << setw(12) << "账号: ";
    cin >> this->userName;

    cout << endl << setw(12) << "密码: ";
    cin >> this->userPwd;
    cout << endl << endl;

    // 连接服务器


    // 进入登录
}

//菜单列表
void Index::menuList()
{
    cout << "----------- sindftp ------------" << endl << endl;
    cout << "--------------------------------" << endl << endl;
    cout << "显示菜单\t menu" << endl << endl;
    cout << "切换目录\t cwd dirname" << endl << endl;
    cout << "文件列表\t list dirname" << endl << endl;
    //cout << "文件重命名\t rename filename" << endl << endl;
    cout << "上传文件\t upload filename" << endl << endl;
    cout << "下载文件\t download filename" << endl << endl;
    cout << "删除文件\t del filename" << endl << endl;
    cout << "帮助信息\t help" << endl << endl;
    cout << "清除屏幕\t clear" << endl << endl;
    cout << "退出平台\t quit" << endl << endl;
    cout << "--------------------------------" << endl;
    cout << endl;

    localInput();
}

//本地命令输入处理
void Index::localInput()
{
    requestString = "";
    responseString = "";
    errorMsg = "";

    string userInputCmd = "";
    cout << endl;
    cout << "input cmd ### " ;
    cin.sync();
    getline(cin, userInputCmd);

    if(userInputCmd.length() < 1){
        cout <<  "请输入正确的命令！" << endl;
        localInput();
        return;
    }
    //分割命令与参数
    string cmd = "";
    string param = "";
    bool cmdOver = false;
    for(char ch:userInputCmd){
        if(ch == ' '){
            cmdOver = true;
            continue;
        }

        if(cmdOver){
            param += ch;
        }else{
            cmd += ch;
        }
    }
    //处理命令
    if(cmd == "help"){  //帮助
        requestString = "HELP";
        sendToRemote(true);
    }else if(cmd == "menu"){  //菜单
        menuList();
    }else if(cmd == "cwd"){  //改变目录
        //改变数据目录
        requestString = "CWD "+param;
        sendToRemote(true);
    }else if(cmd == "list"){  //列表
        getList(param);
    }else if(cmd == "rename"){  // 重命名

    }else if(cmd == "upload"){  //上传
        uploadFile(param);
    }else if(cmd == "download"){  //下载
        downloadFile(param);
    }else if(cmd == "del"){  //删除
        deleteFile(param);
    }else if(cmd == "clear"){  //清除屏幕
        system("cls");
        menuList();
    }else if(cmd == "quit"){  //退出
        cout << "Bye bye!" <<endl;
        exit(-1);
    }else{
        //errorMsg = "请输入正确的命令！";
        //error(errorMsg);
        requestString = userInputCmd;
        sendToRemote(true);
    }

    localInput();
}

//远程命令发送
bool Index::sendToRemote(bool returnReplyCode) {
    if(this->requestString.length() < 1){
        this->errorMsg = "ftp命令不能为空";
        return false;
    }
    cout << endl << setw(10) << "发送命令: " << setw(10) << requestString << endl;

    requestString += "\r\n";
    // requestString = this->pCommon->gbkToUtf8(requestString.c_str());
    unsigned int cmdLength = requestString.length();
    char *tmpCmdStr = new char[cmdLength];
    memset(tmpCmdStr, 0, cmdLength);
    memcpy(tmpCmdStr, requestString.c_str(), cmdLength);

    int sendStatus = send(this->ctrlSock, tmpCmdStr, static_cast<int>(cmdLength), 0);
    delete [] tmpCmdStr;
    if(sendStatus == -1){
        this->errorMsg = "请求失败";
        return false;
    }
    if(returnReplyCode){
        return recvFromRemote();
    }
    return true;
}

//接收远程响应
bool Index::recvFromRemote() {
    if(this->ctrlSock == INVALID_SOCKET){
        this->errorMsg = "服务已断开 ";
        return false;
    }
    char buf[MAX_MSG_SIZE];
    int recvStatus = -1;
    responseString = "";

    while(true){
        memset(buf, 0, MAX_MSG_SIZE);
        Sleep(500);
        recvStatus = recv(this->ctrlSock, buf, MAX_MSG_SIZE, MSG_PARTIAL);
        //cout << "######### " << buf << endl;
        if(recvStatus > 0){
            responseString = buf;
            break;
        }
    }
    if(responseString.length() == 0){
        this->errorMsg = "接收数据失败";
        return false;
    }
    this->success(responseString);

    return this->getReplyCode();
}

//处理远程响应，返回响应码
bool Index::getReplyCode() {
    string tmpMsg = "";
    tmpMsg = responseString.substr(0, 3);
    char charCode[3] = {};

    memcpy(charCode, tmpMsg.c_str(), tmpMsg.length());
    this->replyCode = static_cast<unsigned int>(atoi(charCode));
    if( !(this->replyCode >= 1 && this->replyCode <= 1000)){
        this->errorMsg = "无法获取到响应码";
        return false;
    }

    return true;
}

//上传文件
bool Index::uploadFile(string filePath) {
    if(!transModelSelect()){
        return false;
    }
    char *fileName = strrchr(filePath.c_str(), '/');
    requestString = "STOR ";
    requestString += fileName +1;
    if(!sendToRemote(true)){
        errorMsg = "命令发送失败";
        return false;
    }
    if(!allowAccept()){
        return false;
    }

    fstream fs;
    fs.open(filePath, ios::in);
    char buf[MAX_MSG_SIZE];
    memset(buf, 0, MAX_MSG_SIZE);
    bool status = true;
    while(!fs.eof()){
        fs.read(buf, MAX_MSG_SIZE);
        int storStatus = send(dataTransSock, buf, sizeof (buf), 0);
        if(storStatus == -1){
            errorMsg = "上传文件异常";
            status = false;
            break;
        }
    }
    closesocket(dataTransSock);
    if(!status){
        return false;
    }

    return true;
}

//下载文件
bool Index::downloadFile(string fileName) {
    //先获取文件大小
    requestString = "SIZE ";
    requestString += fileName;
    if(!sendToRemote(true)){
        errorMsg = "命令发送失败";
        return false;
    }
    int fileSize = atoi(responseString.substr(4).c_str());

    if(!transModelSelect()){
        return false;
    }
    requestString = "RETR " + fileName;
    if(!sendToRemote(true)){
        errorMsg = "命令发送失败";
        return false;
    }
    if(!allowAccept()){
        errorMsg = "连接失败";
        return false;
    }
    //开始进行下载动作
    string localPath = "d:/test/d_" + fileName;
    fstream fs;
    fs.open(localPath, ios::app);
    int countFileSize = 0;
    char *buf = new char[MAX_MSG_SIZE];
    while (countFileSize < fileSize) {
        memset(buf, 0, MAX_MSG_SIZE);
        recv(dataTransSock, buf, MAX_MSG_SIZE, 0);
        if(strlen(buf) < 1){
            break;
        }
        fs << buf ;
        countFileSize += strlen(buf);
    }
    fs.close();

    return true;
}

//删除文件
bool Index::deleteFile(string fileName) {
    if(!transModelSelect()){
        return false;
    }
    requestString = "DELE " + fileName;
    if(!sendToRemote(true)){
        errorMsg = "命令发送失败";
        return false;
    }


    return true;
}














