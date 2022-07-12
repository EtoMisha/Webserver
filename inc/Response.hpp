#pragma once

#include <iostream>
#include <map>

class Response
{
	public:
		Response();
		Response(Response const & other);
		~Response();

		Response &operator=(Response const & other);

		std::string getStatusText();
		std::string getBodyFile();
		std::string getHeaders();
		int getLength();
		int getStatus();
		std::string getContentType(std::string ext);
		bool isSent();

		void setStatusCode(int statusCode);
		void setStatusText();
		void setHttpVersion(std::string version);
		void setContentLength(int length);
		void setHeaders(std::string headers);
		void setBodyFile(std::string body);
		void setSent(bool sent);

		int fd;
		std::string toString();

	private:
		
		std::string httpVersion;
		int statusCode;
		std::string statusText;
		std::string headers;
		std::string bodyFile;
		int contentLength;

		bool sent;
	
		std::map<int, std::string> codes;
		std::map<std::string, std::string> contentTypes;

		

		void initializeCodes();
		void initializeContentTypes();
};
