#include "HttpRequest.h"
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<assert.h>
#include<ctype.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
//struct HttpRequest* HttpRequestInit()
//{
//    struct HttpRequest* req = (struct HttpRequest*)malloc(sizeof(struct HttpRequest));
//    httpRequestReset(req);
//    req->headers = (struct HttpHeader*)malloc(sizeof(struct HttpHeader) * req->reqHeadersNum);
//    return req;
//}

//void httpRequestReset(struct HttpRequest* req)
//{
//    req->curState = ParseReqLine;
//    req->method = NULL;
//    req->url = NULL;
//    req->version = NULL;
//    req->reqHeadersNum = 0;
//}

//void httpRequestResetEx(struct HttpRequest* req)
//{
//    free(req->url);
//    free(req->method);
//    free(req->version);
//    if (req->headers != NULL) {
//        for (int i = 0;i < req->reqHeadersNum;++i) {
//            free(req->headers[i].key);
//            free(req->headers[i].value);
//        }
//        free(req->headers);
//    }
//    httpRequestReset(req);
//}

//void httpRequestDestroy(struct HttpRequest* req)
//{
//    if (req != NULL) {
//        httpRequestResetEx(req);
//        free(req);
//    }
//}
//
//enum HttpReqStatus getStatus(HttpRequest* req)
//{
//    return req->curState;
//}

//void httpRequestAddHeader(HttpRequest* req, const char* key, const char* value)
//{
//    req->headers[req->reqHeadersNum].key = (char*)key;
//    req->headers[req->reqHeadersNum].value = (char*)value;
//    req->reqHeadersNum++;
//}

//char* httpRequestGetHeader(HttpRequest* req, const char* key)
//{
//    if (req != NULL) {
//        for (int i = 0;i < req->reqHeadersNum;++i) {
//            if (strncasecmp(req->headers[i].key, key, strlen(key)) == 0) {
//                return req->headers[i].value;
//            }
//        }
//    }
//    return NULL;
//}

//char* splitRequestLine(const char* start, char* end, const char* sub, char** ptr)
//{
//    char* space = end;
//    if (sub != NULL)
//    {
//        space = (char*)memmem(start, end - start, sub, strlen(sub));
//        assert(space != NULL);
//    }
//    int length = space - start;
//    char* tmp = (char*)malloc(length + 1);
//    strncpy(tmp, start, length);
//    tmp[length] = '\0';
//    *ptr = tmp;
//    return space + 1;
//}

//bool parseHttpLine(struct HttpRequest* req, struct Buffer* buf)
//{
//    char* end = buf->bufferFindCRLF();
//    char* start = buf->data + buf->readPos;
//    int lineSize = end - start;
//    if (lineSize) {
//        start = splitRequestLine(start, end, " ", &req->method);
//        start = splitRequestLine(start, end, " ", &req->url);
//        splitRequestLine(start, end, " ", &req->version);
//        buf->readPos += lineSize;
//        buf->readPos += 2; //挪到headers的头部
//        req->curState = ParseReqHeaders;
//        return true;
//    }
//    return false;
//}

//bool parseHttpHeader(struct HttpRequest* req,struct Buffer* buf)
//{
//    char* end = bufferFindCRLF(buf);
//    if (end != NULL) {
//        char* start = buf->data + buf->readPos;
//        int lineSize = end - start;
//        char* middle = (char*)memmem(start, lineSize, ": ", 2);
//        if (middle != NULL) {
//            char* key = (char*)malloc(middle - start + 1);
//            strncpy(key, start, middle - start);
//            key[middle - start] = '\0';
//            char* value = (char*)malloc(end - middle - 2 + 1);
//            strncpy(value, middle + 2, end - middle - 2);
//            value[end - middle - 2] = '\0';
//            httpRequestAddHeader(req, key, value);
//            // 移动读数据的位置
//            buf->readPos += lineSize;
//            buf->readPos += 2;
//        }
//        else {
//            //解析完了
//            //如果是post这里修改！ TODO
//            buf->readPos += 2;
//            req->curState = ParseReqDone;
//        }
//        return true;
//    }
//    return false;
//}

