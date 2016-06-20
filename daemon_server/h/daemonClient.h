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


namespace daemon
{
class DaemonClient : boost::noncopyable
{
public:

	DaemonClient(EventLoop* loop,
		const InetAddress& serverAddr)
		: // loop_(loop),
	client_(loop, serverAddr, "DaemonClient"),
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

	void register(const daemon::registerReq & req, daemon::registerRsp & rsp)
	{
		stub_.Register(NULL,&req,&rsp,NewCallback(this, &DaemonClient::_handleRegister, rsp));
	}

	void LoginOut(const daemon::loginOutReq & req, daemon::loginOutRsp & rsp)
	{
		stub_.LoginOut(NULL,&req,&rsp,NewCallback(this, &DaemonClient::_handleLoginOut, rsp));
	}

private:
	void onConnection(const TcpConnectionPtr& conn)
	{
		if (conn->connected())
		{
			//channel_.reset(new RpcChannel(conn));
			conn->setTcpNoDelay(true);
			channel_->setConnection(conn);
			allConnected_->countDown();
		}
	}


	void _handleRegister(daemon::registerRsp* resp)
	{

		LOG_INFO << "DaemonClient " << this << " finished";
	}

	void _handleLoginOut(daemon::loginOutRsp* resp)
	{

		LOG_INFO << "DaemonClient " << this << " finished";
	}

	// EventLoop* loop_;
	TcpClient client_;
	RpcChannelPtr channel_;
	daemon::DaemonService::Stub stub_;

};
}

