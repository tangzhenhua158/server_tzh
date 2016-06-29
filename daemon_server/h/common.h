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

class IApp
{
public:
	virtual ~IApp(){}
	virtual void notifyMasterDaemon(const serverPort & portinfo)=0;
};




#endif //__DAEMON_COMM_H__

