
#include "testServer.h"

TestServer::TestServer(EventLoop* loop,const InetAddress& listenAddr):
	server_(loop, listenAddr, "DaemonServer"),
	codec_(boost::bind(&TestServer::onStringMessage, this, _1, _2, _3))
{
	server_.setConnectionCallback(
		boost::bind(&TestServer::onConnection, this, _1));
	server_.setMessageCallback(
		boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
}

void TestServer::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
		<< conn->peerAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");

	if (conn->connected())
	{
		connections_.insert(conn);
	}
	else
	{
		connections_.erase(conn);
	}
}

void TestServer::onStringMessage(const TcpConnectionPtr& conPtr,const string& message,Timestamp time_s)
{
	/*for (ConnectionList::iterator it = connections_.begin();
		it != connections_.end();
		++it)
	{
		codec_.send(get_pointer(*it), message);
	}*/
	LOG_INFO<<"recvmsg:"<<message.c_str()<< conPtr.get();
}