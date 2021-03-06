#ifndef __MOBSRV_APP_H__
#define __MOBSRV_APP_H__

#include <muduo/net/EventLoop.h>
#include "common.h"

using namespace muduo;
using namespace muduo::net;


class DaemonClient;
//class EventLoop;

namespace mobsrv
{
	const std::string servername = "mobsrv";
	class App:public IApp
	{
	public:
		App(EventLoop *pLoop,const std::string& servername,const InetAddress & address);
		~App();
		void start();
		void onQueryMasterDaemonRsp(const serverPort & portinfo);
		void onDaemonConnect();
		void onMasterDaemonConnect();
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
		std::string  m_servername;
		serverPort m_DaemonPortInfo;
		Client_State_t  m_conn_state;
	};
}

#endif //__MOBSRV_APP_H__

