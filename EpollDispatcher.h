#pragma once
#include"Channel.h"
#include"EventLoop.h"
#include<string>
#include<sys/epoll.h>
class EpollDispatcher:public Dispatcher {
private:
	int m_epfd;
	struct epoll_event* m_ev;
	const int max_node = 520;
	int epollCtl(int op);
public:
	 EpollDispatcher(EventLoop* evloop);
	 int add() override; //evloop里面的data存数据，channel存epfd和待添加的文件描述符
	 ~EpollDispatcher();
	 int remove() override;
	 int modify() override;
	 int dispatch(int timeout = 2) override;
};