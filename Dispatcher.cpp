#include "Dispatcher.h"
#include "EpollDispatcher.h"

Dispatcher::Dispatcher(EventLoop* m_evloop){
	this->m_eventloop = m_evloop;
}


Dispatcher::~Dispatcher()
{
}
