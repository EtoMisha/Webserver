#pragma once

#include <iostream>
#include <map>

class Response
{
	public:
		Response();
		Response(std::string rawData);
		Response(Response const & other);
		~Response();

		Response operator=(Response const & other);

		std::string getStatusText();
		std::string getBodyFile();
		int getLength();

		void setStatusCode(int statusCode);
		void setStatusText();
		void setHttpVersion(std::string version);
		void setContentLength(int length);
		void setHeaders(std::string headers);
		void setBody(std::string body);


		std::string toString();

	private:
		std::string rawData;
		
		std::string httpVersion;
		int statusCode;
		std::string statusText;
		std::string headers;
		std::string bodyFile;
		int contentLength;
	
		std::map<int, std::string> codes;

		void initializeCodes();
};
