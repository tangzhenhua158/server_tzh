#include "protocol_include/rpc.pb.h"
#include "protocol_include/daemon.pb.h"

#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/protorpc/RpcChannel.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include "h/common.h"

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;


typedef boost::function<void ()> connectCallBack_t;

namespace daemon_name
{
class DaemonClient : boost::noncopyable
{
public:

	DaemonClient(EventLoop* loop,
		const InetAddress& serverAddr)
	:client_(loop, serverAddr, "DaemonClient"),
		channel_(new RpcChannel),
		stub_(get_pointer(channel_))
	{
		LOG_INFO <<"DaemonClient "<< __FUNCTION__ << " runing "<< " channel<<"<<channel_.get();
		client_.setConnectionCallback(
			boost::bind(&DaemonClient::onConnection, this, _1));
		client_.setMessageCallback(
			boost::bind(&RpcChannel::onMessage, get_pointer(channel_), _1, _2, _3));
		// client_.enableRetry();
	}

	void connect()
	{
		client_.connect();
	}

	void disconnect()
	{
		client_.disconnect();
	}

	void setAppClient(IApp * p){m_AppClient = p;}

	void register_server(const daemon_name::registerReq & req, daemon_name::registerRsp* rsp)
	{
		LOG_INFO <<"DaemonClient "<< __FUNCTION__;
		stub_.Register(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleRegister, rsp));
	}

	void LoginOut(const daemon_name::loginOutReq & req, daemon_name::loginOutRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.LoginOut(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleLoginOut, rsp));
	}

	void queryMasterDaemon(const daemon_name::queryDaemonMasterReq & req, daemon_name::queryDaemonMasterRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.queryDaemonMaster(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleQueryMasterDaemon, rsp));
	}

	void heart(const daemon_name::heartReq & req, daemon_name::heartRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.heart(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleHeart, rsp));
	}

	void queryServer(const daemon_name::queryServerReq & req, daemon_name::queryServerRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.queryServer(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleQueryServer, rsp));
	}

	void syncServer(const daemon_name::syncToServerReq & req, daemon_name::syncToServerRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.syncServer(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleSyncServer, rsp));
	}

	void election(const daemon_name::electionMasterReq & req, daemon_name::electionMasterRsp * rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.election(NULL,&req,rsp,NewCallback(this, &DaemonClient::_handleElection, rsp));
	}

	void setConnectCallback(connectCallBack_t _callback){m_connCallback = _callback;}
private:
	void onConnection(const TcpConnectionPtr& conn)
	{
		if (conn->connected())
		{
			//channel_.reset(new RpcChannel(conn));
			conn->setTcpNoDelay(true);
			channel_->setConnection(conn);

			if(m_connCallback) m_connCallback();
		}
	}


	void _handleRegister(daemon_name::registerRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}

	void _handleLoginOut(daemon_name::loginOutRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}

	void _handleQueryMasterDaemon(daemon_name::queryDaemonMasterRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
		if(m_AppClient)
		{
			serverPort portInfo;
			portInfo.ip = resp->serinfo().ip();
			portInfo.port = resp->serinfo().port();
			portInfo.servername = resp->serinfo().servername();
			m_AppClient->notifyMasterDaemon(portInfo);
		}
	}

	void _handleHeart(daemon_name::heartRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}

	void _handleQueryServer(daemon_name::queryServerRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}

	void _handleSyncServer(daemon_name::syncToServerRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}

	void _handleElection(daemon_name::electionMasterRsp* resp)
	{

		LOG_INFO << "DaemonClient " << __FUNCTION__ << " finished";
	}


	// EventLoop* loop_;
	TcpClient client_;
	RpcChannelPtr channel_;
	daemon_name::DaemonService::Stub stub_;
	IApp * m_AppClient;
	connectCallBack_t m_connCallback;

};
}

