#include"Dispatcher.h"
#include<sys/epoll.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include "EpollDispatcher.h"
//static void* EpollInit();
//static int EpollAdd(struct EventLoop* evloop, struct Channel* channel); //evloop里面的data存数据，channel存epfd和待添加的文件描述符
//static int EpollRemove(struct EventLoop* evloop, struct Channel* channel);
//static int EpollModify(struct EventLoop* evloop, struct Channel* channel);
//static int EpollDispatch(struct EventLoop* evloop, int timeout);
//static int EpollClear(struct EventLoop* evloop);
//static int EpollCtl(struct Channel*channel,struct EventLoop*lp,int op);
//
//struct EpollData
//{
//	int epfd;
//	struct epoll_event* ev;
//};
//struct Dispatcher EpollDispatcher {
//	EpollInit,
//	EpollAdd,
//	EpollRemove,
//	EpollModify,
//	EpollDispatch,
//	EpollClear
//};
//void* EpollInit()
//{
//	struct EpollData* epev = (struct EpollData*)malloc(sizeof(EpollData));
//	epev->epfd = epoll_create(10);
//	if (epev->epfd == -1) {
//		perror("epoll_create");
//		exit(0);
//	}
//	epev->ev = (struct epoll_event*)calloc(NUM, sizeof(epoll_event));
//	return epev;
//}

//int EpollDispatcher::epollAdd(EventLoop* evloop, Channel* channel)
//{
//	int ret = epollCtl(EPOLL_CTL_ADD);
//	if (ret == -1)
//	{
//		perror("epoll_crl add");
//		exit(0);
//	}
//	return ret;
//}
//int EpollRemove(EventLoop* evloop, Channel* channel)
//{
//	int ret = EpollCtl(channel, evloop, EPOLL_CTL_DEL);
//	if (ret == -1)
//	{
//		std::cout<<"Error "
//		exit(0);
//	}
//	channel->destroyCallback(channel->arg);
//	return ret;
//}

//int EpollModify(EventLoop* evloop, Channel* channel)
//{
//	int ret = EpollCtl(channel, evloop, EPOLL_CTL_MOD);
//	if (ret == -1)
//	{
//		perror("epoll_crl mod");
//		exit(0);
//	}
//	return ret;
//}

//int EpollDispatch(EventLoop* evloop, int timeout)
//{
//	struct EpollData* data = (EpollData*)evloop->DispatcherData;
//	int count = epoll_wait(data->epfd, data->ev, NUM, timeout * 1000);
//	for (int i = 0;i < count;i++) {
//		int events = data->ev[i].events;
//		int fd = data->ev[i].data.fd;
//		if (events & EPOLLHUP || events & EPOLLERR) { //对方断开连接
//			continue;
//		}
//		if (events&EPOLLIN) {
//			eventActivate(fd, evloop, Read);
//		}
//		if (events&EPOLLOUT) {
//			eventActivate(fd, evloop, Write);
//		}
//	}
//	return 0;
//}



//int EpollCtl(Channel* channel, EventLoop* lp, int op)
//{
//	struct EpollData* epdata = (EpollData*)lp->DispatcherData;
//	struct epoll_event ev;
//	ev.data.fd = channel->fd;
//	int events = 0;
//	if (channel->events & Read) {
//		events |= EPOLLIN;
//	}
//	if (channel->events & Write) {
//		events |= EPOLLOUT;
//	}
//	ev.events = events;
//	int ret = epoll_ctl(epdata->epfd, op, channel->fd, &ev);
//	return ret;
//}

int EpollDispatcher::epollCtl(int op)
{
	struct epoll_event ev;
	ev.data.fd = m_channel->get_fd();
	int events = 0;
	if (m_channel->get_event() & (int)FDEvent::Read) {
		events |= EPOLLIN;
	}
	if (m_channel->get_event() & (int)FDEvent::Write) {
		events |= EPOLLOUT;
	}
	ev.events = events;
	int ret = epoll_ctl(m_epfd, op, m_channel->get_fd(), &ev);
	return ret;
}

EpollDispatcher::EpollDispatcher(EventLoop* evloop):Dispatcher(evloop)
{
	m_epfd = epoll_create(10);
	if (m_epfd == -1) {
		std::cout << "error epoll_create" << std::endl;
		exit(0);
	}
	m_ev = new struct epoll_event[max_node];
}

int EpollDispatcher::add()
{
	int ret = epollCtl(EPOLL_CTL_ADD);
	if (ret == -1)
	{
		std::cout << "Error epoll_add" << std::endl;
		exit(0);
	}
	return ret;
}

EpollDispatcher::~EpollDispatcher()
{
	close(m_epfd);
	delete [] m_ev;
}

int EpollDispatcher::remove()
{
	int ret = epollCtl(EPOLL_CTL_DEL);
	if (ret == -1)
	{
		std::cout << "Error epoll_remove" << std::endl;
			exit(0);
	}
	
	return ret;
}

int EpollDispatcher::modify()
{
	int ret = epollCtl(EPOLL_CTL_MOD);
	if (ret == -1)
	{
		std::cout << "Error epoll_mod" << std::endl;
		exit(0);
	}
	m_channel->destroyCallback(const_cast<void*>(m_channel->get_arg()));
	return ret;
}

int EpollDispatcher::dispatch(int timeout)
{
	int count = epoll_wait(m_epfd, m_ev, max_node, timeout * 1000);
	for (int i = 0;i < count;i++) {
		int events = m_ev[i].events;
		int fd = m_ev[i].data.fd;
		if (events & EPOLLHUP || events & EPOLLERR) { //对方断开连接
			continue;
		}
		if (events & EPOLLIN) {
			m_eventloop->eventActivate(fd,(int)FDEvent::Read);
		}
		if (events & EPOLLOUT) {
			m_eventloop->eventActivate(fd, (int)FDEvent::Write);
		}
	}
	return 0;
}