//bool parseHttpRequest(struct HttpRequest* req, struct Buffer* writeBuf, struct Buffer* readBuf, struct HttpResponse* rsp,int socket)
//{
//    bool flag = true;
//    while (req->curState != ParseReqDone) {
//        switch (req->curState)
//        {
//        case ParseReqLine:
//           flag = parseHttpLine(req, readBuf);
//            break;
//        case ParseReqHeaders:
//           flag = parseHttpLine(req, readBuf);
//            break;
//        case ParseReqBody: //添加post处理函数
//            break;
//        default:
//            break;
//        }
//        if (!flag) {
//            return flag;
//        }
//        if (req->curState == ParseReqDone) {
//            flag = processHttpGetRequest(req, rsp);
//            httpResponsePrepareMsg(rsp, writeBuf,socket);
//        }
//    }
//    req->curState = ParseReqLine;
//    return flag;
//}
//bool processHttpGetRequest(HttpRequest* req, HttpResponse* rsp)
//{
//    //判断请求类型
//    if (strcasecmp("get", req->method) != 0) {
//        return -1;
//    }
//    //处理特殊字符
//    decodeMsg(req->url, req->url);
//    char* file = NULL;
//    if (strcmp(req->url, "/") == 0) {
//        file = "./";
//    }
//    else {
//        file = req->url + 1;
//    }
//    //获取文件属性
//    struct stat st;
//    int ret = stat(file, &st);
//    if (ret == -1) {
//        //文件不存在就返回404
//        strcpy(rsp->fileName, "404.html");
//        strcpy(rsp->statusMsg, "Not Found");
//        rsp->status = NotFound;
//        httpResponseAddHeader(rsp, "Content-type", getFileType(".html"));
//        rsp->sendDataFunc = sendFile;
//        return 0;
//    }
//    
//}
//const char* getFileType(const char* name)
//{
//    // a.jpg a.mp4 a.html
//    // 自右向左查找‘.’字符, 如不存在返回NULL
//    const char* dot = strrchr(name, '.');
//    if (dot == NULL)
//        return "text/plain; charset=utf-8";	// 纯文本
//    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
//        return "text/html; charset=utf-8";
//    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
//        return "image/jpeg";
//    if (strcmp(dot, ".gif") == 0)
//        return "image/gif";
//    if (strcmp(dot, ".png") == 0)
//        return "image/png";
//    if (strcmp(dot, ".css") == 0)
//        return "text/css";
//    if (strcmp(dot, ".au") == 0)
//        return "audio/basic";
//    if (strcmp(dot, ".wav") == 0)
//        return "audio/wav";
//    if (strcmp(dot, ".avi") == 0)
//        return "video/x-msvideo";
//    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
//        return "video/quicktime";
//    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
//        return "video/mpeg";
//    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
//        return "model/vrml";
//    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
//        return "audio/midi";
//    if (strcmp(dot, ".mp3") == 0)
//        return "audio/mpeg";
//    if (strcmp(dot, ".ogg") == 0)
//        return "application/ogg";
//    if (strcmp(dot, ".pac") == 0)
//        return "application/x-ns-proxy-autoconfig";
//
//    return "text/plain; charset=utf-8";
//}
//void decodeMsg(char* to, char* from)
//{
//    for (; *from != '\0'; ++to, ++from)
//    {
//        // isxdigit -> 判断字符是不是16进制格式, 取值在 0-f
//        // Linux%E5%86%85%E6%A0%B8.jpg
//        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
//        {
//            // 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
//            // B2 == 178
//            // 将3个字符, 变成了一个字符, 这个字符就是原始数据
//            *to = hexToDec(from[1]) * 16 + hexToDec(from[2]);
//
//            // 跳过 from[1] 和 from[2] 因此在当前循环中已经处理过了
//            from += 2;
//        }
//        else
//        {
//            // 字符拷贝, 赋值
//            *to = *from;
//        }
//
//    }
//    *to = '\0';
//}

