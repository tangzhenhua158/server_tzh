#pragma once
#ifndef __DAEMON_DAEMONSERVER_H__
#define __DAEMON_DAEMONSERVER_H__

#include "../../muduo/net/TcpServer.h"
#include "codec.h"
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Timestamp.h>
#include <muduo/base/Types.h>
#include <boost/bind.hpp>

#include <set>

using namespace muduo;
using namespace muduo::net;

class TestServer
{
public:
	TestServer(EventLoop* loop,const InetAddress& listenAddr);

	void start()
	{
		server_.start();
	}

private:
	void onConnection(const TcpConnectionPtr& conn);

	void onStringMessage(const TcpConnectionPtr&,const string& message,Timestamp time_s);

	typedef std::set<TcpConnectionPtr> ConnectionList;
	TcpServer server_;
	LengthHeaderCodec codec_;
	ConnectionList connections_;
};

#endif