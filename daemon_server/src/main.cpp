#include "h/daemonClient.h"
#include "h/daemonServer.h"
#include "h/app.h"
#include "muduo/base/Exception.h"
#include <signal.h>

using namespace daemon_name;


int main(int argc ,char *argv[])
{
	LOG_INFO <<__FUNCTION__<<" start............";
	try{
		signal(SIGPIPE, SIG_IGN);
		EventLoop loop;
		int port = 8888;
		InetAddress listenAddr(static_cast<uint16_t>(port));
		daemon_name::DaemonServiceImpl impl;
		RpcServer server(&loop, listenAddr);
		server.setThreadNum(1);
		server.registerService(&impl);
		LOG_INFO <<__FUNCTION__<<" registerService: "<<impl.GetDescriptor()->name()<<" ";
		server.start();

		InetAddress serverAddr("127.0.0.1",port);
		App _app(&loop,"daemonserver",serverAddr);
		_app.start();
		loop.loop();
		return 0;
	}catch(Exception & e)
	{
		LOG_INFO <<"except:"<< __FUNCTION__<<e.what();
	}catch(...){
		LOG_INFO <<"except:"<< __FUNCTION__;
	}
	LOG_INFO <<__FUNCTION__<<" end............";
	return 0;
}