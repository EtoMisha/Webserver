#pragma once
// #include <iostream>
// #include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"


enum ConStatus
{
	READ, READ_DONE, WRITE, WRITE_DONE, CLOSE
};

class Connection
{
	public:
		Connection();
		Connection(int fd, int listen_fd);
		~Connection();
		Connection(Connection const & other);
		Connection & operator=(Connection const & other);
		
		int getFd() const;
		int getListenFd() const;
		Request & getRequest();
		Response & getResponse();
		int getPosition() const;
		ConStatus getStatus();
		ft::Server getServer();
		
		void setRequest(Request request);
		void setResponse(Response response);
		void setPosition(int counter);
		void setStatus(ConStatus status);

		int readRequest();
		int sendHeaders();
		int sendBody();
		
	private:
		
		int fd;
		int listen_fd;
		Request request;
		Response response;
		int position;
		ConStatus status;
		std::string filename;
};
