#include "../inc/Handler.hpp"
#include "../inc/Request.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <fcntl.h>

Handler::Handler() {}

Handler::Handler(Request req) : request(req) 
{
	std::cout << "METHOD: " << request.getMethod() << std::endl;
	
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
	this->returnFile();
}

void Handler::methodPost()
{
	std::cout << "hello I'm POST meth" << std::endl;
	std::ofstream file(request.getUrl() + ".txt");
	std::cout << request.getBodyPOST().begin()->first << std::endl;

	std::map<std::string, std::string>::iterator it;
	for (it = request.getBodyPOST().begin(); it != request.getBodyPOST().end(); it++)
	{
		std::cout << it->first << " - " << it->second << std::endl;
		file << it->first
				<< ':'
				<< it->second 
				<< std::endl;
	}
	this->returnFile();
}

void Handler::methodDelete()
{

}

void Handler::returnFile()
{
	if (request.getUrl() == HOME_DIR)
		request.setUrl(request.getUrl() + HOME_PAGE);
	const char *file_path = request.getUrl().c_str();
	FILE* file = fopen(file_path, "rb");
	if (file == NULL)
	{
		std::cout << "Can't open file" << request.getUrl() << std::endl;
		response.setStatusCode(404);
	}
	else 
	{
		std::cout << "File opened OK, URL: " << request.getUrl() << std::endl;
		response.setStatusCode(200);
		
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);

		response.setContentLength(size);
		response.setBody(request.getUrl());
		
		fclose(file);
	}
	response.setHttpVersion(request.getHttp());
	response.setHeaders("Version: HTTP/1.1");//\r\nContent-Type: text/html; charset=utf-8");
}
