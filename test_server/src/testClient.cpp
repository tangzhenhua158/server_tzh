
#include "testClient.h"

void TestClient::connect()
{
	client_.connect();
}

void TestClient::disconnect()
{
	client_.disconnect();
}

void TestClient::write(const StringPiece& message)
{
	MutexLockGuard lock(mutex_);
	if (connection_)
	{
		codec_.send(get_pointer(connection_), message);
	}
}

void TestClient::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << conn->localAddress().toIpPort() << " -> "
		<< conn->peerAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");

	MutexLockGuard lock(mutex_);
	if (conn->connected())
	{
		connection_ = conn;
	}
	else
	{
		connection_.reset();
	}
}

void TestClient::onStringMessage(const TcpConnectionPtr&,
	const string& message,
	Timestamp)
{
	printf("<<< %s\n", message.c_str());
}

int main()
{
	EventLoop loop;
	uint16_t port = 5080;
	InetAddress serverAddr(port);
	TestClient client(&loop, serverAddr);
	client.connect();
	
	double timeout = 2;
	loop.runEvery(timeout, boost::bind(&TestClient::write, &client,"hello to"));
	loop.loop();
}