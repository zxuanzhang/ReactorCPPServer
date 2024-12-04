#pragma once
#include"EventLoop.h"
#include"WorkThread.h"
#include<vector>
class ThreadPool {
private:
	EventLoop* mainloop;
	vector<WorkThread*>threads;
	bool isStart;
	int threadNums;
	int index; //ÂÖÑ¯²ÎÊý
public:
	ThreadPool(EventLoop* mainloop, int count);
	~ThreadPool();
	void run();
	EventLoop* takeEventLoop();
};
//struct ThreadPool* ThreadPoolInit(struct EventLoop* mainloop, int count);
//void ThreadPoolRun(struct ThreadPool* pool);
//struct EventLoop* takeEventLoop(struct ThreadPool* pool);


