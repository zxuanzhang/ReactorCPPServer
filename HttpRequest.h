#pragma once
#include"Buffer.h"
#include"HttpResponse.h"
#include<string>
#include<map>
#include<functional>
using namespace std;
//struct HttpHeader {
//	char* key;
//	char* value;
//};

enum class HttpReqStatus:char {
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};

class HttpRequest {
private:
	string m_method;
	string m_url;
	string m_version;
	map<string, string> m_headers;
	HttpReqStatus m_curState;
public:
	HttpRequest();
	~HttpRequest();
	void reset();
	inline HttpReqStatus getStatus() {
		return m_curState;
	}
	void addHeader(const string key, const string value);
	string getHeader(const string key);
	bool parseHttpLine(Buffer* buf);
	bool parseHttpHeader(Buffer* buf);
	bool parseHttpRequest(Buffer* writeBuf, Buffer* readBuf, HttpResponse* rsp, int socket);
	char* splitRequestLine(const char* start, const char* end, const char* sub, function<void(string)>callback);
	string decodeMsg(string from);
	inline void setMethod(string method) {
		m_method = method;
	}
	inline void seturl(string url) {
		m_url = url;
	}
	inline void setVersion(string version) {
		m_version = version;
	}
	inline void setState(HttpReqStatus state) {
		m_curState = state;
	}
	bool processHttpGetRequest(HttpResponse* rsp);
	const string getFileType(const string name);
	inline int hexToDec(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'F')
			return c - 'A' + 10;

		return 0;
	}
	static void sendFile(string fileName, struct Buffer* sendBuf, int socket);
	static void sendDir(string dirName, struct Buffer* sendBuf, int cfd);
};

//struct HttpRequest* HttpRequestInit();

//void httpRequestReset(struct HttpRequest* req);
//void httpRequestResetEx(struct HttpRequest* req);
//void httpRequestDestroy(struct HttpRequest* req);

// enum HttpReqStatus getStatus(struct HttpRequest*req);
//void httpRequestAddHeader(struct HttpRequest* req,const char*key,const char*value);
//char* httpRequestGetHeader(struct HttpRequest* req,const char*key);

//bool parseHttpLine(struct HttpRequest* req,struct Buffer*buf);
//bool parseHttpHeader(struct HttpRequest* req, struct Buffer* buf);
//×éÖ¯»Ø¸´
//bool parseHttpRequest(struct HttpRequest* req, struct Buffer* writeBuf, struct Buffer* readBuf, struct HttpResponse* rsp,int socket);
//bool processHttpGetRequest(struct HttpRequest* req,struct HttpResponse*rsp);

//const string getFileType(const string name);
//void decodeMsg(char* to, char* from);
//int hexToDec(char c);

//void sendFile(string fileName, struct Buffer* sendBuf, int socket);
//void sendDir(string dirName, struct Buffer* sendBuf, int socket);