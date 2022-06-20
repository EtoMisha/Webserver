#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

// #define HOME_PAGE "index.html"
// #define HOME_DIR "res/"

class Handler
{
	public:
		Handler();
		Handler(Request request, Config config);
		~Handler();

		Response getResponse();

	private:
		Config config;

		Request request;
		Response response;

		void methodGet();
		void methodPost();
		void methodDelete();

		void returnFile();
		std::string contentType();

		// void readFile(unsigned char* buffer, size_t size, const char* file_path);
};