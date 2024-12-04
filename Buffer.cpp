#include "Buffer.h"
#include<stdlib.h>
#include<string.h>
#include<sys/uio.h>
#include <sys/socket.h>
#include <unistd.h>

//struct Buffer* bufferInit(int size)
//{
//	struct Buffer* buffer = (struct Buffer*)malloc(sizeof(struct Buffer));
//	if (!buffer) {
//		return NULL;
//	}
//	buffer->capicity = size;
//	buffer->readPos = buffer->writePos = 0;
//	buffer->data = (char*)malloc(size);
//	memset(buffer->data, 0, size);
//	return buffer;
//}

//void bufferDestroy(struct Buffer* buffer)
//{
//	if (buffer != NULL) {
//		if (buffer->data != NULL) {
//			free(buffer->data);
//		}
//		free(buffer);
//	}
//}

//void bufferExtend(struct Buffer* buffer, int size)
//{
//	//内存够用
//	if (bufferWriteable(buffer) >= size) {
//		return;
//	}
//	//内存挪动后够用
//	else if (buffer->readPos + bufferWriteable(buffer) >= size) {
//		int readable = bufferReadable(buffer);
//		memcpy(buffer->data,buffer->data + buffer->readPos, readable);
//		buffer->readPos = readable;
//		buffer->writePos = 0;
//	}
//	//内存不够用
//	else {
//		void* tmp = realloc(buffer->data, buffer->capicity + size);
//		if (!tmp) {
//			return;
//		}
//		memset(tmp + buffer->capicity, 0, size);
//		buffer->data = (char*)tmp;
//		buffer->capicity += size;
//	}
//}

//int bufferWriteable(Buffer* buffer)
//{
//	return buffer->capicity - buffer->writePos;
//}
//
//int bufferReadable(Buffer* buffer)
//{
//	return buffer->writePos - buffer->readPos;
//}

//int bufferAppendData(struct Buffer* buffer, char* data, int size)
//{
//	if (buffer == NULL || data == NULL) {
//		return -1;
//	}
//	bufferExtend(buffer, size);
//	memcpy(buffer->data + buffer->writePos, data, size);
//	buffer->writePos += size;
//	return 0;
//}

//int bufferAppendDataString(Buffer* buffer, char* data)
//{
//	int size = strlen(data);
//	int ret = bufferAppendData(buffer, data, size);
//	return ret;
//}
//
//int bufferSocketRead(Buffer* buffer, int fd)
//{
//	struct iovec vec[2];
//	int writeable = bufferWriteable(buffer);
//	vec[0].iov_len = writeable;
//	vec[0].iov_base = buffer->data + buffer->writePos;
//	vec[1].iov_len = 409600;
//	char* chardata = (char*)malloc(409600);
//	vec[1].iov_base = chardata;
//	int result = readv(fd, vec, 2);
//	if (result == -1) {
//		return -1;
//	}
//	else if (result <= writeable) {
//		buffer->writePos += result;
//	}
//	else {
//		//扩容
//		buffer->writePos = buffer->capicity;
//		bufferAppendData(buffer, chardata, result - writeable);
//	}
//	free(chardata);
//	return result;
//}

//char* bufferFindCRLF(struct Buffer* buffer)
//{
//	char* ptr = (char*)memmem(buffer->data + buffer->readPos, bufferReadable(buffer), "\r\n", 2);
//	return ptr;
//}

Buffer::Buffer(int size)
{
	m_capicity = size;
	m_readPos = m_writePos = 0;
	m_data = (char*)malloc(size);
	bzero(m_data, size);
}

Buffer::~Buffer()
{
	if (!m_data) {
		free(m_data);
	}
}

void Buffer::extend(int size)
{
	if (writeable() >= size) {
		return;
	}
	//内存挪动后够用
	else if (m_readPos + writeable() >= size) {
		int readable = this->readable();
		memcpy(m_data, m_data + m_readPos, readable);
		m_readPos = readable;
		m_writePos = 0;
	}
	//内存不够用
	else {
		void* tmp = realloc(m_data, m_capicity + size);
		if (!tmp) {
			return;
		}
		memset((char*)tmp + m_capicity, 0, size);
		m_data = (char*)tmp;
		m_capicity += size;
	}
}

int Buffer::appendDataString(char* data)
{
		int size = strlen(data);
		int ret = appendDataString(data, size);
		return ret;
}

int Buffer::appendDataString(string data)
{
	appendDataString(data.data());
	return 0;
}

int Buffer::appendDataString(char* data, int size)
{
	if (data == nullptr) {
		return -1;
	}
	extend(size);
	memcpy(m_data + m_writePos, data, size);
	m_writePos += size;
	return 0;
}

int Buffer::socketRead(int fd)
{
	struct iovec vec[2];
	// 初始化数组元素
	int writeable = this->writeable();
	vec[0].iov_base = m_data + m_writePos;
	vec[0].iov_len = writeable;
	char* tmpbuf = (char*)malloc(40960);
	vec[1].iov_base = m_data + m_writePos;
	vec[1].iov_len = 40960;
	int result = readv(fd, vec, 2);
	if (result == -1)
	{
		return -1;
	}
	else if (result <= writeable)
	{
		m_writePos += result;
	}
	else
	{
		m_writePos = m_capicity;
		appendDataString(tmpbuf, result - writeable);
	}
	free(tmpbuf);
	return result;
}

int Buffer::sendData(int fd)
{
	int readableSize = readable();
	if (readableSize > 0) {
		int count = send(fd, m_data+m_readPos, readableSize, MSG_NOSIGNAL);//发生错误发送信号断开
		if (count > 0) {
			m_readPos += count;
			usleep(1);
		}
		return count;
	}
	return 0;
}

char* Buffer::bufferFindCRLF()
{
	char* ptr = (char*)memmem(m_data + m_readPos, readable(), "\r\n", 2);
	return ptr;
}
