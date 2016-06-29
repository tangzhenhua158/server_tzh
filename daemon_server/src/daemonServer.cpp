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
	response->set_ret(daemon_name::ok);
	response->set_serverid(121313);
	response->set_servername("ssss");
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
	::daemon_name::serverInfo *pserinfo = response->mutable_serinfo();
	pserinfo->set_ip(htonl(inet_addr("127.0.0.1")) );
	pserinfo->set_port(htons(8888));
	pserinfo->set_serverid(1234234);
	pserinfo->set_servername("daemonserver");
	done->Run();
}