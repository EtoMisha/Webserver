#include "../inc/Config.hpp"

Config::Config() {}

Config::Config(	
			std::string host,
			int port,
			std::string homeDir,
			std::string homePage,
			int bufferSize
		)
{
	this->host = host;
	this->port = port;
	this->homeDir = homeDir;
	this->homePage = homePage;
	this->bufferSize = bufferSize;
}

Config::~Config() {}

Config::Config(Config const & other)
{
	this->host = other.host;
	this->port = other.port;
	this->homeDir = other.homeDir;
	this->homePage = other.homePage;
	this->bufferSize = other.bufferSize;
}

Config Config::operator=(Config const & other)
{
	*this = Config(other);
	return *this;
}

std::string Config::getHost()
{
	return this->host;
}

int Config::getPort()
{
	return this->port;
}

std::string Config::getHomeDir()
{
	return this->homeDir;
}

std::string Config::getHomePage()
{
	return this->homePage;
}

int Config::getBufferSize()
{
	return this->bufferSize;
}