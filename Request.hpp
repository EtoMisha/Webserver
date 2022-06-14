#pragma once
#include <iostream>
#include <map>

#define HOME_DIR "files/"

class Request
{
	public:
		Request();
		Request(std::string rawData);
		Request(Request const & other);
		~Request();

		Request operator=(Request const & other);
		
		std::string const getMethod() const;
		std::string const getUrl() const;
		std::string const getHttp() const;

		void setUrl(std::string url);
		
	private:
		std::string method;
		std::string url;
		std::string httpVersion;
		std::map<std::string, std::string> headers;
		std::string body;
		
		
		void parseRequest(std::string rawData);
};
