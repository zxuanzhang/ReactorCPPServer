#include "EventLoop.h"
#include<stdlib.h>
#include <string.h>
#include<assert.h>
#include<iostream>
#include <unistd.h>
#include"EpollDispatcher.h"
#include <sys/socket.h>
#include<functional>
#include"Log.h"
using namespace std;
//void AddTaskQueueNode(struct TaskQueue* queue,struct ChannelElement* node) {
//	if (queue->head == NULL) {
//		queue->head = queue->tail = node;
//	}
//	else {
//		queue->tail->next = node;
//		queue->tail = node;
//	}
//}

//struct EventLoop* EventLoopInitex(char* ThreadName)
//{
//	struct EventLoop* evLoop = (struct EventLoop*)malloc(sizeof(struct EventLoop));
//	evLoop->isQuit = false;
//	evLoop->channelmap = ChannelMapInit(128);
//	evLoop->dispatcher = &EpollDispatcher;
//	evLoop->pid = pthread_self();
//	pthread_mutex_init(&evLoop->mutex, NULL);
//	strcpy(evLoop->threadname, ThreadName == NULL ? "MainThread" : ThreadName);
//	evLoop->DispatcherData = evLoop->dispatcher->init();
//	evLoop->taskqueue->head = evLoop->taskqueue->tail = NULL;
//	int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, evLoop->socketPair);
//	if (ret == -1) {
//		perror("socket_pair");
//		exit(0);
//	}
//	struct Channel* channel = channelInit(evLoop->socketPair[1], Read, ReadLocalMsg, NULL, NULL, evLoop);
//	eventLoopAddTask(evLoop, ADD, channel);
//	return evLoop;
//}

//int EventLoopRun(struct EventLoop* evLoop)
//{
//	assert(evLoop != NULL);
//	if (evLoop->pid != pthread_self()) {
//		return -1;
//	}
//	struct Dispatcher* dispatcher = evLoop->dispatcher;
//	while (!evLoop->isQuit) {
//		dispatcher->dispatch(evLoop, 2);
//	}
//	return 0;
//}
//int eventActivate(int fd, struct EventLoop* evloop, int event)
//{
//	if (evloop == NULL || fd < 0) {
//		return -1;
//	}
//	struct Channel* channel = evloop->channelmap->list[fd];
//	assert(fd == channel->fd);
//	if (event & Read && channel->readCallback) {
//		channel->readCallback(channel->arg);
//	}
//	if (event & Write && channel->writeCallback) {
//		channel->writeCallback(channel->arg);
//	}
//	return 0;
//}
//
//int eventLoopAddTask(struct EventLoop* evloop, int type,struct Channel* channel)
//{
//	struct ChannelElement* node = (struct ChannelElement*)malloc(sizeof(struct ChannelElement));
//	node->channel = channel;
//	node->type = type;
//	node->next = NULL;
//	pthread_mutex_lock(&evloop->mutex);
//	AddTaskQueueNode(evloop->taskqueue, node);
//	pthread_mutex_unlock(&evloop->mutex);
//	if (evloop->pid==pthread_self()) { //子线程本身
//
//	}
//	else { //主线程
//		taskWakeUp(evloop);
//	}
//	return 0;
//}
//
//int eventLoopProcess(struct EventLoop* evloop)
//{
//	pthread_mutex_lock(&evloop->mutex);
//	struct TaskQueue* taskqueue = evloop->taskqueue;
//	while (taskqueue->head != NULL) {
//		struct Channel* channel = taskqueue->head->channel;
//		if (taskqueue->head->type == ADD) {
//			eventLoopAdd(evloop, channel);
//		}
//		if (taskqueue->head->type == DELETE) {
//			eventLoopRemove(evloop, channel);
//		}
//		if (taskqueue->head->type == MODIFY) {
//			eventLoopModify(evloop, channel);
//		}
//		struct ChannelElement* tmp = taskqueue->head;
//		taskqueue->head = taskqueue->head->next;
//		free(tmp);
//	}
//	evloop->taskqueue->head = evloop->taskqueue->tail = NULL;
//	pthread_mutex_unlock(&evloop->mutex);
//	return 0;
//}
//
//int eventLoopAdd(struct EventLoop* evloop,struct Channel* channel)
//{
//	int fd = channel->fd;
//	struct ChannelMap* channelmap = evloop->channelmap;
//	if (fd >= channelmap->size) {
//		if (!makeMapRoom(channelmap, fd, sizeof(struct Channel*))) {
//			return -1;
//		}
//	}
//	if (channelmap->list[fd] == NULL) {
//		channelmap->list[fd] = channel;
//		evloop->dispatcher->add(evloop, channel);
//	}
//	return 0;
//}

