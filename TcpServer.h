#pragma once
#include"EventLoop.h"
#include"Channel.h"
#include"ThreadPool.h"


//struct Listener {
//	int lfd;
//	unsigned short port;
//};
class TcpServer {
private:
	int lfd;
	unsigned short port;
	int threadNum;
//	struct Listener* listener;
	struct EventLoop* mainloop;
	struct ThreadPool* pool;
public:
	TcpServer(int threadNum, unsigned short port);
	void run();
	void setListion();
	static int acceptConn(void* arg);
};
//struct Listener* ListenerInit(unsigned short port);
//struct TcpServer* TcpServerInit(int threadNum, unsigned short port);
//void TcpServerRun(TcpServer* server);


