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
#include "Server.hpp"

#define NUSERS 10

class Webserver
{
	public:
		Webserver();
		Webserver(std::vector<ft::Server> &_servers);
		~Webserver();
		
		void run();
		void prepare(int serv_id);

		const std::vector<ft::Server> &getServers();

		std::map<int, ft::Server> servs_fd;

	private:

		struct pollfd listen_fds;

		std::vector<ft::Server> servers;


		int listen_socket;
		bool end_server;

		struct pollfd fds[200];
		int nfds;

		std::map<int, Connection*> connections;
		std::vector<int> sockets;

		void listenLoop();
		int sendAndReceive(struct pollfd fds, struct pollfd listen_fds);
		void closeConnection(int i);
		
		std::string readRequest(int fd, int serv_id);
		int sendHeader(Connection & connection);
		int sendBody(Connection & connection);

		void sendResponse(Connection &connection);
		// int sendFile(Connection &connection, int serv_id);

		void printFds();
		void printConnections();

		int err(std::string msg);

};