#pragma once

#include "Request.hpp"
#include "Response.hpp"

#define HOME_PAGE "index.html"
// #define HOME_DIR "res/"

class Handler
{
	public:
		Handler();
		Handler(Request request);
		~Handler();

		Response getResponse() const;

	private:
		Request request;
		Response response;

		void methodGet();
		void methodPost();
		void methodDelete();

		// void readFile(unsigned char* buffer, size_t size, const char* file_path);
};