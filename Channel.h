#pragma once
#include<functional>
using namespace std;
//using handlefunc = int(*)(void*);//����ص�����
enum class FDEvent { //ǿ����ö��
	Timeout = 0x01,
	Read = 0x02,
	Write = 0x04
};

class Channel {
private:
	int fd;
	int events;//����д������д
	void* arg;
public:
	using handlefunc = function<int(void*)>;
	Channel(int fd, FDEvent events, handlefunc read, handlefunc write, handlefunc destroy, void* arg);
	handlefunc readCallback;
	handlefunc writeCallback; 
	handlefunc destroyCallback;
	void writeEventEnable(bool flag);
	bool isWriteEnable();
	inline int get_fd() {
		return fd;
	}
	inline int get_event() {
		return events;
	}
	inline const void* get_arg() {
		return arg;
	}
};


