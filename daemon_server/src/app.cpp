#include "h/daemonClient.h"
#include "h/app.h"
#include "muduo/base/Exception.h"

using namespace daemon_name;

namespace daemon_name
{
	App::App(EventLoop *pLoop,const InetAddress & address):
			m_pDaemonClient( new DaemonClient(pLoop,address)),
			m_pLoop(pLoop)
	{

	}

	App::~App()
	{
		if(m_pDaemonClient.get()) 
		{
			daemon_name::loginOutReq  req;
			daemon_name::loginOutRsp  rsp;
			m_pDaemonClient->LoginOut(req,rsp);
		}
	}

	void App::start()
	{
		if(!m_pLoop) throw Exception("m_pLoop is null");

		if(!m_pDaemonClient.get()) throw Exception("m_pDaemonClient is null");

		m_pDaemonClient->connect();

		//注册初始化函数
		m_pLoop->runAfter(1.0,boost::bind(&App::init,this));

		//注册定时器
		m_pLoop->runEvery(5.0,boost::bind(&App::timer,this));
	}

	void App::timer()
	{
		//heart
		printf("%s ,%s",__FUNCTION__,"runing");
	}

	void App::init()
	{
		//register
		daemon_name::registerReq req;
		req.set_ip(112);
		req.set_port(433);
		req.set_servername("testdaemon");

		daemon_name::registerRsp rsp;
		m_pDaemonClient->register_server(req,rsp);
		printf("%s ,%s",__FUNCTION__,"runing");

	}
}