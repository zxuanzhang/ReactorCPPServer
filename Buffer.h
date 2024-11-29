#pragma once
struct Buffer {
	char* data;
	int capicity;
	int readPos;
	int writePos;
};
struct Buffer* bufferInit(int size);
void bufferDestroy(struct Buffer* buffer);
//À©ÈÝ
void bufferExtend(struct Buffer* buffer, int size);
int bufferWriteable(struct Buffer* buffer);
int bufferReadable(struct Buffer* buffer);
int bufferAppendData(struct Buffer* buffer, char* data, int size);
int bufferAppendDataString(struct Buffer* buffer, char* data);
int bufferSocketRead(struct Buffer* buffer, int fd);
