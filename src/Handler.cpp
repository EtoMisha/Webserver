#include "../inc/Handler.hpp"
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

// void Handler::readFile(unsigned char* buffer, size_t size, const char* file_path)
// {
// 	FILE* fin = fopen(file_path, "rb");  // r for read, b for binary
// 	if (fin == NULL) {
// 		printf("open %s failed.", file_path);
// 		return;
// 	}

// 	fread(buffer, sizeof(unsigned char), size, fin); // read sizeof(buffer) elements to our buffer
// }

void Handler::methodGet()
{
	// unsigned char *buffer;
	
	// std::stringstream resp;
	// std::stringstream resp_body;
		
	
	if (request.getUrl() == HOME_DIR)
		request.setUrl(request.getUrl() + HOME_PAGE);
	const char *file_path = request.getUrl().c_str();
	FILE* file = fopen(file_path, "rb");  // r for read, b for binary
	if (file == NULL)
	{
		std::cout << "Can't open file" << request.getUrl() << std::endl;
		response.setStatusCode(404);
	}
	else 
	{
		std::cout << "File opened OK" << std::endl;
		response.setStatusCode(200);
		
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);

		response.setContentLength(size);
					std::cout << "CONTENT LENGTH - " << response.getLength() << std::endl;
					std::cout << "URL - " << request.getUrl() << std::endl;
		response.setBody(request.getUrl());
		// resp_body << file.rdbuf();
		// response.setBody(resp_body.str());
		fclose(file);
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

