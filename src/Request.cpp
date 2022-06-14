#include "../inc/Request.hpp"

Request::Request() {}

Request::Request(std::string rawData)
{
	parseRequest(rawData);
}

Request::Request(Request const & other)
{
	this->method = other.method;
	this->url = other.url;
	this->httpVersion = other.httpVersion;
	this->headers = other.headers;
	this->body = other.body;
}

Request Request::operator=(Request const & other)
{
	this->method = other.method;
	this->url = other.url;
	this->httpVersion = other.httpVersion;
	this->headers = other.headers;
	this->body = other.body;
	return *this;
}

Request::~Request() {}
		
std::string const Request::getMethod() const
{
	return this->method;
}

std::string const Request::getUrl() const
{
	return this->url;
}

std::string const Request::getHttp() const
{
	return this->httpVersion;
}

std::map<std::string, std::string> Request::getBodyPOST()
{
	return this->bodyPOST;
}

void Request::setUrl(std::string url)
{
	this->url = url;
}


void Request::parseRequest(std::string rawData)
{
	std::string line = rawData.substr(0, rawData.find('\n', 0));
	size_t start;
	size_t end = line.find(' ', 0);
	this->method = line.substr(0, end);

	start = end + 1;
	end = line.find(' ', start);
	std::string homeDir = HOME_DIR;
	this->url = homeDir + line.substr(start + 1, end - start - 1);

	start = end + 1;
	end = line.length();
	this->httpVersion = rawData.substr(start, end - start - 1);
	// std::cout << this->method << " - " << this->url << " - " << this->httpVersion << std::endl;

	start = end + 1;
	end = rawData.find('\n', start);
	while (end < rawData.length() - 1 && line != "")
	{
		line = rawData.substr(start, end - start - 1);
		int delimiter = line.find(':', 0);

		if (line.length() > 0)
		{
			std::string key = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 2, line.length());
			// std::cout << "- " << key << " - " << value << std::endl;
			this->headers[key] = value;
		}	
		start = end + 1;
		end = rawData.find('\n', start);
	}
	//  для метода POST надо еще спарсить body
	if(this->method == "POST")
	{
		start++;
		while (end < rawData.length() - 1 && line != "")
		{
			line = rawData.substr(start, end - start - 1);
			std::cout << "parsing line " << line << std::endl; //ex: key=val&key1=val1&key2=val2\n

			std::string subLine = line.substr(start, line.find('&', 0));
			std::cout << "sub line " << subLine << std::endl; //ex: key=val
			int delimiter = subLine.find('=', 0);
			std::string key = subLine.substr(0, delimiter);
			std::string value = subLine.substr(delimiter + 2, line.length());
			this->bodyPOST[key] = value;
			
			start = line.find('&', 0) + 1;
			end = rawData.find('\n', start);
		}
	}
}
