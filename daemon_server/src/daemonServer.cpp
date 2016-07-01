#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "h/daemonServer.h"

using namespace daemon_name;

void DaemonServiceImpl::Register(::google::protobuf::RpcController* controller,
	const ::daemon_name::registerReq* request,
	::daemon_name::registerRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO <<"DaemonServiceImpl "<< __FUNCTION__<<"  servername: "<<request->servername();

	serverInfo_t server;
	server.ip = request->ip();
	server.port = request->port();
	server.exd = request->extend();
	server.servername = request->servername();
	server.serverid = id_.incrementAndGet();

	response->set_ret(daemon_name::ok);
	response->set_serverid(server.serverid);
	response->set_servername(server.servername);

	m_nameServers[server.servername].insert(server);
	m_allServers[server.serverid] = server;
	done->Run();
}

void DaemonServiceImpl::LoginOut(::google::protobuf::RpcController* controller,
	const ::daemon_name::loginOutReq* request,
	::daemon_name::loginOutRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO << __FUNCTION__;
	response->set_ret(daemon_name::ok);
	done->Run();
}

void DaemonServiceImpl::queryDaemonMaster(::google::protobuf::RpcController* controller,
	const ::daemon_name::queryDaemonMasterReq* request,
	::daemon_name::queryDaemonMasterRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO << __FUNCTION__;

	bool bfind = false;
	std::map<std::string,std::set<serverInfo_t> >::iterator it = m_nameServers.find(daemon_server);
	if(it != m_nameServers.end())
	{
		std::set<serverInfo_t>::iterator itt = it->second.begin();
		for(;itt != it->second.end() ; ++itt)
		{
			if(itt->exd == "master" )
			{
				bfind = true;
				::daemon_name::serverInfo *pserinfo = response->mutable_serinfo();
				pserinfo->set_ip(itt->ip );
				pserinfo->set_port(itt->port);
				pserinfo->set_serverid(itt->serverid );
				pserinfo->set_servername(itt->servername);
				break;
			}
		}
	}

	if(!bfind)
	{
		::daemon_name::serverInfo *pserinfo = response->mutable_serinfo();
		pserinfo->set_ip(htonl(inet_addr("0.0.0.0")) );
		pserinfo->set_port(htons(0));
		pserinfo->set_serverid(0);
		pserinfo->set_servername("");
	}
	
	done->Run();
}

void DaemonServiceImpl::heart(::google::protobuf::RpcController* controller,
	const ::daemon_name::heartReq* request,
	::daemon_name::heartRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO << __FUNCTION__<<" servername: "<<request->servername() << " serverid: "<< request->serverid();
	response->set_ret(daemon_name::ok);
	done->Run();
}