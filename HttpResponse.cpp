#include "HttpResponse.h"
#include <string.h>
#include <strings.h>
#include<stdlib.h>
#include<stdio.h>
#include"Buffer.h"
#include"Log.h"
//#define ResHeaderSize 16
//struct HttpResponse* HttpResponseInit()
//{
//    struct HttpResponse* response = (struct HttpResponse*)malloc(sizeof(struct HttpResponse));
//    response->num = 0;
//    int size = sizeof(struct HttpResponseHeader) * ResHeaderSize;
//    response->headers = (struct HttpResponseHeader*)malloc(size);
//    response->status = Unknown;
//    // 初始化数组
//    bzero(response->headers, size);
//    bzero(response->statusMsg, sizeof(response->statusMsg));
//    bzero(response->fileName, sizeof(response->fileName));
//    // 函数指针
//    response->sendDataFunc = NULL;
//
//    return response;
//}

//void httpResponseDestroy(HttpResponse* response)
//{
//    if (response != NULL)
//    {
//        free(response->headers);
//        free(response);
//    }
//}

//void httpResponseAddHeader(HttpResponse*response, const char* key, const char* value)
//{
//    if (response == NULL || key == NULL || value == NULL)
//    {
//        return;
//    }
//    strcpy(response->headers[response->num].key, key);
//    strcpy(response->headers[response->num].value, value);
//    response->num++;
//}

//void httpResponsePrepareMsg(HttpResponse* response, Buffer* sendBuf, int socket)
//{
//    char tmp[1024] = { 0 };
//    sprintf(tmp, "HTTP/1.1 %d %s\r\n", response->status, response->statusMsg);
//    bufferAppendDataString(sendBuf, tmp);
//    for (int i = 0;i < response->num;++i) {
//        sprintf(tmp, "%s: %s\r\n", response->headers[i].key, response->headers[i].value);
//        bufferAppendDataString(sendBuf, tmp);
//    }
//    bufferAppendDataString(sendBuf, "\r\n");
//    response->sendDataFunc(response->fileName, sendBuf, socket);
//}

HttpResponse::HttpResponse()
{
    status = HttpStatusCode::Unknown;
    // 初始化数组
    headers.clear();
    fileName = "";
    // 函数指针
    sendDataFunc = nullptr;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::addHeader(const string key, const string value)
{
    if (key.empty() || value.empty())
    {
        return;
    }
    headers.insert(make_pair(key, value));
}

void HttpResponse::prepareMsg(Buffer* sendBuf, int socket)
{
    char tmp[1024] = { 0 };
    int code = static_cast<int>(status);
    Debug("status code%d", code);
    sprintf(tmp, "HTTP/1.1 %d %s\r\n", code, m_info.at(code).data());
    sendBuf->appendDataString(tmp);
    for (auto it = headers.begin();it != headers.end();++it) {
        sprintf(tmp, "%s: %s\r\n", it->first.data(), it->second.data());
        sendBuf->appendDataString(tmp);
    }

    sendBuf->appendDataString("\r\n");
#ifndef SEND_DATA_AUTO
   sendBuf->sendData(socket);
#endif // SEND_DATA_AUTO
   sendDataFunc(fileName, sendBuf, socket);
}
