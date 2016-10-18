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

	m_addServers.push_back(*pServer.get());
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
		m_delServers.push_back(*it->second.get());
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

void DaemonServiceImpl::syncServer(::google::protobuf::RpcController* controller,
	const ::daemon_name::syncToServerReq* request,
	::daemon_name::syncToServerRsp* response,
	::google::protobuf::Closure* done)
{
	//更新数据

	LOG_INFO <<"DaemonServiceImpl "<< __FUNCTION__<<"  servername: "<<request->serinfo().servername();

	if(request->opt() == 1)
	{
		pShardServerInfo_t pServer(new serverInfo_t);
		pServer->ip = request->serinfo().ip();
		pServer->port = (uint16_t)request->serinfo().port();
		pServer->servername = request->serinfo().servername();
		pServer->serverid = request->serinfo().serverid();
		pServer->uLastUpdateTime = time(0);
	
		//master没法同步过来，需增加字段
		m_nameServers[pServer->servername].insert(pServer);
		m_allServers[pServer->serverid] = pServer;

		m_addServers.push_back(*pServer.get());
	}

	if(request->opt() == 2)
	{
		std::map<uint64_t,pShardServerInfo_t >::iterator it =  m_allServers.find(request->serinfo().serverid());
		if(it != m_allServers.end())
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
	}
	response->set_ret(daemon_name::ok);
	done->Run();
}

void DaemonServiceImpl::election(::google::protobuf::RpcController* controller,
	const ::daemon_name::electionMasterReq* request,
	::daemon_name::electionMasterRsp* response,
	::google::protobuf::Closure* done)
{
	//暂不实现
}

void DaemonServiceImpl::timeCheck()
{
	if(m_serverInfo.isMaster)
	{
		//master执行 ，由定时器执行此方法更新到其他daemon服务
		//std::vector<serverInfo_t>	m_addServers;
		//std::vector<serverInfo_t>	m_delServers;

		//过期的
		//checkTimeoutServer
		//syncServer();
	}
}

bool IsDelAll(const std::pair<uint64_t,pShardServerInfo_t > & info )
{
	return info.second->uLastUpdateTime > 60;
}

void DaemonServiceImpl::checkTimeoutServer()
{
	__foreach(it,m_allServers)
	{
		if ( (it->second)->uLastUpdateTime > 60)
		{
			m_delServers.push_back(*it->second);
		}	
	}

	//remove
	std::map<uint64_t,pShardServerInfo_t > tempAllServers;
	std::remove_copy_if(m_allServers.begin(),m_allServers.end(),std::inserter(tempAllServers,tempAllServers.begin()),IsDelAll);
	m_allServers.swap(tempAllServers);

	__foreach(itt,m_delServers)
	{
		std::map<std::string,std::set<pShardServerInfo_t> >::iterator it1 = m_nameServers.find( (*itt).servername);
		if(it1 != m_nameServers.end())
		{
			std::set<pShardServerInfo_t>::iterator it2 = it1->second.begin();
			for(;it2 != it1->second.end();)
			{
				if( (*it2)->serverid == (*itt).serverid )
				{
					it1->second.erase(it2++);
				}else
				{
					++it2;
				}
			}
		}
	}
	
	
}