//int eventLoopRemove(struct EventLoop* evloop,struct Channel* channel)
//{
//	int fd = channel->fd;
//	struct ChannelMap* channelmap = evloop->channelmap;
//	if (fd >= channelmap->size) {
//		return -1;
//	}
//	int ret = evloop->dispatcher->remove(evloop, channel);
//	return ret;
//}
//
//int eventLoopModify(struct EventLoop* evloop,struct Channel* channel)
//{
//	int fd = channel->fd;
//	struct ChannelMap* channelmap = evloop->channelmap;
//	if (channelmap->list[fd]==NULL) {
//		return -1;
//	}
//	int ret = evloop->dispatcher->modify(evloop, channel);
//	return ret;
//}
//
//int destroyChannel(struct EventLoop* evloop,struct Channel* channel)
//{
//	evloop->channelmap->list[channel->fd] = NULL;
//	close(channel->fd);
//	free(channel);
//	return 0;
//}

EventLoop::EventLoop():EventLoop(string()) //委托构造函数
{
}

EventLoop::EventLoop(string name)
{
	isQuit = true;
	threadId = this_thread::get_id();
	this->threadname = name == string() ? "MainThread" : name;
	dispatcher = new EpollDispatcher(this);
	channelMap.clear();
	int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socketPair);
	if (ret == -1) {
		cout << "socketpair" << endl;
		exit(0);
	}
	auto obj = bind(&EventLoop::readMessage, this);
	Channel* channel = new Channel(socketPair[1], FDEvent::Read, obj, nullptr, nullptr, this);
	addTask(ElemType::ADD, channel);
}

EventLoop::~EventLoop()
{
}

int EventLoop::run()
{
	isQuit = false;
	if (threadId != this_thread::get_id()) {
		return -1;
	}
	while (!isQuit) {
		dispatcher->dispatch();
		process();
	}
	return 0;
}

int EventLoop::eventActivate(int fd, int event)
{
		if (fd < 0) {
			return -1;
			}
		Channel* channel = channelMap[fd];
		assert(fd == channel->get_fd());
			if (event & (int)FDEvent::Read && channel->readCallback) {
				channel->readCallback(const_cast<void*>(channel->get_arg()));
			}
			if (event & (int)FDEvent::Write && channel->writeCallback) {
				channel->writeCallback(const_cast<void*>(channel->get_arg()));
			}
			return 0;
}

int EventLoop::addTask(ElemType type, Channel* channel)
{
	m_mutex.lock();
	ChannelElement* node = new ChannelElement;
	node->channel = channel;
	node->type = type;
	taskQueue.push(node);
	//cout << "taskq node fd" << node->channel->get_fd() << endl;
	m_mutex.unlock();
	if (threadId == this_thread::get_id()) {
		process();
	}
	else {
		taskWakeUp();
	}
	return 0;
}

int EventLoop::process()//处理taskqueue内容
{
	while (!taskQueue.empty()) {
		m_mutex.lock();
		ChannelElement* element = taskQueue.front();
		taskQueue.pop();
		m_mutex.unlock();
		Debug("main thread process taskq:%d", element->channel->get_fd());
		Channel* channel = element->channel;
		if (element->type == ElemType::ADD) {
			add(channel);
		}
		else if (element->type == ElemType::DELETE) {
			remove(channel);
		}
		else {
			modify(channel);
		}
		delete element;
	}
	return 0;
}

int EventLoop::add(Channel* channel)
{
	if (channelMap.find(channel->get_fd()) != channelMap.end()) {
		return -1;
	}
	int fd = channel->get_fd();
	channelMap.insert(make_pair(fd, channel));
	dispatcher->setChannel(channel);
	int ret = dispatcher->add();
	return ret;
}

int EventLoop::remove(Channel* channel)
{
	if (channelMap.find(channel->get_fd()) == channelMap.end()) {
		return -1;
	}
	dispatcher->setChannel(channel);
	int ret = dispatcher->remove();
	return ret;
}

int EventLoop::modify(Channel* channel)
{
	if (channelMap.find(channel->get_fd()) == channelMap.end()) {
		return -1;
	}
	dispatcher->setChannel(channel);
	int ret = dispatcher->modify();
	return ret;
}

int EventLoop::destroyChannel(Channel* channel)
{
	auto it = channelMap.find(channel->get_fd());
	if (it != channelMap.end()) {
		channelMap.erase(it);
		close(channel->get_fd());
		delete channel;
	}
	return 0;
}
int EventLoop::readMessage()
{
	char buf[256];
	read(socketPair[1], buf, sizeof(buf));
	return 0;
}
void EventLoop:: taskWakeUp() {
	const char* msg = "wakeup";
	write(socketPair[0], msg, strlen(msg));
}
