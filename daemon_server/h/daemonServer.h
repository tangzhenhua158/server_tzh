#include "protocol_include/rpc.pb.h"
#include "protocol_include/daemon.pb.h"
#include "common.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/protorpc/RpcServer.h>
#include <map>
#include <set>

using namespace muduo;
using namespace muduo::net;

namespace daemon_name
{
	const std::string daemon_server = "daemonserver";
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

		void heart(::google::protobuf::RpcController* controller,
			const ::daemon_name::heartReq* request,
			::daemon_name::heartRsp* response,
			::google::protobuf::Closure* done);

		void queryServer(::google::protobuf::RpcController* controller, 
			const ::daemon_name::queryServerReq* request,
			::daemon_name::queryServerRsp* response,
			::google::protobuf::Closure* done);

		virtual void syncServer(::google::protobuf::RpcController* controller,
			const ::daemon_name::syncToServerReq* request,
			::daemon_name::syncToServerRsp* response,
			::google::protobuf::Closure* done);

		virtual void election(::google::protobuf::RpcController* controller,
			const ::daemon_name::electionMasterReq* request,
			::daemon_name::electionMasterRsp* response,
			::google::protobuf::Closure* done);

		void timeCheck();
		void checkTimeoutServer();

	private:
		std::map<std::string,std::set<pShardServerInfo_t> >m_nameServers;
		std::map<uint64_t,pShardServerInfo_t > m_allServers;
		std::vector<serverInfo_t>	m_addServers;
		std::vector<serverInfo_t>	m_delServers;
		AtomicInt32 id_;
		serverInfo_t				m_serverInfo;
	};
}