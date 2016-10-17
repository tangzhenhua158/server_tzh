#ifndef __DAEMON_APP_H__
#define __DAEMON_APP_H__

/*****************************************************************************
*daemonclient ��װ��
*app�������һ̨deamon server
*�Ӹ�daemon_server�в�ѯmaster daemon������masterdaemon ��������������ע��
*�������daemonserver��ȡ�����б�
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
		//��ȡmaster����ȡ�б�
		boost::shared_ptr<DaemonClient> m_pDaemonClient;
		//����ר��
		boost::shared_ptr<DaemonClient> m_pheartDaemonClient;
		std::string  m_servername;
		serverPort m_DaemonPortInfo;
		Client_State_t  m_conn_state;
		std::vector<std::string >	m_mapHost;
		TimerId						m_timerId;
	};
}

#endif //__DAEMON_APP_H__

