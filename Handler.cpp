#include "Handler.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

Handler::Handler() {}

Handler::Handler(Request req) : request(req) 
{
	if (request.getMethod() == "GET")
		methodGet();
	else if (request.getMethod() == "POST")
		methodPost();
	else if (request.getMethod() == "DELETE")
		methodDelete();
}

Handler::~Handler() {}

Response Handler::getResponse() const
{
	return this->response;
}

void Handler::methodGet()
{
	std::stringstream resp;
	std::stringstream resp_body;
		
	if (request.getUrl() == "")
		request.setUrl(HOME_PAGE);
	std::ifstream file(HOME_DIR + request.getUrl());
	if (file.is_open())
	{
		std::cout << "File opened OK" << std::endl;
		
		response.setStatusCode(200);		
		resp_body << file.rdbuf();
		response.setBody(resp_body.str());
		file.close();
	}
	else
	{
		std::cout << "Can't open file" << "|" << request.getUrl() << "|" << std::endl;
		response.setStatusCode(404);
	}
	response.setHttpVersion(request.getHttp());
	response.setHeaders("Version: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8");
}

void Handler::methodPost()
{

}

void Handler::methodDelete()
{

}

