#pragma once
#include"Dispatcher.h"
#include<thread>
#include<queue>
#include<map>
#include<mutex>
using namespace std;
enum class ElemType:char
{
	ADD,
	DELETE,
	MODIFY
};

//任务队列用于收集各种文件描述符操作请求
struct ChannelElement {
	ElemType type;
	Channel* channel;
};
// 
class Dispatcher;
class EventLoop
{
private:
	bool isQuit;
	Dispatcher* dispatcher;
	thread::id threadId;
	string threadname; //可以给子线程起名字，主线程名字固定
	mutex m_mutex;
	int socketPair[2]; //用于本地套接字通信
	map<int, Channel*>channelMap;
	queue<ChannelElement*>taskQueue;
public:
	EventLoop();
	EventLoop(string name);
	~EventLoop();
	int run();
	int eventActivate(int fd, int event);
	int addTask(ElemType type, Channel* channel);
	int process();
	int add(Channel* channel);
	int remove(Channel* channel);
	int modify(Channel* channel);
	int destroyChannel(Channel* channel);
	int readMessage();
	inline thread::id get_threadId() {
		return threadId;
	}
	inline string get_threadName(){
		return threadname;
	}
private:
	void taskWakeUp();
};








