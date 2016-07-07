#ifndef __DAEMON_SERVERCONFIG_H__
#define  __DAEMON_SERVERCONFIG_H__
#pragma once

#include <vector>
#include <string>

namespace daemon_name
{
	struct connInfo
	{
		std::string ip;
		uint16_t port;
		std::string name;

		connInfo():ip("")
			,port(0),name("")
		{
		}
	};

	class serverConfig
	{
	public:
		serverConfig(std::string strConfig = "../etc/config.xml");
		void parse();
		void getkey(std::string strkey, std::string & strValue);
	private:
		std::vector<connInfo> m_vecConnInfos;
		uint16_t			  m_serverPort;
		uint16_t			  m_consolPort;
		std::string			  m_strConfigFile;
	};
}

#endif //__DAEMON_SERVERCONFIG_H__