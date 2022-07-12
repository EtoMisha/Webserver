#pragma once
#include <iostream>
#include <map>
#include <sys/stat.h>

#include "Server.hpp"

#define BUFFER_SIZE 3000
#define BOUNDARY "------WebKitFormBoundary"

enum ReqType
{
	HEADERS, DATA_START, DATA_CONT, DATA_END
};

class Request
{
	public:
		Request();
		Request(char *rawData, int size);
		Request(Request const & other);
		~Request();

		Request operator=(Request const & other);
		
		std::string const getMethod() const;
		std::string const getUrl() const;
		std::string const getHttp() const;
		std::string const getFilename() const;
		char *getBody();
		ReqType const getType() const;
		int getSize() const;

		std::map<std::string, std::string> &getHeaders();
		std::map<std::string, std::string> &getBodyPOST();

		void setUrl(std::string url);
		void setFilename(std::string filename);

		int check();
		
	private:
		char *rawData;
		int size;
		ft::Server server;
		
		std::string method;
		std::string url;
		std::string httpVersion;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> bodyPOST; // post method
		char body[BUFFER_SIZE];
		std::string filename;
		ReqType type;		
		
		void parseRequest(char *rawData);
		void parseUrlencoded(std::string rawData, int start);
		void parseMultipart(char *rawData);
};
