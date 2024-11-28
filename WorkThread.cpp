#include "WorkThread.h"
#include<stdio.h>
int WorkThreadInit(struct WorkThread* thread, int index)
{
	thread->evloop = NULL;
	sprintf(thread->name, "SubThread-%d", index);
	pthread_mutex_init(&thread->mutex, NULL);
	pthread_cond_init(&thread->cond, NULL);
	return 0;
}

void* subThreadRun(void* arg) {
	struct WorkThread* thread = (struct WorkThread*)arg;
	pthread_mutex_lock(&thread->mutex);
	thread->evloop = EventLoopInitex(thread->name);
	pthread_mutex_unlock(&thread->mutex);
	pthread_cond_signal(&thread->cond);
	EventLoopRun(thread->evloop);
	return NULL;
}

void WorkThreadRun(struct WorkThread* thread)
{
	pthread_create(&thread->pid, NULL, subThreadRun, thread);
	pthread_mutex_lock(&thread->mutex);
	while (thread->evloop == NULL) { //ѭ����ԭ���Ǳ�����ٻ��ѣ�ȷ�������ȴ�
		pthread_cond_wait(&thread->cond, &thread->mutex); //������������ڳ��л�����������²��ܱ�����
	}
	pthread_mutex_unlock(&thread->mutex);

}


