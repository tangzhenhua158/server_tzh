#ifndef __DAEMON_COMM_H__
#define __DAEMON_COMM_H__

#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

struct serverPort
{
	uint32_t ip;
	uint16_t port;
	std::string   servername;

	void operator=(const serverPort & portInfo)
	{
		ip = portInfo.ip;
		port = portInfo.port;
		servername = portInfo.servername;
	}
};

struct serverInfo_t
{
	uint32_t serverid;
	uint32_t ip;
	uint16_t port;
	std::string   servername;
	std::string   exd;

	void operator=(const serverInfo_t & server)
	{
		ip = server.ip;
		port = server.port;
		servername = server.servername;
		exd = server.exd;
	}

	bool operator <(const serverInfo_t & server) const
	{
		return serverid < server.serverid;
	}

	/*bool operator <( serverInfo_t  server)
	{
		return serverid < server.serverid;
	}*/
};

struct Client_State_t
{
	enum state_ret{UNCONNECT = 0,CONNECT,OK,FAIL};
	uint32_t connect_daemon;
	uint32_t connect_master_daemon;
	uint32_t regMasterDaemon;
	uint32_t heart_state;
	uint32_t unRecvHeartRspCount;

	Client_State_t():connect_daemon(UNCONNECT),
		connect_master_daemon(UNCONNECT),regMasterDaemon(FAIL),
		unRecvHeartRspCount(0),heart_state(FAIL)
	{

	}
};

class IApp
{
public:
	virtual ~IApp(){}
	virtual void onQueryMasterDaemonRsp(const serverPort & portinfo)=0;
};




#endif //__DAEMON_COMM_H__

