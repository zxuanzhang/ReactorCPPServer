#include "EventLoop.h"
#include<stdlib.h>
#include <string.h>
#include<assert.h>
#include<stdio.h>
#include <unistd.h>
struct EventLoop* EventLoopInit()
{
	return EventLoopInitex(NULL);
}
void taskWakeUp(struct EventLoop*evloop) {
	const char* msg = "wakeup";
	write(evloop->socketPair[0], msg, strlen(msg));
}
int ReadLocalMsg(void*arg) {
	struct EventLoop* evloop = (struct EventLoop*)arg;
	char buf[256];
	read(evloop->socketPair[1], buf, sizeof(buf));
	return 0;
}

void AddTaskQueueNode(struct TaskQueue* queue,struct ChannelElement* node) {
	if (queue->head == NULL) {
		queue->head = queue->tail = node;
	}
	else {
		queue->tail->next = node;
		queue->tail = node;
	}
}

struct EventLoop* EventLoopInitex(char* ThreadName)
{
	struct EventLoop* evLoop = (struct EventLoop*)malloc(sizeof(struct EventLoop));
	evLoop->isQuit = false;
	evLoop->channelmap = ChannelMapInit(128);
	evLoop->dispatcher = &EpollDispatcher;
	evLoop->pid = pthread_self();
	pthread_mutex_init(&evLoop->mutex, NULL);
	strcpy(evLoop->threadname, ThreadName == NULL ? "MainThread" : ThreadName);
	evLoop->DispatcherData = evLoop->dispatcher->init();
	evLoop->taskqueue->head = evLoop->taskqueue->tail = NULL;
	int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, evLoop->socketPair);
	if (ret == -1) {
		perror("socket_pair");
		exit(0);
	}
	struct Channel* channel = channelInit(evLoop->socketPair[1], Read, ReadLocalMsg, NULL, NULL, evLoop);
	eventLoopAddTask(evLoop, ADD, channel);
	return evLoop;
}

int EventLoopRun(EventLoop* evLoop)
{
	assert(evLoop != NULL);
	if (evLoop->pid != pthread_self()) {
		return -1;
	}
	struct Dispatcher* dispatcher = evLoop->dispatcher;
	while (!evLoop->isQuit) {
		dispatcher->dispatch(evLoop, 2);
	}
	return 0;
}
int eventActivate(int fd, struct EventLoop* evloop, int event)
{
	if (evloop == NULL || fd < 0) {
		return -1;
	}
	struct Channel* channel = evloop->channelmap->list[fd];
	assert(fd == channel->fd);
	if (event & Read && channel->readCallback) {
		channel->readCallback(channel->arg);
	}
	if (event & Write && channel->writeCallback) {
		channel->writeCallback(channel->arg);
	}
	return 0;
}

int eventLoopAddTask(EventLoop* evloop, int type, Channel* channel)
{
	struct ChannelElement* node = (struct ChannelElement*)malloc(sizeof(struct ChannelElement));
	node->channel = channel;
	node->type = type;
	node->next = NULL;
	pthread_mutex_lock(&evloop->mutex);
	AddTaskQueueNode(evloop->taskqueue, node);
	pthread_mutex_unlock(&evloop->mutex);
	if (evloop->pid==pthread_self()) { //子线程本身

	}
	else { //主线程
		taskWakeUp(evloop);
	}
	return 0;
}

int eventLoopProcess(EventLoop* evloop)
{
	pthread_mutex_lock(&evloop->mutex);
	struct TaskQueue* taskqueue = evloop->taskqueue;
	while (taskqueue->head != NULL) {
		struct Channel* channel = taskqueue->head->channel;
		if (taskqueue->head->type == ADD) {

		}
		if (taskqueue->head->type == DELETE) {

		}
		if (taskqueue->head->type == MODIFY) {

		}
		struct ChannelElement* tmp = taskqueue->head;
		taskqueue->head = taskqueue->head->next;
		free(tmp);
	}
	evloop->taskqueue->head = evloop->taskqueue->tail = NULL;
	pthread_mutex_unlock(&evloop->mutex);
	return 0;
}





