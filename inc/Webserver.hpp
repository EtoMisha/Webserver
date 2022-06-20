#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <vector>

#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Connection.hpp"

#define NUSERS 10

class Webserver
{
	public:
		Webserver();
		Webserver(Config config);
		~Webserver();
		
		void run();

	private:

		Config config;

		int listen_socket;
		bool end_server;

		struct pollfd fds[200];
		int nfds;

		std::map<int, Connection*> connections;

		void listenLoop();
		int sendAndReceive(int fd, int i);
		void closeConnection(int i);
		
		std::string readRequest(int fd);
		int sendHeader(Connection & connection);
		int sendBody(Connection & connection);

		void sendResponse(Connection &connection);
		int sendFile(Connection &connection);

		int err(std::string msg);

};