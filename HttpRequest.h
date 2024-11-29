#pragma once
struct HttpHeader {
	char* key;
	char* value;
};

enum HttpReqStatus {
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};

struct HttpRequest {
	char* method;
	char* url;
	char* version;
	struct HttpHeader* headers;
	int reqHeadersNum;
	enum HttpReqStatus curState;
};

struct HttpRequest* HttpRequestInit();

void httpRequestReset(struct HttpRequest* req);
void httpRequestResetEx(struct HttpRequest* req);
void httpRequestDestroy(struct HttpRequest* req);


