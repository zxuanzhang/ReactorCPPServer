#pragma once
#include"Channel.h"
#include"EventLoop.h"

struct Dispatcher {
	void* (*init)();
	int(*add)(struct EventLoop*evloop,struct Channel* channel); //evloop�����data�����ݣ�channel��epfd�ʹ���ӵ��ļ�������
	int (*remove)(struct EventLoop* evloop, struct Channel* channel);
	int (*modify)(struct EventLoop* evloop, struct Channel* channel);
	int (*dispatch)(struct EventLoop* evloop, int timeout);
	int (*clear)(struct EventLoop* evloop);
};