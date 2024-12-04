#include "WorkThread.h"
//#include<stdio.h>
//int WorkThreadInit(struct WorkThread* thread, int index)
//{
//	thread->evloop = NULL;
//	sprintf(thread->name, "SubThread-%d", index);
//	pthread_m_mutex_init(&thread->m_mutex, NULL);
//	pthread_cond_init(&thread->cond, NULL);
//	return 0;
//}

//void* subThreadRun(void* arg) {
//	struct WorkThread* thread = (struct WorkThread*)arg;
//	pthread_m_mutex_lock(&thread->m_mutex);
//	thread->evloop = EventLoopInitex(thread->name);
//	pthread_m_mutex_unlock(&thread->m_mutex);
//	pthread_cond_signal(&thread->cond);
//	EventLoopRun(thread->evloop);
//	return NULL;
//}

//void WorkThreadRun(struct WorkThread* thread)
//{
//	pthread_create(&thread->pid, NULL, subThreadRun, thread);
//	pthread_m_mutex_lock(&thread->m_mutex);
//	while (thread->evloop == NULL) { //循环的原因是避免虚假唤醒，确保继续等待
//		pthread_cond_wait(&thread->cond, &thread->m_mutex); //这个函数必须在持有互斥锁的情况下才能被调用
//	}
//	pthread_m_mutex_unlock(&thread->m_mutex);
//
//}

WorkThread::WorkThread(int index)
{
	m_thread = nullptr;
	evloop = nullptr;
	name = "SubThread-" + to_string(index);
	threadId = thread::id();
}

WorkThread::~WorkThread()
{
}

void WorkThread::run()
{
	m_thread = new thread(&WorkThread::subRun, this);
	unique_lock<std::mutex>locker(this->m_mutex);
	while (evloop == nullptr) { //循环的原因是避免虚假唤醒，确保继续等待
		cond.wait(locker); //这个函数必须在持有互斥锁的情况下才能被调用
	}
}

void WorkThread::subRun()
{
	this->m_mutex.lock();
	evloop = new EventLoop(name);
	this->m_mutex.unlock();
	cond.notify_one();
	evloop->run();
}
