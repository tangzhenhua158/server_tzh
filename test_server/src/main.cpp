#include "testServer.h"
#include <map>
using namespace std;


typedef boost::function<void ()> funptr;

void test()
{
	//
}
int main(int argc ,char *argv[])
{

	EventLoop loop;
	uint16_t port = 5080;
	InetAddress serverAddr(port);
	TestServer server(&loop, serverAddr);
	server.start();
	loop.loop();

	//funptr p1 = boost::bind(test);
	//p1();
	return 0;
}
