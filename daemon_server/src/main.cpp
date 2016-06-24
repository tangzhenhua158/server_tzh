#include "h/daemonClient.h"
#include "h/daemonServer.h"
#include "h/app.h"

using namespace daemon_name;


int main(int argc ,char *argv[])
{
	EventLoop loop;
	int port = 8888;
	InetAddress listenAddr(static_cast<uint16_t>(port));
	daemon_name::DaemonServiceImpl impl;
	RpcServer server(&loop, listenAddr);
	server.setThreadNum(1);
	server.registerService(&impl);
	server.start();

	InetAddress serverAddr("127.0.0.1",port);
	App _app(&loop,serverAddr);
	_app.start();
	loop.loop();
	return 0;
}