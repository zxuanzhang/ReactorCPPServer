#pragma once
#include"EventLoop.h"
#include"WorkThread.h"
#include<stdbool.h>
struct ThreadPool {
	struct EventLoop* mainloop;
	struct WorkThread* threads;
	bool isStart;
	int threadNums;
	int index; //ÂÖÑ¯²ÎÊý
};
struct ThreadPool* ThreadPoolInit(struct EventLoop* mainloop, int count);
void ThreadPoolRun(struct ThreadPool* pool);
struct EventLoop* takeEventLoop(struct ThreadPool* pool);


