#include "h/daemonServer.h"

using namespace daemon_name;

void DaemonServiceImpl::Register(::google::protobuf::RpcController* controller,
	const ::daemon_name::registerReq* request,
	::daemon_name::registerRsp* response,
	::google::protobuf::Closure* done)
{
	response->set_ret(daemon_name::ok);
	response->set_serverid(121313);
	done->Run();
}

void DaemonServiceImpl::LoginOut(::google::protobuf::RpcController* controller,
	const ::daemon_name::loginOutReq* request,
	::daemon_name::loginOutRsp* response,
	::google::protobuf::Closure* done)
{
	response->set_ret(daemon_name::ok);
	done->Run();
}