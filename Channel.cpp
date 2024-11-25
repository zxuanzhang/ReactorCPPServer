#include "Channel.h"
#include<stdlib.h>

struct Channel* channelInit(int fd, int events, handlefunc read, handlefunc write, handlefunc destroy, void* arg)
{
	struct Channel* channel = (Channel*)malloc(sizeof (struct Channel));
	channel->arg = arg;
	channel->fd = fd;
	channel->events = events;
	channel->readCallback = read;
	channel->writeCallback = write;
	channel->destroyCallback = destroy;
	return channel;
}

void writeEventEnable(bool flag, Channel* channel)
{
	if (flag) {
		channel->events |= FDEvent::Write;
	}
	else {
		channel->events = channel->events & ~FDEvent::Write;
	}
}

bool isWriteEnable(Channel* channel)
{
	return channel->events & FDEvent::Write;
}

