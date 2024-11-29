#include "TcpServer.h"
#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
Listener* ListenerInit(unsigned short port)
{
	struct Listener* listener = (struct Listener*)malloc(sizeof(struct Listener));
	listener->port = port;
	int lfd = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp
	if (lfd == -1) {
		perror("socket");
		return NULL;
	}
	int opt = 1;
	int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1) {
		perror("setsockopt");
		return NULL;
	}
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	ret = bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1) {
		perror("bind");
		return NULL;
	}
	ret = listen(lfd, 128);
	if (ret == -1) {
		perror("listen");
		return NULL;
	}
	listener->lfd = lfd;
	return listener;
}

struct TcpServer* TcpServerInit(int threadNum, unsigned short port)
{
	struct TcpServer* tcpserver = (struct TcpServer*)malloc(sizeof(struct TcpServer));
	tcpserver->listener = ListenerInit(port);
	tcpserver->mainloop = EventLoopInit();
	tcpserver->pool = ThreadPoolInit(tcpserver->mainloop, threadNum);
	tcpserver->threadNum = threadNum;
	return tcpserver;
}

int acceptConn(void* arg) {
	struct TcpServer* server = (struct TcpServer*)arg;
	int cfd = accept(server->listener->lfd, NULL, NULL);//�������������ڽ�������Ϣ�洢��ĳ���ṹ����
	struct EventLoop* evloop = takeEventLoop(server->pool);
	//����cfd����tcp���� todo
	return 0;
}

void TcpServerRun(TcpServer* server)
{
	ThreadPoolRun(server->pool);
	struct Channel* channel = channelInit(server->listener->lfd, Read ,acceptConn, NULL, NULL, server);
	eventLoopAddTask(server->mainloop, ADD, channel);
	EventLoopRun(server->mainloop);
}