// 将字符转换为整形数
//int hexToDec(char c)
//{
//    if (c >= '0' && c <= '9')
//        return c - '0';
//    if (c >= 'a' && c <= 'f')
//        return c - 'a' + 10;
//    if (c >= 'A' && c <= 'F')
//        return c - 'A' + 10;
//
//    return 0;
//}

HttpRequest::HttpRequest()
{
    reset();
}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::reset()
{
    m_curState = HttpReqStatus::ParseReqLine;
    m_method = m_url = m_version = string();
    m_headers.clear();
}

void HttpRequest::addHeader(const string key, const string value)
{
    if (key.empty() || value.empty())
    {
        return;
    }
    m_headers.insert(make_pair(key, value));
}

string HttpRequest::getHeader(const string key)
{
    auto item = m_headers.find(key);
    if (item == m_headers.end()) {
        return string();
    }
    return item->second;
}

bool HttpRequest::parseHttpLine(Buffer* buf)
{
    char* end = buf->bufferFindCRLF();
    char* start = buf->readData();
    int lineSize = end - start;
    if (lineSize) {
        auto methodFunc = bind(&HttpRequest::setMethod, this, placeholders::_1);
        start = splitRequestLine(start, end, " ", methodFunc);
        auto urlFunc = bind(&HttpRequest::seturl, this, placeholders::_1);
        start = splitRequestLine(start, end, " ", urlFunc);
        auto versionFunc = bind(&HttpRequest::setVersion, this, placeholders::_1);
        splitRequestLine(start, end, nullptr, versionFunc);
        buf->readPosIncreasement(lineSize + 2);
        setState(HttpReqStatus::ParseReqHeaders);
        return true;
    }
    return false;
}

bool HttpRequest::parseHttpHeader(Buffer* buf)
{
    char* end = buf->bufferFindCRLF();
    if (end != nullptr) {
        char* start = buf->readData();
        int lineSize = end - start;
        char* middle = (char*)memmem(start, lineSize, ": ", 2);
        if (middle != nullptr) {
            int keyLen = middle - start;
            int valueLen = end - middle - 2;
            if(keyLen>0&&valueLen>0){
                string key(start, keyLen);
                string value(middle + 2, valueLen);
                addHeader(key, value);
            }
            buf->readPosIncreasement(lineSize + 2);
        }
        else {
            //解析完了
            //如果是post这里修改！ TODO
            buf->readPosIncreasement(2);//跳过空行
            setState(HttpReqStatus::ParseReqDone);
        }
        return true;
    }
    return false;
}

bool HttpRequest::parseHttpRequest(Buffer* writeBuf, Buffer* readBuf, HttpResponse* rsp, int socket)
{
    bool flag = true;
    while (m_curState != HttpReqStatus::ParseReqDone) {
        switch (m_curState)
        {
        case HttpReqStatus::ParseReqLine:
            flag = parseHttpLine(readBuf);
            break;
        case HttpReqStatus::ParseReqHeaders:
            flag = parseHttpHeader(readBuf);
            break;
        case HttpReqStatus::ParseReqBody: //添加post处理函数
            break;
        default:
            break;
        }
        if (!flag) {
            return flag;
        }
        if (m_curState == HttpReqStatus::ParseReqDone) {
            flag = processHttpGetRequest(rsp);
            rsp->prepareMsg(writeBuf, socket);
        }
    }
    m_curState = HttpReqStatus::ParseReqLine;
    return flag;
}

char* HttpRequest::splitRequestLine(const char* start,const char* end, const char* sub, function<void(string)> callback)
{
    char* space = const_cast<char*>(end);
    if (sub != nullptr) {
        space = static_cast<char*>(memmem(start, end - start, sub, strlen(sub)));
        assert(space != nullptr);
    }
    int length = space - start;
    callback(string(start, length));
    return space + 1;
}

