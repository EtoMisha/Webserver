#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Location.hpp"


#include <fstream>
#include <iostream>
#include <dirent.h>

// #define HOME_PAGE "index.html"
// #define HOME_DIR "res/"

class Handler
{
	public:
		// Handler();
		// Handler(Request request, Config config);
		Handler(Request & request, ft::Server server);
		~Handler();

		Response getResponse();

	private:
		// Config config;
		ft::Server server;

		Request & request;
		Response response;

		void methodGet();
		void methodPost();
		void methodDelete();
		void saveFile();

		void returnFile();
		void checkLocation();

		bool checkCGI();
		void runCGI();
		char ** getCGIargs();

		void returnErrorPage();
		void listing();

		std::string contentType();

		// void readFile(unsigned char* buffer, size_t size, const char* file_path);
};