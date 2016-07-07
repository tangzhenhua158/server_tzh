
#include "h/serverConfig.h"
#include "tinyxml.h"

namespace daemon_name
{
	serverConfig::serverConfig(std::string strConfig)
	{
		m_strConfigFile = strConfig;
	}

	void serverConfig::parse()
	{
		TiXmlDocument xmlDocument;
		assert(xmlDocument.LoadFile(m_strConfigFile.c_str()));
		TiXmlElement * xmlElement = xmlDocument.RootElement();
		

	}

	void serverConfig::getkey(std::string strkey, std::string & strValue)
	{

	}


}