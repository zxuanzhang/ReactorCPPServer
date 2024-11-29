#include "Buffer.h"
#include<stdlib.h>
#include<string.h>
#include<sys/uio.h>
struct Buffer* bufferInit(int size)
{
	struct Buffer* buffer = (struct Buffer*)malloc(sizeof(struct Buffer));
	if (!buffer) {
		return NULL;
	}
	buffer->capicity = size;
	buffer->readPos = buffer->writePos = 0;
	buffer->data = (char*)malloc(size);
	memset(buffer->data, 0, size);
	return buffer;
}

void bufferDestroy(struct Buffer* buffer)
{
	if (buffer != NULL) {
		if (buffer->data != NULL) {
			free(buffer->data);
		}
		free(buffer);
	}
}

void bufferExtend(struct Buffer* buffer, int size)
{
	//内存够用
	if (bufferWriteable(buffer) >= size) {
		return;
	}
	//内存挪动后够用
	else if (buffer->readPos + bufferWriteable(buffer) >= size) {
		int readable = bufferReadable(buffer);
		memcpy(buffer->data,buffer->data + buffer->readPos, readable);
		buffer->readPos = readable;
		buffer->writePos = 0;
	}
	//内存不够用
	else {
		void* tmp = realloc(buffer->data, buffer->capicity + size);
		if (!tmp) {
			return;
		}
		memset(tmp + buffer->capicity, 0, size);
		buffer->data = (char*)tmp;
		buffer->capicity += size;
	}
}

int bufferWriteable(Buffer* buffer)
{
	return buffer->capicity - buffer->writePos;
}

int bufferReadable(Buffer* buffer)
{
	return buffer->writePos - buffer->readPos;
}

int bufferAppendData(struct Buffer* buffer, char* data, int size)
{
	if (buffer == NULL || data == NULL) {
		return -1;
	}
	bufferExtend(buffer, size);
	memcpy(buffer->data + buffer->writePos, data, size);
	buffer->writePos += size;
	return 0;
}

int bufferAppendDataString(Buffer* buffer, char* data)
{
	int size = strlen(data);
	int ret = bufferAppendData(buffer, data, size);
	return ret;
}

int bufferSocketRead(Buffer* buffer, int fd)
{
	struct iovec vec[2];
	int writeable = bufferWriteable(buffer);
	vec[0].iov_len = writeable;
	vec[0].iov_base = buffer->data + buffer->writePos;
	vec[1].iov_len = 409600;
	char* chardata = (char*)malloc(409600);
	vec[1].iov_base = chardata;
	int result = readv(fd, vec, 2);
	if (result == -1) {
		return -1;
	}
	else if (result <= writeable) {
		buffer->writePos += result;
	}
	else {
		//扩容
		buffer->writePos = buffer->capicity;
		bufferAppendData(buffer, chardata, result - writeable);
	}
	free(chardata);
	return result;
}
