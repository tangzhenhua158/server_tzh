#include "protocol_include/rpc.pb.h"
#include "protocol_include/daemon.pb.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/protorpc/RpcServer.h>

using namespace muduo;
using namespace muduo::net;

namespace daemon_name
{
	class DaemonServiceImpl : public DaemonService
	{
	public:
		virtual void Register(::google::protobuf::RpcController* controller,
			const ::daemon_name::registerReq* request,
			::daemon_name::registerRsp* response,
			::google::protobuf::Closure* done);

		void LoginOut(::google::protobuf::RpcController* controller,
			const ::daemon_name::loginOutReq* request,
			::daemon_name::loginOutRsp* response,
			::google::protobuf::Closure* done);

		void queryDaemonMaster(::google::protobuf::RpcController* controller,
			const ::daemon_name::queryDaemonMasterReq* request,
			::daemon_name::queryDaemonMasterRsp* response,
			::google::protobuf::Closure* done);
	};
}