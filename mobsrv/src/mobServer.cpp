
#include "MobServer.h"

MobServer::MobServer(EventLoop* loop,const InetAddress& listenAddr):
	server_(loop, listenAddr, "DaemonServer"),
	codec_(boost::bind(&MobServer::onStringMessage, this, _1, _2, _3))
{
	server_.setConnectionCallback(
		boost::bind(&MobServer::onConnection, this, _1));
	server_.setMessageCallback(
		boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
}

void MobServer::onConnection(const TcpConnectionPtr& conn)
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

void MobServer::onStringMessage(const TcpConnectionPtr& conPtr,const string& message,Timestamp time_s)
{
	/*for (ConnectionList::iterator it = connections_.begin();
		it != connections_.end();
		++it)
	{
		codec_.send(get_pointer(*it), message);
	}*/
	LOG_INFO<<"recvmsg:"<<message.c_str()<< conPtr.get();
}