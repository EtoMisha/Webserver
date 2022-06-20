#pragma once

#include <string>

class Config
{
	public:
		Config(	
			std::string host,
			int port,
			std::string homeDir,
			std::string homePage,
			int bufferSize
		);
		Config();
		~Config();
		Config(Config const & other);
		Config operator=(Config const & other);
		
		std::string getHost();
		int getPort();
		std::string getHomeDir();
		std::string getHomePage();
		int getBufferSize();

	private:
		std::string host;
		int port;
		std::string homeDir;
		std::string homePage;
		int bufferSize;

};