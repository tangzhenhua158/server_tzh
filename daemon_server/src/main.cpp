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
		signal(SIGSEGV, SIG_IGN);
		EventLoop loop;
		InetAddress listenAddr(static_cast<uint16_t>(daemon_port));
		daemon_name::DaemonServiceImpl impl;
		RpcServer server(&loop, listenAddr);
		server.setThreadNum(1);
		server.registerService(&impl);
		LOG_INFO <<__FUNCTION__<<" registerService: "<<impl.GetDescriptor()->name()<<" ";
		server.start();

		std::string ip1 = "daemonserver2.com";
		std::string ip2 = "daemonserver.com";
		std::string ip3 = "daemonserver1.com";
		InetAddress out;
		InetAddress::resolve(ip1,&out);
		InetAddress serverAddr(out.toIp(),daemon_port);
		App _app(&loop,servername,serverAddr);
		_app.addServerInfo(ip1);
		_app.addServerInfo(ip2);
		_app.addServerInfo(ip3);
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