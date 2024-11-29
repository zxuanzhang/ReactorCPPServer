#include "TcpConnection.h"
#include<stdlib.h>
#include<stdio.h>

int processRead(void* arg) {
	struct TcpConnection* tcp = (struct TcpConnection*)arg;
	int count = bufferSocketRead(tcp->readBuffer, tcp->channel->fd);
	if (count > 0) {

	}
	else {

	}
	return 0;
}

struct TcpConnection* tcpConnInit(struct EventLoop* evloop, int fd)
{
	struct TcpConnection* conn = (struct TcpConnection*)malloc(sizeof(struct TcpConnection));
	conn->evloop = evloop;
	conn->readBuffer = bufferInit(10240);
	conn->writeBuffer = bufferInit(10240);
	sprintf(conn->name, "Connection-%d", fd);
	conn->channel = channelInit(fd, Read, processRead, NULL, NULL, conn);
	eventLoopAddTask(conn->evloop, ADD, conn->channel);
	return conn;
}
