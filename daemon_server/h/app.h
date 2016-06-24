#ifndef __DAEMON_APP_H__
#define __DAEMON_APP_H__

#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;


class DaemonClient;
//class EventLoop;

namespace daemon_name
{
	class App
	{
	public:
		App(EventLoop *pLoop,const InetAddress & address);
		~App();
		void start();
	private:
		void timer();
		void init();
	private:
		EventLoop *m_pLoop;
		boost::shared_ptr<DaemonClient> m_pDaemonClient;
	};
}

#endif //__DAEMON_APP_H__

