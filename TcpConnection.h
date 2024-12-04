#pragma once
#include"EventLoop.h"
#include"Buffer.h"
#include"Channel.h"
#include"HttpRequest.h"
#include"HttpRequest.h"
class TcpConnection { //一个connection实际上属于一个对应的Eventloop
private:
	string m_name;
	EventLoop* m_evloop;
	Buffer* m_readBuffer;
	Buffer* m_writeBuffer;
	Channel* m_channel;
	HttpRequest* m_request;
	HttpResponse* m_response;
public:
	TcpConnection(EventLoop* evloop, int fd);
	~TcpConnection();
	static int processRead(void* arg);
	static int processWrite(void* arg);
	static int processDestroy(void* arg);
};

//struct TcpConnection* tcpConnInit(struct EventLoop* evloop, int fd);

