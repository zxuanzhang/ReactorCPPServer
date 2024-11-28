#pragma once
#include"Channel.h"
#include"Dispatcher.h"
#include"ChannelMap.h"
#include<stdbool.h>
#include<pthread.h>
#include<sys/socket.h>

extern struct Dispatcher EpollDispatcher;
extern struct Dispatcher SelectDispatcher;
extern struct Dispatcher PollDispatcher;
enum type 
{
	ADD,
	DELETE,
	MODIFY
};

//������������ռ������ļ���������������
struct ChannelElement {
	int type;
	Channel* channel;
	ChannelElement* next;
};
struct TaskQueue {
	ChannelElement* head;
	ChannelElement* tail;
};
struct EventLoop
{
	bool isQuit;
	struct Dispatcher* dispatcher;
	void* DispatcherData;
	struct TaskQueue* taskqueue;
	struct ChannelMap* channelmap;
	pthread_t pid;
	char threadname[32]; //���Ը����߳������֣����߳����̶ֹ�
	pthread_mutex_t mutex;
	int socketPair[2]; //���ڱ����׽���ͨ��
};

struct EventLoop* EventLoopInit();
struct EventLoop* EventLoopInitex(char* ThreadName);
int EventLoopRun(EventLoop* evLoop);
int eventActivate(int fd,struct EventLoop*evloop,int event); 
int eventLoopAddTask(struct EventLoop* evloop, int type, struct Channel* channel);
int eventLoopProcess(struct EventLoop* evloop);
int eventLoopAdd(struct EventLoop* evloop,struct Channel*channel);
int eventLoopRemove(struct EventLoop* evloop, struct Channel* channel);
int eventLoopModify(struct EventLoop* evloop, struct Channel* channel);
int destroyChannel(struct EventLoop* evloop, struct Channel* channel);
