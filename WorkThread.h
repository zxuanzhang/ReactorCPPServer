#pragma once
#include<pthread.h>
#include"EventLoop.h"

struct WorkThread {
	pthread_t pid;
	char name[24];
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct EventLoop* evloop;
};
int WorkThreadInit(struct WorkThread* thread, int index);
void WorkThreadRun(struct WorkThread* thread);



