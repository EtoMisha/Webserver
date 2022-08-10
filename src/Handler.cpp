#include "../inc/Handler.hpp"
#include "../inc/CGI.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

#include <fcntl.h>

#include <unistd.h>

// Handler::Handler() {}

Handler::Handler(Request & req, ft::Server server) : request(req), server(server)
{
	response.setHttpVersion(request.getHttp());
	response.setHeaders("Version: HTTP/1.1");
	
	if (request.getType() != HEADERS)
		saveFile();
	else {
		checkLocation();
		if (checkCGI()) 
			runCGI();
		else if (request.getMethod() == "GET")
			methodGet();
		else if (request.getMethod() == "POST")
			methodPost();
		else if (request.getMethod() == "DELETE")
			methodDelete();
		if (response.getStatus() > 400)
			returnErrorPage();
	}
}

bool Handler::checkCGI()
{
	std::string scriptName = server.getRoot() + "cgi-bin/" + request.getUrl();
	FILE *file = fopen(scriptName.c_str(), "r");
	if (request.getUrl() != "" && file != NULL)
	{
		std::cout << "script is found" << std::endl;
		fclose(file);
		return true;
	}	
	return false;
}

void Handler::runCGI() 
{
	std::cout << "get root" << server.getRoot() << " url " << request.getUrl() << std::endl;
	std::string scriptName = server.getRoot() + "cgi-bin/" + request.getUrl();
	CGI cgi(request);
	cgi.launchScript(scriptName);

	std::string tempFile = "temp";
	FILE *file = fopen(tempFile.c_str(), "r");
	if (file != NULL) {
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);

		response.setStatusCode(200);
		response.setContentLength(size);
		response.setBodyFile(tempFile);
		response.setHeaders(response.getHeaders() + "\r\nContent-Type: " + contentType() + "; charset=utf-8");
	} else
		response.setStatusCode(404);
	fclose(file);
}

void Handler::checkLocation()
{
	size_t find = request.getUrl().find("/");
	std::string loc;
	if (find != std::string::npos)
		loc = request.getUrl().substr(0, find + 1);
	else 
		loc = request.getUrl() + "/";
	// std::cout << "LOC " << loc << std::endl;
	std::vector<ft::Location>::iterator it = server.getLocations().begin();
	while (it != server.getLocations().end()) {
		if (it->getName().substr(1, it->getName().length() - 1) == loc) {
			// std::cout << "LOCATION FOUND " << loc << std::endl;
			if (it->getIndex() != "") {
				server.setIndex(it->getIndex());
				if (request.getUrl() == it->getRoot().substr(0, it->getRoot().length() - 1))
					request.setUrl("");
				else
					request.setUrl(request.getUrl().substr(
						it->getRoot().length(), request.getUrl().length() - it->getRoot().length()));
			}
			if (it->getRoot() != "") 
				server.setRoot(server.getRoot() + it->getRoot());
			if (!it->getMethods().empty())
				server.setMethods(it->getMethods());
			if (!it->getErrorPages().empty())
				server.setErrorPages(it->getErrorPages());
			if (it->getRedirectionCode()) {
				server.setRedirectionCode(it->getRedirectionCode());
				server.setRedirection(it->getRedirection());
			}
			// Добавить max body size
			server.setAutoIndex(it->getAutoIndex());
		}
		it++;
	}
}

void Handler::returnErrorPage()
 {
	if (server.getErrorPages().find(response.getStatus()) != server.getErrorPages().end()){
		std::string url = server.getRoot().substr(0, server.getRoot().length() - 1) 
					+ server.getErrorPages().at(response.getStatus());
		std::cout << "Error page " << url << std::endl;
		FILE* file = fopen(url.c_str(), "rb");
		if (file != NULL) {
			fseek(file, 0L, SEEK_END);
			int size = ftell(file);
			response.setContentLength(size);
			response.setBodyFile(url);
			response.setHeaders(response.getHeaders() + "\r\nContent-Type: text/html; charset=utf-8");
			fclose(file);
		}
		else {
			std::cout << "Can't open error page\n";
		}
	}
}

Handler::~Handler() {}

Response Handler::getResponse()
{
	return this->response;
}

