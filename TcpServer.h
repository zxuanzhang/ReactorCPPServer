#pragma once
#include"EventLoop.h"
#include"Channel.h"
#include"ThreadPool.h"


struct Listener {
	int lfd;
	unsigned short port;
};
struct TcpServer {
	int threadNum;
	struct Listener* listener;
	struct EventLoop* mainloop;
	struct ThreadPool* pool;
};
struct Listener* ListenerInit(unsigned short port);
struct TcpServer* TcpServerInit(int threadNum, unsigned short port);
void TcpServerRun(TcpServer* server);