string HttpRequest::decodeMsg(string msg)
{
    const char* from = msg.data();
    string str = string();
    for (; *from != '\0'; ++from)
    {
        // isxdigit -> 判断字符是不是16进制格式, 取值在 0-f
        // Linux%E5%86%85%E6%A0%B8.jpg
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
        {
            // 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
            // B2 == 178
            // 将3个字符, 变成了一个字符, 这个字符就是原始数据
            str.append(1,hexToDec(from[1]) * 16 + hexToDec(from[2]));

            // 跳过 from[1] 和 from[2] 因此在当前循环中已经处理过了
            from += 2;
        }
        else
        {
            // 字符拷贝, 赋值
            str.append(1, *from);
        }

    }
    str.append(1, '\0');
    return str;
}

bool HttpRequest::processHttpGetRequest(HttpResponse* rsp)
{
    if (strcasecmp("get", m_method.data()) != 0) {
        return false;
    }
    //处理特殊字符
    m_url = decodeMsg(m_url);
    const char* file = NULL;
    if (strcmp(m_url.data(), "/") == 0) {
        file = "./";
    }
    else {
        file = m_url.data() + 1;
    }
    //获取文件属性
    struct stat st;
    int ret = stat(file, &st);
    if (ret == -1) {
        //文件不存在就返回404
        rsp->setFilename("404.html");
        rsp->setStatusCode(HttpStatusCode::NotFound);
        rsp->addHeader("Content-type", getFileType(".html"));
        rsp->sendDataFunc = sendFile;
        return true;
    }
    rsp->setFilename(file);
    rsp->setStatusCode(HttpStatusCode::OK);
    if (S_ISDIR(st.st_mode)) {
        rsp->addHeader("Content-type", getFileType(".html"));
        rsp->sendDataFunc = sendDir;
    }
    else {
        rsp->addHeader("Content-type", getFileType("file"));
        rsp->sendDataFunc = sendFile;
    }
    return true;
}

const string HttpRequest::getFileType(const string name)
{
    const char* dot = strrchr(name.data(), '.');
    if (dot == NULL)
        return "text/plain; charset=utf-8";	// 纯文本
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp(dot, ".wav") == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

void HttpRequest::sendFile(string fileName, Buffer* sendBuf, int socket)
{
    int fd = open(fileName.data(), O_RDONLY);
    assert(fd > 0);
    while (1) {
        char buf[1024];
        int len = read(fd, buf, sizeof buf);
        if (len > 0) {
            sendBuf->appendDataString(buf,len);
#ifndef SEND_DATA_AUTO
            sendBuf->sendData(socket);
#endif
        }
        else if (len == 0) {
            break;
        }
        else {
            close(fd);
            perror("read fd");
        }
    }
    close(fd);
}


void HttpRequest::sendDir(string dirName, Buffer* sendBuf, int cfd)
{
    char buf[4096] = { 0 };
    sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName.data());
    struct dirent** namelist;
    int num = scandir(dirName.data(), &namelist, NULL, alphasort);
    for (int i = 0; i < num; ++i)
    {
        // 取出文件名 namelist 指向的是一个指针数组 struct dirent* tmp[]
        char* name = namelist[i]->d_name;
        struct stat st;
        char subPath[1024] = { 0 };
        sprintf(subPath, "%s/%s", dirName.data(), name);
        stat(subPath, &st);
        if (S_ISDIR(st.st_mode))
        {
            // a标签 <a href="">name</a>
            sprintf(buf + strlen(buf),
                "<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>",
                name, name, st.st_size);
        }
        else
        {
            sprintf(buf + strlen(buf),
                "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                name, name, st.st_size);
        }
        // send(cfd, buf, strlen(buf), 0);
        sendBuf->appendDataString(buf);
#ifndef MSG_SEND_AUTO
        sendBuf->sendData(cfd);
#endif
        memset(buf, 0, sizeof(buf));
        free(namelist[i]);
    }
    sprintf(buf, "</table></body></html>");
    // send(cfd, buf, strlen(buf), 0);
    sendBuf->appendDataString(buf);
#ifndef MSG_SEND_AUTO
    sendBuf->sendData(cfd);
#endif
    free(namelist);
}
