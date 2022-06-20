#pragma once
#include <iostream>
#include <map>

#include "Request.hpp"
#include "Response.hpp"

class Connection
{
	public:
		Connection();
		Connection(Request req, Response resp, int fd);
		~Connection();
		Connection(Connection const & other);
		Connection & operator=(Connection const & other);
		
		int const getFd() const;
		Request & getRequest();
		Response & getResponse();
		bool isFinished() const;
		int getPosition() const;

		void setResponse(Response response);
		void setRequest(Request request);
		void setFinished(bool finished);
		void setPosition(int counter);
		
	private:
		
		int fd;
		Request request;
		Response response;
		bool finished;
		int position;
};
