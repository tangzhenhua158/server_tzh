#ifndef __DAEMON_APP_H__
#define __DAEMON_APP_H__

class DaemonClient;
class EventLoop;

namespace daemon
{
	class App
	{
	public:
		App(EventLoop *pLoop);
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

