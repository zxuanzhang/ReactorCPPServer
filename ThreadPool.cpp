#include "ThreadPool.h"
#include<stdlib.h>
#include<assert.h>
struct ThreadPool* ThreadPoolInit(struct EventLoop* mainloop, int count)
{
	struct ThreadPool* pool = (struct ThreadPool*)malloc(sizeof(struct ThreadPool));
	pool->index = 0;
	pool->isStart = false;
	pool->threadNums = count;
	pool->mainloop = mainloop;
	pool->threads = (struct WorkThread*)malloc(sizeof(struct WorkThread) * count);
	return pool;
}

void ThreadPoolRun(ThreadPool* pool)
{
	assert(pool && !pool->isStart);
	//确定是在主线程运行的
	if (pool->mainloop->pid != pthread_self()) {
		exit(0);
	}
	pool->isStart = true;
	if (pool->threadNums) {//threadNums大于零证明有子线程，是多反应堆模型，否则是但反应堆模型
		for (int i = 0;i < pool->threadNums;++i) {
			WorkThreadInit(&pool->threads[i], i);
			WorkThreadRun(&pool->threads[i]);
		}
	}
}

struct EventLoop* takeEventLoop(struct ThreadPool* pool)
{
	assert(pool->isStart);
	if (pool->mainloop->pid != pthread_self()) {
		exit(0);
	}
	struct EventLoop* evloop = pool->mainloop;
	if (pool->threadNums) {
		evloop = pool->threads[pool->index].evloop;
		pool->index = ++pool->index % pool->threadNums;
	}
	return evloop;
}
