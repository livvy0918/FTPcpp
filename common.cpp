#include "head.h"

Common::Common()
{

}

//将utf8转换为GBK
string Common::utf8ToGbk(const char *src_str)
{
    // utf8转为宽字符   
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    wstring wide_string = converter.from_bytes(src_str); 

    // 宽字符转为GBK
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string string = converter.to_bytes(wide_string);

    return string;
}

//将gbk编码字符转换为utf8
string Common::gbkToUtf8(const char *src_str)
{


    // int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, nullptr, 0);
    // wchar_t* wstr = new wchar_t[len + 1];
    // memset(wstr, 0, static_cast<unsigned int>(len + 1));
    // MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
    // len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    // char* str = new char[len + 1];
    // memset(str, 0, static_cast<unsigned int>(len + 1));
    // WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, nullptr, nullptr);
    // string strTemp = str;
    // if (wstr) delete[] wstr;
    // if (str) delete[] str;
    // return strTemp;
}