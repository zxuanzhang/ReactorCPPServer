#pragma once
#include<stdbool.h>

typedef int(*handlefunc)(void* arg); //����ص�����
struct Channel {
	int fd;
	int events;//����д������д
	handlefunc readCallback;
	handlefunc writeCallback;
	handlefunc destroyCallback;
	void* arg;
};
enum FDEvent {
	Timeout = 0x01,
	Read = 0x02,
	Write = 0x04
};
struct Channel* channelInit(int fd,int events,handlefunc read,handlefunc write,handlefunc destroy,void*arg);
void writeEventEnable(bool flag,struct Channel* channel);
bool isWriteEnable(struct Channel*channel);
