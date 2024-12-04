#include<unistd.h>
#include<stdlib.h>
#include"TcpServer.h"

int main(int argc,char*argv[]) {
	unsigned short port = 10005;
	chdir("/home");
	TcpServer* server = new TcpServer(4,port);
	server->run();
	return 0;
}