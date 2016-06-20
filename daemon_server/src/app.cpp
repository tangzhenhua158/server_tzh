#include "h/app.h"
#include "h/daemonClient.h"

using namespace daemon;

namespace daemon
{
	App::App(EventLoop *pLoop):
			m_pDaemonClient( new DaemonClient),
			m_pLoop(pLoop)
	{

	}

	~App::App()
	{
		if(m_pDaemonClient.get()) 
		{
			daemon::loginOutReq  req;
			daemon::loginOutRsp  rsp
			m_pDaemonClient->LoginOut(req,rsp);
		}
	}

	void App::start()
	{
		if(!m_pLoop) throw Exception("m_pLoop is null");

		if(!m_pDaemonClient.get()) throw Exception("m_pDaemonClient is null");

		//ע���ʼ������
		m_pLoop->runAfter(1.0,boost::bind(App::init,this));

		//ע�ᶨʱ��
		m_pLoop->runEvery(5.0,boost::bind(App::timer,this));
	}

	void App::timer()
	{
		//heart
		printf("%s ,%s",__FUNCTION__,"runing");
	}

	void App::init()
	{
		//register
		printf("%s ,%s",__FUNCTION__,"runing");
	}
}