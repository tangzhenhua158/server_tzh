#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "boost/lexical_cast.hpp"

#include "h/daemonServer.h"

using namespace daemon_name;

void DaemonServiceImpl::Register(::google::protobuf::RpcController* controller,
	const ::daemon_name::registerReq* request,
	::daemon_name::registerRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO <<"DaemonServiceImpl "<< __FUNCTION__<<"  servername: "<<request->servername();

	pShardServerInfo_t pServer(new serverInfo_t);
	pServer->ip = request->ip();
	pServer->port = (uint16_t)request->port();
	//pServer->isMaster = request->extend();
	pServer->servername = request->servername();
	pServer->serverid = id_.incrementAndGet();
	pServer->uLastUpdateTime = time(0);

	response->set_ret(daemon_name::ok);
	response->set_serverid(pServer->serverid);
	response->set_servername(pServer->servername);

	if( (m_nameServers.find(pServer->servername) == m_nameServers.end()) && (pServer->servername == "daemonserver")) pServer->isMaster = 1;
	m_nameServers[pServer->servername].insert(pServer);
	m_allServers[pServer->serverid] = pServer;
	done->Run();
}

void DaemonServiceImpl::LoginOut(::google::protobuf::RpcController* controller,
	const ::daemon_name::loginOutReq* request,
	::daemon_name::loginOutRsp* response,
	::google::protobuf::Closure* done)
{
	LOG_INFO << __FUNCTION__;

	std::map<uint64_t,pShardServerInfo_t >::iterator it =  m_allServers.find(request->serverid());
	if(it != m_allServers.end()
		)
	{
		std::map<std::string,std::set<pShardServerInfo_t> >::iterator itt = m_nameServers.find((it->second)->servername);
		if(itt != m_nameServers.end())
		{
			std::set<pShardServerInfo_t>::iterator it2 = itt->second.begin();
			for(;it2 != itt->second.end(); )
			{
				if(it->second->serverid == (*it2)->serverid)
					itt->second.erase(it2++);
				else
					++it2;
			}
		}
		m_allServers.erase(it);
	}

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
	std::map<std::string,std::set<pShardServerInfo_t> >::iterator it = m_nameServers.find(daemon_server);
	if(it != m_nameServers.end())
	{
		std::set<pShardServerInfo_t>::iterator itt = it->second.begin();
		for(;itt != it->second.end() ; ++itt)
		{
			if((*itt)->isMaster == 1 )
			{
				bfind = true;
				::daemon_name::serverInfo *pserinfo = response->mutable_serinfo();
				pserinfo->set_ip((*itt)->ip );
				pserinfo->set_port((*itt)->port);
				pserinfo->set_serverid((*itt)->serverid );
				pserinfo->set_servername((*itt)->servername);
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

	std::map<uint64_t,pShardServerInfo_t >::iterator it;
	if( (it = m_allServers.find(request->serverid()) ) != m_allServers.end()
		)
	{
		it->second->uLastUpdateTime = time(0);
	}
	response->set_ret(daemon_name::ok);
	done->Run();
}

void DaemonServiceImpl::queryServer(::google::protobuf::RpcController* controller, 
	const ::daemon_name::queryServerReq* request,
	::daemon_name::queryServerRsp* response,
	::google::protobuf::Closure* done)
{
	std::map<std::string,std::set<pShardServerInfo_t> >::iterator it = m_nameServers.find(request->servername());
	uint32_t uIndex = 0 ;
	if(it != m_nameServers.end())
	{
		std::set<pShardServerInfo_t>::iterator itt = it->second.begin();
		for(;itt != it->second.end(); ++itt)
		{
			::daemon_name::serverInfo *pserinfo = response->mutable_serinfos(uIndex); ++uIndex;
			pserinfo->set_ip((*itt)->ip);
			pserinfo->set_port((*itt)->port);
			pserinfo->set_serverid((*itt)->serverid);
			pserinfo->set_servername((*itt)->servername);
		}

		
	}
	
}