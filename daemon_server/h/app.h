#ifndef __DAEMON_APP_H__
#define __DAEMON_APP_H__

#include <muduo/net/EventLoop.h>
#include "h/common.h"

using namespace muduo;
using namespace muduo::net;


class DaemonClient;
//class EventLoop;



namespace daemon_name
{
	class App:public IApp
	{
	public:
		App(EventLoop *pLoop,const InetAddress & address);
		~App();
		void start();
		void notifyMasterDaemon(serverPort & portinfo);
	private:
		void timer();
		void init();
	private:
		void register_to_daemonserver();
		serverPort& updateMasterDeamon();
		serverPort& getMasterDeamon();
		void heart();
	private:
		EventLoop *m_pLoop;
		//拉取master跟获取列表
		boost::shared_ptr<DaemonClient> m_pDaemonClient;
		//心跳专用
		boost::shared_ptr<DaemonClient> m_pheartDaemonClient;
		std::string & m_servername;
		serverPort m_DaemonPortInfo;
	};
}

#endif //__DAEMON_APP_H__

