#pragma once
#include<string>
using namespace std;
class Buffer {
private:
	char* m_data;
	int m_capicity;
	int m_readPos;
	int m_writePos;
public:
	Buffer(int size);
	~Buffer();
	void extend(int size);
	int appendDataString(char* data);
	int appendDataString(string data);
	int appendDataString(char* data, int size);
	int socketRead(int fd);
	int sendData(int fd);
	char* bufferFindCRLF();
	inline int writeable() {
		return m_capicity - m_writePos;
	}
	inline int readable() {
		return m_writePos - m_readPos;
	}
	inline char* readData() {
		return m_data + m_readPos;
	}
	inline int readPosIncreasement(int inc) {
		m_readPos += inc;
		return m_readPos;
	}
	
};
//struct Buffer* bufferInit(int size);
//void bufferDestroy(struct Buffer* buffer);
//扩容
//void bufferExtend(struct Buffer* buffer, int size);
//int bufferWriteable(struct Buffer* buffer);
//int bufferReadable(struct Buffer* buffer);
//int bufferAppendData(struct Buffer* buffer, char* data, int size);
//int bufferAppendDataString(struct Buffer* buffer, char* data);
//int bufferSocketRead(struct Buffer* buffer, int fd);
//char* bufferFindCRLF(struct Buffer* buffer); //找\r\n取出一行