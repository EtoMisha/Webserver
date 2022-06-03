#pragma once

#include "Request.hpp"
#include "Response.hpp"

#define HOME_PAGE "index.html"

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
};