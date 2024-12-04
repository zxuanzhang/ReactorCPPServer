#include "Channel.h"
#include<stdlib.h>



Channel::Channel(int fd, FDEvent events, handlefunc read, handlefunc write, handlefunc destroy, void* arg)
{
	this->arg = arg;
	this->fd = fd;
	this->events = (int)events;
	readCallback = read;
	writeCallback = write;
	destroyCallback = destroy;
}

void Channel::writeEventEnable(bool flag)
{
	if (flag) {
		events |= static_cast<int>(FDEvent::Write);
	}
	else {
		events = events & ~static_cast<int>(FDEvent::Write);
	}
}

bool Channel::isWriteEnable()
{
	return events & static_cast<int>(FDEvent::Write);
}
