
#include "login.h"

int main(int argc, char* argv[])
{
	LOG_INFO << "pid = " << getpid() << " threads = " << 1;
	EventLoop loop;
	int port =   8888;
	InetAddress listenAddr(static_cast<uint16_t>(port));
	user::LoginServiceImpl impl;
	RpcServer server(&loop, listenAddr);
	server.setThreadNum(1);
	server.registerService(&impl);
	server.start();
	loop.loop();
}