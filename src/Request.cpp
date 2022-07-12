#include "../inc/Request.hpp"

Request::Request() {}

Request::Request(char *rawData, int size) : rawData(rawData), size(size)
{
	parseRequest(this->rawData);
}

Request::Request(Request const & other)
{
	this->method = other.method;
	this->url = other.url;
	this->httpVersion = other.httpVersion;
	this->headers = other.headers;
	this->bodyPOST = other.bodyPOST;
	this->filename = other.filename;
	this->type = other.type;
	this->size = other.size;
	memcpy(this->body, other.body, BUFFER_SIZE);
}

Request Request::operator=(Request const & other)
{
	this->method = other.method;
	this->url = other.url;
	this->httpVersion = other.httpVersion;
	this->headers = other.headers;
	this->bodyPOST = other.bodyPOST;
	this->filename = other.filename;
	this->type = other.type;
	this->size = other.size;
	memcpy(this->body, other.body, BUFFER_SIZE);
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

std::string const Request::getFilename() const
{
	return this->filename;
}

char* Request::getBody()
{
	return &(body[0]);
}

ReqType const Request::getType() const
{
	return this->type;
}

int Request::getSize() const
{
	return this->size;
}

std::map<std::string, std::string> &Request::getHeaders()
{
	return this->headers;
}

std::map<std::string, std::string> &Request::getBodyPOST()
{
	return this->bodyPOST;
}

void Request::setUrl(std::string url)
{
	this->url = url;
}

void Request::setFilename(std::string filename)
{
	this->filename = filename;
}

void Request::parseRequest(char *rawData)
{
						// std::cout << "*** Request ***\n" << rawData << "\n***\n";
	std::string rawLine = rawData;
	std::string line = rawLine.substr(0, rawLine.find('\n', 0));

	if (line.find("HTTP") != std::string::npos) {
		type = HEADERS;
		size_t start;
		size_t end = line.find(' ', 0);
		this->method = line.substr(0, end);

		start = end + 1;
		end = line.find(' ', start);
		this->url = line.substr(start + 1, end - start - 1);
		if (url != "" && url.at(url.length() - 1) == '/')
			this->url = this->url.substr(0, url.length() - 1);
	
		start = end + 1;
		end = line.length();
		this->httpVersion = rawLine.substr(start, end - start - 1);
		std::cout << this->method << " - " << this->url << std::endl;

		start = end + 1;
		end = rawLine.find('\n', start);
		while (end < rawLine.length() - 1 && line != "")
		{
			line = rawLine.substr(start, end - start - 1);
			int delimiter = line.find(':', 0);

			if (line.length() > 0)
			{
				std::string key = line.substr(0, delimiter);
				std::string value = line.substr(delimiter + 2, line.length());
				this->headers[key] = value;
			}
			start = end + 1;
			end = rawLine.find('\n', start);
		}
		if (this->method == "POST" && headers["Content-Type"] == "application/x-www-form-urlencoded")
			parseUrlencoded(rawData, start);
	} else
		parseMultipart(rawData);
	
	// std::cout << "parse request done \n";
}		

void Request::parseMultipart(char *rawData)
{
	std::string rawLine = rawData;
	std::string line = rawLine.substr(0, rawLine.find('\n', 0));
	if (line.find(BOUNDARY) != std::string::npos) {
		type = DATA_START;
		int found = rawLine.find("filename=");
		if (found != std::string::npos)
		{
			int start = found + 10;
			int end = rawLine.find("\"", start);
			filename = rawLine.substr(start, end - start);
			start = rawLine.find("\r\n\r\n", found) + 4;
			
			char *find = strstr(&rawData[start], BOUNDARY);
			int len;
			if (find == NULL)
				len = size - start;
			else
				len = find - &rawData[start] - 2;
			size = len;
			memcpy(body, &rawData[start], size);
		}
	} else {
		type = DATA_CONT;
		char *bound = strdup(BOUNDARY);
		void *find = memmem(rawData, size, bound, strlen(bound));
		free(bound);
		if (find == NULL)
			memcpy(body, rawData, size);
		else {
			int len = (char *)find - rawData - 2;
			size = len;
			memcpy(body, rawData, size);
		}
	}
}

void Request::parseUrlencoded(std::string rawData, int start)
{
	int end = rawData.length();
	std::string line = rawData.substr(start, end - start);
	start = 0;
	std::string subLine;
	while(line != "")
	{
		if(line.find('&', 0) != std::string::npos)
			subLine = line.substr(start, line.find('&', 0));
		else
			subLine = line;
		int delimiter = subLine.find('=', 0);
		std::string key = subLine.substr(0, delimiter);
		std::string value = subLine.substr(delimiter + 1, line.length());
		this->bodyPOST[key] = value;
		
		if (line.find('&', 0) != std::string::npos)
		{
			start = line.find('&', 0) + 1;
			end = line.length();
			line = line.substr(start, end - start);
		}
		else
			line = "";
	}
}