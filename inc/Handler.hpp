#pragma once

#include "Request.hpp"
#include "Response.hpp"
// #include "Config.hpp"
#include "Server.hpp"

// #define HOME_PAGE "index.html"
// #define HOME_DIR "res/"

class Handler
{
	public:
		Handler();
		// Handler(Request request, Config config);
		Handler(Request request, ft::Server _server);
		~Handler();

		Response getResponse();

	private:
		// Config config;
		ft::Server server;

		Request request;
		Response response;

		void methodGet();
		void methodPost();
		void methodDelete();

		void returnFile();
		std::string contentType();

		// void readFile(unsigned char* buffer, size_t size, const char* file_path);
};