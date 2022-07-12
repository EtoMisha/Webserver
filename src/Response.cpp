#include "../inc/Response.hpp"
#include <sstream>

Response::Response()
{
	initializeCodes();
	initializeContentTypes();
	this->sent = true;
	contentLength = 0;
	bodyFile = "";
}

Response::~Response() {}


Response::Response(Response const & other)
{
	this->httpVersion = other.httpVersion;
	this->statusCode = other.statusCode;
	this->statusText = other.statusText;
	this->headers = other.headers;
	this->bodyFile = other.bodyFile;
	this->contentLength = other.contentLength;
	this->sent = other.sent;
	this->codes = other.codes;
	this->contentTypes = other.contentTypes;
}

Response & Response::operator=(Response const & other)
{
	this->httpVersion = other.httpVersion;
	this->statusCode = other.statusCode;
	this->statusText = other.statusText;
	this->headers = other.headers;
	this->bodyFile = other.bodyFile;
	this->contentLength = other.contentLength;
	this->sent = other.sent;
	this->codes = other.codes;
	this->contentTypes = other.contentTypes;
	return *this;
}

void Response::setStatusCode(int statusCode)
{
	this->statusCode = statusCode;
	this->statusText = codes[statusCode];
}

std::string Response::getStatusText()
{
	return this->statusText;
}

int Response::getLength()
{
	return this->contentLength;
}

int Response::getStatus()
{
	return this->statusCode;
}

std::string Response::getBodyFile()
{
	return this->bodyFile;
}

std::string Response::getHeaders()
{
	return this->headers;
}

std::string Response::getContentType(std::string ext)
{
	return this->contentTypes[ext];
}

bool Response::isSent()
{
	return this->sent;
}

void Response::setHttpVersion(std::string version)
{
	this->httpVersion = version;
}

void Response::setContentLength(int length)
{
	this->contentLength = length;
}

void Response::setHeaders(std::string headers)
{
	this->headers = headers;
}

void Response::setBodyFile(std::string body)
{
	this->bodyFile = body;
}

void Response::setSent(bool sent)
{
	this->sent = sent;
}

std::string Response::toString()
{
	std::stringstream resp;
	std::string delimiter = "\r\n";
	resp << httpVersion << " " << statusCode << " " << this->getStatusText() << delimiter
	<< headers << delimiter
	<< "Content-Length: " << this->getLength()
	<< delimiter << delimiter;

	return resp.str();
}

void Response::initializeCodes()
{
	// Информационные
	codes[100] = "Continue";
	codes[101] = "Switching Protocol";
	codes[102] = "Processing";
	codes[103] = "Early Hints";
	
	// Успешные
	codes[200] = "OK";
	codes[201] = "Created";
	codes[202] = "Accepted";
	codes[203] = "Non-Authoritative Information";
	codes[204] = "No Content";
	codes[205] = "Reset Content";
	codes[206] = "Partial Content";
	
	// Сообщения о перенаправлениях
	codes[300] = "Multiple Choice";
	codes[301] = "Moved Permanently";
	codes[302] = "Found";
	codes[303] = "See Other";
	codes[304] = "Not Modified";
	codes[305] = "Use Proxy";
	codes[306] = "Switch Proxy";
	codes[307] = "Temporary Redirect";
	codes[308] = "Permanent Redirect";
	
	// Клиентские ошибки
	codes[400] = "Bad Request";
	codes[401] = "Unauthorized";
	codes[402] = "Payment Required";
	codes[403] = "Forbidden";
	codes[404] = "Not Found";
	codes[405] = "Method Not Allowed";
	codes[406] = "Not Acceptable";
	codes[407] = "Proxy Authentication Required";
	codes[408] = "Request Timeout";
	codes[409] = "Conflict";
	codes[410] = "Gone";
	codes[411] = "Length Required";
	codes[412] = "Precondition Failed";
	codes[413] = "Request Entity Too Large";
	codes[414] = "Request-URI Too Long";
	codes[415] = "Unsupported Media Type";
	codes[416] = "Requested Range Not Satisfiable";
	codes[417] = "Expectation Failed";

	// Серверные ошибки
	codes[500] = "Internal Server Error";
	codes[501] = "Not Implemented";
	codes[502] = "Bad Gateway";
	codes[503] = "Service Unavailable";
	codes[504] = "Gateway Timeout";
	codes[505] = "HTTP Version Not Supported";
}

void Response::initializeContentTypes()
{
	contentTypes["gif"] = "image/gif";
	contentTypes["jpg"] = "image/jpeg";
	contentTypes["jpeg"] = "image/jpeg";
	contentTypes["png"] = "image/png";
	contentTypes["svg"] = "image/svg+xml";
	contentTypes["webp"] = "image/webp";
	contentTypes["ico"] = "image/vnd.microsoft.icon";

	contentTypes["css"] = "text/css";
	contentTypes["csv"] = "text/csv";
	contentTypes["html"] = "text/html";
	contentTypes["htm"] = "text/html";
	// contentTypes["php"] = "text/php";
	contentTypes["xml"] = "text/xml";
	contentTypes["htm"] = "text/html";
	contentTypes["pdf"] = "application/pdf";
}
