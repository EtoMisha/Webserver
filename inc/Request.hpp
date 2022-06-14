#pragma once
#include <iostream>
#include <map>

#define HOME_DIR "res/"

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

		std::map<std::string, std::string> getBodyPOST();

		void setUrl(std::string url);
		
	private:
		std::string method;
		std::string url;
		std::string httpVersion;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> bodyPOST; // post method
		std::string body;
		
		
		void parseRequest(std::string rawData);
};
