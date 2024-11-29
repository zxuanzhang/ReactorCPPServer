#pragma once
#include"EventLoop.h"
#include"Channel.h"
#include"Buffer.h"
struct TcpConnection { //一个connection实际上属于一个对应的Eventloop
	char name[32];
	struct EventLoop* evloop;
	struct Buffer* readBuffer;
	struct Buffer* writeBuffer;
	struct Channel* channel;
};

struct TcpConnection* tcpConnInit(struct EventLoop* evloop, int fd);

