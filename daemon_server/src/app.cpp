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
		m_pDaemonClient->setClientState(&m_conn_state);
		m_pDaemonClient->setErrCallback(boost::bind(&App::_handleConnectErr,this));
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

		//注册初始化函数
		m_pLoop->runAfter(1.0,boost::bind(&App::init,this));

		//注册定时器
		m_timerId = m_pLoop->runEvery(1.0,boost::bind(&App::timer,this));
	}

	void App::onDaemonConnect()
	{
		LOG_INFO << __FUNCTION__;
		m_conn_state.connect_daemon = Client_State_t::CONNECT;
	}

	void App::onMasterDaemonConnect()
	{
		LOG_INFO << __FUNCTION__;
		m_conn_state.connect_master_daemon = Client_State_t::CONNECT;
		register_to_daemonserver();
	}

	void App::timer()
	{
		if(m_conn_state.connect_daemon != Client_State_t::CONNECT)
		{
			if(!m_pDaemonClient.get()) throw Exception("m_pDaemonClient is null");
			return;
		}

		if(m_conn_state.connect_master_daemon != Client_State_t::CONNECT)
		{
			updateMasterDeamon();
			return;
		}

		if(m_conn_state.regMasterDaemon != Client_State_t::OK)
		{
			register_to_daemonserver();
			return;
		}

		heart();

		printf("%s ,%s\n",__FUNCTION__,"runing");
	}

	void App::init()
	{	
		if(m_conn_state.connect_daemon == Client_State_t::CONNECT)
		{
			updateMasterDeamon();
		}
	}

	void App::register_to_daemonserver()
	{
		//register
		daemon_name::registerReq req;
		req.set_ip(inet_addr("127.0.0.1"));
		req.set_port(htons(8888));
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
		if(!m_pheartDaemonClient.get()) return;

		daemon_name::heartReq req;
		req.set_serverid(m_conn_state.serverid);
		req.set_servername(m_servername);
		daemon_name::heartRsp *rsp = new daemon_name::heartRsp;
		m_conn_state.unRecvHeartRspCount++;
		if(m_conn_state.unRecvHeartRspCount >= 3) 
		{
			m_conn_state.heart_state = Client_State_t::FAIL;
			m_conn_state.connect_daemon = Client_State_t::UNCONNECT;
			m_conn_state.connect_master_daemon = Client_State_t::UNCONNECT;
		}
		m_pheartDaemonClient->heart(req,rsp);
		printf("%s ,%s req:%s \n",__FUNCTION__,"runing",req.servername().c_str());
	}

	void App::onQueryMasterDaemonRsp(const serverPort & portinfo)
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
			in.s_addr = portinfo.ip	;
			string strip = inet_ntoa(in);
			short port = ntohs(portinfo.port);

			if(portinfo.servername.empty())//no daemonserver 
			{
				LOG_WARN << __FUNCTION__ << " no daemonserver...................";
				strip = "127.0.0.1";
				port = 8888;
			}

			LOG_INFO << __FUNCTION__<< strip <<" "<< port;
			InetAddress address(strip,port);
			m_pheartDaemonClient.reset(new DaemonClient(m_pLoop,address));
			m_conn_state.connect_master_daemon = Client_State_t::UNCONNECT;
			m_pheartDaemonClient->setConnectCallback(boost::bind(&App::onMasterDaemonConnect,this));
			m_pheartDaemonClient->setClientState(&m_conn_state);
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

	void App::addServerInfo(const std::string & host )
	{
		m_mapHost.push_back(host);
	}

	void App::_handleConnectErr()
	{
		//rest daemonclient, retry next	
		static uint32_t errCount = 0 ;
		if(++errCount > 3) {m_pDaemonClient->stop(); errCount = 0;m_pLoop->cancel(m_timerId);resetDaemonClient();}
		LOG_WARN << __FUNCTION__ ;
	}

	void App::resetDaemonClient()
	{
		assert(m_mapHost.size() > 0);

		static uint32_t nIndex = 1;
		std::string host  = m_mapHost[nIndex%m_mapHost.size()];

		InetAddress out;
		InetAddress::resolve(host,&out);
		InetAddress address(out.toIp(),daemon_port);
		m_pDaemonClient.reset(new DaemonClient(m_pLoop,address));
		m_pDaemonClient->setAppClient(this);
		m_pDaemonClient->setConnectCallback(boost::bind(&App::onDaemonConnect,this));
		m_pDaemonClient->setClientState(&m_conn_state);
		m_pDaemonClient->setErrCallback(boost::bind(&App::_handleConnectErr,this));

		start();
	}
}