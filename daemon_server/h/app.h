#ifndef __DAEMON_APP_H__
#define __DAEMON_APP_H__

/*****************************************************************************
*daemonclient 包装类
*app随机连接一台deamon server
*从该daemon_server中查询master daemon，并向masterdaemon 发送心跳，或者注册
*从随机的daemonserver获取服务列表
*****************************************************************************/
#include <muduo/net/EventLoop.h>
#include "common.h"

using namespace muduo;
using namespace muduo::net;


class DaemonClient;

namespace daemon_name
{
	class App:public IApp
	{
	public:
		App(EventLoop *pLoop,const std::string& servername,const InetAddress & address);
		~App();
		void start();
		void onQueryMasterDaemonRsp(const serverPort & portinfo);
		void onDaemonConnect();
		void onMasterDaemonConnect();
		void addServerInfo(const std::string & host );
	public:
		void _handleConnectErr();
	private:
		void timer();
		void init();
	private:
		void register_to_daemonserver();
		serverPort& updateMasterDeamon();
		serverPort& getMasterDeamon();
		void heart();
		void resetDaemonClient();
	private:
		EventLoop *m_pLoop;
		//拉取master跟获取列表
		boost::shared_ptr<DaemonClient> m_pDaemonClient;
		//心跳专用
		boost::shared_ptr<DaemonClient> m_pheartDaemonClient;
		std::string  m_servername;
		serverPort m_DaemonPortInfo;
		Client_State_t  m_conn_state;
		std::vector<std::string >	m_mapHost;
		TimerId						m_timerId;
	};
}

#endif //__DAEMON_APP_H__

