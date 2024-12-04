#include "TcpConnection.h"
#include<stdlib.h>
#include<stdio.h>
#include"Log.h"

//int processRead(void* arg) {
//	struct TcpConnection* tcp = (struct TcpConnection*)arg;
//	int count = bufferSocketRead(tcp->readBuffer, tcp->channel->fd);
//	if (count > 0) {
//
//	}
//	else {
//
//	}
//	return 0;
//}
//int processWrite(void* arg) {
//
//}

//struct TcpConnection* tcpConnInit(struct EventLoop* evloop, int fd)
//{
//	m_evloop = evloop;
//	conn->readBuffer = bufferInit(10240);
//	conn->writeBuffer = bufferInit(10240);
//	sprintf(conn->name, "Connection-%d", fd);
//	conn->channel = channelInit(fd, Read, processRead, NULL, NULL, conn);
//	eventLoopAddTask(conn->evloop, ADD, conn->channel);
//	return conn;
//}

TcpConnection::TcpConnection(EventLoop* evloop, int fd)
{
	m_evloop = evloop;
	m_readBuffer = new Buffer(10240);
	m_writeBuffer = new Buffer(10240);
	m_request = new HttpRequest;
	m_response = new HttpResponse;
	m_name = "Connection-%d" + to_string(fd);
	m_channel = new Channel(fd, FDEvent::Read, processRead, processWrite, processDestroy, this);
	evloop->addTask(ElemType::ADD, m_channel);
}

TcpConnection::~TcpConnection()
{
	if (m_readBuffer && m_readBuffer->readable() == 0 && m_writeBuffer && m_writeBuffer->readable() == 0) {
		delete m_readBuffer;
		delete m_writeBuffer;
		delete m_request;
		delete m_response;
		m_evloop->destroyChannel(m_channel);
	}
	Debug("connection release......%s",m_name);
}

int TcpConnection::processRead(void* arg)
{
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	//接收
	int socket = conn->m_channel->get_fd();
	int count = conn->m_readBuffer->socketRead(socket);

	Debug("recieve http msg: %s", conn->m_readBuffer->readData());
	if (count > 0) {
		bool flag = conn->m_request->parseHttpRequest(conn->m_writeBuffer, conn->m_readBuffer, conn->m_response, socket);
		if (!flag) {
			string errMsg = "Http/1.1 400 Bad Request\r\n\r\n";
			conn->m_writeBuffer->appendDataString(errMsg);
		}
	}
	else {
		conn->m_evloop->addTask(ElemType::DELETE, conn->m_channel);
	}
	return 0;
}

int TcpConnection::processWrite(void* arg)
{
	Debug("send http msg....");
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	int count = conn->m_writeBuffer->sendData(conn->m_channel->get_fd());
	if (count > 0) {
		if (conn->m_writeBuffer->readable() == 0) { //数据全部被发送完毕
			conn->m_channel->writeEventEnable(false);
			conn->m_evloop->addTask(ElemType::MODIFY, conn->m_channel);
			conn->m_evloop->addTask(ElemType::DELETE, conn->m_channel);
		}
	}
	return 0;
}

int TcpConnection::processDestroy(void* arg)
{
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	if (conn != nullptr) {
		delete conn;
	}
	return 0;
}
