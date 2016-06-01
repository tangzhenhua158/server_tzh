
#include "protocol_include/rpc.pb.h"
#include "protocol_include/login.pb.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/protorpc/RpcServer.h>

using namespace muduo;
using namespace muduo::net;

namespace user
{

	class LoginServiceImpl : public LoginService
	{
	public:
		virtual void Login(::google::protobuf::RpcController* controller,
			const ::user::LoginReq* request,
			::user::LoginRsp* response,
			::google::protobuf::Closure* done)
		{
			//LOG_INFO << "EchoServiceImpl::Solve";
			//response->set_payload(request->payload());
			response->set_uid(request->uid());
			response->set_username(request->username());
			response->set_status(1);
			done->Run();
		}
	};

}