#pragma once
#include<thread>
#include<mutex>
#include<condition_variable>
#include"EventLoop.h"
using namespace std;

class WorkThread {
private:
	thread::id threadId;
	string name;
	mutex m_mutex;
	condition_variable cond;
	EventLoop* evloop;
	thread* m_thread;
public:
	WorkThread(int index);
	~WorkThread();
	void run();
	void subRun();
	inline EventLoop* getEventLoop() {
		return evloop;
	}
};
//int WorkThreadInit(struct WorkThread* thread, int index);
//void WorkThreadRun(struct WorkThread* thread);



