#include "h/daemonClient.h"
#include "h/daemonServer.h"

using namespace daemon;


int main(int argc ,char *argv[])
{
	EventLoop loop;
	int port = 8888;
	InetAddress listenAddr(static_cast<uint16_t>(port));
	daemon::DaemonServiceImpl impl;
	RpcServer server(&loop, listenAddr);
	server.setThreadNum(1);
	server.registerService(&impl);
	server.start();
	app _app(&loop);
	_app.start();
	loop.loop();
	return 0;
}