void Handler::methodGet()
{
	if (std::find(server.getMethods().begin(), server.getMethods().end(), "GET") == server.getMethods().end()) {
		response.setStatusCode(405);
		return;
	}
	if (server.getRedirectionCode()) {
		response.setStatusCode(server.getRedirectionCode());
		response.setHeaders("Location: " + server.getRedirection());
		return;
	}
	std::cout << "---- " << request.getUrl() << " " << server.getRoot() << std::endl;
	if (server.getAutoIndex()) { // && server.getRoot().find(request.getUrl()) != std::string::npos ) { 
		listing();
	} else 
		this->returnFile();
}

void Handler::listing()
{
	std::cout << "URL " << request.getUrl() << std::endl;
	
	// int len = request.getUrl().size();
	std::string dir = server.getRoot();
	DIR *dirp = opendir(dir.c_str());
	struct dirent *dp;

	std::ofstream out;
	out.open("listing.html");
	if (out.is_open()) {
		out << "<html><head><title>" << request.getUrl() << " listing</title></head><body>"
		<< "<h1>Список файлов в папке " << request.getUrl() << "</h1>";
		while ((dp = readdir(dirp)) != NULL) {
			if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
				out << "<p><a href=\"" << request.getUrl() + "/" + dp->d_name << "\">" << dp->d_name << "</a></p>";
		}
		out << "<p>&nbsp;</p><p><a href=\"../index.html\">Вернуться на главную</a></p></body></html>";
		response.setBodyFile("listing.html");
		response.setStatusCode(200);
		response.setContentLength(out.tellp());
		response.setHeaders(response.getHeaders() + "\r\nContent-Type: " + contentType() + "; charset=utf-8");
	}
	
	out.close();
	closedir(dirp);
}

void Handler::methodPost()
{
	if (std::find(server.getMethods().begin(), server.getMethods().end(), "POST") == server.getMethods().end())
		response.setStatusCode(405);
	else {
		std::stringstream ss;
		ss << request.getHeaders().find("Content-Length")->second;
		int contentLength;
		ss >> contentLength;
		if (contentLength > server.getMaxBodySize()) {
			response.setStatusCode(413);
			return ;
		}
		
		std::ofstream file(request.getUrl() + ".txt");
		std::map<std::string, std::string>::iterator it;
		for (it = request.getBodyPOST().begin(); it != request.getBodyPOST().end(); it++)
		{
			file << it->first
					<< ':'
					<< it->second 
					<< std::endl;
		}

		this->returnFile();
	}
}


void Handler::saveFile() // добавить exception
{
	std::cout << "saving file " << request.getFilename() << std::endl;
	std::string upload = server.getUploadPath().substr(1, server.getUploadPath().size() - 1);
	mkdir(upload.c_str(), 0777);
	std::string filename = upload + request.getFilename();
	
	FILE *file;
	if (request.getType() == DATA_START)
		file = fopen(filename.c_str(), "w");
	else
		file = fopen(filename.c_str(), "a");
	fwrite(request.getBody(), sizeof(char), request.getSize(), file);
	fclose(file);
}

void Handler::methodDelete()
{
	if (std::find(server.getMethods().begin(), server.getMethods().end(), "DELETE") == server.getMethods().end())
		response.setStatusCode(405);
}

void Handler::returnFile()
{	
	std::string url = server.getRoot() + request.getUrl();
	if (url == server.getRoot())
		url = url + server.getIndex();
	const char *file_path = url.c_str();

	struct stat buf;
	lstat(file_path, &buf);
	FILE* file = fopen(file_path, "rb");
	if (!S_ISREG(buf.st_mode) || file == NULL)
	{
		std::cout << "Can't open file " << url << std::endl;
		response.setStatusCode(404);
	}
	else
	{
		std::cout << "File opened OK, URL: " << url << std::endl;
		response.setStatusCode(200);
		
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);

		response.setContentLength(size);
		response.setBodyFile(url);
		response.setHeaders(response.getHeaders() + "\r\nContent-Type: " + contentType() + "; charset=utf-8");
		fclose(file);
	}

	// std::cout << "*** Response ***\n" << response.toString() << "\n***\n";
}

std::string Handler::contentType()
{
	std::string file = response.getBodyFile();
	int dot = file.find(".");
	std::string extension = file.substr(dot + 1, file.length() - dot);
	return response.getContentType(extension);
}
