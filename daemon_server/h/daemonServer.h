#include "protocol_include/rpc.pb.h"
#include "protocol_include/daemon.pb.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/protorpc/RpcServer.h>

using namespace muduo;
using namespace muduo::net;

namespace daemon
{
	class DaemonServiceImpl : public DaemonService
	{
	public:
		virtual void Register(::google::protobuf::RpcController* controller,
			const ::daemon::registerReq* request,
			::daemon::registerRsp* response,
			::google::protobuf::Closure* done);

		void LoginOut(::google::protobuf::RpcController* controller,
			const ::daemon::loginOutReq* request,
			::daemon::loginOutRsp* response,
			::google::protobuf::Closure* done);
	};
}