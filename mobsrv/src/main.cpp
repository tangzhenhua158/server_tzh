#include "h/daemonClient.h"
#include "h/app.h"
#include "muduo/base/Exception.h"
#include "h/mobServer.h"
#include <signal.h>

using namespace daemon_name;
using namespace mobsrv;


int main(int argc ,char *argv[])
{
	LOG_INFO <<__FUNCTION__<<" start............";
	try{
		signal(SIGPIPE, SIG_IGN);
		EventLoop loop;
		int port = 5555;
		InetAddress listenAddr(static_cast<uint16_t>(port));

		MobServer server(&loop, listenAddr);
		server.start();

		InetAddress serverAddr("127.0.0.1",8888);
		App _app(&loop,servername,serverAddr);
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