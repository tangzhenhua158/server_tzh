#include "h/daemonServer.h"

using namespace daemon;

void DaemonServiceImpl::Register(::google::protobuf::RpcController* controller,
	const ::daemon::registerReq* request,
	::daemon::registerRsp* response,
	::google::protobuf::Closure* done)
{
	done->Run();
}

void DaemonServiceImpl::LoginOut(::google::protobuf::RpcController* controller,
	const ::daemon::loginOutReq* request,
	::daemon::loginOutRsp* response,
	::google::protobuf::Closure* done)
{
	done->Run();
}