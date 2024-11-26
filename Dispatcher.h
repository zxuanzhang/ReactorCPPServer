#pragma once
#include"Channel.h"
#include"EventLoop.h"

struct Dispatcher {
	void* (*init)();
	int(*add)(struct EventLoop*evloop,struct Channel* channel); //evloop里面的data存数据，channel存epfd和待添加的文件描述符
	int (*remove)(struct EventLoop* evloop, struct Channel* channel);
	int (*modify)(struct EventLoop* evloop, struct Channel* channel);
	int (*dispatch)(struct EventLoop* evloop, int timeout);
	int (*clear)(struct EventLoop* evloop);
};