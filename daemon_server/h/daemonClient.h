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

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;


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

	void register_server(const daemon_name::registerReq & req, daemon_name::registerRsp & rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.Register(NULL,&req,&rsp,NewCallback(this, &DaemonClient::_handleRegister, &rsp));
	}

	void LoginOut(const daemon_name::loginOutReq & req, daemon_name::loginOutRsp & rsp)
	{
		LOG_INFO << __FUNCTION__;
		stub_.LoginOut(NULL,&req,&rsp,NewCallback(this, &DaemonClient::_handleLoginOut, &rsp));
	}

private:
	void onConnection(const TcpConnectionPtr& conn)
	{
		if (conn->connected())
		{
			//channel_.reset(new RpcChannel(conn));
			conn->setTcpNoDelay(true);
			channel_->setConnection(conn);
		}
	}


	void _handleRegister(daemon_name::registerRsp* resp)
	{

		LOG_INFO << "DaemonClient " << this << " finished";
	}

	void _handleLoginOut(daemon_name::loginOutRsp* resp)
	{

		LOG_INFO << "DaemonClient " << this << " finished";
	}

	// EventLoop* loop_;
	TcpClient client_;
	RpcChannelPtr channel_;
	daemon_name::DaemonService::Stub stub_;

};
}

