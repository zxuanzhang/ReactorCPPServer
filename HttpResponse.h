#pragma once
#include"Buffer.h"
#include <string>
#include<map>
#include<functional>
using namespace std;
enum class HttpStatusCode
{
	Unknown,
	OK=200,
	MovedPermanently = 301,
	MovedTemporarily = 302,
	BadRequest = 400,
	NotFound = 404
};

//struct HttpResponseHeader {
//	char key[128];
//	char value[128];
//};
//typedef void (*responseBody)(const char* fileName, struct Buffer* sendBuf, int socket);//response回调
class HttpResponse {
private:
	HttpStatusCode status;
	//char statusMsg[128];
	string fileName;
	map<string,string> headers;
	const map<int, string> m_info = {
		{200, "OK"},
		{301, "MovedPermanently"},
		{302, "MovedTemporarily"},
		{400, "BadRequest"},
		{404, "NotFound"},	
	};
public:
	function<void(string, Buffer*, int)>sendDataFunc;
	HttpResponse();
	~HttpResponse();
	void addHeader(const string key, const string value);
	void prepareMsg(Buffer* sendBuf, int socket);
	inline void setFilename(string name) {
		fileName = name;
	}
	inline void setStatusCode(HttpStatusCode code) {
		status = code;
	}
	//inline string 
};
//struct HttpResponse* HttpResponseInit();
// 销毁
//void httpResponseDestroy(struct HttpResponse* response);
// 添加响应头
//void httpResponseAddHeader(struct HttpResponse*response, const char* key, const char* value);
// 组织http响应数据
//void httpResponsePrepareMsg(struct HttpResponse* response, struct Buffer* sendBuf, int socket);
