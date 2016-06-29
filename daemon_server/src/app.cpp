#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "h/daemonClient.h"
#include "h/app.h"
#include "muduo/base/Exception.h"
#include <boost/shared_ptr.hpp>

using namespace daemon_name;

namespace daemon_name
{
	App::App(EventLoop *pLoop,const std::string& servername,const InetAddress & address):
			m_pDaemonClient( new DaemonClient(pLoop,address)),
			m_pLoop(pLoop),m_servername(servername)
	{
		m_pDaemonClient->setAppClient(this);
		m_pDaemonClient->setConnectCallback(boost::bind(&App::onDaemonConnect,this));
	}

	App::~App()
	{
		LOG_INFO << __FUNCTION__;
		if(m_pDaemonClient.get()) 
		{
			daemon_name::loginOutReq  req;
			daemon_name::loginOutRsp  *rsp = new daemon_name::loginOutRsp;
			m_pDaemonClient->LoginOut(req,rsp);
		}
	}

	void App::start()
	{
		if(!m_pLoop) throw Exception("m_pLoop is null");

		if(!m_pDaemonClient.get()) throw Exception("m_pDaemonClient is null");

		m_pDaemonClient->connect();

		//ע���ʼ������
		m_pLoop->runAfter(1.0,boost::bind(&App::init,this));

		//ע�ᶨʱ��
		m_pLoop->runEvery(1.0,boost::bind(&App::timer,this));
	}

	void App::onDaemonConnect()
	{
		LOG_INFO << __FUNCTION__;
	}

	void App::onMasterDaemonConnect()
	{
		LOG_INFO << __FUNCTION__;
		register_to_daemonserver();
	}

	void App::timer()
	{
		//heart
		printf("%s ,%s\n",__FUNCTION__,"runing");
	}

	void App::init()
	{
		
		updateMasterDeamon();
	}

	void App::register_to_daemonserver()
	{
		//register
		daemon_name::registerReq req;
		req.set_ip(112);
		req.set_port(433);
		req.set_servername(m_servername);

		daemon_name::registerRsp *rsp = new daemon_name::registerRsp;
		m_pheartDaemonClient->register_server(req,rsp);
		printf("%s ,%s req:%u ,rsp:%s\n",__FUNCTION__,"runing",req.ip() ,rsp->servername().c_str());
	}

	serverPort& App::updateMasterDeamon()
	{
		daemon_name::queryDaemonMasterReq req;


		daemon_name::queryDaemonMasterRsp *rsp = new daemon_name::queryDaemonMasterRsp;
		m_pDaemonClient->queryMasterDaemon(req,rsp);
		printf("%s ,%s \n",__FUNCTION__,"runing" );
	}

	void  App::heart()
	{
		daemon_name::heartReq req;
		req.set_serverid(1);
		req.set_servername("daemon");

		daemon_name::heartRsp *rsp = new daemon_name::heartRsp;
		m_pDaemonClient->heart(req,rsp);
		printf("%s ,%s req:%s \n",__FUNCTION__,"runing",req.servername().c_str());
	}

	void App::notifyMasterDaemon(const serverPort & portinfo)
	{
		LOG_INFO << __FUNCTION__;
#pragma warning "err operator ="
		m_DaemonPortInfo = portinfo;
		//m_DaemonPortInfo = *const_cast<serverPort*>(&portinfo);
		
		if(m_pheartDaemonClient.get())
		{
			m_pheartDaemonClient->disconnect();
		}

		{
			in_addr in;
			in.s_addr = ntohl(portinfo.ip)	;

			string strip = inet_ntoa(in);
			short port = ntohs(portinfo.port);

			LOG_INFO << __FUNCTION__<< strip <<" "<< port;
			InetAddress address(strip,port);
			m_pheartDaemonClient.reset(new DaemonClient(m_pLoop,address));
			m_pheartDaemonClient->setConnectCallback(boost::bind(&App::onMasterDaemonConnect,this));
		}
		
		if(m_pheartDaemonClient.get() && m_pLoop )
		{
			m_pheartDaemonClient->connect();
		}
	}

	serverPort& App::getMasterDeamon()
	{
		
		return m_DaemonPortInfo;
	}
}