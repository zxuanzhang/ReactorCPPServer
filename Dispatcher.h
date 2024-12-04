#pragma once
#include"Channel.h"
#include"EventLoop.h"
#include<string>
class EventLoop;
class Dispatcher {
protected:
	Channel* m_channel;
	EventLoop* m_eventloop;
	std::string m_name = std::string();
public:
	Dispatcher(EventLoop* m_evloop);
	virtual ~Dispatcher();
	virtual int add()=0; //evloop�����data�����ݣ�channel��epfd�ʹ���ӵ��ļ�������
	virtual int remove()=0;
	virtual int modify()=0;
	virtual int dispatch(int timeout = 2)=0;
	inline void setChannel(Channel* channel) {
		m_channel = channel;
	}
};