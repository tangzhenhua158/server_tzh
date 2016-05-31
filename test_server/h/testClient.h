#include "codec.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#include <iostream>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

class TestClient : boost::noncopyable
{
public:
	TestClient(EventLoop* loop, const InetAddress& serverAddr)
		: client_(loop, serverAddr, "TestClient"),
		codec_(boost::bind(&TestClient::onStringMessage, this, _1, _2, _3))
	{
		client_.setConnectionCallback(
			boost::bind(&TestClient::onConnection, this, _1));
		client_.setMessageCallback(
			boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
		client_.enableRetry();
	}

	void connect();

	void disconnect();

	void write(const StringPiece& message);

private:
	void onConnection(const TcpConnectionPtr& conn);

	void onStringMessage(const TcpConnectionPtr&,
		const string& message,
		Timestamp);

	TcpClient client_;
	LengthHeaderCodec codec_;
	MutexLock mutex_;
	TcpConnectionPtr connection_;
};