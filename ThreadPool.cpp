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
	//ȷ���������߳����е�
	if (pool->mainloop->pid != pthread_self()) {
		exit(0);
	}
	pool->isStart = true;
	if (pool->threadNums) {//threadNums������֤�������̣߳��Ƕ෴Ӧ��ģ�ͣ������ǵ���Ӧ��ģ��
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
